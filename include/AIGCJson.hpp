// Copyright (C) 2020 THL A29 Limited, Yaronzz(yaronhuang@foxmail.com). All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// author: yaronzz
// email:  yaronhuang@foxmail.com
// gituhb: https://github.com/yaronzz
//

#pragma once

#include <vector>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define AIGC_JSON_STRING_LIST(...) #__VA_ARGS__
template <typename T> using stringlist = std::initializer_list<T>;

#define AIGC_JSON_LOAD_MEMBERS(...)                                 \
private:                                                            \
    AIGCJson m_aigcjson;                                            \
                                                                    \
public:                                                             \
    bool ConverJsonToClass(std::string sjon)                        \
    {                                                               \
        if (!m_aigcjson.LoadJsonString(sjon))                       \
            return false;                                           \
        m_aigcjson.LoadMembers(AIGC_JSON_STRING_LIST(__VA_ARGS__)); \
        return m_aigcjson.JsonToClass(0, __VA_ARGS__);              \
    }                                                               \
    bool ConverClassToJson(std::string &sjson)                      \
    {                                                               \
        m_aigcjson.LoadMembers(AIGC_JSON_STRING_LIST(__VA_ARGS__)); \
        if (!m_aigcjson.ClassToJson(0, __VA_ARGS__))                \
            return false;                                           \
        return m_aigcjson.ExportJsonString(sjson);                  \
    }

class AIGCJson
{
public:
    bool LoadJsonString(const std::string& jsonStr);
    bool ExportJsonString(std::string& jsonStr);
    void LoadMembers(std::string membersStr);

    template <typename TYPE, typename... TYPES>
    bool JsonToClass(int index, TYPE &arg, TYPES &... args);
    template <typename TYPE>
    bool JsonToClass(int index, TYPE &arg);

    template <typename TYPE, typename... TYPES>
    bool ClassToJson(int index, TYPE &arg, TYPES &... args);
    template <typename TYPE>
    bool ClassToJson(int index, TYPE &arg);

private:
    rapidjson::Document m_doc;
    std::vector<std::string> m_members;

    bool GetJsonValue(const stringlist<const std::string> &names, rapidjson::Value &value);

    bool SetMemberValue(int &person, rapidjson::Value &value);
    bool SetMemberValue(bool &person, rapidjson::Value &value);
    bool SetMemberValue(double &person, rapidjson::Value &value);
    bool SetMemberValue(std::string &person, rapidjson::Value &value);
    bool SetMemberValue(float &person, rapidjson::Value &value);
    bool SetMemberValue(unsigned int &person, rapidjson::Value &value);
    bool SetMemberValue(uint64_t &person, rapidjson::Value &value);
    bool SetMemberValue(int64_t &person, rapidjson::Value &value);

    bool SetJsonItem(std::string name, int &person);
    bool SetJsonItem(std::string name, std::string &person);
};





bool AIGCJson::LoadJsonString(const std::string& jsonStr)
{
    if (jsonStr != "")
        m_doc.Parse(jsonStr.c_str());
    return !m_doc.IsNull();
}

bool AIGCJson::ExportJsonString(std::string& jsonStr)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    m_doc.Accept(writer);
    jsonStr = std::string(buffer.GetString());
    return true;
}

void AIGCJson::LoadMembers(std::string membersStr)
{
    if (m_members.size() > 0)
        return;

    std::string::size_type pos1, pos2;
    pos2 = membersStr.find(',');
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        m_members.push_back(membersStr.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = membersStr.find(',', pos1);
    }
    if (pos1 != membersStr.length())
        m_members.push_back(membersStr.substr(pos1));

    for (int i = 0; i < m_members.size(); i++)
    {
        std::string newStr = m_members[i];
        if (!newStr.empty())
        {
            newStr.erase(0, newStr.find_first_not_of(" "));
            newStr.erase(newStr.find_last_not_of(" ") + 1);
        }
        m_members[i] = newStr;
    }
}

template <typename TYPE, typename... TYPES>
bool AIGCJson::JsonToClass(int index, TYPE &arg, TYPES &... args)
{
    if (!JsonToClass(index, arg))
        return false;
    return JsonToClass(++index, args...);
}

template <typename TYPE>
bool AIGCJson::JsonToClass(int index, TYPE &arg)
{
    rapidjson::Value value;
    if (!GetJsonValue({m_members[index]}, value))
        return false;
    return SetMemberValue(arg, value);
}

template <typename TYPE, typename... TYPES>
bool AIGCJson::ClassToJson(int index, TYPE &arg, TYPES &... args)
{
    if (index == 0)
        m_doc.SetObject();
    if (!ClassToJson(index, arg))
        return false;
    return ClassToJson(++index, args...);
}

template <typename TYPE>
bool AIGCJson::ClassToJson(int index, TYPE &arg)
{
    return SetJsonItem({m_members[index]}, arg);
    return true;
}

bool AIGCJson::GetJsonValue(const stringlist<const std::string> &names, rapidjson::Value &value)
{
    if (m_doc.IsNull())
        return false;

    rapidjson::Value newValue;
    for (const std::string *key = names.begin(); key != names.end(); ++key)
    {
        const char *find = (*key).c_str();
        if (newValue.IsNull())
        {
            if (!m_doc.HasMember(find))
                return false;
            newValue = m_doc[find];
        }
        else
        {
            if (newValue.IsNull() || !newValue.IsObject() || !newValue.HasMember(find))
                return false;
            newValue = newValue[find];
        }
    }

    if (newValue.IsNull())
        return false;
    value = newValue;
    return true;
}

bool AIGCJson::SetMemberValue(int &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsInt())
        return false;
    person = value.GetInt();
    return true;
}

bool AIGCJson::SetMemberValue(bool &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsBool())
        return false;
    person = value.GetBool();
    return true;
}

bool AIGCJson::SetMemberValue(double &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsDouble())
        return false;
    person = value.GetDouble();
    return true;
}

bool AIGCJson::SetMemberValue(std::string &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsString())
        return false;
    person = value.GetString();
    return true;
}

bool AIGCJson::SetMemberValue(float &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsFloat())
        return false;
    person = value.GetFloat();
    return true;
}

bool AIGCJson::SetMemberValue(unsigned int &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsUint())
        return false;
    person = value.GetUint();
    return true;
}

bool AIGCJson::SetMemberValue(uint64_t &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsUint64())
        return false;
    person = value.GetUint64();
    return true;
}

bool AIGCJson::SetMemberValue(int64_t &person, rapidjson::Value &value)
{
    if (value.IsNull() || !value.IsInt64())
        return false;
    person = value.GetInt64();
    return true;
}

bool AIGCJson::SetJsonItem(std::string name, int &person)
{
    m_doc.AddMember(rapidjson::StringRef(name.c_str()), person, m_doc.GetAllocator());
    return true;
}

bool AIGCJson::SetJsonItem(std::string name, std::string &person)
{
    m_doc.AddMember(rapidjson::StringRef(name.c_str()), rapidjson::StringRef(person.c_str()), m_doc.GetAllocator());
    return true;
}