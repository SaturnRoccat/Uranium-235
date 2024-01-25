#pragma once
#include "Texture.hpp"
#include <unordered_map>

namespace Uranium
{
    enum class TextureType
    {
        Block,
        Entity,
        Item,
        GUI,
    };

    class TextureRegistery
    {
    public:
        TextureRegistery(const std::string& RP) : m_RPPath(RP) {}

        void inline RegisterTexture(const std::string& name, const std::string& path, TextureType type)
        {
            if (!StrUtils::isPath(path))
            {
                Logs::Logger::Error("If you are trying to register a texture from a string that isnt a path, Use a Texture object instead!");
                return;
			}
            switch (type)
            {
            case TextureType::Block:
                m_BlockMap[name] = Texture(path);
                break;
            case TextureType::Entity:
                m_EntityMap[name] = Texture(path);
                break;
            case TextureType::Item:
                m_ItemMap[name] = Texture(path);
                break;
            case TextureType::GUI:
                m_GUIMap[name] = Texture(path);
                break;
            }
        }

        void inline RegisterTexture(const std::string& name, Texture& texture, TextureType type)
        {
            switch (type)
            {
			case TextureType::Block:
				m_BlockMap[name] = texture;
				break;
			case TextureType::Entity:
				m_EntityMap[name] = texture;
				break;
			case TextureType::Item:
				m_ItemMap[name] = texture;
				break;
			case TextureType::GUI:
				m_GUIMap[name] = texture;
				break;
			}
		}

        Texture inline GetTexture(const std::string& name, TextureType type)
        {
            switch (type)
            {
            case TextureType::Block:
                return m_BlockMap[name];
            case TextureType::Entity:
                return m_EntityMap[name];
            case TextureType::Item:
                return m_ItemMap[name];
            case TextureType::GUI:
                return m_GUIMap[name];
            }
        }

        void WriteBlockTextures();

    private:
        std::unordered_map<std::string, Texture> m_BlockMap;
        std::unordered_map<std::string, Texture> m_EntityMap;
        std::unordered_map<std::string, Texture> m_ItemMap;
        std::unordered_map<std::string, Texture> m_GUIMap;

        std::string m_RPPath;
        std::string m_Prefix;

        class StrUtils
        {
        public:
            static bool isPath(const std::string& path)
            {
                // Look for a / or \ in the string
                return path.find('/') != std::string::npos || path.find('\\') != std::string::npos;
			}

            static std::string GetFileName(const std::string& path)
            {
                // Find end of string
                size_t end = path.size() - 1;
                // Walk backwards until you find a / or a \ or you reach the start of the string
                size_t start = 0;
                for (size_t i = path.size() - 1; i > 0; i--)
                {
                    if (path[i] == '/' || path[i] == '\\')
                    {
                        start = i + 1;
						break;
                    }
                }
                return path.substr(start, end - start);
            }
        };
    };
}