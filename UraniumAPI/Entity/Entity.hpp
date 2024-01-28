#include "../Utils/rapidProxy.hpp"
#include "../Component/EntityComponents/EntityComponent.hpp"
#include "../Events/Event.hpp"
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

		bool hasRuntimeIdentifier() const { return m_runtimeIdentifier != ""; };
	private:
		std::vector<Components::Entity::EntityComponent*> m_bpComponents;
		std::vector<Events::Event*> m_bpEvents;
		EntitySettings m_settings;
		Version m_formatVersion;
		std::string m_runtimeIdentifier = "";
	};
}