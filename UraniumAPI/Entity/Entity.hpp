#include "../Utils/rapidProxy.hpp"
#include "../Component/EntityComponents/EntityComponent.hpp"
#include <vector>

namespace Uranium::Entities
{
	struct EntitySettings
	{
		bool isSummonable = true;
		bool isSpawnable = true;
		bool isExperimental = false;
	};
	class Entity
	{
	public:
	private:
		std::vector<Components::Entity::EntityComponent*> m_bpComponents;
		EntitySettings m_settings;
		Version m_formatVersion;
	};
}