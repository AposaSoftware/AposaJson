```
     _                              _                 
    / \   _ __   ___  ___  __ _    | |___  ___  _ __  
   / _ \ | '_ \ / _ \/ __|/ _` |_  | / __|/ _ \| '_ \ 
  / ___ \| |_) | (_) \__ \ (_| | |_| \__ \ (_) | | | |
 /_/   \_\ .__/ \___/|___/\__,_|\___/|___/\___/|_| |_|
         |_|                        
```

## A header-only JSON parser library written in C++. It provides a simple and efficient way to parse JSON data and convert it to C++ objects.

## Example

~~~~~~~~~~cpp
#include "AposaJson/AposaJson.hpp"
#include <string>
#include <iostream>

using namespace AposaJson;

int main()
{
    // 1. Parse a JSON string into DOM.
    std::string json = R"({
        "string": "test_string",
        "integer": 123,
        "float": 3.14,
        "boolean": true,
        "null": null
    })";
    JsonDocument doc;
    JsonParser parser;
    doc = parser.Parse(json);

    // 2. Modifying object in DOM.
    doc["string"].SetString("Modified string");

    // 3. Output object in DOM.
    std::cout << doc["string"].GetString(); << std::endl;

    // 4. Serialize DOM to string.
    JsonSerializer serializer;
    std::string result = serializer.SerializeObject(doc);

    return 0;
}
~~~~~~~~~~
