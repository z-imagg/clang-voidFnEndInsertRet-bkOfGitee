#define CASE_SAME(x) case x:
#define INT_T int
typedef double DOUBLE_typedef;

struct Point{
    float x;
    double y;
};
class UserEntity{};
class ScoreRelation{};
class MyClass {
public:
    static int ZERO;
public:

  char* switchDemo(int cnt, short chr){
  int* myIntPtr;//单变量声明
  char x,y,z;//多变量声明
  int i1,i2,*p_i3;//多变量声明
  UserEntity admin;
  ScoreRelation scoreRlatn;
  INT_T num_int;
  DOUBLE_typedef num_double;
  double sum=1.*cnt*chr;
  float PI;
    start: char L;
    switch(chr){
      case -4:  case -3:
      case -2:case-1:
      sum++;
      break;
      case 0:
      {
      struct Point pnt4;
        L=(cnt+chr)%50;
        return &L+1;//case0 结束位置左移1
      }


      case 1:
        int/*此非case1结束*/ unit;//case1共3行单语句, 这里是case1的第1行单语句
        unit=18;
        return 0;//case1 结束位置
        CASE_SAME(-100)
      case 2:
        cnt/*此非case2结束*/=L*L;
        cnt++;
        break;//case2 结束
      case 3:
        return/*此非case3结束*/ &L;//case3 结束
      case 4:
      {
        L++;
        sum=L+sum;
        if (L)  goto start;//这里并不是case4结束, 这里是case4的块1结束, case4 : 块1+散语句，
      struct Point pnt5,pnt6,pnt7,*ptr_pnt8;
      }
      PI=3.14;
      return (char*)0;
      case 5:{
        int zB;
      struct Point pnt9,pnt10,pnt11,pnt12;
        break;//case5 结束位置左移1
      }//case5语句结束, 但后面还有一行注释
      ////////
      case 6:{
        int uuuuu;
        break;//case6 结束位置左移1
      }//case6 结束
      default:
        sum/*此非default结束*/+=chr*cnt;
        return &L;//default结束
    }
    return 0;
  }
};

int MyClass::ZERO=0;

int main(int argc, char** argv){
    MyClass varMyClass;
    static Point pnt1;
    struct Point pnt2;
    {
        struct Point * ptr1=&pnt1;
        struct Point * ptr2=&pnt2;
        struct Point pnt3;
    }

  return 0;
}