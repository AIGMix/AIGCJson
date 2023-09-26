/**
 * Copyright (C) 2020 - 2023, Yaronzz(yaronhuang@foxmail.com). All rights reserved.
 * 
 * Licensed under the MIT License (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 * http://opensource.org/licenses/MIT
 * 
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 * 
 * @author: yaronzz
 * @email:  yaronhuang@foxmail.com
 * @github: https://github.com/yaronzz
 * @note:   Support type -->> int、uint、short, ushort, int64、uint64、bool、
 *                            float、double、string、vector、list、set、unordered_set
 *                            map<string,XX>、unordered_map<string,XX>
 * @version:1.0.3
 * 
 */
#pragma once
#include <list>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace aigc
{

/******************************************************
 * Register class or struct members
 * eg:
 * struct Test
 * {
 *      string A;
 *      string B;
 *      AIGC_JSON_HELPER(A, B)
 * };         
 ******************************************************/
#define AIGC_JSON_HELPER(...)                                                            \
    std::map<std::string, std::string> __aigcDefaultValues;                              \
    bool AIGCJsonToObject(aigc::JsonHelperPrivate &handle,                               \
                          rapidjson::Value &jsonValue,                                   \
                          std::vector<std::string> &names)                               \
    {                                                                                    \
        std::vector<std::string> standardNames = handle.GetMembersNames(#__VA_ARGS__);   \
        if (names.size() <= standardNames.size())                                        \
        {                                                                                \
            for (int i = names.size(); i < (int)standardNames.size(); i++)               \
                names.push_back(standardNames[i]);                                       \
        }                                                                                \
        return handle.SetMembers(names, 0, jsonValue, __aigcDefaultValues, __VA_ARGS__); \
    }                                                                                    \
    bool AIGCObjectToJson(aigc::JsonHelperPrivate &handle,                               \
                          rapidjson::Value &jsonValue,                                   \
                          rapidjson::Document::AllocatorType &allocator,                 \
                          std::vector<std::string> &names)                               \
    {                                                                                    \
        std::vector<std::string> standardNames = handle.GetMembersNames(#__VA_ARGS__);   \
        if (names.size() <= standardNames.size())                                        \
        {                                                                                \
            for (int i = names.size(); i < (int)standardNames.size(); i++)               \
                names.push_back(standardNames[i]);                                       \
        }                                                                                \
        return handle.GetMembers(names, 0, jsonValue, allocator, __VA_ARGS__);           \
    }

/******************************************************
 * Rename members
 * eg:
 * struct Test
 * {
 *      string A;
 *      string B;
 *      AIGC_JSON_HELPER(A, B)
 *      AIGC_JSON_HELPER_RENAME("a", "b")
 * };         
 ******************************************************/
#define AIGC_JSON_HELPER_RENAME(...)                                            \
    std::vector<std::string> AIGCRenameMembers(aigc::JsonHelperPrivate &handle) \
    {                                                                           \
        return handle.GetMembersNames(#__VA_ARGS__);                            \
    }

/******************************************************
 * Register base-class
 * eg:
 * struct Base
 * {
 *      string name;
 *      AIGC_JSON_HELPER(name)
 * };  
 * struct Test : Base
 * {
 *      string A;
 *      string B;
 *      AIGC_JSON_HELPER(A, B)
 *      AIGC_JSON_HELPER_BASE((Base*)this)
 * };         
 ******************************************************/
#define AIGC_JSON_HELPER_BASE(...)                                           \
    bool AIGCBaseJsonToObject(aigc::JsonHelperPrivate &handle,               \
                              rapidjson::Value &jsonValue)                   \
    {                                                                        \
        return handle.SetBase(jsonValue, __VA_ARGS__);                       \
    }                                                                        \
    bool AIGCBaseObjectToJson(aigc::JsonHelperPrivate &handle,               \
                              rapidjson::Value &jsonValue,                   \
                              rapidjson::Document::AllocatorType &allocator) \
    {                                                                        \
        return handle.GetBase(jsonValue, allocator, __VA_ARGS__);            \
    }

/******************************************************
 * Set default value
 * eg:
 * struct Base
 * {
 *      string name;
 *      int age;
 *      AIGC_JSON_HELPER(name, age)
 *      AIGC_JSON_HELPER_DEFAULT(age=18)
 * };  
 ******************************************************/
#define AIGC_JSON_HELPER_DEFAULT(...)                                  \
    void AIGCDefaultValues(aigc::JsonHelperPrivate &handle)            \
    {                                                                  \
        __aigcDefaultValues = handle.GetMembersValueMap(#__VA_ARGS__); \
    }


class JsonHelperPrivate
{
public:
    /******************************************************
         *
         * enable_if
         *
         ******************************************************/
    template <bool, class TYPE = void>
    struct enable_if
    {
    };

    template <class TYPE>
    struct enable_if<true, TYPE>
    {
        typedef TYPE type;
    };

public:
    /******************************************************
         * 
         * Check Interface
         *      If class or struct add AIGC_JSON_HELPER\AIGC_JSON_HELPER_RENAME\AIGC_JSON_HELPER_BASE,
         *      it will go to the correct conver function.
         *
         ******************************************************/
    template <typename T>
    struct HasConverFunction
    {
        template <typename TT>
        static char func(decltype(&TT::AIGCJsonToObject));

        template <typename TT>
        static int func(...);

        const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
    };

    template <typename T>
    struct HasRenameFunction
    {
        template <typename TT>
        static char func(decltype(&TT::AIGCRenameMembers));
        template <typename TT>
        static int func(...);
        const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
    };

    template <typename T>
    struct HasBaseConverFunction
    {
        template <typename TT>
        static char func(decltype(&TT::AIGCBaseJsonToObject));
        template <typename TT>
        static int func(...);
        const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
    };

    template <typename T>
    struct HasDefaultValueFunction
    {
        template <typename TT>
        static char func(decltype(&TT::AIGCDefaultValues));
        template <typename TT>
        static int func(...);
        const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
    };

public:
    /******************************************************
         *
         * Interface of JsonToObject\ObjectToJson
         *
         ******************************************************/
    template <typename T, typename enable_if<HasConverFunction<T>::has, int>::type = 0>
    bool JsonToObject(T &obj, rapidjson::Value &jsonValue)
    {
        if (!BaseConverJsonToObject(obj, jsonValue))
            return false;

        LoadDefaultValuesMap(obj);
        std::vector<std::string> names = LoadRenameArray(obj);
        return obj.AIGCJsonToObject(*this, jsonValue, names);
    }

    template <typename T, typename enable_if<!HasConverFunction<T>::has, int>::type = 0>
    bool JsonToObject(T &obj, rapidjson::Value &jsonValue)
    {
        if (std::is_enum<T>::value)
        {
            int ivalue;
            if (!JsonToObject(ivalue, jsonValue))
                return false;

            obj = static_cast<T>(ivalue);
            return true;
        }

        m_message = "unsupported this type.";
        return false;
    }

    template <typename T, typename enable_if<HasConverFunction<T>::has, int>::type = 0>
    bool ObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        if (jsonValue.IsNull())
            jsonValue.SetObject();
        if (!BaseConverObjectToJson(obj, jsonValue, allocator))
            return false;
        std::vector<std::string> names = LoadRenameArray(obj);
        return obj.AIGCObjectToJson(*this, jsonValue, allocator, names);
    }

    template <typename T, typename enable_if<!HasConverFunction<T>::has, int>::type = 0>
    bool ObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        if (std::is_enum<T>::value)
        {
            int ivalue = static_cast<int>(obj);
            return ObjectToJson(ivalue, jsonValue, allocator);
        }

        m_message = "unsupported this type.";
        return false;
    }

    /******************************************************
         *
         * Interface of LoadRenameArray
         *
         ******************************************************/
    template <typename T, typename enable_if<HasRenameFunction<T>::has, int>::type = 0>
    std::vector<std::string> LoadRenameArray(T &obj)
    {
        return obj.AIGCRenameMembers(*this);
    }

    template <typename T, typename enable_if<!HasRenameFunction<T>::has, int>::type = 0>
    std::vector<std::string> LoadRenameArray(T &obj)
    {
        return std::vector<std::string>();
    }

    /******************************************************
         *
         * Interface of BaseConverJsonToObject\BaseConverObjectToJson
         *
         ******************************************************/
    template <typename T, typename enable_if<HasBaseConverFunction<T>::has, int>::type = 0>
    bool BaseConverJsonToObject(T &obj, rapidjson::Value &jsonValue)
    {
        return obj.AIGCBaseJsonToObject(*this, jsonValue);
    }

    template <typename T, typename enable_if<!HasBaseConverFunction<T>::has, int>::type = 0>
    bool BaseConverJsonToObject(T &obj, rapidjson::Value &jsonValue)
    {
        return true;
    }

    template <typename T, typename enable_if<HasBaseConverFunction<T>::has, int>::type = 0>
    bool BaseConverObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        return obj.AIGCBaseObjectToJson(*this, jsonValue, allocator);
    }

    template <typename T, typename enable_if<!HasBaseConverFunction<T>::has, int>::type = 0>
    bool BaseConverObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        return true;
    }

    /******************************************************
         *
         * Interface of Default value
         *
         ******************************************************/
    template <typename T, typename enable_if<HasDefaultValueFunction<T>::has, int>::type = 0>
    void LoadDefaultValuesMap(T &obj)
    {
        obj.AIGCDefaultValues(*this);
    }

    template <typename T, typename enable_if<!HasDefaultValueFunction<T>::has, int>::type = 0>
    void LoadDefaultValuesMap(T &obj)
    {
        (void)obj;
    }

public:
    /******************************************************
        *
        * Tool function
        *
        ******************************************************/
    static std::vector<std::string> StringSplit(const std::string &str, char sep = ',')
    {
        std::vector<std::string> array;
        std::string::size_type pos1, pos2;
        pos1 = 0;
        pos2 = str.find(sep);
        while (std::string::npos != pos2)
        {
            array.push_back(str.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            pos2 = str.find(sep, pos1);
        }
        if (pos1 != str.length())
            array.push_back(str.substr(pos1));

        return array;
    }

    static std::string StringTrim(std::string key)
    {
        std::string newStr = key;
        if (!newStr.empty())
        {
            newStr.erase(0, newStr.find_first_not_of(" "));
            newStr.erase(newStr.find_last_not_of(" ") + 1);
        }
        if (!newStr.empty())
        {
            newStr.erase(0, newStr.find_first_not_of("\""));
            newStr.erase(newStr.find_last_not_of("\"") + 1);
        }
        return newStr;
    }

    static void StringTrim(std::vector<std::string> &array)
    {
        for (int i = 0; i < (int)array.size(); i++)
        {
            array[i] = StringTrim(array[i]);
        }
    }

    /**
         * Get json value type
         */
    static std::string GetJsonValueTypeName(rapidjson::Value &jsonValue)
    {
        switch (jsonValue.GetType())
        {
        case rapidjson::Type::kArrayType:
            return "array";
        case rapidjson::Type::kFalseType:
        case rapidjson::Type::kTrueType:
            return "bool";
        case rapidjson::Type::kObjectType:
            return "object";
        case rapidjson::Type::kStringType:
            return "string";
        case rapidjson::Type::kNumberType:
            return "number";
        default:
            return "string";
        }
    }

    static std::string GetStringFromJsonValue(rapidjson::Value &jsonValue)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

        jsonValue.Accept(writer);
        std::string ret = std::string(buffer.GetString());
        return ret;
    }

    static std::string FindStringFromMap(std::string name, std::map<std::string, std::string> &stringMap)
    {
        std::map<std::string, std::string>::iterator iter = stringMap.find(name);
        if (iter == stringMap.end())
            return "";
        return iter->second;
    }

public:
    /******************************************************
        *
        * Set class/struct members value
        *
        ******************************************************/
    std::vector<std::string> GetMembersNames(const std::string membersStr)
    {
        std::vector<std::string> array = StringSplit(membersStr);
        StringTrim(array);
        return array;
    }

    std::map<std::string, std::string> GetMembersValueMap(const std::string valueStr)
    {
        std::vector<std::string> array = StringSplit(valueStr);
        std::map<std::string, std::string> ret;
        for (int i = 0; i < array.size(); i++)
        {
            std::vector<std::string> keyValue = StringSplit(array[i], '=');
            if (keyValue.size() != 2)
                continue;

            std::string key = StringTrim(keyValue[0]);
            std::string value = StringTrim(keyValue[1]);
            if (ret.find(key) != ret.end())
                continue;
            ret.insert(std::pair<std::string, std::string>(key, value));
        }
        return ret;
    }

    template <typename TYPE, typename... TYPES>
    bool SetMembers(const std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, std::map<std::string, std::string> defaultValues, TYPE &arg, TYPES &...args)
    {
        if (!SetMembers(names, index, jsonValue, defaultValues, arg))
            return false;

        return SetMembers(names, ++index, jsonValue, defaultValues, args...);
    }

    template <typename TYPE>
    bool SetMembers(const std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, std::map<std::string, std::string> defaultValues, TYPE &arg)
    {
        if (jsonValue.IsNull())
            return true;

        const char *key = names[index].c_str();
        if (!jsonValue.IsObject())
            return false;
        if (!jsonValue.HasMember(key))
        {
            std::string defaultV = FindStringFromMap(names[index], defaultValues);
            if (!defaultV.empty())
                StringToObject(arg, defaultV);
            return true;
        }

        if (!JsonToObject(arg, jsonValue[key]))
        {
            m_message = "[" + names[index] + "] " + m_message;
            return false;
        }
        return true;
    }

    /******************************************************
        *
        * Get class/struct members value
        *
        ******************************************************/
    template <typename TYPE, typename... TYPES>
    bool GetMembers(const std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE &arg, TYPES &...args)
    {
        if (!GetMembers(names, index, jsonValue, allocator, arg))
            return false;
        return GetMembers(names, ++index, jsonValue, allocator, args...);
    }

    template <typename TYPE>
    bool GetMembers(const std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE &arg)
    {
        rapidjson::Value item;
        bool check = ObjectToJson(arg, item, allocator);
        if (!check)
        {
            m_message = "[" + names[index] + "] " + m_message;
            return false;
        }

        if (jsonValue.HasMember(names[index].c_str()))
        {
            jsonValue.RemoveMember(names[index].c_str());
        }

        rapidjson::Value key;
        key.SetString(names[index].c_str(), names[index].length(), allocator);
        jsonValue.AddMember(key, item, allocator);
        return true;
    }

public:
    /******************************************************
        *
        * Set base class value
        *
        ******************************************************/
    template <typename TYPE, typename... TYPES>
    bool SetBase(rapidjson::Value &jsonValue, TYPE *arg, TYPES *...args)
    {
        if (!SetBase(jsonValue, arg))
            return false;
        return SetBase(jsonValue, args...);
    }

    template <typename TYPE>
    bool SetBase(rapidjson::Value &jsonValue, TYPE *arg)
    {
        return JsonToObject(*arg, jsonValue);
    }

    /******************************************************
        *
        * Get base class value
        *
        ******************************************************/
    template <typename TYPE, typename... TYPES>
    bool GetBase(rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE *arg, TYPES *...args)
    {
        if (!GetBase(jsonValue, allocator, arg))
            return false;
        return GetBase(jsonValue, allocator, args...);
    }

    template <typename TYPE>
    bool GetBase(rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE *arg)
    {
        return ObjectToJson(*arg, jsonValue, allocator);
    }

public:
    /******************************************************
         * Conver base-type : string to base-type
         * Contain: int\uint、int64_t\uint64_t、bool、float
         *          double、string
         *
         ******************************************************/
    template <typename TYPE>
    void StringToObject(TYPE &obj, std::string &value)
    {
        return;
    }

    void StringToObject(std::string &obj, std::string &value)
    {
        obj = value;
    }

    void StringToObject(int &obj, std::string &value)
    {
        obj = atoi(value.c_str());
    }

    void StringToObject(unsigned int &obj, std::string &value)
    {
        char *end;
        obj = strtoul(value.c_str(), &end, 10);
    }

    void StringToObject(int64_t &obj, std::string &value)
    {
        char *end;
        obj = strtoll(value.c_str(), &end, 10);
    }

    void StringToObject(uint64_t &obj, std::string &value)
    {
        char *end;
        obj = strtoull(value.c_str(), &end, 10);
    }

    void StringToObject(bool &obj, std::string &value)
    {
        obj = (value == "true");
    }

    void StringToObject(float &obj, std::string &value)
    {
        obj = atof(value.c_str());
    }

    void StringToObject(double &obj, std::string &value)
    {
        obj = atof(value.c_str());
    }

public:
    /******************************************************
         * Conver base-type : Json string to base-type
         * Contain: int\uint、int64_t\uint64_t、bool、float、set、unordered_set
         *          double、string、vector、list、map<string,XX>、unordered_map<string,XX>
         *
         ******************************************************/
    bool JsonToObject(int &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsInt())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is int.";
            return false;
        }
        obj = jsonValue.GetInt();
        return true;
    }

    bool JsonToObject(unsigned int &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsUint())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is unsigned int.";
            return false;
        }
        obj = jsonValue.GetUint();
        return true;
    }

    bool JsonToObject(short &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsInt())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is short.";
            return false;
        }
        obj = jsonValue.GetInt();
        return true;
    }

    bool JsonToObject(unsigned short &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsUint())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is unsigned short.";
            return false;
        }
        obj = jsonValue.GetUint();
        return true;
    }

    bool JsonToObject(int64_t &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsInt64())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is int64_t.";
            return false;
        }
        obj = jsonValue.GetInt64();
        return true;
    }

    bool JsonToObject(uint64_t &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsUint64())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is uint64_t.";
            return false;
        }
        obj = jsonValue.GetUint64();
        return true;
    }

    bool JsonToObject(bool &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsBool())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is bool.";
            return false;
        }
        obj = jsonValue.GetBool();
        return true;
    }

    bool JsonToObject(float &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsNumber())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is float.";
            return false;
        }
        obj = jsonValue.GetFloat();
        return true;
    }

    bool JsonToObject(double &obj, rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsNumber())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is double.";
            return false;
        }
        obj = jsonValue.GetDouble();
        return true;
    }

    bool JsonToObject(std::string &obj, rapidjson::Value &jsonValue)
    {
        obj = "";
        if (jsonValue.IsNull())
            return true;
        //object or number conver to string
        else if (jsonValue.IsObject() || jsonValue.IsNumber())
            obj = GetStringFromJsonValue(jsonValue);
        else if (!jsonValue.IsString())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is string.";
            return false;
        }
        else
            obj = jsonValue.GetString();

        return true;
    }

    template <typename TYPE>
    bool JsonToObject(std::vector<TYPE> &obj, rapidjson::Value &jsonValue)
    {
        obj.clear();
        if (!jsonValue.IsArray())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is std::vector<TYPE>.";
            return false;
        }

        auto array = jsonValue.GetArray();
        for (int i = 0; i < array.Size(); i++)
        {
            TYPE item;
            if (!JsonToObject(item, array[i]))
                return false;
            obj.push_back(item);
        }
        return true;
    }

    template <typename TYPE>
    bool JsonToObject(std::list<TYPE> &obj, rapidjson::Value &jsonValue)
    {
        obj.clear();
        if (!jsonValue.IsArray())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is std::list<TYPE>.";
            return false;
        }

        auto array = jsonValue.GetArray();
        for (int i = 0; i < array.Size(); i++)
        {
            TYPE item;
            if (!JsonToObject(item, array[i]))
                return false;
            obj.push_back(item);
        }
        return true;
    }

    template <typename TYPE>
    bool JsonToObject(std::set<TYPE> &obj, rapidjson::Value &jsonValue)
    {
        obj.clear();
        if (!jsonValue.IsArray())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is std::set<TYPE>.";
            return false;
        }

        auto array = jsonValue.GetArray();
        for (int i = 0; i < array.Size(); i++)
        {
            TYPE item;
            if (!JsonToObject(item, array[i]))
                return false;
            obj.insert(item);
        }
        return true;
    }

    template <typename TYPE>
    bool JsonToObject(std::unordered_set<TYPE> &obj, rapidjson::Value &jsonValue)
    {
        obj.clear();
        if (!jsonValue.IsArray())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is std::unordered_set<TYPE>.";
            return false;
        }
        
        auto array = jsonValue.GetArray();
        for (int i = 0; i < array.Size(); i++)
        {
            TYPE item;
            if (!JsonToObject(item, array[i]))
                return false;
            obj.insert(item);
        }
        return true;
    }

    template <typename TYPE>
    bool JsonToObject(std::map<std::string, TYPE> &obj, rapidjson::Value &jsonValue)
    {
        obj.clear();
        if (!jsonValue.IsObject())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is std::map<std::string, TYPE>.";
            return false;
        }

        for (auto iter = jsonValue.MemberBegin(); iter != jsonValue.MemberEnd(); ++iter)
        {
            auto key = (iter->name).GetString();
            auto &value = jsonValue[key];

            TYPE item;
            if (!JsonToObject(item, value))
                return false;

            obj.insert(std::pair<std::string, TYPE>(key, item));
        }
        return true;
    }

    template <typename TYPE>
    bool JsonToObject(std::unordered_map<std::string, TYPE> &obj, rapidjson::Value &jsonValue)
    {
        obj.clear();
        if (!jsonValue.IsObject())
        {
            m_message = "json-value is " + GetJsonValueTypeName(jsonValue) + " but object is std::unordered_map<std::string, TYPE>.";
            return false;
        }

        for (auto iter = jsonValue.MemberBegin(); iter != jsonValue.MemberEnd(); ++iter)
        {
            auto key = (iter->name).GetString();
            auto &value = jsonValue[key];

            TYPE item;
            if (!JsonToObject(item, value))
                return false;

            obj.insert(std::pair<std::string, TYPE>(key, item));
        }
        return true;
    }

