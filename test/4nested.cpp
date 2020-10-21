#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

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

int main()
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

    return 0;
}