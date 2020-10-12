#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class Student
{
public:
    string Name;
    int Age;

    AIGC_JSON_HELPER(Name, Age)
    AIGC_JSON_HELPER_RENAME("name","age")
};

void Test1()
{
    Student person;
    JsonHelper::JsonToObject(person, R"({"name":"XiaoMing", "age":15})");

    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    return;
}

class Record
{
public:
    int iValue;
    unsigned int uiValue;
    float fValue;
    double dValue;
    string sValue;
    bool bValue;
    char cValue;
    list<int> vValue;

    AIGC_JSON_HELPER(iValue, uiValue, vValue);
};

void Test2()
{
    bool check;
    Record person;
    JsonHelper::JsonToObject(person, R"({"iValue": 100, 
                                        "uiValue": 15,
                                        "vValue": [1,2,3,4,5]})");
    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    return;
}

class BaseLeason
{
public:
    int Type;
    string StartTime;
    string EndTime;

    AIGC_JSON_HELPER(Type, StartTime, EndTime);
};

class MathLeason
{
public:
    string TeacherName;
    int Count;
    BaseLeason Leason;

    AIGC_JSON_HELPER(TeacherName, Count, Leason);
};

class AllLeason
{
public:
    int Count;
    std::list<BaseLeason> Leasons;
    AIGC_JSON_HELPER(Count, Leasons);
};

void Test3()
{
    bool check;
    string testjson = R"({"TeacherName": "XiaoHong", 
                                              "Count" : 15,
                                              "Leason": { 
                                                    "Type"     : 0,
                                                    "StartTime": "8:00",
                                                    "EndTime"  : "10:00"}})";
    MathLeason person;
    check = JsonHelper::JsonToObject(person, testjson);

    BaseLeason baseLeason;
    check = JsonHelper::JsonToObject(baseLeason, testjson, {"Leason"});

    AllLeason all;
    check = JsonHelper::JsonToObject(all, R"({"Count" : 15,
                                              "Leasons": [{ 
                                                    "Type"     : 0,
                                                    "StartTime": "8:00",
                                                    "EndTime"  : "10:00"},
                                                    { 
                                                    "Type"     : 1,
                                                    "StartTime": "8:00",
                                                    "EndTime"  : "10:00"}
                                                    ]})");

    return;
}

int main()
{
    Test1();
    Test2();
    Test3();
    return 0;
}