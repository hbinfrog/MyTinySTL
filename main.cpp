#include <iostream>
#include "construct.h"
#include "allocator.h"
#include "util.h"
using namespace mystl;
#include <iterator>



void init(int * a){
    ::new ((void *) a) int(2);
}
class test{
public:
    int * age;
    test(int Age){
        age = new int (Age);
    }
    ~test(){
        ::delete(age);
    }

};
int test1(){
    int a = 10;
    return a;
}


int main() {
    test t(10);
    std::cout << *t.age << std::endl;
    test * t1 = new test(100);

    mystl::destroy(t1);

    int a = 100;
    int& b = a;
    int&& c = 10;
    /*
     int temp = 10;
     int & c = temp;
    */

    typename std::remove_reference<int>::type&& res = mystl::move(c);
    //cout <<
    int&& d = mystl::forward<int>(res);






    return 0;
}
