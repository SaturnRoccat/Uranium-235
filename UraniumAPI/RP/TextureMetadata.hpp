#pragma once
#include "../Utils/strUtil.hpp"
#include <string>

namespace Uranium
{
	class TextureMetadata
	{
	public:
		TextureMetadata(CStrWithLength path = "NULL") : m_path(path) {}
		inline bool isNull() const { return m_path == "NULL"; }
	private:
		CStrWithLength m_path;
	};


	class BlockTextureMetadata : public TextureMetadata
	{
	public:
		BlockTextureMetadata(CStrWithLength path = "NULL", bool usesCustomModel = false) : m_usesCustomModel(usesCustomModel), TextureMetadata(path) {}
		bool UsesCustomModel() const { return m_usesCustomModel; }
		void SetCustomModel(class Geometry* geometry) { m_geometry = geometry; m_usesCustomModel = true; }
		class Geometry* GetCustomModel() const { return m_geometry; }
		void DisableCustomModel() { m_usesCustomModel = false; m_geometry = nullptr; }


	private:
		class Geometry* m_geometry = nullptr;
		bool m_usesCustomModel = false;
	};
}