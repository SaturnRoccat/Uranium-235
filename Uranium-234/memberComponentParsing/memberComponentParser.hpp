#pragma once
#include "../Utils/rapidProxy.hpp"
#include "../Utils/experimentals.hpp"
#include "../keyGenerator/keyGenerator.hpp"
#include <vector>

namespace Uranium
{
	struct MemberComponentData
	{
		std::string Key;
		std::string Name;
		Experimentals experiments;
	};


	class MemberComponentParser
	{
	public:
		MemberComponentParser() = default;

		std::vector<MemberComponentData> Parse(const rapidjson::Value& parserTarget, const KeyInformation& keyInfo);

	private:
		void internalParse(const rapidjson::Value& parserTarget, const KeyInformation& keyInfo, std::vector<MemberComponentData>& out, Experimentals experimentalFlags);

	};
}