#include <iostream>
#include <stdio.h>
#include "construct.h"
#include "allocator.h"
#include "util.h"
#include "algobase.h"
#include "vector.h"
#include <string>
using namespace mystl;
using std::cout;
using std::endl;



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
void show(vector<int> v){
    for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
        cout << *it << " ";
    }
    cout << endl;

}




int main() {
//    char dest[] = "oldstring";
//    char src[]  = "newstring";
//    char * c = mystl::unchecked_copy(src,src + 9,dest);
    int a[] = {1,2,3,4,5};
    size_t n = 5;
    vector<int> v(n, 1);
    v[4] = 2;
    show(v);
    vector<int> v2(v);
    show(v2);
    printf("%d\n", v2[4]);
    vector<int> v3(a, a+5);
    show(v3);

    printf("%d\n", v.size());
    printf("%d\n", v.capasity());



    return 0;
}
