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
    vector<int> vValue;

    AIGC_JSON_HELPER(iValue, uiValue, vValue);
};

void Test2()
{
    bool check;
    Record person;
    JsonHelper::JsonToObject(person, R"({"iValue": 100, 
                                        "uiValue": 15,
                                        "vValue": [1,2,3,4,5]})");

    int size = person.vValue.size();
    for (int i = 0; i < size; i++)
    {
        cout << person.vValue[i] << endl;
    }

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

void Test3()
{
    bool check;
    MathLeason person;
    JsonHelper::JsonToObject(person, R"({"TeacherName": "XiaoHong", 
                                              "Count" : 15,
                                              "Leason": { 
                                                    "Type"     : 0,
                                                    "StartTime": "8:00",
                                                    "EndTime"  : "10:00"}})");

    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    cout<< jsonStr <<endl;
    return;
}

int main()
{
    Test1();
    Test2();
    Test3();
    return 0;
}