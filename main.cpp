#include <iostream>
#include <stdio.h>
#include "construct.h"
#include "allocator.h"
#include "util.h"
#include "algobase.h"
#include "vector.h"
#include "list.h"
#include <string>
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





int main() {
    student s1(23,95);
    student s2(21,100);
    student s3(22,99);
    student s4(23,88);
    student s5(18,67);
    student s6(27,88);
    student s7(25,87);
    student s[3] = {s3,s2,s1};
    list<student> l(2,s1);
    show(l);
    l.insert(l.begin(),s, s + 3);
    show(l);
    l.emplace(l.begin(), mystl::move(s2));
    show(l);
    l.pop_back();
    show(l);
    l.pop_front();
    show(l);
    l.reverse();
    show(l);
    //mystl::swap()
//    show(l1);
    l.remove_if(com());
    show(l);
    l.emplace_back(s1);
    l.emplace_front(s1);
    show(l);
    l.emplace_front(s4);
    l.emplace_front(s4);
    l.emplace_back(s4);
    l.emplace_back(s4);
    show(l);
    l.unique(un());
    show(l);
    list<student> l3({s2,s3,s1});
    list<student> l2({s5,s7,s6});
    l3.merge(l2);
    //l3.merge(l2,comp());
    show(l3);












    return 0;
}
