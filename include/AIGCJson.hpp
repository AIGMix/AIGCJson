/**
 * Copyright (C) 2020 THL A29 Limited, Yaronzz(yaronhuang@foxmail.com). All rights reserved.
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
 * @note:   Support type -->> int、uint、int64、uint64、bool、float、double、string、vector、list、map<string,XX>
 * 
 */

#include <list>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace aigc
{
#define AIGC_JSON_HELPER(...)                                                                                                                    \
    bool AIGC_CONVER_JSON_TO_OBJECT(rapidjson::Value &jsonValue, std::vector<std::string> &names)                                                \
    {                                                                                                                                            \
        std::vector<std::string> standardNames = aigc::JsonHelper::GetMembersNames(#__VA_ARGS__);                                                \
        if (names.size() <= standardNames.size())                                                                                                \
        {                                                                                                                                        \
            for (int i = names.size(); i < standardNames.size(); i++)                                                                            \
                names.push_back(standardNames[i]);                                                                                               \
        }                                                                                                                                        \
        return aigc::JsonHelper::WriteMembers(names, 0, jsonValue, __VA_ARGS__);                                                                 \
    }                                                                                                                                            \
    bool AIGC_CONVER_OBJECT_TO_JSON(rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, std::vector<std::string> &names) \
    {                                                                                                                                            \
        if (names.size() <= 0)                                                                                                                   \
            names = aigc::JsonHelper::GetMembersNames(#__VA_ARGS__);                                                                             \
        return aigc::JsonHelper::ReadMembers(names, 0, jsonValue, allocator, __VA_ARGS__);                                                       \
    }

#define AIGC_JSON_HELPER_RENAME(...)                            \
    std::vector<std::string> AIGC_MEMBERS_RENAME()              \
    {                                                           \
        return aigc::JsonHelper::GetMembersNames(#__VA_ARGS__); \
    }

#define AIGC_JSON_HELPER_BASE(...)                                                                                   \
    bool AIGC_BASE_CONVER_JSON_TO_OBJECT(rapidjson::Value &jsonValue)                                                \
    {                                                                                                                \
        rapidjson::StringBuffer buffer;                                                                              \
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);                                                   \
        jsonValue.Accept(writer);                                                                                    \
        std::string jsonStr = std::string(buffer.GetString());                                                       \
        return aigc::JsonHelper::WriteBase(jsonStr, __VA_ARGS__);                                                    \
    }                                                                                                                \
    bool AIGC_BASE_CONVER_OBJECT_TO_JSON(rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator) \
    {                                                                                                                \
        return aigc::JsonHelper::ReadBase(jsonValue, allocator, __VA_ARGS__);                                        \
    }

    class JsonHelper
    {
    private:
        /**
         * Conver Json To Obj. Support Type: 
         *          int、uint、bool、float、double、string、vector、list
         */
        static bool JsonToObject(int &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsInt())
                return false;
            obj = jsonValue.GetInt();
            return true;
        }

        static bool JsonToObject(unsigned int &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsUint())
                return false;
            obj = jsonValue.GetUint();
            return true;
        }

        static bool JsonToObject(int64_t &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsInt64())
                return false;
            obj = jsonValue.GetInt64();
            return true;
        }

        static bool JsonToObject(uint64_t &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsUint64())
                return false;
            obj = jsonValue.GetUint64();
            return true;
        }

        static bool JsonToObject(bool &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsBool())
                return false;
            obj = jsonValue.GetBool();
            return true;
        }

        static bool JsonToObject(float &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsNumber())
                return false;
            obj = jsonValue.GetFloat();
            return true;
        }

        static bool JsonToObject(double &obj, rapidjson::Value &jsonValue)
        {
            if (jsonValue.IsNull() || !jsonValue.IsNumber())
                return false;
            obj = jsonValue.GetDouble();
            return true;
        }

        static bool JsonToObject(std::string &obj, rapidjson::Value &jsonValue)
        {
            obj = "";
            if (jsonValue.IsNull())
                return true;
            else if (jsonValue.IsObject() || jsonValue.IsNumber())
            {
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                jsonValue.Accept(writer);
                obj = std::string(buffer.GetString());
            }
            else if (!jsonValue.IsString())
                return false;
            else
                obj = jsonValue.GetString();
            return true;
        }

        template <typename TYPE>
        static bool JsonToObject(std::vector<TYPE> &obj, rapidjson::Value &jsonValue)
        {
            obj.clear();
            if (jsonValue.IsNull() || !jsonValue.IsArray())
                return false;

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
        static bool JsonToObject(std::list<TYPE> &obj, rapidjson::Value &jsonValue)
        {
            obj.clear();
            if (jsonValue.IsNull() || !jsonValue.IsArray())
                return false;

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
        static bool JsonToObject(std::map<std::string, TYPE> &obj, rapidjson::Value &jsonValue)
        {
            obj.clear();
            if (jsonValue.IsNull() || !jsonValue.IsObject())
                return false;

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

    private:
        /**
         * Conver Obj To String. 
         */
        static bool ObjectToJson(int &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetInt(obj);
            return true;
        }

        static bool ObjectToJson(unsigned int &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetUint(obj);
            return true;
        }

        static bool ObjectToJson(int64_t &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetInt64(obj);
            return true;
        }

        static bool ObjectToJson(uint64_t &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetUint64(obj);
            return true;
        }

        static bool ObjectToJson(bool &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetBool(obj);
            return true;
        }

        static bool ObjectToJson(float &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetFloat(obj);
            return true;
        }

        static bool ObjectToJson(double &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetDouble(obj);
            return true;
        }

        static bool ObjectToJson(std::string &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            jsonValue.SetString(obj.c_str(), obj.length(), allocator);
            return true;
        }

        template <typename TYPE>
        static bool ObjectToJson(std::vector<TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
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
        static bool ObjectToJson(std::list<TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
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
        static bool ObjectToJson(std::map<std::string, TYPE> &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
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

    private:
        /**
         * Check : Class Support Conver Json To Obj ?  
         */
        template <bool, class TYPE = void>
        struct enable_if
        {
        };

        template <class TYPE>
        struct enable_if<true, TYPE>
        {
            typedef TYPE type;
        };

    private:
        // AIGC_JSON_HELPER
        template <typename T>
        struct HasConverFunction
        {
            template <typename TT>
            static char func(decltype(&TT::AIGC_CONVER_JSON_TO_OBJECT));
            template <typename TT>
            static int func(...);
            const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
        };

        template <typename T, typename enable_if<HasConverFunction<T>::has, int>::type = 0>
        static inline bool JsonToObject(T &obj, rapidjson::Value &jsonValue)
        {
            if (!BaseConverJsonToObject(obj, jsonValue))
                return false;
            std::vector<std::string> names = LoadRenameArray(obj);
            return obj.AIGC_CONVER_JSON_TO_OBJECT(jsonValue, names);
        }

        template <typename T, typename enable_if<!HasConverFunction<T>::has, int>::type = 0>
        static inline bool JsonToObject(T &obj, rapidjson::Value &jsonValue)
        {
            return false;
        }

        template <typename T, typename enable_if<HasConverFunction<T>::has, int>::type = 0>
        static inline bool ObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            if (jsonValue.IsNull())
                jsonValue.SetObject();
            if (!BaseConverObjectToJson(obj, jsonValue, allocator))
                return false;
            std::vector<std::string> names = LoadRenameArray(obj);
            return obj.AIGC_CONVER_OBJECT_TO_JSON(jsonValue, allocator, names);
        }

        template <typename T, typename enable_if<!HasConverFunction<T>::has, int>::type = 0>
        static inline bool ObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            return false;
        }

    private:
        // AIGC_JSON_HELPER_RENAME
        template <typename T>
        struct HasRenameFunction
        {
            template <typename TT>
            static char func(decltype(&TT::AIGC_MEMBERS_RENAME));
            template <typename TT>
            static int func(...);
            const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
        };

        template <typename T, typename enable_if<HasRenameFunction<T>::has, int>::type = 0>
        static inline std::vector<std::string> LoadRenameArray(T &obj)
        {
            return obj.AIGC_MEMBERS_RENAME();
        }

        template <typename T, typename enable_if<!HasRenameFunction<T>::has, int>::type = 0>
        static inline std::vector<std::string> LoadRenameArray(T &obj)
        {
            return std::vector<std::string>();
        }

    private:
        // AIGC_JSON_HELPER_BASE
        template <typename T>
        struct HasBaseConverFunction
        {
            template <typename TT>
            static char func(decltype(&TT::AIGC_BASE_CONVER_JSON_TO_OBJECT));
            template <typename TT>
            static int func(...);
            const static bool has = (sizeof(func<T>(NULL)) == sizeof(char));
        };

        template <typename T, typename enable_if<HasBaseConverFunction<T>::has, int>::type = 0>
        static inline bool BaseConverJsonToObject(T &obj, rapidjson::Value &jsonValue)
        {
            return obj.AIGC_BASE_CONVER_JSON_TO_OBJECT(jsonValue);
        }

        template <typename T, typename enable_if<!HasBaseConverFunction<T>::has, int>::type = 0>
        static inline bool BaseConverJsonToObject(T &obj, rapidjson::Value &jsonValue)
        {
            return true;
        }

        template <typename T, typename enable_if<HasBaseConverFunction<T>::has, int>::type = 0>
        static inline bool BaseConverObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            return obj.AIGC_BASE_CONVER_OBJECT_TO_JSON(jsonValue, allocator);
        }

        template <typename T, typename enable_if<!HasBaseConverFunction<T>::has, int>::type = 0>
        static inline bool BaseConverObjectToJson(T &obj, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
        {
            return true;
        }

    private:
        /**
         * Tool
         */
        static std::vector<std::string> StringSplit(const std::string &str)
        {
            std::vector<std::string> array;
            std::string::size_type pos1, pos2;
            pos1 = 0;
            pos2 = str.find(',');
            while (std::string::npos != pos2)
            {
                array.push_back(str.substr(pos1, pos2 - pos1));
                pos1 = pos2 + 1;
                pos2 = str.find(',', pos1);
            }
            if (pos1 != str.length())
                array.push_back(str.substr(pos1));

            return array;
        }

        static void StringTrim(std::vector<std::string> &array)
        {
            for (int i = 0; i < array.size(); i++)
            {
                std::string newStr = array[i];
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
                array[i] = newStr;
            }
        }

    public:
        template <typename T>
        static T GetObject(const std::string &jsonStr, bool *isSuccess = NULL)
        {
            T obj;
            bool check = JsonToObject(obj, jsonStr, {});
            if (isSuccess)
                *isSuccess = check;
            return obj;
        }

        template <typename T>
        static T GetObject(const std::string &jsonStr, std::vector<std::string> keys, bool *isSuccess = NULL)
        {
            T obj;
            bool check = JsonToObject(obj, jsonStr, keys);
            if (isSuccess)
                *isSuccess = check;
            return obj;
        }

        /**
         * @brief conver json string to class | struct
         * @param obj : class or struct or base-types
         * @param jsonStr : json string 
         * @param keys : obj-item keys
         */
        template <typename T>
        static inline bool JsonToObject(T &obj, const std::string &jsonStr, std::vector<std::string> keys = {})
        {
            rapidjson::Document root;
            root.Parse(jsonStr.c_str());
            if (root.IsNull())
                return false;

            rapidjson::Value value;
            for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); ++it)
            {
                const char *find = (*it).c_str();
                if (value.IsNull())
                {
                    if (!root.HasMember(find))
                        return false;
                    value = root[find];
                }
                else if (!value.IsObject() || !value.HasMember(find))
                    return false;
                else
                    value = value[find];
            }

            if (value.IsNull())
                return JsonToObject(obj, root);
            else
                return JsonToObject(obj, value);
        }

        template <typename T>
        static std::string GetJson(T &obj, bool *isSuccess)
        {
            std::string ret = "";
            bool check = ObjectToJson(obj, ret);
            if (isSuccess)
                *isSuccess = check;
            return ret;
        }

        /**
         * @brief conver class | struct to json string
         * @param obj : class or struct
         * @param jsonStr : json string 
         */
        template <typename T>
        static inline bool ObjectToJson(T &obj, std::string &jsonStr)
        {
            rapidjson::Document root;
            root.SetObject();
            rapidjson::Document::AllocatorType &allocator = root.GetAllocator();

            if (!ObjectToJson(obj, root, allocator))
                return false;

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            root.Accept(writer);
            jsonStr = std::string(buffer.GetString());
            return true;
        }

    public:
        static std::vector<std::string> GetMembersNames(const std::string membersStr)
        {
            std::vector<std::string> array = StringSplit(membersStr);
            StringTrim(array);
            return array;
        }

        template <typename TYPE, typename... TYPES>
        static bool WriteMembers(std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, TYPE &arg, TYPES &... args)
        {
            if (!WriteMembers(names, index, jsonValue, arg))
                return false;
            return WriteMembers(names, ++index, jsonValue, args...);
        }

        template <typename TYPE>
        static bool WriteMembers(std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, TYPE &arg)
        {
            const char *key = names[index].c_str();
            if (!jsonValue.IsObject())
                return false;
            if (!jsonValue.HasMember(key))
                return true;

            if (!JsonToObject(arg, jsonValue[key]))
                return false;
            return true;
        }

        template <typename TYPE, typename... TYPES>
        static bool ReadMembers(std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE &arg, TYPES &... args)
        {
            if (!ReadMembers(names, index, jsonValue, allocator, arg))
                return false;
            return ReadMembers(names, ++index, jsonValue, allocator, args...);
        }

        template <typename TYPE>
        static bool ReadMembers(std::vector<std::string> &names, int index, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE &arg)
        {
            rapidjson::Value item;
            if (!ObjectToJson(arg, item, allocator))
                return false;

            if (jsonValue.HasMember(names[index].c_str()))
            {
                jsonValue.RemoveMember(names[index].c_str());
            }

            rapidjson::Value key;
            key.SetString(names[index].c_str(), names[index].length(), allocator);
            jsonValue.AddMember(key, item, allocator);
            return true;
        }

        template <typename TYPE, typename... TYPES>
        static bool WriteBase(std::string &sjson, TYPE *arg, TYPES *... args)
        {
            if (!WriteBase(sjson, arg))
                return false;
            return WriteBase(sjson, args...);
        }

        template <typename TYPE>
        static bool WriteBase(std::string &sjson, TYPE *arg)
        {
            return aigc::JsonHelper::JsonToObject(*arg, sjson);
        }

        template <typename TYPE, typename... TYPES>
        static bool ReadBase(rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE *arg, TYPES *... args)
        {
            if (!ReadBase(jsonValue, allocator, arg))
                return false;
            return ReadBase(jsonValue, allocator, args...);
        }

        template <typename TYPE>
        static bool ReadBase(rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator, TYPE *arg)
        {
            return aigc::JsonHelper::ObjectToJson(*arg, jsonValue, allocator);
        }
    };
} // namespace aigc
