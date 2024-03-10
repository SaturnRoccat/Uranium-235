#include "cxxType.hpp"
#include <unordered_map>

namespace Uranium
{
	std::unordered_map<std::string, std::string> customCxxTypeConversions;

	void registerCustomCxxTypeConversion(const std::string& type, const std::string& conversion)
	{
		customCxxTypeConversions[type] = conversion;
	}
	std::string_view getCustomCxxTypeConversion(const std::string& type)
	{
		auto it = customCxxTypeConversions.find(type);
		if (it != customCxxTypeConversions.end())
		{
			return it->second;
		}
		return "";
	}

	std::unordered_map<std::string, std::function<std::string(const std::string&)>> customCxxTypeCompilers;

	void registerCustomCxxTypeCompiler(const std::string& type, std::function<std::string(const std::string&)> convFunc)
	{
		//Q: how can i do this because std::function is not copyable
//A: use std::function::target to get the function pointer
		customCxxTypeCompilers[type] = convFunc;
	}

	std::function<std::string(const std::string&)> getCustomCxxTypeCompiler(const std::string& type)
	{
		auto it = customCxxTypeCompilers.find(type);
		if (it != customCxxTypeCompilers.end())
		{
			return it->second;
		}
		return nullptr;
	}

	std::unordered_map<std::string, std::string> customCxxTypeIncludes;

	void registerRequiredInclude(const std::string& include)
	{
		customCxxTypeIncludes[include] = include;
	}

	std::string_view getRequiredInclude(const std::string& include)
	{
		auto it = customCxxTypeIncludes.find(include);
		if (it != customCxxTypeIncludes.end())
		{
			return it->second;
		}
		return "";
	}


}