//来源: https://gitee.com/repok/sleuthkit-4.12.1/blob/master/tsk/fs/ntfs_dent.cpp
class NTFS_PAR_MAP  {
public:

        /**
         * Test if there are any children for this directory at a given sequence.
         * @param seq Sequence to test.
         * @returns true if children exist
         */
        bool exists (int seq) {
            if ((seq) > 0) 
                return true;
            else
                return false;
            //这里是不应该插入return语句的
            //错误的在这里插入return语句的原因是, 判断'如果是当前块语句的父节点' 写反了, 没有正确的容纳c++普通函数， 只容纳了普通函数
        }
 
 };

 int main(int argc, char** argv){
  return 0;
 }