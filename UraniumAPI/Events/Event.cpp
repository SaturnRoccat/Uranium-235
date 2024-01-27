#include "Event.hpp"
#pragma warning(disable: 4267)
#pragma warning(disable: 6387)

namespace Uranium::Events
{
	// Events Implementation
	void FixedResponseEvent::CompileEvent(RapidProxy::DefaultValueWriter writer)
	{
		// Pulls down the data and allocator
		DVAP()

		rapidjson::Value eventObject(rapidjson::kObjectType);

		for (auto& e : this->m_eventComponents)
			e->CompileComponent({ &eventObject, allocator });
		
		RJ_SAFE_STL_S(m_eventIdentifier)
		data->AddMember(m_eventIdentifierCstr, eventObject, allocator);
	}

	void SequencedEvent::CompileEvent(RapidProxy::DefaultValueWriter writer)
	{
		// Pulls down the data and allocator
		DVAP()
		rapidjson::Value sequenceArray(rapidjson::kArrayType);

		for (auto& e : this->m_eventSequence)
		{
			rapidjson::Value sequenceStep(rapidjson::kObjectType);
			e.CompileEventSequencePart({ &sequenceStep, allocator });
			sequenceArray.PushBack(sequenceStep, allocator);
		}
		data->AddMember("sequence", sequenceArray, allocator);
	}

	void RandomEvent::CompileEvent(RapidProxy::DefaultValueWriter writer)
	{
		// Pulls down the data and allocator
		DVAP()
		rapidjson::Value sequenceArray(rapidjson::kArrayType);
		for (auto& e : this->m_eventChance)
		{
			rapidjson::Value sequenceStep(rapidjson::kObjectType);
			e.CompileEventChancePart({ &sequenceStep, allocator });
			sequenceArray.PushBack(sequenceStep, allocator);
		}
		data->AddMember("randomize", sequenceArray, allocator);
	}

	// The cancer that is handling everything else

	void SequencedEvent::SequencedEventPart::CompileEventSequencePart(RapidProxy::DefaultValueWriter writer)
	{
		// Pulls down the data and allocator
		DVAP()
		if (this->condition != "")
		{
			auto conditionStr = this->condition;
			RJ_SAFE_STL_S(conditionStr)
			data->AddMember("condition", conditionStrCstr, allocator);
		}

		if (std::holds_alternative<FixedResponseEvent*>(this->eventSequencePart))
		{
			FixedResponseEvent* e = std::get<FixedResponseEvent*>(this->eventSequencePart);
			e->CompileEvent(writer);
		}
		else // This is the brach i REALLY dont wanna implement cuz its painful
		{
			EventDataPair e = std::get<EventDataPair>(this->eventSequencePart); 
			rapidjson::Value triggerObject(rapidjson::kObjectType);
			auto targetStr = e.GetTarget();
			RJ_SAFE_STL_S(targetStr)
			triggerObject.AddMember("target", targetStrCstr, allocator);
			auto eventStr = e.GetEventID();
			RJ_SAFE_STL_S(eventStr)
			triggerObject.AddMember("event", eventStrCstr, allocator);
			data->AddMember("trigger", triggerObject, allocator);
		}
	}

	void RandomEvent::EventChancePart::CompileEventChancePart(
		RapidProxy::DefaultValueWriter writer)
	{
		// Pulls down the data and allocator
		DVAP()
		data->AddMember("weight", this->weight, allocator); 
		if (std::holds_alternative<FixedResponseEvent*>(this->eventSequencePart))
		{
			FixedResponseEvent* e = std::get<FixedResponseEvent*>(this->eventSequencePart);
			e->CompileEvent(writer);
		}
		else // This is the brach i REALLY dont wanna implement cuz its painful
		{
			EventDataPair e = std::get<EventDataPair>(this->eventSequencePart);
			rapidjson::Value triggerObject(rapidjson::kObjectType);
			auto targetStr = e.GetTarget();
			RJ_SAFE_STL_S(targetStr)
			triggerObject.AddMember("target", targetStrCstr, allocator);
			auto eventStr = e.GetEventID();
			RJ_SAFE_STL_S(eventStr)
			triggerObject.AddMember("event", eventStrCstr, allocator);
			data->AddMember("trigger", triggerObject, allocator);
		}
	}
}

#pragma warning(default: 6387)
#pragma warning(default: 4267)