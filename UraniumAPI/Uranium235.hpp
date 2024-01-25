#pragma once
#include "Utils/rapidProxy.hpp"
#include "RP/TextureRegistery.hpp"
#include "uuidv4/uuid_v4.h"
#include "ProjectConfig.hpp"
#include "Block/Block.hpp"

namespace Uranium
{
	class Uranium235
	{
	private:
		UUIDv4::UUIDGenerator<std::mt19937_64> m_UUIDGenerator;
		ProjectSettings* m_ProjectSettings;
		TextureRegistery m_TextureRegistery;
	private:
	public:
		Uranium235(
			ProjectSettings* projectSettings
		);

		~Uranium235()
		{
			delete m_ProjectSettings; // I hope to god this doesnt cause any memory issues with other threads
		}
	};
}