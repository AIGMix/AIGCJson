#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class Student
{
public:
    string Name;
    int Age;

    AIGC_JSON_HELPER(Name, Age)
    AIGC_JSON_HELPER_RENAME("name", "age")
};

int main()
{
    Student person;
    JsonHelper::JsonToObject(person, R"({"name":"XiaoMing", "age":15})");

    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    return 0;
}