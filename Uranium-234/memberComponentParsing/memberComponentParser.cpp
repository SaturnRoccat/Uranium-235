#include "memberComponentParser.hpp"
#include "../Utils/Logger/Logger.hpp"

std::vector<Uranium::MemberComponentData> Uranium::MemberComponentParser::Parse(const rapidjson::Value& parserTarget, const KeyInformation& keyInfo)
{
	std::vector<MemberComponentData> out;
	if (!parserTarget.HasMember("allOf"))
	{
		internalParse(parserTarget, keyInfo, out, Experimentals::None);
		return out;
	}
	auto allOf = parserTarget["allOf"].GetArray();
	for (auto& obj : allOf)
	{
		Experimentals flags = Experimentals::None;
		if (obj.HasMember("$ref"))
		{
			flags = KeyGenerator::resolveExperimentalData(std::string_view(obj["$ref"].GetString(), (size_t)obj["$ref"].GetStringLength()));
			auto& parseData = obj["then"];
			internalParse(parseData, keyInfo, out, flags);
		}
		else
			internalParse(obj, keyInfo, out, flags);
	}
	return out;
}

void Uranium::MemberComponentParser::internalParse(const rapidjson::Value& parserTarget, const KeyInformation& keyInfo, std::vector<MemberComponentData>& out, Experimentals experimentalFlags)
{
	if (!parserTarget.HasMember("type"))
		throw std::runtime_error("MemberComponentParser::internalParse: No type member found in parserTarget");
	if (parserTarget["type"] != "object")
		throw std::runtime_error("MemberComponentParser::internalParse: parserTarget type is not object");
	if (!parserTarget.HasMember("properties"))
		throw std::runtime_error("MemberComponentParser::internalParse: No properties member found in parserTarget");
	if (!parserTarget["properties"].IsObject())
		throw std::runtime_error("MemberComponentParser::internalParse: properties member is not an object");
	auto properties = parserTarget["properties"].GetObject();
	for (rapidjson::Value::ConstMemberIterator itr = properties.MemberBegin(); itr != properties.MemberEnd(); ++itr)
	{
		MemberComponentData data;
		data.Name = itr->name.GetString();
		data.experiments = experimentalFlags;
		if (itr->value.HasMember("then"))
		{
			auto& then = itr->value["then"];
			auto& ref = then["$ref"];
			data.Key = Uranium::KeyGenerator::resolveRefCallPath(std::string_view(ref.GetString(), (size_t)ref.GetStringLength()), keyInfo.longPrefix, keyInfo.shortPrefix);
		}
		else
			data.Key = Uranium::KeyGenerator::resolveRefCallPath(std::string_view(itr->value["$ref"].GetString(), (size_t)itr->value["$ref"].GetStringLength()), keyInfo.longPrefix, keyInfo.shortPrefix);
		out.push_back(data);
	}
}
