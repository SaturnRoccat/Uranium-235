#pragma once 
#include <string>
#include <functional>
#include <string_view>
#include "rapidProxy.hpp"
#include "packedLengthBool.hpp"


namespace Uranium
{
    // Mostly a utility class for handling strings so RJ doesnt have to do like 6000 allocations and string refs
    class CStrWithLength
    {
    public:
        CStrWithLength() : str(NULL), lengthAndAutoRelease(0) {}
        CStrWithLength(const char* str, unsigned long long length, bool autoRelease = true)
        {
            if (length < 15)
            {
                lengthAndAutoRelease = pFlagMask;
                SSOSizeAndAutoRelease = pSSOFlag | (length & pSSOSizeMask);
                if (autoRelease)
                    SSOSizeAndAutoRelease |= pLastBitMask;
                memcpy(SSO, str, length);
                SSO[length] = '\0';
            }
            else
            {
                this->lengthAndAutoRelease.setLength(length);
                this->lengthAndAutoRelease.setBool(autoRelease);
                char* newStr = new char[length + 1];
                this->str = newStr;
                memcpy(this->str, str, length);
                this->str[length] = '\0';
            }
        }
        template<size_t N>
        CStrWithLength(const char(&str)[N], bool autoRelease = true) : CStrWithLength(str, N - 1, autoRelease) {}

        explicit CStrWithLength(const std::string& str, bool autoRelease = true) : CStrWithLength(str.c_str(), str.size(), autoRelease) {}
        CStrWithLength(CStrWithLength& other) : CStrWithLength(other.c_str(), other.size(), other.getAutoRelease()) {}
        CStrWithLength(CStrWithLength&& other) noexcept : CStrWithLength(other.c_str(), other.size(), other.getAutoRelease()) { other.release(); }
        CStrWithLength(const CStrWithLength& other) 
        {
            if (other.isSSO())
            {
                SSOSizeAndAutoRelease = other.SSOSizeAndAutoRelease;
                memcpy(SSO, other.SSO, other.size());
            }
            else
            {
                char* newStr = new char[other.size() + 1];
                memcpy(newStr, const_cast<CStrWithLength&>(other).c_str(), other.size());
                newStr[other.size()] = '\0';
				lengthAndAutoRelease = other.lengthAndAutoRelease;
				str = newStr;
            }
        }

        ~CStrWithLength() { if (getAutoRelease()) release(); } 
    public:
        inline rapidjson::Value toValue() { return rapidjson::Value(getRawData(), rapidjson::SizeType(getLength())); }
        inline char* c_str() { return getRawData(); }
        inline size_t size() const { return getLength(); }
        inline void setAutoDelete(bool autoDelete) { setAutoRelease(autoDelete); }
        inline std::string toString() { return std::string(getRawData(), getLength()); }
        CStrWithLength substr(size_t start, size_t end);
        inline bool empty() const { return getLength() == 0; }
        inline void clear() {release();}
    public:
        operator const char* () { return getRawData(); }
        operator char* () { return getRawData(); }
        operator size_t () { return getLength(); }
        explicit operator rapidjson::Value() { return rapidjson::Value(str, rapidjson::SizeType(getLength())); }
        operator std::string_view() { return std::string_view(str, getLength()); }
        char& operator[](size_t index) { return *(getRawData() + index); }

        bool operator==(const char* other) const { return strcmp(getRawData(), other) == 0; }
        bool operator==(const std::string& other) { return strcmp(getRawData(), other.c_str()) == 0; }
        bool operator==(const std::string_view& other) { return strncmp(getRawData(), other.data(), other.size()) == 0; }
        bool operator==(CStrWithLength& other) const { return strncmp(getRawData(), other.c_str(), other.size()) == 0; }

        bool operator!=(const char* other) { return strcmp(getRawData(), other) != 0; }
        bool operator!=(const std::string& other) { return strcmp(getRawData(), other.c_str()) != 0; }
        bool operator!=(const std::string_view& other) { return strncmp(getRawData(), other.data(), other.size()) != 0; }
        bool operator!=(CStrWithLength& other) const { return strncmp(getRawData(), other.getRawData(), other.size()) != 0; }

