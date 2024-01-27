#pragma once
#include "../Component.hpp"

namespace Uranium::Components::Events
{
	class EventComponent : public Component
	{
	public:
		EventComponent(const char* eventComponentIdentifier) : Component(eventComponentIdentifier) {}
		/*Never Invoke This Function This is an internal Stub*/
		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer, ProjectSettings* settings) {};

		/*Dont invoke this just let the compilation step handle it*/
		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer) = 0;
	};
#ifdef ENABLE_TEST_COMPONENTS
	class TestEventComponentOne : public EventComponent
	{
	public:
		TestEventComponentOne() : EventComponent("TestEventComponent") {}
		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP();
			data->AddMember("TestEventComponent", "TestEventComponent", allocator);
		}
	};
	class TestEventComponentTwo : public EventComponent
	{
	public:
		TestEventComponentTwo() : EventComponent("TestEventComponentTwo") {}
		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP();
			data->AddMember("TestEventComponentTwo", 1, allocator);
		}
	};
	class TestEventComponentThree : public EventComponent
	{
	public:
		TestEventComponentThree() : EventComponent("TestEventComponentThree") {}
		virtual void CompileComponent(RapidProxy::DefaultValueWriter writer) override
		{
			DVAP();
			data->AddMember("TestEventComponentThree", 1.0f, allocator);
		}
	};
#endif
}