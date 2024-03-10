#pragma once
#include "../Utils/rapidProxy.hpp"
#include "../Utils/experimentals.hpp"
#include "../keyGenerator/keyGenerator.hpp"
#include "../cxxHeaderGeneration/cxxClass.hpp"
#include <optional>
#include <vector>

namespace Uranium
{
	struct MemberComponentData
	{
		std::string Key;
		std::string Name;
		Experimentals experiments;
	};

	enum class MemberComponentType : uint8_t
	{
		Primitive,
		Object,
		Array,
		Enum,
		Unknown
	};

	class MemberComponentParser
	{
	public:
		MemberComponentParser() = default;

		std::vector<MemberComponentData> Parse(const rapidjson::Value& parserTarget, const KeyInformation& keyInfo);

		std::vector<CxxClass> ParseMemberComponents(
			const std::vector<MemberComponentData>& compData,
			const rapidjson::Value& parserTarget,
			const std::string& parent = "",
			const std::string& namesp = "");

		std::optional<CxxClass> ParseMemberComponent(
			const MemberComponentData& compData,
			const rapidjson::Value& parserTarget,
			const std::string& parent = "",
			const std::string& namesp = "");

	private:
		void internalParse(const rapidjson::Value& parserTarget, const KeyInformation& keyInfo, std::vector<MemberComponentData>& out, Experimentals experimentalFlags);
		void internalProccessObject(const rapidjson::Value& properties, CxxClass& out);

		MemberComponentType internalCalculateType(const std::string& name, const rapidjson::Value& parserTarget);

	};
}