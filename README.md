# AIGCJson
AIGCJson is a library with only header files, which can convert classes to json. 

## 📺 Use 
1. Download the folder: include
3. Add line `#include "AIGCJson.hpp"`
4. Add `AIGC_JSON_HELPER(xxx,yyy,zzz)` in the class
   
## 🤖 Example
```cpp
#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class Student
{
public:
    string Name;
    int Age;

    AIGC_JSON_HELPER(Name, Age)
};

int main()
{
    Student person;
    JsonHelper::JsonToObject(person, R"({"Name":"XiaoMing", "Age":15})");

    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    return 0;
}
```
