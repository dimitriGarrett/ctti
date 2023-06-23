# ctti
Compile time type info

# Quick example: 
```
#include <iostream>
#include "ctti/ctti.h"

std::unordered_map<ctti::cstring, int> map = {};

map[ctti::type_name<int>()] = 5;
std::cout << map["int"] << std::endl; // outputs 5
```
