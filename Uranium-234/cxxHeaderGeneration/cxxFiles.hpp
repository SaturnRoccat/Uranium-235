#pragma once
#include <vector>
#include <string>


namespace Uranium
{
	class CxxGenericTypeData
	{
	public:
		std::vector<std::string> requiredIncludes = {};
		CxxGenericTypeData(const std::vector<std::string>& includes = {}) : requiredIncludes(includes) {}
	};

	struct CxxFileData
	{
		std::string headerData = "";
		std::string sourceData = "";
	};
}
