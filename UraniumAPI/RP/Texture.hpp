#pragma once
#include <string>
#include "../Utils/Logger/Logger.hpp"
#include "../Utils/strUtil.hpp"
#include <string>

namespace Uranium
{
    struct Color
    {
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
        Color(uint64_t data) : data(data) {}
        Color() = default;
        union 
        {
            uint64_t data = 0;
            struct
            {
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
        };
	};

    class Texture
    {
    public:
        Texture(CStrWithLength path) : m_path(path) {}
        Texture(bool) : m_isPath(false) {} // Unused param just for the constructor to be different
        Texture() = default;
        ~Texture()
        {
            if (data != nullptr)
            {
				delete[] data;
			}
        }

        void LoadToMemory();

        void WriteToLocation(CStrWithLength path);

        void inline SetPixel(size_t x, size_t y, Color color)
        {
            if (m_isPath)
                LoadToMemory();
            size_t maxsize = m_width * m_height * m_channels;
            size_t index = (x + y * m_width) * m_channels;
            if (index < maxsize)
            {
                memcpy(&data[index], &color, sizeof(Color));
                return; // Success
            }
            // Failed
            Logs::Logger::Error("Failed to set pixel at: ({}, {})", x, y);
        }

        inline Color GetPixel(size_t x, size_t y)
        {
            if (m_isPath)
                LoadToMemory();
			size_t maxsize = m_width * m_height * m_channels;
			size_t index = (x + y * m_width) * m_channels;
            if (index < maxsize)
            {
				return *(Color*)&data[index];
			}
			// Failed
			Logs::Logger::Error("Failed to get pixel at: ({}, {})", x, y);
			return Color(); // Just to keep the compiler happy
		}

		inline void SetData(unsigned char* data, int width, int height, int channels)
		{
			m_width = width;
			m_height = height;
			m_channels = channels;
			this->data = data;
            m_isPath = false;
		}

        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }
        inline int GetChannels() const { return m_channels; }

        CStrWithLength GetOutputLocation() const { return ""; }

        // More compilex writters
        void DrawLine(int x1, int y1, int x2, int y2, Color color);
        void DrawRect(int x, int y, int width, int height, Color color);
        void FillRect(int x, int y, int width, int height, Color color);
        void DrawCircle(int x, int y, int radius, Color color);
        void FillCircle(int x, int y, int radius, Color color);



        CStrWithLength GetPath() { return m_path; }
    private:
        CStrWithLength m_path;

        uint16_t m_width = 0;
        uint16_t m_height = 0;
        uint16_t m_channels = 0;

        bool m_isPath = true;

        unsigned char* data = nullptr;
    };

    class BlockTexture : public Texture
    {
    public:
        BlockTexture(CStrWithLength path) : Texture(path) {}
        BlockTexture(bool) : Texture(false) {} // Unused param just for the constructor to be different
        BlockTexture() = default;

    private:
    };

    
}