#pragma once
#include "../Component.hpp"

namespace Uranium::Components 
{

    // Just a base class for all block components to inherit from just searving as a way to group them together
    class BlockComponent : public Component
    {
    public:
        BlockComponent(const std::string& name, const std::vector<Experimentals>& reqiredExperiments = {}) : Component(name, reqiredExperiments) {}

        virtual void CompileComponent(RapidProxy::DefaultValueWriter ValueWriter, ProjectSettings* settings) = 0; 
    };

}