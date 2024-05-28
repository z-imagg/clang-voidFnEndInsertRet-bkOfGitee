#define CASE_SAME(x) case x:
#define INT_T int
typedef double DOUBLE_typedef;

struct Point{
    float x;
    double y;
};
class UserEntity{};
class ScoreRelation{
protected:
  float calcScoreSum(int cnt, char t);
};
float ScoreRelation::calcScoreSum(int cnt, char t){
    int dummy;
    return 3.9*cnt+t=='z'?5:-5;
}

class MyClass {
public:
    static int ZERO;
public:
    double m_d1;
    int m_n2;

    MyClass( ){}

    MyClass(float f1, int n1)
    :m_d1(f1),m_n2(n1)
    {
        char c3=1+n1;
    }

    ~MyClass(){
        int x,y,z;
        UserEntity userEntity;

        Point point0;
        auto fn_point = [](const Point& point) {
            if(point.x>point.y)
                return point.x+point.y;
            else
                return 0.1;
        };

        fn_point(point0);


        auto fn_pnt = [](const Point& pnt) {
            return pnt.x+pnt.y;
        };
        fn_pnt(point0);
        
    }

  void voidDemo(int cnt, short chr){
  }

    void voidDemo2(UserEntity userEntity){
      if(true){
          return;
      }
    }



};

int MyClass::ZERO=0;

void voidDemo3(){
    int k=0;
    k++;
    
}
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