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
#include <deque>

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
void show(std::deque<int>& d){
    for(std::deque<int>::iterator it = d.begin(); it != d.end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}





int main() {
    student s1(23,95);
    student s2(21,100);
    student s3(22,99);
    deque<student> d({s1,s2,s3,s2,s3,s1,s2,s3,s2,s1});
    show(d);
    d.push_back(s1);
    show(d);
    d.pop_back();
    d.pop_back();
    d.pop_front();
    d.pop_front();
    show(d);
    std::deque<int> sd;
    for(int i = 0; i < 10; i++){
        sd.push_back(i);
    }
    sd.insert(sd.begin() + 3, 5);
    show(sd);
    sd.insert(sd.begin() + 10, 5);
    show(sd);
    deque<int> dq;
    for(int i = 0; i < 10; i++){
        dq.push_back(i);
    }
    dq.emplace(dq.begin() + 3, 5);
    show(dq);
    dq.emplace(dq.begin() + 10, 5);
    show(dq);
    dq.insert(dq.begin(),5,8);
    show(dq);
    dq.insert(dq.end(),7,1);
    show(dq);
    dq.insert(dq.begin()+5,6,0);
    show(dq);



    return 0;
}
