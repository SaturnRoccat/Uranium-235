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

namespace Uranium {


	void Uranium::MemberComponentParser::internalProccessObject(const rapidjson::Value& properties, CxxClass& out)
	{
		for (rapidjson::Value::ConstMemberIterator itr = properties.MemberBegin(); itr != properties.MemberEnd(); ++itr)
		{
			CxxType type;
			std::string_view key = itr->name.GetString();
			MemberComponentType guessedType = internalCalculateType(key.data(), itr->value);

		}
	}

	MemberComponentType MemberComponentParser::internalCalculateType(const std::string& name, const rapidjson::Value& parserTarget)
	{
		MemberComponentType guessedType = MemberComponentType::Primitive;
		if (name == "type")
		{
			auto enum_ = parserTarget.FindMember("enum");
			if (enum_ != parserTarget.MemberEnd())
				guessedType = MemberComponentType::Enum;
			else
				guessedType = MemberComponentType::Unknown;
		}
		else if (parserTarget.HasMember("enum"))
		{
			guessedType = MemberComponentType::Enum;
		}
		if (guessedType == MemberComponentType::Unknown)
			Logs::Logger::Error("Unknown type for member: {}", name);
		return guessedType;
	}


	std::vector<CxxClass> Uranium::MemberComponentParser::ParseMemberComponents(
		const std::vector<MemberComponentData>& compData,
		const rapidjson::Value& parserTarget,
		const std::string& parent,
		const std::string& namesp)
	{
		std::vector<CxxClass> out;
		out.reserve(compData.size());
		for (auto& comp : compData)
		{
			auto& data = parserTarget[comp.Key.c_str()];
			auto parsed = ParseMemberComponent(comp, data, parent, namesp);
			if (parsed.has_value())
				out.push_back(parsed.value());
		}
	    
		return out;
	}

	std::optional<CxxClass> Uranium::MemberComponentParser::ParseMemberComponent(
		const MemberComponentData& compData,
		const rapidjson::Value& parserTarget,
		const std::string& parent,
		const std::string& namesp)
	{
		if (!parserTarget.IsObject())
		{
			Logs::Logger::NonFatalError("Parser target is not an object in MemberComponentParser::ParseMemberComponent. Debugging Data\nKey: {}", compData.Key);
			return std::nullopt;
		}

		auto dns = parserTarget.FindMember("doNotSuggest");
		if (dns != parserTarget.MemberEnd())
		{
			if (dns->value.GetBool())
				return std::nullopt;
		}
#ifdef DEBUG
		Logs::Logger::Debug("Parsing member component: {}", compData.Name);
		// dump json 
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		parserTarget.Accept(writer);
		Logs::Logger::Debug("Dumping json: {}", buffer.GetString());
#endif

		CxxClass out;
		auto calculatedName = KeyGenerator::parseGenericStringForLastNonJson(compData.Key);
		out.setName(calculatedName);
		out.setParent(parent);
		out.setNamespace(namesp);
		out.addMemberForParent("\"" + calculatedName + "\"");

		auto type = parserTarget.FindMember("type");
		if (type == parserTarget.MemberEnd())
		{
			Logs::Logger::NonFatalError("No type member found in MemberComponentParser::ParseMemberComponent. Debugging Data\nKey: {}", compData.Key);
			return std::nullopt;
		}

		if (!type->value.IsString())
		{
			Logs::Logger::NonFatalError("Type member is not a string in MemberComponentParser::ParseMemberComponent. Debugging Data\nKey: {}", compData.Key);
			return std::nullopt;
		}

		if (type->value.GetString() == "object")
		{
			// stub
		}
		else if (type->value.GetString() == "array")
		{
			// stub
		}
		else
		{
			auto defaul = parserTarget.FindMember("default");
			if (defaul != parserTarget.MemberEnd())
				out.addMember(CxxType("m_" + calculatedName, type->value.GetString(), defaul->value.GetString()));
			else
				out.addMember(CxxType("m_" + calculatedName, type->value.GetString()));
		}


		out.printMembers();
		return out;
	}
};