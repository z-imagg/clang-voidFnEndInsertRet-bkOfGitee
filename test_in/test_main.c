#pragma message("VFIR_inserted")
#define INT_T int
typedef double DOUBLE_typedef;

struct Point{
    float x;
    double y;
};

void voidDemo3(){
    int k=0;
    k++;

return; /* voidFnEndInsertRet: */}

char* charPtrFunc04(){
  if(1){
    char sex;
    return &sex;
  }else{
    char ch;
    return &ch;
  }
return; /* voidFnEndInsertRet:  这里是错误的，不应该加'return;' ，应该不做任何插入 */}
int main(int argc, char** argv){
    struct Point pnt1;
    struct Point pnt2;
    {
        struct Point * ptr1=&pnt1;
        struct Point * ptr2=&pnt2;
        struct Point pnt3;
    }

  return 0;
 }