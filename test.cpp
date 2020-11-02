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
    std::list<Student> students;
    Popole master;
    AIGC_JSON_HELPER(name, students, master) //成员注册
};

string sjson = R"({
    "name" : "小一班", "master" : {"name" : "刘老师", "age" : 35}, "students" : [ {"name" : "张小明", "age" : 5, "grade" : 3, "depart" : "小学"}, {"name" : "张小红", "age" : 7, "grade" : 3, "depart" : "小学"} ]
})";

int main()
{
    Class my = JsonHelper::GetObjectByJson<Class>(sjson);
    return 0;
}