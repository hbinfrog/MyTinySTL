#include <iostream>
#include <stdio.h>
#include "construct.h"
#include "allocator.h"
#include "util.h"
#include "algobase.h"
#include "vector.h"
#include <string>
using namespace mystl;



void init(int * a){
    ::new ((void *) a) int(2);
}
class test{
public:
    int age;
    int score;
    test(int Age, int Score){
        this->age = Age;
        this->score = Score;
    }

    ~test(){}

};
bool operator<(const test& t1, const test& t2){
    return t1.score < t2.score;
}
class MyCompare{
public:
    bool operator()(const test& a, const test& b){
        return a < b ? true : false;
    }
    bool operator()(int a, int b){
        return a < b ? true : false;
    }
};
pair<int, char> test1(){
    pair<int, char> c(10,'s');
    return c;
}



int main() {
    char dest[] = "oldstring";
    char src[]  = "newstring";
    char * c = mystl::unchecked_copy(src,src + 9,dest);
    size_t n = 34;
    vector<int> v(n);
    printf("%d\n", v.size());
    printf("%d\n", v.capasity());



    return 0;
}