public:
    /******************************************************
         * Conver base-type : base-type to json string
         * Contain: int\uint、int64_t\uint64_t、bool、float、set、unordered_set
         *          double、string、vector、list、map<string,XX>、unordered_map<string,XX>
         *
         ******************************************************/
    bool ObjectToJson(int &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetInt(obj);
        return true;
    }

    bool ObjectToJson(unsigned int &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetUint(obj);
        return true;
    }

    bool ObjectToJson(short &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetInt((int)obj);
        return true;
    }

    bool ObjectToJson(unsigned short &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetUint((unsigned int)obj);
        return true;
    }

    bool ObjectToJson(int64_t &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetInt64(obj);
        return true;
    }

    bool ObjectToJson(uint64_t &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetUint64(obj);
        return true;
    }

    bool ObjectToJson(bool &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetBool(obj);
        return true;
    }

    bool ObjectToJson(float &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetFloat(obj);
        return true;
    }

    bool ObjectToJson(double &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetDouble(obj);
        return true;
    }

    bool ObjectToJson(std::string &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetString(obj.c_str(), obj.length(), allocator);
        return true;
    }

    template <typename TYPE>
    bool ObjectToJson(std::vector<TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        rapidjson::Value array(rapidjson::Type::kArrayType);
        for (int i = 0; i < obj.size(); i++)
        {
            rapidjson::Value item;
            if (!ObjectToJson(obj[i], item, allocator))
                return false;

            array.PushBack(item, allocator);
        }

        jsonValue = array;
        return true;
    }

    template <typename TYPE>
    bool ObjectToJson(std::list<TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        rapidjson::Value array(rapidjson::Type::kArrayType);
        for (auto i = obj.begin(); i != obj.end(); i++)
        {
            rapidjson::Value item;
            if (!ObjectToJson(*i, item, allocator))
                return false;

            array.PushBack(item, allocator);
        }

        jsonValue = array;
        return true;
    }

    template <typename TYPE>
    bool ObjectToJson(std::set<TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        rapidjson::Value array(rapidjson::Type::kArrayType);
        for (auto i = obj.begin(); i != obj.end(); i++)
        {
            rapidjson::Value item;
            TYPE value = *i;
            if (!ObjectToJson(value, item, allocator))
                return false;

            array.PushBack(item, allocator);
        }

        jsonValue = array;
        return true;
    }
    
    template <typename TYPE>
    bool ObjectToJson(std::unordered_set<TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        rapidjson::Value array(rapidjson::Type::kArrayType);
        for (auto i = obj.begin(); i != obj.end(); i++)
        {
            rapidjson::Value item;
            TYPE value = *i;
            if (!ObjectToJson(value, item, allocator))
                return false;

            array.PushBack(item, allocator);
        }

        jsonValue = array;
        return true;
    }

    template <typename TYPE>
    bool ObjectToJson(std::map<std::string, TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetObject();
        for (auto iter = obj.begin(); iter != obj.end(); ++iter)
        {
            auto key = iter->first;
            TYPE value = iter->second;

            rapidjson::Value jsonitem;
            if (!ObjectToJson(value, jsonitem, allocator))
                return false;

            rapidjson::Value jsonkey;
            jsonkey.SetString(key.c_str(), key.length(), allocator);

            jsonValue.AddMember(jsonkey, jsonitem, allocator);
        }
        return true;
    }

    template <typename TYPE>
    bool ObjectToJson(std::unordered_map<std::string, TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
    {
        jsonValue.SetObject();
        for (auto iter = obj.begin(); iter != obj.end(); ++iter)
        {
            auto key = iter->first;
            TYPE value = iter->second;

            rapidjson::Value jsonitem;
            if (!ObjectToJson(value, jsonitem, allocator))
                return false;

            rapidjson::Value jsonkey;
            jsonkey.SetString(key.c_str(), key.length(), allocator);

            jsonValue.AddMember(jsonkey, jsonitem, allocator);
        }
        return true;
    }

public:
    std::string m_message;
};

