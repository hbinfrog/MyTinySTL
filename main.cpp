#include <iostream>
#include "construct.h"
#include "allocator.h"
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


int main() {
    test t(10);
    std::cout << *t.age << std::endl;
    test * t1 = new test(100);

    mystl::destroy(t1);





    return 0;
}
