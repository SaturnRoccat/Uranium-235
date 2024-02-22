#pragma once
#include "../Utils/rapidProxy.hpp"
#include "../Semver/semver.hpp"
#include <string>
#include <array>
#include <xstring>
#include "../Utils/predecs.hpp"
#include <iostream>

namespace Uranium
{
	class KeyGenerator
	{
		rapidjson::Document* doc;
	public:
		KeyGenerator(rapidjson::Document* doc) : doc(doc) {}

		template<size_t expectedSize, size_t enddingTestSize = 1>
		std::array<std::string, expectedSize> findHighestVersionKey(const std::array<std::string, expectedSize>& searchKeys, const std::array<std::string, enddingTestSize>& ends)
		{
			if (doc == nullptr)
				throw std::runtime_error("Document is null");
			if (doc->IsObject() == false)
				throw std::runtime_error("Document is not an object");
			std::array<std::string, expectedSize> highestVersionKeys = {};
			for (auto& endData : ends)
			{
				size_t state = 0;
				for (auto& searchKey : searchKeys)
				{
					std::string_view highestVersionKey = "";
					semver highestVersion = { 0,0,0 };
					for (rapidjson::Value::ConstMemberIterator itr = doc->MemberBegin(); itr != doc->MemberEnd(); ++itr)
					{
						size_t offset = (SCHEMA_START_SIZE - 1) + searchKey.length();
						std::string_view fullKey{ itr->name.GetString(), itr->name.GetStringLength() };
						if (fullKey.length() < offset + endData.length()) // If the key is too short to contain the search key and the end data, skip it
							continue;
						std::string partialKey = SCHEMA_START + searchKey;
						size_t fullKeyStart = fullKey.find(partialKey); // Find the start of the key
						if (fullKeyStart == std::string_view::npos) // If the key doesn't contain the search key, skip it
							continue; 
						bool fullKeyEndData = fullKey.ends_with(endData.data()); // Check if the key ends with the end data
						if (!fullKeyEndData) // If the key doesn't end with the end data, skip it
							continue;
						std::string_view semVerStr = fullKey.substr(offset); // Pulls out the version number
						if (semVerStr.at(0) != 'v') // If the version number doesn't start with a v, skip it
							continue;
						size_t positionOfSlash = semVerStr.find('/'); // Find the end of the version number
						if (positionOfSlash == std::string_view::npos) // If the version number doesn't contain a slash, skip it
							continue;
						semVerStr = semVerStr.substr(0, positionOfSlash); // Pulls out the version number
						semver tempVersion = semver(semVerStr); // Convert the version number to a semver
						if (tempVersion > highestVersion)
						{
							highestVersion = tempVersion;
							highestVersionKey = fullKey;
						}


					}
					highestVersionKeys[state] = std::string(highestVersionKey);
					state++;
					
				}
			}
			return highestVersionKeys;
		}


	private:
	};
}