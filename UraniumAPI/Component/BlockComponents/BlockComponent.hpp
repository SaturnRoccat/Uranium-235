#pragma once
#include "../Component.hpp"

namespace Uranium::Components 
{

    // Just a base class for all block components to inherit from just searving as a way to group them together
    class BlockComponent : public Component
    {
    public:
        BlockComponent(const char* name, const unsigned char reqiredExperiments = 0) : Component(name)
        {
			m_reqiredExperimentsBitfield = reqiredExperiments;
        }

        virtual void CompileComponent(RapidProxy::DefaultValueWriter writer, ProjectSettings* settings) = 0;
        
        /*
        O(1) time complexity due to the fact that the bitfield is precomputed
        The compiler probably will optimize it into an index off the base class pointer
        */
        unsigned char GetReqiredExperimentsBitfield() const { return m_reqiredExperimentsBitfield; }
    private:
        unsigned char m_reqiredExperimentsBitfield;
    };

#ifdef ENABLE_TEST_COMPONENTS
    class TestBlockComponent : public BlockComponent
	{
	public:
		TestBlockComponent() : BlockComponent("TestBlockComponent")
		{
		}

		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer, ProjectSettings* settings) override
		{
            DVAP()
            data->AddMember("TestBlockComponent", "TestBlockComponent", allocator);
		}
	};

    class TestBlockComponent2 : public BlockComponent
	{
    public:
		TestBlockComponent2() : BlockComponent("TestBlockComponent2")
		{
		}

		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer, ProjectSettings* settings) override
		{
			DVAP()
			data->AddMember("TestBlockComponent2", 2, allocator);
		}
	};
#endif
}