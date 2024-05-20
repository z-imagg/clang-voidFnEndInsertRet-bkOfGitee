#define INT_T int
typedef double DOUBLE_typedef;

struct Point{
    float x;
    double y;
};


char* charPtrFunc04(){
  if(1){
    char sex;
    return &sex;
  }else{
    char ch;
    return &ch;
  }
}