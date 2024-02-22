#pragma once
#include <stdint.h>
#include <xstring>

namespace Uranium
{
	class semver
	{
	public:
		semver(uint8_t major, uint8_t minor, uint8_t patch, uint8_t prerelease = 0)
			: major(major), minor(minor), patch(patch) {}
		semver(std::string_view versionString);
		semver(uint32_t version)
			: version(version) {}
		semver() : version(0) {}
		semver(const semver& other) : version(other.version) {}
		uint32_t getPacked() const { return version; }
		uint8_t getMajor() const { return major; }
		uint8_t getMinor() const { return minor; }
		uint8_t getPatch() const { return patch; }

		std::string toString() const;

		bool operator==(const semver& other) const { return version == other.version; }
		bool operator!=(const semver& other) const { return version != other.version; }
		bool operator<(const semver& other) const { return version < other.version; }
		bool operator>(const semver& other) const { return version > other.version; }
		bool operator<=(const semver& other) const { return version <= other.version; }
		bool operator>=(const semver& other) const { return version >= other.version; }

	private:
		union
		{
			struct
			{
				uint8_t patch;
				uint8_t pad;
				uint8_t minor;
				uint8_t major;
			};
			uint32_t version : 32;
		};
	};
}