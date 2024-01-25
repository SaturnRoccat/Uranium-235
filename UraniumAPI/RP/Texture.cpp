#include "Texture.hpp"
#include <fstream>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <filesystem>
#include "../Utils/Logger/Logger.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

namespace Uranium
{

    std::string pathToDir(const std::string& path)
    {
		std::string dir = "";
        // Walk backwards until you find a / or a \ or you reach the start of the string
        for (size_t i = path.size() - 1; i > 0; i--)
        {
            if (path[i] == '/' || path[i] == '\\')
            {
				dir = path.substr(0, i + 1);
				break;
			}
        }
		return dir;
	}

    void Texture::WriteToLocation(const std::string& path)
    {
        // If path doesnt exist create it
        std::string dir = pathToDir(path);
        if (!std::filesystem::exists(dir))
        {
			std::filesystem::create_directories(dir);
		}

        if (m_isPath)
        {
            // If its a path just copy the file
            std::ifstream src(m_path, std::ios::binary);
            std::ofstream dst(path, std::ios::binary);
            dst << src.rdbuf();

            return;
        }
        if (!stbi_write_png(path.c_str(), m_width, m_height, m_channels, data, m_width * m_channels))
        {
            Logs::Logger::Error("Failed to write to location: {}", path);
        }

    }

    void Texture::LoadToMemory()
    {
        m_isPath = false;
        data = stbi_load(m_path.c_str(), &m_width, &m_height, &m_channels, 0);
        if (!data)
        {
			Logs::Logger::Error("Failed to load texture: {}", m_path);
		}
	}

    void Texture::DrawCircle(int x, int y, int radius, Color color)
    {
        if (m_isPath)
            LoadToMemory();
        for (size_t i = 0; i < m_width; i++)
        {
            for (size_t j = 0; j < m_height; j++)
            {
                if (sqrt(pow(i - x, 2) + pow(j - y, 2)) <= radius)
                {
					SetPixel(i, j, color);
				}
			}
		}
	}

    void Texture::DrawRect(int x, int y, int width, int height, Color color)
    {
        if (m_isPath)
            LoadToMemory();
        for (size_t i = 0; i < m_width; i++)
        {
            for (size_t j = 0; j < m_height; j++)
            {
                if (i >= x && i <= x + width && j >= y && j <= y + height)
                {
					SetPixel(i, j, color);
				}
            }
        }
    }

    void Texture::DrawLine(int x1, int y1, int x2, int y2, Color color)
    {
        if (m_isPath)
            LoadToMemory();
        for (size_t i = 0; i < m_width; i++)
        {
            for (size_t j = 0; j < m_height; j++)
            {
                float distance = sqrt(pow(i - x1, 2) + pow(j - y1, 2)) + sqrt(pow(i - x2, 2) + pow(j - y2, 2));
                if (distance <= sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)))
                {
					SetPixel(i, j, color);
				}
			}
		}
	}

    void Texture::FillRect(int x, int y, int width, int height, Color color)
    {
        if (m_isPath)
            LoadToMemory();
        for (size_t i = 0; i < m_width; i++)
        {
            for (size_t j = 0; j < m_height; j++)
            {
                if (i >= x && i <= x + width && j >= y && j <= y + height)
                {
                    SetPixel(i, j, color);
                }
            }
        }
    }

    void Texture::FillCircle(int x, int y, int radius, Color color)
    {
        if (m_isPath)
            LoadToMemory();
        for (size_t i = 0; i < m_width; i++)
        {
            for (size_t j = 0; j < m_height; j++)
            {
                if (sqrt(pow(i - x, 2) + pow(j - y, 2)) <= radius)
                {
                    SetPixel(i, j, color);
                }
            }
        }

    }

}