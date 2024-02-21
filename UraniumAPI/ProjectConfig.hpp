#pragma once
#include <string>
#include <tuple>
#include <vector>
#include "Utils/strUtil.hpp"
#include <uuidv4/uuid_v4.h>
#include <format>

namespace Uranium
{
	class Version
	{
	public:
		Version(uint8_t major = 1, uint8_t minor = 0, uint8_t patch = 0);
		
		CStrWithLength ToString() const;

		uint8_t GetMajor() const { return major; }
		uint8_t GetMinor() const { return minor; }
		uint8_t GetPatch() const { return patch; }

		bool operator==(const Version& other) const;
		bool operator!=(const Version& other) const;
		bool operator>(const Version& other) const;
		bool operator<(const Version& other) const;
		bool operator>=(const Version& other) const;
		bool operator<=(const Version& other) const;


	private:
		uint8_t major, minor, patch;
	};

	enum class Experimentals : unsigned char
	{
		HolidayCreatorFeatures,
		CustomBiomes,
		UpcomingCreatorFeatures = 2,
		BetaAPIs = 4,
		MoLangFeatures = 8,
		ExperimentalCameras = 16,
		VillagerTradeRebalancing = 32,
		UraniumExperimentalAPIs = 64, // This isnt an actual experimental we can add to the manifest, but this should allow us to add experimental APIs to Uranium and just hope lmao
		ALL = 0xFF // Used as a hacky way to get all experimentals
	};

	class ProjectSettings
	{
	public:
		ProjectSettings(
			const Version& version = Version(),
			CStrWithLength name = "Uranium Project",
			CStrWithLength description = "A Uranium Project",
			CStrWithLength author = "Uranium",
			CStrWithLength outputLocation = "UraniumOut/",
			CStrWithLength namespace_ = "uranium",
			const std::vector<std::pair<Experimentals, bool>>& experimentals = {}
		)
			: version(version), name(name), description(description), author(author), experimentals(experimentals), namespace_(namespace_)
		{
			(void)gen.getUUID(); // Cycles the generator to initialize it
		}

		const CStrWithLength getNameWithNamespace(CStrWithLength Name) const
		{
			return CStrWithLength(namespace_ + ":" + Name, false);
		}

		UUIDv4::UUIDGenerator<std::mt19937_64>* getUUID() 
		{
			return &gen;
		}

	public:
		CStrWithLength namespace_;
		Version version;
		CStrWithLength name;
		CStrWithLength description;
		CStrWithLength author;
		std::vector<std::pair<Experimentals, bool>> experimentals;
		CStrWithLength outputLocation;
		UUIDv4::UUIDGenerator<std::mt19937_64> gen;
	};
}
