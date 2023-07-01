# ctti
The Compile-Time Type Information is a header only library that allows for keys that are types in stl maps. Requires C++ 20 (pun intended)

# Example

```
#include <iostream>
#include <unordered_map>

#include "type_name.h"

int main()
{
	static_assert(ctti::type_name<int>() == "int", "something messed up!");

#ifdef _MSC_VER
	static_assert(ctti::type_name<int, int>() == "int,int", "MSVC does not have a space...");
#elif defined(__GNUC__)
	static_assert(ctti::type_name<int, int>() == "int, int", "But GCC does!");
#endif

	std::unordered_map<ctti::hash_t, long long> map = {};

	map["int"] = 3735928559;

	std::cout << std::hex << map[ctti::type_name<int>()] << std::endl; // deadbeef

	return 0;
}
```

## above code tested with Visual Studio 22, and GCC 11.2.0