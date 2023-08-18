/**
 * @file AposaJson.hpp
 * @brief A lightweight JSON parser library.
 * 
 * AposaJson is a header-only JSON parser library written in C++. It provides
 * a simple and efficient way to parse JSON data and convert it to C++ objects.
 *
 * @license MIT License
 * Copyright (c) 2023 Xiaojun Chen
 * http://opensource.org/licenses/MIT
 */

#ifndef APOSA_JSON_H
#define APOSA_JSON_H

#include <string> // std::string
#include <vector> // std::vector

#ifdef APOSA_JSON_USE_STDMAP
    #include <map> // std::map
#else
    #include <unordered_map> // std::unordered_map
#endif

#define APOSAJSON_NAMESPACE_BEGIN namespace AposaJson {
#define APOSAJSON_NAMESPACE_END }

APOSAJSON_NAMESPACE_BEGIN

enum class JsonValueType
{
	Null,
	Boolean,
	Number,
	String,
	Array,
	Object
};
enum class JsonNumberType
{
    Int,
    Uint,
    Int64,
    Uint64,
    Double,
    Float,
    Int16,
    String
};
class JsonValue
{
private:
	JsonValueType _type;
    JsonNumberType _number_type;
    union {
        int int_value;
        unsigned int uint_value;
        int64_t int64t_value;
        uint64_t uint64t_value;
        double double_value;
        float float_value;
        short int16_value;
    };
    std::string number_string;
    bool _boolean;
    std::string _string;
    std::vector<JsonValue> _array;

#ifdef APOSA_JSON_USE_STDMAP
    std::map<std::string, JsonValue> _object;
#else
    std::unordered_map<std::string, JsonValue> _object;
#endif

public:
	JsonValue() :_type(JsonValueType::Null) {}
	JsonValue(JsonValueType type) :_type(type) {}

	JsonValueType GetType() const 
	{
		return _type;
	}
    JsonNumberType GetNumberType() const
    {
        return _number_type;
    }

    void SetBoolean(bool value)
    {
        _type = JsonValueType::Boolean;
        _boolean = value;
    }
    bool GetBoolean() const 
    {
        return _boolean;
    }

    void SetNumberString(const std::string& value)
    {
        _number_type = JsonNumberType::String;
        number_string = value;
    }
    std::string GetNumberString() const
    {
        return number_string;
    }
    void SetInt(const int value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Int;
        int_value = value;
    }
    int GetInt() const
    {
        if (_number_type == JsonNumberType::String) return(std::stoi(number_string));
        return int_value;
    }
    void SetUint(const unsigned int value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Uint;
        uint_value = value;
    }
    unsigned int GetUint() const
    {
        if (_number_type == JsonNumberType::String) return(std::stoul(number_string));
        return uint_value;
    }
    void SetInt64(const int64_t value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Int64;
        int64t_value = value;
    }
    int64_t GetInt64() const
    {
        if (_number_type == JsonNumberType::String) return(std::stoll(number_string));
        return int64t_value;
    }
    void SetUint64(const uint64_t value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Uint64;
        uint64t_value = value;
    }
    uint64_t GetUint64() const
    {
        if (_number_type == JsonNumberType::String) return(std::stoull(number_string));
        return uint64t_value;
    }
    void SetDouble(const double value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Double;
        double_value = value;
    }
    double GetDouble() const
    {
        if (_number_type == JsonNumberType::String) return(std::stod(number_string));
        return double_value;
    }
    void SetFloat(const float value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Float;
        float_value = value;
    }
    float GetFloat() const
    {
        if (_number_type == JsonNumberType::String) return(std::stof(number_string));
        return float_value;
    }
    void SetInt16(const short value)
    {
        _type = JsonValueType::Number;
        _number_type = JsonNumberType::Int16;
        int16_value = value;
    }
    short GetInt16() const
    {
        if (_number_type == JsonNumberType::String) return(std::stoi(number_string));
        return int16_value;
    }

    void SetString(const std::string& value) 
    {
        _type = JsonValueType::String;
        _string = value;
    }
    const std::string& GetString() const 
    {
        return _string;
    }

    void AddElement(const JsonValue& value) 
    {
        _type = JsonValueType::Array;
        _array.push_back(value);
    }
    const std::vector<JsonValue>& GetArray() const 
    {
        return _array;
    }

    void AddMember(const std::string& key, const JsonValue& value) 
    {
        _type = JsonValueType::Object;
        _object[key] = value;
    }

#ifdef APOSA_JSON_USE_STDMAP
    const std::map<std::string, JsonValue>& GetObject() const
    {
        return _object;
    }
#else
    const std::unordered_map<std::string, JsonValue>& GetObject() const
    {
        return _object;
    }
#endif
};
class JsonDocument
{
private:
#ifdef APOSA_JSON_USE_STDMAP
    std::map<std::string, JsonValue> _map;
#else
    std::unordered_map<std::string, JsonValue> _map;
#endif

public:
	JsonDocument() {}

