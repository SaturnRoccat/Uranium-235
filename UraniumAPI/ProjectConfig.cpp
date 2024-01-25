#include "ProjectConfig.hpp"


namespace Uranium
{
	Version::Version(uint8_t major, uint8_t minor, uint8_t patch)
		: major(major), minor(minor), patch(patch)
	{
	}

	std::string Version::ToString() const
	{
		return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
	}

	bool Version::operator==(const Version& other) const
	{
		return major == other.major && minor == other.minor && patch == other.patch;
	}

	bool Version::operator!=(const Version& other) const
	{
		return !(*this == other);
	}

	bool Version::operator>(const Version& other) const
	{
		if (major > other.major)
		{
			return true;
		}
		else if (major == other.major)
		{
			if (minor > other.minor)
			{
				return true;
			}
			else if (minor == other.minor)
			{
				if (patch > other.patch)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Version::operator<(const Version& other) const
	{
		if (major < other.major)
		{
			return true;
		}
		else if (major == other.major)
		{
			if (minor < other.minor)
			{
				return true;
			}
			else if (minor == other.minor)
			{
				if (patch < other.patch)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Version::operator>=(const Version& other) const
	{
		return !(*this < other);
	}

	bool Version::operator<=(const Version& other) const
	{
		return !(*this > other);
	}
}