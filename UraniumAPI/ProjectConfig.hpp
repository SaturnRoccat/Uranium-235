#pragma once
#include <string>
#include <tuple>
#include <vector>

namespace Uranium
{
	class Version
	{
	public:
		Version(uint8_t major = 1, uint8_t minor = 0, uint8_t patch = 0);
		
		std::string ToString() const;

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

	enum class Experimentals
	{
		HolidayCreatorFeatures,
		CustomBiomes,
		UpcomingCreatorFeatures = 2,
		BetaAPIs = 4,
		MoLangFeatures = 8,
		ExperimentalCameras = 16,
		VillagerTradeRebalancing = 32,
	};

	class ProjectSettings
	{
	public:
		ProjectSettings(
			const Version& version = Version(),
			const std::string& name = "Uranium Project",
			const std::string& description = "A Uranium Project",
			const std::string& author = "Uranium",
			const std::string& outputLocation = "UraniumOut/",
			const std::vector<std::pair<Experimentals, bool>>& experimentals = {}
		)
			: version(version), name(name), description(description), author(author), experimentals(experimentals)
		{
		}

		const std::string getNameWithNamespace(const std::string& Name) const
		{
			return name + ":" + Name;
		}


	public:
		Version version;
		std::string name;
		std::string description;
		std::string author;
		std::vector<std::pair<Experimentals, bool>> experimentals;
		std::string outputLocation;
	};
}
