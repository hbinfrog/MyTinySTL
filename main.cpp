#include <iostream>
#include <stdio.h>
#include "construct.h"
#include "allocator.h"
#include "util.h"
#include "algobase.h"
#include "vector.h"
#include "list.h"
#include <string>
#include "deque.h"
#include "stack.h"
#include "queue.h"
#include <algorithm>
#include <queue>

using namespace mystl;
using std::cout;
using std::endl;
class student{
public:
    int Age;
    int *Score;
    student(){
        Age = 0;
        Score = new int (0);
    }
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
    bool operator==(const student& s){
        return (Age == s.Age && *Score == *s.Score);
    }
    bool operator!=(const student& s){
        return Age != s.Age || *Score != *s.Score;
    }
    ~student(){
        if(Score != nullptr){
            delete Score;
            Score = nullptr;
        }
        //cout << 1 << endl;
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
void show(list<int>& l){
    for(list<int>::iterator it = l.begin(); it != l.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;

}
void show(list<student>& l){
    for(list<student>::iterator it = l.begin(); it != l.end(); ++it){
        cout << *it->Score << " " << it->Age << " ";
    }
    cout << endl;

}
class com{
public:
    bool operator()(const student& s1){
        return s1.Age < 22;
    }
};
class comp{
public:
    bool operator()(const student& s1, const student& s2){
        return s1.Age < s2.Age;
    }
};
bool operator==(const student& s1, const student& s2){
    return (s1.Age == s2.Age && *s1.Score == *s2.Score);
}

bool operator<(const student& s1, const student& s2){
    return s1.Age < s2.Age;
}
class un{
public:
    bool operator()(const student& s1, const student& s2){
        return s1.Age == s2.Age;
    }
};
void show(deque<int>& d){
    for(deque<int>::iterator it = d.begin(); it != d.end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}
void show(deque<student>& d){
    for(deque<student>::iterator it = d.begin(); it != d.end(); it++){
        cout << it->Age << " " << *it->Score << " ";
    }
    cout << endl;
}
void show(stack<int> s) {
    while (!s.empty()) {
        cout << s.pop() << " ";
    }
    cout << endl;
}
void show(std::priority_queue<int> pq){
    while (!pq.empty()){
        int a = pq.top();
        cout << a << " ";
        pq.pop();
    }
    cout << endl;
}






//int main() {
//    deque<int> d({2,5,8,7,4,1,3,6,9});
//    stack<int> s1(d);
//
//    bool ok = d.empty();
//    show(s1);
//
//    stack<int> s2(s1);
//    show(s2);
//    queue<int> q(d);
//    bool ok1 = q.empty();
//    int a[] = {6,4,7,1};
//    cout << -1 / 2 << endl;
//    //std::push_heap()
//
//    std::priority_queue<int> pq(a,a+4);
//    pq.push(10);
int main () {
    vector<int> v1({1,2,3,4,5});
    v1.assign(3,4);
    show(v1);


    return 0;
}


