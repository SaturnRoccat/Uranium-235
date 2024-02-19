#pragma once
#include <cstdint>

#if defined(__GNUC__) || defined(__GNUG__) // Check if GCC or compatible compiler is being used
#define __forceinline inline __attribute__((always_inline))
#endif

namespace Uranium
{
	class PackedLengthBool
	{
	private:
		static constexpr uint64_t lengthMask = 0x7FFFFFFFFFFFFFFF;
		static constexpr uint64_t boolMask = 0x8000000000000000;
	public:
		PackedLengthBool() : data(0) {}
		PackedLengthBool(uint64_t length, bool value = false) : data((length & lengthMask) | (value ? boolMask : 0)) {}
		PackedLengthBool(const PackedLengthBool& other) : data(other.data) {}

		operator bool() { return getBool(); }
		operator uint64_t() { return getLength(); }

		PackedLengthBool operator+(uint64_t length)
		{
			PackedLengthBool result = *this;
			result.setLength(getLength() + length);
			return result;
		}
		PackedLengthBool operator-(uint64_t length)
		{
			PackedLengthBool result = *this;
			result.setLength(getLength() - length);
			return result;
		}
		PackedLengthBool operator++()
		{
			PackedLengthBool result = *this;
			result.setLength(getLength() + 1);
			return result;
		}
		PackedLengthBool operator--()
		{
			PackedLengthBool result = *this;
			result.setLength(getLength() - 1);
			return result;
		}
		PackedLengthBool operator+=(uint64_t length)
		{
			setLength(getLength() + length);
			return *this;
		}
		PackedLengthBool operator-=(uint64_t length)
		{
			setLength(getLength() - length);
			return *this;
		}

		bool operator==(const PackedLengthBool& other)
		{
			return data == other.data;
		}
		bool operator!=(const PackedLengthBool& other)
		{
			return data != other.data;
		}
		bool operator==(uint64_t other)
		{
			return getLength() == other;
		}
		bool operator!=(uint64_t other)
		{
			return getLength() != other;
		}
		bool operator==(bool other)
		{
			return getBool() == other;
		}
		bool operator!=(bool other)
		{
			return getBool() != other;
		}

		bool operator!()
		{
			return !getBool();
		}
		bool operator<(uint64_t other)
		{
			return getLength() < other;
		}
		bool operator>(uint64_t other)
		{
			return getLength() > other;
		}
		bool operator<=(uint64_t other)
		{
			return getLength() <= other;
		}
		bool operator>=(uint64_t other)
		{
			return getLength() >= other;
		}

		uint64_t operator&(uint64_t other)
		{
			return getLength() & other;
		}
		uint64_t operator&=(uint64_t other)
		{
			setLength(getLength() & other);
			return getLength();
		}
		uint64_t operator|(uint64_t other)
		{
			return getLength() | other;
		}
		uint64_t operator|=(uint64_t other)
		{
			setLength(getLength() | other);
			return getLength();
		}
		uint64_t operator^(uint64_t other)
		{
			return getLength() ^ other;
		}
		uint64_t operator^=(uint64_t other)
		{
			setLength(getLength() ^ other);
			return getLength();
		}

		// only reserve 63 bits for the length, the 64th bit is for the bool
		__forceinline void setLength(uint64_t length)
		{
			data = (data & boolMask) | (length & lengthMask);
		}
		__forceinline uint64_t getLength()
		{
			return data & lengthMask;
		}
		__forceinline void setBool(bool value)
		{
			data = (data & lengthMask) | (value ? boolMask : 0);
		}
		__forceinline bool getBool()
		{
			return data & boolMask;
		}

	private:
		uint64_t data;
	};
};