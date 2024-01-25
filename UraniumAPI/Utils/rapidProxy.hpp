#pragma once
// This file only exists because of rapidjson not compiling on C++17 or higher
#undef min
#undef max
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)
#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#include <rapidjson/include/rapidjson/document.h>
#include <rapidjson/include/rapidjson/istreamwrapper.h>
#include <rapidjson/include/rapidjson/ostreamwrapper.h>
#include <rapidjson/include/rapidjson/prettywriter.h>
#include <rapidjson/include/rapidjson/stringbuffer.h>
#pragma warning(default : 26451)
#pragma warning(default : 4996)
#pragma warning(default : 4267)
#pragma warning(default : 26495)

namespace RapidProxy
{
	template<class Container, class Allocater>
	struct DataWriter
	{
		Container* data;
		Allocater& allocator;
	};


	typedef DataWriter<rapidjson::Value, rapidjson::MemoryPoolAllocator<>> DefaultValueWriter;
	typedef DataWriter<rapidjson::Document, rapidjson::MemoryPoolAllocator<>> DocumentWriter;


	// Pulls the data and allocator from the writer into a variable called data and allocator
#define DVAP() \
rapidjson::Value* data = (rapidjson::Value*)writer.data; \
rapidjson::MemoryPoolAllocator<>& allocator = writer.allocator;

	// Pulls the data and allocator from the writer into a variable called data and allocator
#define DDAP() \
rapidjson::Document* data = (rapidjson::Document*)writer.data; \
rapidjson::MemoryPoolAllocator<>& allocator = writer.allocator;

#define RJ_STL_S(str) rapidjson::StringRef(str.c_str(), str.size())

#define RJ_STL_V_A(name, x)\
rapidjson::Value name(rapidjson::kArrayType);\
name.Reserve(x.size(), allocator);\
for (auto& i : x) {\
	name.PushBack(i, allocator);\
}

#define RJ_STL_V_A_P(name, x)\
rapidjson::Value name = rapidjson::Value(rapidjson::kArrayType);\
name.Reserve(x.size(), allocator);\
for (auto& i : x) {\
	name.PushBack(i, allocator);\
}

/**
* @brief This macro is used to convert a STL vector into a rapidjson array value with a convertion call.
* * @param name The name of the array.
* * @param x The vector to convert.
* * @param allocator The allocator to use.
* * @param ConvertionCall The call to convert the type.
* * @return void
*
*/
#define RJ_STL_V_A_EX(name, x, ConvertionCall)\
rapidjson::Value name(rapidjson::kArrayType);\
name.Reserve(x.size(), allocator);\
for (auto& i : x) {\
	name.PushBack(ConvertionCall(i), allocator);\
}
}