	void AddMember(const std::string& key, const JsonValue& value)
	{
		_map[key] = value;
	}

#ifdef APOSA_JSON_USE_STDMAP
    std::map<std::string, JsonValue> GetMember() const
    {
        return _map;
    }
#else
    std::unordered_map<std::string, JsonValue> GetMember() const
    {
        return _map;
    }
#endif

	JsonValue& operator[](const std::string key)
	{
		return _map[key];
	}
};

class JsonSerializer
{
private:
    void GenKey(std::string& tmp_str, const std::string& key)
    {
        tmp_str += "\"" + key + "\":";
    }

    void GenArray(std::string& tmp_str, const JsonValue& value)
    {
        const auto& arr = value.GetArray();
        tmp_str += "[";
        int i = 0;
        for (const auto& item : arr)
        {
            switch (item.GetType())
            {
            case JsonValueType::Null:
                tmp_str += "null";
                break;

            case JsonValueType::Boolean:
                tmp_str += item.GetBoolean() ? "true" : "false";
                break;

            case JsonValueType::Number:
                switch (item.GetNumberType())
                {
                case JsonNumberType::Int:
                    tmp_str += std::to_string(item.GetInt());
                    break;
                case JsonNumberType::Uint:
                    tmp_str += std::to_string(item.GetUint());
                    break;
                case JsonNumberType::Int64:
                    tmp_str += std::to_string(item.GetInt64());
                    break;
                case JsonNumberType::Uint64:
                    tmp_str += std::to_string(item.GetUint64());
                    break;
                case JsonNumberType::Double:
                    tmp_str += std::to_string(item.GetDouble());
                    break;
                case JsonNumberType::Float:
                    tmp_str += std::to_string(item.GetFloat());
                    break;
                case JsonNumberType::Int16:
                    tmp_str += std::to_string(item.GetInt16());
                    break;
                case JsonNumberType::String:
                    tmp_str += item.GetNumberString();
                    break;
                }
                break;

            case JsonValueType::String:
                tmp_str += "\"" + item.GetString() + "\"";
                break;

            case JsonValueType::Array:
                GenArray(tmp_str, item);
                break;

            case JsonValueType::Object:
                GenObject(tmp_str, item);
                break;  
            }
            if (++i < arr.size()) tmp_str += ",";
        }
        tmp_str += "]";
    }
    void GenObject(std::string& tmp_str, const JsonValue& obj)
    {
        const auto& members = obj.GetObject();
        tmp_str += "{";
        int i = 0;
        for (auto const& member : members)
        {
            GenKey(tmp_str, member.first);
            switch (member.second.GetType())
            {
            case JsonValueType::Null:
                tmp_str += "null";
                break;

            case JsonValueType::Boolean:
                tmp_str += member.second.GetBoolean() ? "true" : "false";
                break;

            case JsonValueType::Number:
                switch (member.second.GetNumberType())
                {
                case JsonNumberType::Int:
                    tmp_str += std::to_string(member.second.GetInt());
                    break;
                case JsonNumberType::Uint:
                    tmp_str += std::to_string(member.second.GetUint());
                    break;
                case JsonNumberType::Int64:
                    tmp_str += std::to_string(member.second.GetInt64());
                    break;
                case JsonNumberType::Uint64:
                    tmp_str += std::to_string(member.second.GetUint64());
                    break;
                case JsonNumberType::Double:
                    tmp_str += std::to_string(member.second.GetDouble());
                    break;
                case JsonNumberType::Float:
                    tmp_str += std::to_string(member.second.GetFloat());
                    break;
                case JsonNumberType::Int16:
                    tmp_str += std::to_string(member.second.GetInt16());
                    break;
                case JsonNumberType::String:
                    tmp_str += member.second.GetNumberString();
                    break;
                }
                break;

            case JsonValueType::String:
                tmp_str += "\"" + member.second.GetString() + "\"";
                break;

            case JsonValueType::Array:
                GenArray(tmp_str, member.second);
                break;

            case JsonValueType::Object:
                GenObject(tmp_str, member.second);
                break;
            }
            if (++i < members.size()) tmp_str += ",";
        }
        tmp_str += "}";
    }

public:
    JsonSerializer() {}

