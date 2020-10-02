# AIGCJson
AIGCJson 是一个用于类|结构体与Json相互转换的工具，简单易用，支持多种数据类型，支持嵌套关系，只有头文件，方便使用与移植。

## 📺 使用方法
1. 下载文件夹: include
3. 在文件中添加包含 `#include "AIGCJson.hpp"`
4. 在类|结构体中添加成员注册接口 `AIGC_JSON_HELPER(xxx,yyy,zzz)` 
   
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

## 🍟 支持的成员类型
- string
- int\uint
- int64\uint64
- float
- double
- bool
- vector<T>
- list<T>
- map<string,T>
