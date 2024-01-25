#include "TextureRegistery.hpp"



namespace Uranium
{
	void TextureRegistery::WriteBlockTextures()
	{
		for (auto& [name, Texture] : m_BlockMap)
		{
			Texture.WriteToLocation(m_RPPath + "textures/blocks/" + name + ".png");
		}
	}
}