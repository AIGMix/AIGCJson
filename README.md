[ENGLISH](https://github.com/yaronzz/AIGCJson) | [中文文档](https://github.com/yaronzz/AIGCJson/blob/master/README_CN.md)

# AIGCJson
[AIGCJson](https://github.com/yaronzz/AIGCJson) is a tool for converting between classes and Json，which supports multiple data types and nested relationship.Only header file.(Depend on[Tencent/rapidjson](https://github.com/Tencent/rapidjson)）

## 🍟 Support
1. [x] Supports multiple data types, include int\uint、int64\uint64、float、double、bool、string、list、vector、map<string,T>、unordered_map<string,T>
2. [x] Supports nested relationship
3. [x] Only need two lines of code to convert
4. [x] Support rename class-members
5. [x] Support set default value 

## 📺 Use
1. Download folder: include
2. Add include line `#include "AIGCJson.hpp"`
3. Add class-members registered line `AIGC_JSON_HELPER(xxx,yyy,zzz)` 
   
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

    AIGC_JSON_HELPER(Name, Age) //class-members register
    AIGC_JSON_HELPER_RENAME("name","age")//rename class-members
};

int main()
{
    int age;
    string jsonStr = R"({"name":"XiaoMing", "age":15})";
    Student person;

    JsonHelper::JsonToObject(person, R"({"name":"XiaoMing", "age":15})");
    //get base-type or class from json string by keys
    JsonHelper::JsonToObject(age, R"({"name":"XiaoMing", "age":15})", {"age"});
    
    jsonStr = "";
    JsonHelper::ObjectToJson(person, jsonStr);
    return 0;
}
```
more example:[test](https://github.com/yaronzz/AIGCJson/blob/master/test/)

## 💻 Debug and Expand

### **Debug**
1. Downlad and install [VSCode](https://code.visualstudio.com/)、[MinGW](http://www.mingw.org/)
2. Download [this repository](https://github.com/yaronzz/AIGCJson) and open by vscode
3. Select debug option: “windows g++” (“linux g++" if in linux)
4. Open `test.cpp` and press F5

### **Expand**
If you want to support other types, you just need to add two functions to the `AIGCJson.hpp`,int-type example:
```cpp
static bool JsonToObject(int &obj, rapidjson::Value &jsonValue)
{
    if (jsonValue.IsNull() || !jsonValue.IsInt())
        return false;
    obj = jsonValue.GetInt();
    return true;
}
static bool ObjectToJson(int &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
{
    jsonValue.SetInt(obj);
    return true;
}
```