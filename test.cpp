#include "AIGCJson.hpp"
#include <string>
using namespace std;

class Student
{
public:
    string Name;
    int Age;
    AIGC_JSON_LOAD_MEMBERS(Name, Age)
};

int main()
{
    std::string ojson = "";
    std::string tjson = "{	\"Name\" : \"Boo\",	\"Age\" : 28}";

    Student test;
    test.Name = "huang";
    test.Age = 90;

    test.ConverClassToJson(ojson);
    test.ConverJsonToClass(tjson);
    return 0;
}