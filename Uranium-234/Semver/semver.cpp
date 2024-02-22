#include "semver.hpp"
#include <stdexcept>
#include <algorithm>
#include <array>
#include <string>

Uranium::semver::semver(std::string_view versionString)
{
	if (versionString.at(0) != 'v')
		throw std::invalid_argument("Invalid version string");
	std::string_view version = versionString.substr(1);
	std::array<size_t, 3> parts = {};
	size_t lastPos = 0;
	size_t pos = 0;
	size_t i = 0;
	for (; i < 3; i++)
	{
		pos = version.find('.', lastPos);
		if (pos == std::string::npos)
		{
			pos = version.size();
		}
		parts[i] = std::stoul(version.substr(lastPos, pos - lastPos).data()); // using data() so i dont have to use std::string
		lastPos = pos + 1;
	}
	if (i < 3)
		throw std::invalid_argument("Invalid version string");
	major = parts[0];
	minor = parts[1];
	patch = parts[2];
}

std::string Uranium::semver::toString() const
{
	return "v" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
}
