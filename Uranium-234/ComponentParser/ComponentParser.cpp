#include "ComponentParser.hpp"
#include <fstream>
#include "../Utils/Logger/Logger.hpp"
#include "../Utils/Timer.hpp"
Uranium::ComponentParser::ComponentParser(): m_KeyGen(&this->m_Document) // even though Doc will have no data in it we have to invoke keygen so just wait till after construction
{
	std::ifstream file("schemas.json");
	if (!file.is_open()) {
		std::cerr << "Failed to open file." << std::endl;
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
		highestVersionKeys = m_KeyGen.findHighestVersionKey<3, 1>(this->m_ComponentTypes, this->m_EndData);
	}
	Logs::Logger::Info("Found most up to date keys!");
	for (auto& key : highestVersionKeys) {
		Logs::Logger::Info("Highest Version Key: {}\nLongPrefix Key: {}\nShortPrefix Key: {}", key.highestVersionKey, key.longPrefix, key.shortPrefix);
	}
	Logs::Logger::Info("Generating component lists!");

}
