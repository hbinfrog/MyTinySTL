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
//    std::cout << *t.age << std::endl;
//    test * t1 = new test(100);
//
//    mystl::destroy(t1);

//    int a = 100;
//    int& b = a;
//    int&& c = 10;
//    /*
//     int temp = 10;
//     int & c = temp;
//     destroy temp;
//    */
//    int g = c;
//    b = c;
//    g = 67;
//
//
//    typename std::remove_reference<int>::type&& res = mystl::move(c);
//    //cout <<
//    int&& d = mystl::forward<int>(res);
//    typename std::remove_reference<char>::type&& arg  = 'a';//char&& arg = 'a';
//    int&& e = mystl::forward<int>(arg);  //int&& e = arg;     (arg from char to int)
    int x = 10;
    int y = 20;
    //int temp(10);
    mystl::swap(x,y);
    int a[10] = {0,1,2,3,4,5,6,7,8,9};
    int b[10] ={78,9,8,7,6,5,100};
    int res = *(b+12);
    int * temp = swap_range(a,a+10,b);
    pair<int, char> p(10,'a');
    std::cout << p.first << std::endl;
    std::cout << p.second << std::endl;
    pair<test, int> p1(t,10);
    char c1 = 'a', c2 = 'b';
    pair<int, int> p2(c1,c2);
    std::cout << p2.second << " " << p2.first << std::endl;
    pair<int, char> p3(p2);
    std::cout << p3.second << " " << p3.first << std::endl;
    pair<test, char> p4(p2);
    std::cout << p4.second << " " << *(p4.first.age) << std::endl;












    return 0;
}
