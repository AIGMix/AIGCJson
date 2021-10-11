[ENGLISH](https://github.com/yaronzz/AIGCJson) | [中文文档](https://github.com/yaronzz/AIGCJson/blob/master/README_CN.md)

# AIGCJson
[AIGCJson](https://github.com/yaronzz/AIGCJson) 是一个用于类 | 结构体与Json相互转换的工具，简单易用，支持多种数据类型，支持嵌套关系，只有头文件，方便使用与移植。（基于[Tencent/rapidjson](https://github.com/Tencent/rapidjson)）

## 🍟 功能
1. [x] 支持多种数据类型，包括int\uint、int64\uint64、float、double、bool、string、list、vector、map<string,T>
2. [x] 支持嵌套关系 
3. [x] 两行代码即可完成类 | 结构体与Json的相互转换
4. [x] 支持成员重命名，比方说json中的关键字是name,成员命名可写成Name或其他  
5. [X] 支持设置成员默认值 

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
    int age;
    string jsonStr = R"({"name":"XiaoMing", "age":15})";
    Student person;

    JsonHelper::JsonToObject(person, R"({"name":"XiaoMing", "age":15})");
    //按key获取json中的值或类
    JsonHelper::JsonToObject(age, R"({"name":"XiaoMing", "age":15})", {"age"}); 
    
    jsonStr = "";
    JsonHelper::ObjectToJson(person, jsonStr);
    return 0;
}
```
更多例子可以查看[test](https://github.com/yaronzz/AIGCJson/blob/master/test/)

## 💻 调试与扩展

### **调试**
1. 下载安装[VSCode](https://code.visualstudio.com/)、[MinGW](http://www.mingw.org/)
2. 下载[本项目](https://github.com/yaronzz/AIGCJson)并用VSCode打开
3. 调试选项选择“windows g++”,如果是Linux系统，则选择“linux g++"
4. 打开`test.cpp`,按F5即可调试样例代码

### **扩展**
如果想支持其他类型，在AIGCJson.hpp中添加两条函数接口，模板可以以下对int的支持：

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