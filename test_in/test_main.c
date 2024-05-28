#define INT_T int
typedef double DOUBLE_typedef;

struct Point{
  float x;
  double y;
};

void voidDemo3(){
  int k=0;
  k++;

}

int intDemo4(){ return 9; }


void voidDemo5(){ return ; }


char* charPtrFunc04(){
  if(1){
    char sex;
    return &sex;
  }else{
    char ch;
    return &ch;
  }
}
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