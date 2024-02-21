#include "Uranium235.hpp"

namespace Uranium
{
	Uranium235::Uranium235(ProjectSettings* settings) : m_ProjectSettings(settings), m_UUIDGenerator()//, m_TextureRegistery(
		//settings->outputLocation + settings->name + "_RP/"
	//)
	{
		Logs::Logger::Info("Starting Uranium 235");
	}
}