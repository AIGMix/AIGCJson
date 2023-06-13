#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class Record
{
public:
    int iValue;
    unsigned int uiValue;
    list<int> vValue;
    map<string, int> mapValue;
    AIGC_JSON_HELPER(iValue, uiValue, vValue, mapValue);
};

int main()
{
    Record person;
    JsonHelper::JsonToObject(person, R"({"iValue": 100, 
                                        "uiValue": 15,
                                        "vValue": [1,2,3,4,5],
                                        "mapValue": { "girl":1, "boy":2}})");

    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    return 0;
}