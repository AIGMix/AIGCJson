#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class Popole 
{
public:
    string name;
    string age;

    AIGC_JSON_HELPER(name, age) //成员注册
};
class Student : Popole
{
public:
    string depart;
    int grade;
    AIGC_JSON_HELPER(depart, grade) //成员注册
    AIGC_JSON_HELPER_BASE((Popole*)this) //基类注册
};
class Class 
{
public:
    string name;
    int test;
    std::list<Student> students;
    Popole master;
    AIGC_JSON_HELPER(name, test, students, master) //成员注册
    AIGC_JSON_HELPER_DEFAULT("test=123")
};

string sjson = R"({
    "name": "yaronzz", "master" : {"name" : "刘老师", "age" : 35}, "students" : [ {"name" : "张小明", "age" : 5, "grade" : 3, "depart" : "小学"}, {"name" : "张小红", "age" : 7, "grade" : 3, "depart" : "小学"} ]
})";

int main()
{
    Class my;
    JsonHelper::JsonToObject(my, sjson);
    return 0;
}