class JsonHelper
{
public:
    /**
         * @brief conver json string to class | struct
         * @param obj : class or struct
         * @param jsonStr : json string 
         * @param keys : the path of the object
         * @param message : printf err message when conver failed
         */
    template <typename T>
    static bool JsonToObject(T &obj, const std::string &jsonStr, const std::vector<std::string> keys = {}, std::string *message = NULL)
    {
        //Parse json string
        rapidjson::Document root;
        root.Parse(jsonStr.c_str());
        if (root.IsNull())
        {
            if (message)
                *message = "Json string can't parse.";
            return false;
        }

        //Go to the key-path
        std::string path;
        rapidjson::Value &value = root;
        for (int i = 0; i < (int)keys.size(); i++)
        {
            const char *find = keys[i].c_str();
            if (!path.empty())
                path += "->";
            path += keys[i];

            if (!value.IsObject() || !value.HasMember(find))
            {
                if (message)
                    *message = "Can't parse the path [" + path + "].";
                return false;
            }
            value = value[find];
        }

        //Conver
        JsonHelperPrivate handle;
        if (!handle.JsonToObject(obj, value))
        {
            if (message)
                *message = handle.m_message;
            return false;
        }
        return true;
    }

    /**
         * @brief conver json string to class | struct
         * @param jsonStr : json string
         * @param defaultT : default value
         * @param keys : the path of the object
         * @param message : printf err message when conver failed
         */
    template <typename T>
    static T Get(const std::string &jsonStr, T defaultT, const std::vector<std::string> keys = {}, std::string *message = NULL)
    {
        T obj;
        if (JsonToObject(obj, jsonStr, keys, message))
            return obj;

        return defaultT;
    }

    /**
         * @brief conver class | struct to json string 
         * @param errMessage : printf err message when conver failed
         * @param obj : class or struct
         * @param jsonStr : json string 
         */
    template <typename T>
    static bool ObjectToJson(T &obj, std::string &jsonStr, std::string *message = NULL)
    {
        rapidjson::Document root;
        root.SetObject();
        rapidjson::Document::AllocatorType &allocator = root.GetAllocator();

        //Conver
        JsonHelperPrivate handle;
        if (!handle.ObjectToJson(obj, root, allocator))
        {
            if (message)
                *message = handle.m_message;
            return false;
        }

        jsonStr = handle.GetStringFromJsonValue(root);
        return true;
    }
};

} // namespace aigc