        CStrWithLength operator+(const char* other) { return CStrWithLength(getRawData(), getLength() + strlen(other)); }
        CStrWithLength operator+(const std::string& other) { return CStrWithLength(getRawData(), getLength() + other.size()); }
        CStrWithLength operator+(const std::string_view& other) { return CStrWithLength(getRawData(), getLength() + other.size()); }
		CStrWithLength operator+(CStrWithLength& other) { return CStrWithLength(getRawData(), getLength() + other.size()); }

        CStrWithLength& operator+=(const char* other) { return *this = *this + other; }
		CStrWithLength& operator+=(const std::string& other) { return *this = *this + other; }
		CStrWithLength& operator+=(const std::string_view& other) { return *this = *this + other; }
		CStrWithLength& operator+=(CStrWithLength& other) { return *this = *this + other; }

        CStrWithLength& operator=(const CStrWithLength& other)
        {
            CStrWithLength* self = this;
            if (self == &other) return *this;
            if (getAutoRelease()) release();
            memcpy(self, &other, sizeof(CStrWithLength));
            return *this;
        }


    private:
        union
        {
            struct
            {
                PackedLengthBool lengthAndAutoRelease;
                char* str;
            };
            struct
            {
                unsigned char SSOSizeAndAutoRelease;
                char SSO[15];
            };
        };

    protected:
        constexpr static unsigned char pLastBitMask = 0b10000000;
        constexpr static uint64_t pFlagMask = 0x4000000000000000;
        constexpr static unsigned char pSSOSizeMask = 0b00111111;
        constexpr static unsigned char pSSOFlag = 0x40;
        constexpr static uint8_t SSOSize = 0xF;
    protected:

        __forceinline bool isSSO() const { return (SSOSizeAndAutoRelease & pSSOFlag) != 0; }
        __forceinline size_t getLength() const
        {
            if (isSSO())
				return SSOSizeAndAutoRelease & pSSOSizeMask;
            else
                return lengthAndAutoRelease.getLength();
        }
        __forceinline bool getAutoRelease() const
		{
			if (isSSO())
				return SSOSizeAndAutoRelease & pLastBitMask;
			else
				return lengthAndAutoRelease.getBool();
		}
        __forceinline void release() 
        {
            if (!isSSO())
            {
                lengthAndAutoRelease.setLength(0);
                if (str == nullptr) return;
                delete[] str;
                str = nullptr;
            }
            else
            {

                memset(SSO, 0, SSOSize);
                SSOSizeAndAutoRelease ^= pSSOSizeMask; // xor with 0b00111111 to set the size to 0 but leave the flag bits aone
            }
        }
        __forceinline char* getRawData()
        {
            if (isSSO())
                return &SSO[0];
            else
                return str;
        }
        __forceinline const char* getRawData() const
        {
            if (isSSO())
                return &SSO[0];
            else
                return str;
        }
        
        __forceinline void setLength(size_t length)
		{
			if (isSSO())
				SSOSizeAndAutoRelease = pSSOFlag | (length & pSSOSizeMask) | (SSOSizeAndAutoRelease & pLastBitMask);
			else
				lengthAndAutoRelease.setLength(length);
		}

        __forceinline void setAutoRelease(bool autoRelease)
        {
            char autoReleaseBit = autoRelease ? pLastBitMask : 0;

            if (isSSO())
                SSOSizeAndAutoRelease |= autoReleaseBit;
            else
                lengthAndAutoRelease.setBool(autoRelease);
        }

        
        friend struct CStrWithLengthHasher; // for hashing
    };

    struct CStrWithLengthHasher
    {
        std::size_t operator()(CStrWithLength& k) const
        {
			std::hash<std::string_view> hasher;
			return hasher(std::string_view(k.c_str(), k.size()));
        }
    };
    bool containsUppercase(const std::string& str);
    bool containsUppercase(const char* str);
    bool containsUppercase(std::string_view str);
    bool containsUppercase(CStrWithLength str);

    void toLowercase(std::string& str); 
    void toLowercase(char* str);
    void toLowercase(std::string_view str);
    void toLowercase(CStrWithLength str);




} // namespace Uranium
