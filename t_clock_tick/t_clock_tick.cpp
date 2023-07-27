
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <sstream>
#include <atomic>
#include <fstream>
#include <filesystem>

//////自定义线程id实现
// static std::atomic<int> 用作全局线程id计数器、  thread_local 线程id：  实现自定义进程内全时间唯一线程id
#define FirstThreadId 0
static std::atomic<int> globalThreadIdCounter(FirstThreadId);
int X__nextThreadId(){
  globalThreadIdCounter++;
  return globalThreadIdCounter;
}
#define ThreadIdInitVal -1
thread_local int currentThreadId=ThreadIdInitVal;//当前线程id
int X__curThreadId(){
  if(currentThreadId==ThreadIdInitVal){
    currentThreadId=X__nextThreadId();
  }else{
    return currentThreadId;
  }
}


///////本线程当前变量数目累积值

thread_local int t;//时钟
thread_local int sVarAllocCnt=0;//当前栈变量分配数目 sVarAllocCnt: currentStackVarAllocCnt
thread_local int sVarFreeCnt=0;//当前栈变量释放数目 sVarFreeCnt: currentStackVarFreeCnt
thread_local int sVarCnt=0;//当前栈变量数目（冗余） sVarCnt: currentStackVarCnt
thread_local int hVarAllocCnt=0;//当前堆对象分配数目 hVarAllocCnt: currentHeapObjAllocCnt, var即obj
thread_local int hVarFreeCnt=0;//当前堆对象释放数目 hVarFreeCnt: currentHeapObjcFreeCnt, var即obj
thread_local int hVarCnt=0;//当前堆对象数目（冗余）hVarCnt: currentHeapObjCnt, var即obj

///////工具
std::string X__getCurrentProcessCmdLine() {
  std::ifstream file("/proc/self/cmdline");
  if (file) {
    std::string name;
    std::getline(file, name, '\0');
    return name;
  }
  return "";
}
/**
 * 分割后第一个子串
 * 比如:
 * 输入: line: /usr/bin/ls, delimiter:/
 * 输出: ls
 * @param line
 * @param delimiter
 * @param firstSubStr
 */
void splitGetFirst(std::string  line,std::string delimiter,std::string& firstSubStr ){
//  std::string delimiter = " ";
  firstSubStr = line.substr(0, line.find(delimiter));
}
void splitGetEnd(std::string  line,std::string delimiter,std::string& endSubStr ){
//  std::string delimiter = " ";
  std::string::size_type idxEndSubStr = line.rfind(delimiter)+1;
  if(idxEndSubStr < line.size()){
    endSubStr = line.substr(idxEndSubStr);
  }
}
/**
 * 不支持 进程名全路径中含有空格的 比如 /opt/my\ tool/app1
 * 输入: /snap/chromium/2556/usr/lib/chromium-browser/chrome --type=gpu-process arg2
 * 输出: chrome
 * @return
 */
std::string X__getCurrentProcessName() {
  std::string cmdlineWithArgs=X__getCurrentProcessCmdLine();
  std::string cmdline ;
  splitGetFirst(cmdlineWithArgs," ",cmdline);

  std::string processName ;
  splitGetEnd(cmdline,"/",processName);

  return processName;
}

/**
 * 获取当前时刻毫秒数
 * @return
 */
long X__getNowMilliseconds() {
  auto currentTime = std::chrono::system_clock::now();
  auto duration = currentTime.time_since_epoch();

  // Convert duration to milliseconds
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return milliseconds;
}

///////当前滴答
class Tick{
public:
    int t;//时钟
    int sVarAllocCnt=0;//当前栈变量分配数目 sVarAllocCnt: currentStackVarAllocCnt
    int sVarFreeCnt=0;//当前栈变量释放数目 sVarFreeCnt: currentStackVarFreeCnt
    int sVarCnt=0;//当前栈变量数目（冗余） sVarCnt: currentStackVarCnt
    int hVarAllocCnt=0;//当前堆对象分配数目 hVarAllocCnt: currentHeapObjAllocCnt, var即obj
    int hVarFreeCnt=0;//当前堆对象释放数目 hVarFreeCnt: currentHeapObjcFreeCnt, var即obj
    int hVarCnt=0;//当前堆对象数目（冗余）hVarCnt: currentHeapObjCnt, var即obj
public:
    Tick(int _t,int _sVarAllocCnt, int _sVarFreeCnt, int _sVarCnt, int _hVarAllocCnt, int _hVarFreeCnt, int _hVarCnt)
    :
    t(_t),
    sVarAllocCnt(_sVarAllocCnt),
    sVarFreeCnt(_sVarFreeCnt),
    sVarCnt(_sVarCnt),
    hVarAllocCnt(_hVarAllocCnt),
    hVarFreeCnt(_hVarFreeCnt),
    hVarCnt(_hVarCnt)
    {
//      sVarCnt=sVarAllocCnt-sVarFreeCnt;
//      hVarCnt=hVarAllocCnt-hVarFreeCnt;
    }

    Tick( ){

    }

