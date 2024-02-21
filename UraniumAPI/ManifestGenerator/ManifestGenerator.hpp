#pragma once
#include <uuidv4/uuid_v4.h>
#include "../Utils/rapidProxy.hpp"
#include "../Utils/Logger/Logger.hpp"
#include "../Utils/NonOwningPointer.hpp"
#include "../Utils/macros.hpp"
#include "../ProjectConfig.hpp"
#include <string>

namespace Uranium
{
	namespace Manifest
	{
		class Header
		{
		public:
			Header(
				CStrWithLength name,
				CStrWithLength description,
				NonOwningPointer<ProjectSettings> projectSettings,
				Version minEngineVersion = LATEST_FORMAT_VERSION,
				Version baseGameVersion = LATEST_FORMAT_VERSION
			)
			{
					this->m_name = name;
					this->m_description = description;
					this->m_minEngineVersion = minEngineVersion;
					this->m_baseGameVersion = baseGameVersion;
					this->uuid = projectSettings.Get()->getUUID()->getUUID();
					this->m_version = projectSettings.Get()->version;
			}

			void WriteHeader(RapidProxy::DefaultValueWriter dvw);

			// Getters
			CStrWithLength getName() const { return m_name; }
			CStrWithLength getDescription() const { return m_description; }
			const Version& getMinEngineVersion() const { return m_minEngineVersion; }
			const Version& getBaseGameVersion() const { return m_baseGameVersion; }
			const Version& getVersion() const { return m_version; }
			const UUIDv4::UUID& getUUID() const { return uuid; }

		private:
			Version m_baseGameVersion;
			Version m_version;
			Version m_minEngineVersion;
			CStrWithLength m_description;
			CStrWithLength m_name;
			UUIDv4::UUID uuid;
		};

		class Module
		{
		public:
			Module(
				CStrWithLength type,
				Version version,
				NonOwningPointer<ProjectSettings> projectSettings,
				CStrWithLength language
				)
			{
				this->m_type = type;
				this->m_version = version;
				this->m_description = projectSettings.Get()->description;
				this->language = language;
				this->m_version = projectSettings.Get()->version;
				this->uuid = projectSettings.Get()->getUUID()->getUUID();
			}

			void WriteModule(RapidProxy::DefaultValueWriter dvw);

			// Getters
			CStrWithLength getDescription() const { return m_description; }
			CStrWithLength getType() const { return m_type; }
			const Version& getVersion() const { return m_version; }
			CStrWithLength getLanguage() const { return language; }
			const UUIDv4::UUID& getUUID() const { return uuid; }
		private:
			CStrWithLength m_description;
			CStrWithLength m_type;
			UUIDv4::UUID uuid;
			Version m_version;
			CStrWithLength language;
		};

		class Dependencies
		{
		public:
			Dependencies(
				UUIDv4::UUID& uuid,
				Version version
				)
			{
				this->uuid = uuid;
				this->m_version = version;
			}

			void WriteDependencie(RapidProxy::DefaultValueWriter dvw);

			// Getters
			const UUIDv4::UUID& getUUID() const { return uuid; }
			const Version& getVersion() const { return m_version; }
		private:
			UUIDv4::UUID uuid;
			Version m_version;

		};

		enum class Capabilities
		{
			CHEMISTRY,
			EDITOREXTENSIONS,
			EXPERIMENTALCUSTOMUI,
			RAYTRACED
		};

		constexpr static const char* capabilitiesToString(Capabilities cap)
		{
			switch (cap)
			{
			case Capabilities::CHEMISTRY:
				return "chemistry";
			case Capabilities::EDITOREXTENSIONS:
				return "editor_extensions";
			case Capabilities::EXPERIMENTALCUSTOMUI:
				return "experimental_custom_ui";
			case Capabilities::RAYTRACED:
				return "raytraced";
			default:
				return "unknown";
			}
		}

		class Metadata
		{
		public:
			Metadata(
				NonOwningPointer<ProjectSettings> projectSettings,
				CStrWithLength license = "",
				CStrWithLength url = ""
			)
			{
				this->m_license = license;
				this->m_url = url;
				this->m_author = projectSettings.Get()->author;
			}

			void WriteMetadata(RapidProxy::DefaultValueWriter dvw);

			void inline addGeneratedWith(CStrWithLength name, Version version)
			{
				m_generatedWith.push_back(std::make_pair(name, version));
			}

			// Getters
			CStrWithLength getLicense() const { return m_license; }
			CStrWithLength getURL() const { return m_url; }
			CStrWithLength getAuthor() const { return m_author; }
			const std::vector<std::pair<CStrWithLength, Version>>& getGeneratedWith() const { return m_generatedWith; }

		private:
			CStrWithLength m_license;
			CStrWithLength m_url;
			CStrWithLength m_author;
			std::vector<std::pair<CStrWithLength, Version>> m_generatedWith = {
				{ "Uranium", CURRENT_URANIUM_VERSION}
			};
		};
	
		class Manifest
		{
		public:
			Manifest(
				Header& header,
				Module& _module,
				Dependencies& dependencies,
				Metadata& metadata
			) : m_header(header), m_module(_module), m_dependencies(dependencies), m_metadata(metadata) {}
			virtual void GenerateManifest(NonOwningPointer<ProjectSettings> projectSettings) = 0;
		private:
			Header m_header;
			Module m_module;
			Dependencies m_dependencies;
			Metadata m_metadata;
			size_t formatVersion = 2;
		};
	}
}