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
class student{
public:
    int Age;
    int *Score;
    student(int age, int score){
        Age = age;
        Score = new int (score);
    }
    student(const student& s){
        Age = s.Age;
        Score = new int (*s.Score);
    }
    student& operator=(const student& s){
        Age = s.Age;
        Score = new int (*s.Score);
        return *this;
    }
    bool operator!=(const student& s){
        return (Age == s.Age && Score == s.Score);
    }
    ~student(){
        if(Score != nullptr){
            delete Score;
            Score = nullptr;
        }
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
void show(vector<student> vs){
    for (vector<student>::iterator it = vs.begin(); it != vs.end(); it++) {
        cout << *it->Score << " " << it->Age << " ";

    }
    cout << endl;
}
vector<int> test(){
    vector<int> v(10,2);
    return v;
}




int main() {
    //vector<int> v5
    student s1(23,95);
    vector<student> vs(5, s1);
    show(vs);
    student s2(21,100);
    vector<student> vs1(10,s2);
    cout << vs.size() << endl;
    cout << vs.capasity() << endl;
    vs1 = vs;
    show(vs1);
    int&& v = 10;
    int n = v;



    return 0;
}
