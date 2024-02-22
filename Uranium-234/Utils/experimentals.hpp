#pragma once	
#include <array>
#include <string>

namespace Uranium
{
	enum class Experimentals : unsigned char
	{
		HolidayCreatorFeatures,
		CustomBiomes,
		UpcomingCreatorFeatures = 2,
		BetaAPIs = 4,
		MoLangFeatures = 8,
		ExperimentalCameras = 16,
		VillagerTradeRebalancing = 32,
		None = 0

	};

	const static std::array<std::string, 7> experimentalNames = {
		"holidayCreatorFeatures",
		"CustomBiomes",
		"UpcomingCreatorFeatures",
		"BetaAPIs",
		"MoLangFeatures",
		"ExperimentalCameras",
		"VillagerTradeRebalancing"
	};

	inline std::string_view getExperimentalName(Experimentals experimental)
	{
		switch (experimental)
		{
		case Experimentals::HolidayCreatorFeatures:
			return experimentalNames[0];
		case Experimentals::CustomBiomes:
			return experimentalNames[1];
		case Experimentals::UpcomingCreatorFeatures:
			return experimentalNames[2];
		case Experimentals::BetaAPIs:
			return experimentalNames[3];
		case Experimentals::MoLangFeatures:
			return experimentalNames[4];
		case Experimentals::ExperimentalCameras:
			return experimentalNames[5];
		case Experimentals::VillagerTradeRebalancing:
			return experimentalNames[6];
		default:
			return "Unknown";
		}
	}

	inline Experimentals getExperimentalFromName(std::string_view name)
	{
		for (int i = 0; i < experimentalNames.size(); i++)
		{
			if (experimentalNames[i] == name)
			{
				return static_cast<Experimentals>(i);
			}
		}
		return Experimentals::HolidayCreatorFeatures;
	}
}