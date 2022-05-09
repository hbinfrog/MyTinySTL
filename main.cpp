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
#include "algorithm.h"
#include "string.h"
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
    }
};

pair<int, char> test1(){
    pair<int, char> c(10,'s');
    return c;
}
void show(vector<int>& v){
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



int main () {
    string s(10, 'a');
    for(int i = 0; i < 10; i++){
        cout << s[i] << endl;
    }
    char c = s.at(9);
    std::string s1 = "abcgd";
    unsigned res = s1.find_first_of('z', 0);





    return 0;
}