    void toString(std::string & line){
      char buf[128];
      sprintf(buf,"%d,%d,%d,%d,%d,%d,%d\n",t,sVarAllocCnt,sVarFreeCnt,sVarCnt,hVarAllocCnt,hVarFreeCnt,hVarCnt);
      line.append(buf);
    }
};

///////线程级滴答缓存
#define TickCacheSize 500
#define CacheIdxStart 0
class TickCache {
public:
    bool inited;
    Tick cache[TickCacheSize];
    int curEndIdx;
    std::ofstream fWriter;
    static const std::string tick_data_home;
    TickCache(){
      //构造函数被 "TLS init function for tickCache" 调用，发生在线程创建初始阶段，所以本函数最好少干事。
      inited=false;
    }

    /**
     * tick文件路径格式: /tick_data_home/进程名_进程id_当前时刻毫秒数_线程id
     * 如果不存在目录 /tick_data_home/, tick文件路径是 ./进程名_进程id_当前时刻毫秒数_线程id
     * @return
     */
    static std::string filePath(){
      pid_t processId = getpid();
      const std::string processName = X__getCurrentProcessName();

      long milliseconds=X__getNowMilliseconds();

      int curThreadId=X__curThreadId();
      std::string fileName(processName+"_"+std::to_string(processId)+"_"+std::to_string(milliseconds)+"_"+std::to_string(curThreadId));

      bool tick_data_home_existed=std::filesystem::exists(tick_data_home);
      if(tick_data_home_existed){
        std::string filePath=tick_data_home+"/"+fileName;
        return filePath;
      }else{
        return fileName;
      }
    }

    void my_init(){
      if(inited){
        return;
      }

      inited=true;
      curEndIdx=CacheIdxStart;

      if(!fWriter.is_open()){
        std::string filePath= TickCache::filePath();
        fWriter.open(filePath);

        //刚打开文件时，写入标题行
        std::string title("滴答,栈生,栈死,栈净,堆生,堆死,堆净\n");
        fWriter << title ;
      }
    }
    ~TickCache(){
      if(!inited){
        return;
      }
//      printf("exit:%p,this->init:%d\n",this,this->inited);
      inited=false;//thread_local对象对本线程只有一份，即 thread_local对象的析构函数一定只调用一次， 因此这句话有没有无所谓了
      //此时估计是进程退出阶段，缓存无论是否满都要写盘，否则缓存中的数据就丢失了
      _flushIf(true);
      if(fWriter.is_open()){
        fWriter.close();
      }
    }
private:
    void _flushIf(bool condition){//由于本函数写了返回bool，但少了return，再次导致执行流乱跳。
      /////若条件满足, 则写盘 并 清空缓存.
      //若缓存满了
      if(condition){
        //写盘
        for(int i=0; i <=curEndIdx; i++){
          std::string line;
          cache[i].toString(line);
          fWriter << line ;
        }
        fWriter.flush();

        //清空缓存
        curEndIdx=CacheIdxStart;
      }
    }
public:
    void save(Tick & tick){
      if(!inited){
        my_init();
      }

      /////若缓存满了, 则写盘 并 清空缓存.
      bool full=curEndIdx==TickCacheSize-1;
      _flushIf(full);

      /////当前请求进缓存
      cache[curEndIdx]=tick;
      ++curEndIdx;

    }

};
thread_local TickCache tickCache;

const std::string TickCache::tick_data_home("/tick_data_home");

const std::string X__true("true");
/**
 *
 * @param _sVarAllocCnt  此次滴答期间， 栈变量分配数目
 * @param _sVarFreeCnt   此次滴答期间， 栈变量释放数目
 * @param _hVarAllocCnt   此次滴答期间， 堆对象分配数目
 * @param _hVarFreeCnt   此次滴答期间， 堆对象释放数目
 */
void X__t_clock_tick(int _sVarAllocCnt, int _sVarFreeCnt, int _hVarAllocCnt, int _hVarFreeCnt){

  //时钟滴答一下
  t++;

  //更新 当前栈变量分配数目
  sVarAllocCnt+=_sVarAllocCnt;

  //更新 当前栈变量释放数目
  sVarFreeCnt+=_sVarFreeCnt;

  //更新 当前栈变量数目 == 当前栈变量分配数目 - 当前栈变量释放数目
  sVarCnt= sVarAllocCnt - sVarFreeCnt;

  //更新 当前堆对象分配数目
  hVarAllocCnt+=_hVarAllocCnt;

  //更新 当前堆对象释放数目
  hVarFreeCnt+=_hVarFreeCnt;

  //更新 当前堆对象数目 == 当前堆对象分配数目 - 当前堆对象释放数目
  hVarCnt= hVarAllocCnt - hVarFreeCnt;

  //如果有设置环境变量tick_save,则保存当前滴答
  const char* tick_save=std::getenv("tick_save");
  if(tick_save && X__true==tick_save){
    Tick tick(t,sVarAllocCnt, sVarFreeCnt, sVarCnt, hVarAllocCnt,hVarFreeCnt,hVarCnt);
    tickCache.save(tick);
  }


  return;
}

