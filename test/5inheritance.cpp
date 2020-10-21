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

class EnglishLeason : BaseLeason
{
public:
    string TeacherName;
    int Count;

    AIGC_JSON_HELPER(TeacherName, Count, Type)
    AIGC_JSON_HELPER_BASE((BaseLeason *)this)
};

int main()
{
    bool check;
    string testjson = R"({"TeacherName": "XiaoHong", 
                            "Count" : 15,
                            "Type"     : 1,
                            "StartTime": "8:00",
                            "EndTime"  : "10:00"})";
    EnglishLeason leason;
    check = JsonHelper::JsonToObject(leason, testjson);

    string newjson;
    check = JsonHelper::ObjectToJson(leason, newjson);
    return 0;
}