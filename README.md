# AIGCJson
AIGCJson 是一个用于类 | 结构体与Json相互转换的工具，简单易用，支持多种数据类型，支持嵌套关系，只有头文件，方便使用与移植。（基于[Tencent/rapidjson](https://github.com/Tencent/rapidjson)）

## 🍟 功能
1. [x] 支持多种数据类型，包括int\uint、int64\uint64、float、double、bool、string、list、vector、map<string,T>
2. [x] 支持嵌套关系 
3. [x] 两行代码即可完成类 | 结构体与Json的相互转换
4. [x] 支持成员重命名，比方说json中的关键字是name,成员命名可写成Name或其他  

## 📺 使用方法
1. 下载文件夹: include
3. 在文件中添加包含 `#include "AIGCJson.hpp"`
4. 在类 | 结构体中添加成员注册接口 `AIGC_JSON_HELPER(xxx,yyy,zzz)` 
   
## 🤖 样例代码
```cpp
#include "AIGCJson.hpp"
using namespace std;
using namespace aigc;

class Student
{
public:
    string Name;
    int Age;

    AIGC_JSON_HELPER(Name, Age)//成员注册
    AIGC_JSON_HELPER_RENAME("name","age")//成员重命名，不需要可以删除这条
};

int main()
{
    Student person;
    JsonHelper::JsonToObject(person, R"({"name":"XiaoMing", "age":15})");

    string jsonStr;
    JsonHelper::ObjectToJson(person, jsonStr);
    return 0;
}
```
更多例子可以查看[test.cpp](https://github.com/yaronzz/AIGCJson/blob/master/test.cpp)

