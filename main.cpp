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
pair<int, char> test1(){
    pair<int, char> c(10,'s');
    return c;
}



int main() {
    pair<int, char> p1(100,'a');
    pair<int, char> p2(100,'b');
    pair<int, char> p3;
    pair<char, int> p6;
    p6.operator=(p1);
    std::cout << p6.first << " " << p6.second << std::endl;
    //p6.operator=(test1());//wrong
    //std::cout << p6.first << " " << p6.second << std::endl;
    bool res = p1 <= p2;
    std::cout << res << std::endl;
    int a = 233;
    char c = 'g';
    pair<int, char> ac = make_pair(a,c);
    std::cout << ac.first << " " << ac.second << std::endl;

    return 0;
}