    std::string SerializeObject(const JsonDocument& doc)
    {
        const auto& members = doc.GetMember();

        std::string tmp_result;
        tmp_result += "{";

        int i = 0;
        for (auto const& member : members)
        {
            switch (member.second.GetType())
            {
            case JsonValueType::Null:
                GenKey(tmp_result, member.first);
                tmp_result += "null";
                break;

            case JsonValueType::Boolean:
                GenKey(tmp_result, member.first);
                tmp_result += member.second.GetBoolean() ? "true" : "false";
                break;

            case JsonValueType::Number:
                GenKey(tmp_result, member.first);
                switch (member.second.GetNumberType())
                {
                case JsonNumberType::Int:
                    tmp_result += std::to_string(member.second.GetInt());
                    break;
                case JsonNumberType::Uint:
                    tmp_result += std::to_string(member.second.GetUint());
                    break;
                case JsonNumberType::Int64:
                    tmp_result += std::to_string(member.second.GetInt64());
                    break;
                case JsonNumberType::Uint64:
                    tmp_result += std::to_string(member.second.GetUint64());
                    break;
                case JsonNumberType::Double:
                    tmp_result += std::to_string(member.second.GetDouble());
                    break;
                case JsonNumberType::Float:
                    tmp_result += std::to_string(member.second.GetFloat());
                    break;
                case JsonNumberType::Int16:
                    tmp_result += std::to_string(member.second.GetInt16());
                    break;
                case JsonNumberType::String:
                    tmp_result += member.second.GetNumberString();
                    break;
                }
                break;

            case JsonValueType::String:
                GenKey(tmp_result, member.first);
                tmp_result += "\"";
                tmp_result += member.second.GetString();
                tmp_result += "\"";
                break;

            case JsonValueType::Array:
                GenKey(tmp_result, member.first);
                GenArray(tmp_result, member.second);
                break;

            case JsonValueType::Object:
                GenKey(tmp_result, member.first);
                GenObject(tmp_result, member.second);
                break;
            }
            if (++i < members.size()) tmp_result += ",";
        }
        tmp_result += "}";
        return tmp_result;
    }
};
class JsonParser
{
private:
    std::string::const_iterator _it;
    std::string::const_iterator _it_end;

    JsonDocument ParseJson()
    {
        if (*_it == '{')
        {
            _it++;
            _it_end--;
            JsonDocument doc;

            bool isKey = false;
            std::string key_word;
            for (; _it != _it_end; ++_it)
            {
                switch (*_it)
                {
                case 'n':
                {
                    isKey = false;
                    doc.AddMember(key_word, JsonValue(JsonValueType::Null));
                }
                break;

                case 't':
                case 'f':
                {
                    isKey = false;
                    JsonValue value;
                    value.SetBoolean(ParseBoolean());
                    doc.AddMember(key_word, value);
                }
                break;

                case '\"':
                {
                    if (isKey == false)
                    {
                        isKey = true;
                        key_word = ParseString();
                    }
                    else
                    {
                        isKey = false;
                        JsonValue value;
                        value.SetString(ParseString());
                        doc.AddMember(key_word, value);
                    }
                }
                break;

                case '[':
                {
                    isKey = false;
                    doc.AddMember(key_word, ParseArray());
                }
                break;

                case '{':
                {
                    isKey = false;
                    doc.AddMember(key_word, ParseObject());
                }
                break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                {
                    isKey = false;
                    JsonValue value(JsonValueType::Number);
                    value.SetNumberString(ParseNumber());
                    doc.AddMember(key_word, value);
                }
                break;

                default:
                    continue;
                }
            }
            return doc;
        }
        else return JsonDocument();
    }

    std::string ParseString()
    {
        _it++;
        std::string cache;
        cache.reserve(16);
        while (*_it != '\"')
        {
            cache += *_it;
            _it++;
        }
        _it++;
        return cache;
    }
    void ParseNull()
    {
        _it += 4;
    }
    bool ParseBoolean()
    {
        if (*_it == 't')
        {
            _it += 4;
            return true;
        }
        _it += 5;
        return false;
    }
    std::string ParseNumber()
    {
        std::string cache;
        while (*_it != ',' || (_it == _it_end) == true)
        {
            if (*_it == '}') break;
            cache += *_it;
            _it++;
        }
        cache.erase(std::remove_if(cache.begin(), cache.end(), isspace), cache.end());
        return cache;
    }

    JsonValue ParseValue()
    {
        for (; _it != _it_end; ++_it)
        {
            switch (*_it)
            {
            case 'n':
                return JsonValue(JsonValueType::Null);
            break;

            case 't':
            case 'f':
            {
                JsonValue value;
                value.SetBoolean(ParseBoolean());
                return value;
            }
            break;

            case '\"':
            {
                JsonValue value;
                value.SetString(ParseString());
                return value;
            }
            break;

            case '[':
                return ParseArray();
                break;

            case '{':
                return ParseObject();
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            {
                JsonValue value(JsonValueType::Number);
                value.SetNumberString(ParseNumber());
                return value;
            }
            break;

            default:
                continue;
            }
        }
        return JsonValue();
    }
    JsonValue ParseArray()
    {
        _it++;
        JsonValue value(JsonValueType::Array);
        while (*_it != ']')
        {
            if (*_it == ',' || *_it == ' ' || *_it == '\n')
            {
                _it++;
            }
            else value.AddElement(ParseValue());
        }
        return value;
    }
    JsonValue ParseObject()
    {
        _it++;
        JsonValue refValue(JsonValueType::Object);
        while (*_it != '}')
        {
            if (*_it == ',' || *_it == ' ' || *_it == '\n')
            {
                _it++;
            }
            else
            {
                std::string key = ParseString();
                JsonValue value = ParseValue();
                refValue.AddMember(key, value);
            }
        }
        _it++;
        return refValue;
    }
    
public:
    JsonParser() {}

    JsonDocument Parse(const std::string& json_str)
    {
        if (json_str.empty()) return JsonDocument();
        _it = json_str.begin();
        _it_end = json_str.end();
        return ParseJson();
    }
};

APOSAJSON_NAMESPACE_END

#endif // APOSA_JSON_H