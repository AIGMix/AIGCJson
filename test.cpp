#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class People 
{
public:
    string name;
    string age;

    AIGC_JSON_HELPER(name, age) //成员注册
};
class Student : People
{
public:
    string depart;
    int grade;
    AIGC_JSON_HELPER(depart, grade) //成员注册
    AIGC_JSON_HELPER_BASE((People*)this) //基类注册
};
class Class 
{
public:
    string name;
    int test;
    std::list<Student> students;
    std::unordered_map<std::string, int> property;
    People master;
    std::set<std::string> users;
    AIGC_JSON_HELPER(name, test, students, master, property, users) //成员注册
    AIGC_JSON_HELPER_DEFAULT("test=123")
};

string sjson = R"({
    "name": "yaronzz", "master" : {"name" : "liu", "age" : 35}, 
    "students" : [  {"name" : "zhang", "age" : 5, "grade" : 3, "depart" : "primary school"}, 
                    {"name" : "chen", "age" : 7, "grade" : 3, "depart" : "primary school"} ],
    "property" : {"grade" : 1, "num" : 33},
    "users": ["zhang", "chen"]
})";

int main()
{
    Class my;
    JsonHelper::JsonToObject(my, sjson);
    return 0;
}


