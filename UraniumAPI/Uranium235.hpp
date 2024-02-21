#pragma once
#include "Utils/rapidProxy.hpp"
#include "RP/TextureRegistery.hpp"
#include "Utils/strUtil.hpp"
#include "uuidv4/uuid_v4.h"
#include "ProjectConfig.hpp"
#include "Block/Block.hpp"
#include "Entity/Entity.hpp"
#include <unordered_map>

namespace Uranium
{
	class Uranium235
	{
	private:
		UUIDv4::UUIDGenerator<std::mt19937_64> m_UUIDGenerator;
		ProjectSettings* m_ProjectSettings;
		//TextureRegistery m_TextureRegistery;
	private:
		std::unordered_map<CStrWithLength, Blocks::Block*, CStrWithLengthHasher> m_BlockRegistery;
		std::unordered_map<CStrWithLength, Entities::Entity*, CStrWithLengthHasher> m_EntityRegistery;

	private:
	public:
		Uranium235(
			ProjectSettings* projectSettings
		);

		ProjectSettings* getProjectSettings() const { return m_ProjectSettings; }

		~Uranium235()
		{
			delete m_ProjectSettings; // I hope to god this doesnt cause any memory issues with other threads
		}
	};
}