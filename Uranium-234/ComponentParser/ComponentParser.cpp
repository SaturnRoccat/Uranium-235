#include "ComponentParser.hpp"
#include <fstream>
#include "../Utils/Logger/Logger.hpp"
#include "../Utils/Timer.hpp"
#include "../memberComponentParsing/memberComponentParser.hpp"
Uranium::ComponentParser::ComponentParser(): m_KeyGen(&this->m_Document) 
{
	std::ifstream file("schemas.json");
	if (!file.is_open()) {
		Logs::Logger::Error("Failed to open schemas.json");
	}

	std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	m_Document.Parse(jsonString.c_str());
	Run();
}

void Uranium::ComponentParser::Run()
{
	Logs::Logger::Info("Locating most up to date keys!");
	std::array<KeyInformation, 3> highestVersionKeys = {};
	{
		ScopedTimer timer("Finding most up to date keys");
		highestVersionKeys = m_KeyGen.findHighestVersionKey<3, 1>(gComponentTypes, gEndData);
	}
	Logs::Logger::Info("Found most up to date keys!");
	for (auto& key : highestVersionKeys) {
		Logs::Logger::Info("Highest Version Key: {}\nLongPrefix Key: {}\nShortPrefix Key: {}", key.highestVersionKey, key.longPrefix, key.shortPrefix);
	}
	Logs::Logger::Info("Generating component lists!");
	MemberComponentParser memberComponentParser;
	std::array<std::vector<MemberComponentData>, 3> memberComponentData = {};
	for (int i = 0; i < highestVersionKeys.size(); i++) {
		ScopedTimer timer("Generating component lists");
		auto& componentRef = m_Document[highestVersionKeys[i].highestVersionKey.c_str()];
		memberComponentData[i] = memberComponentParser.Parse(componentRef, highestVersionKeys[i]);
	}
	Logs::Logger::Info("Generated component lists!");
	Logs::Logger::Info("Dumping component lists");
	for (int i = 0; i < highestVersionKeys.size(); i++) {
		Logs::Logger::Info("Dumping component list for version: {}", highestVersionKeys[i].highestVersionKey);
		for (auto& member : memberComponentData[i])
		{
			Logs::Logger::Info("Name: {}\nExperimentals: {}\nKey: {}", member.Name, getExperimentalName(member.experiments), member.Key);
		}
	}
	Logs::Logger::Info("Dumped component lists");
	Logs::Logger::Info("Translating to Cxx");
	for (int i = 0; i < highestVersionKeys.size(); i++) {
		ScopedTimer timer("Translating to Cxx");
		auto classes = memberComponentParser.ParseMemberComponents(memberComponentData[i], m_Document, gParents[i], "Uranium");
	}

}
