# AIGCJson
AIGCJson is a library with only header files, which can convert classes to json. 

## 📺 Use 
1. Download the folder: include
3. Add line `#include "AIGCJson.hpp"`
4. Add `AIGC_JSON_LOAD_MEMBERS(xxx,yyy,zzz)` in the class
   
## 🤖 Example
```cpp
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
```

<!-- ## ☕ Support
If you really like my projects and want to support me, you can buy me a coffee and star this project. 

<a href="https://www.buymeacoffee.com/yaronzz" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/arial-orange.png" alt="Buy Me A Coffee" style="height: 51px !important;width: 217px !important;" ></a> -->