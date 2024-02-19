#pragma once
#include "../Utils/rapidProxy.hpp" // The fact that this file has to exist is a crime against humanity
#include <string>
#include "../Utils/strUtil.hpp"
#include "../Component/EventComponent/EventComponent.hpp"
#include <vector>
#include <variant>
#include <type_traits>
/*
* What are you doing in this file?
* This file is a mess
* Please save yourself and don't look at it
* If you do, I'm sorry
* Forgive me for i have sinned
* If this code is broken, Good luck fixing it (me in the future)
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
*/
#pragma warning(disable: 4267)
#pragma warning(disable: 6387)
namespace Uranium::Events
{
	template<typename T>
	concept IsEventComponent = std::is_base_of<Components::Events::EventComponent, T>::value;
	class Event
	{
	public:
		Event(CStrWithLength name) : m_eventIdentifier(name) {};
		virtual void cleanUp() {};

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) = 0;
	public:
		CStrWithLength m_eventIdentifier;
	};

	template<IsEventComponent T>
	class FixedResponseEvent : public Event
	{
		public:
		FixedResponseEvent(CStrWithLength name) : Event(name) {};

		void AddEventComponent(Components::Events::EventComponent* const component) { m_eventComponents.push_back(component); };

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) override
		{
			// Pulls down the data and allocator
			DVAP()

				rapidjson::Value eventObject(rapidjson::kObjectType);

			for (auto& e : this->m_eventComponents)
				e->CompileComponent({ &eventObject, allocator });

			
		    data->AddMember(m_eventIdentifier, eventObject, allocator);
		}

	private:
		std::vector<T*> m_eventComponents;
	};

	class EventDataPair {
	public:
		EventDataPair(CStrWithLength target, CStrWithLength eventID)
			: target(target), eventID(eventID) {}

		CStrWithLength GetTarget() { return target; }
		CStrWithLength GetEventID() { return eventID; }

	private:
		CStrWithLength target;
		CStrWithLength eventID;
	};

	template<IsEventComponent T>
	class EventPart
	{
	public:
		EventPart(EventDataPair& dataPair)
			: eventSequencePart(dataPair) {}

		EventPart(FixedResponseEvent<T>* evnt)
			: eventSequencePart(evnt) {}
	public:
		std::variant<EventDataPair, FixedResponseEvent<T>*> eventSequencePart;
	};

	template<IsEventComponent T>
	class SequencedEvent : public Event
	{
	public:
		template<IsEventComponent U>
		class SequencedEventPart : protected EventPart<U> {
		public:
			SequencedEventPart(EventDataPair* dataPair, CStrWithLength condition = "")
				: EventPart(*dataPair), condition(condition) {
				delete dataPair;
			}
			SequencedEventPart(EventDataPair& dataPair, CStrWithLength condition = "")
				: EventPart(dataPair), condition(condition) {
			}

			SequencedEventPart(FixedResponseEvent<U>* const event, CStrWithLength condition)
				: EventPart(event), condition(condition) {}

			void CompileEventSequencePart(RapidProxy::DefaultValueWriter writer)
			{
				// Pulls down the data and allocator
				DVAP()
					if (this->condition != "")
					{
						data->AddMember("condition", condition, allocator);
					}

				if (std::holds_alternative<FixedResponseEvent*>(this->eventSequencePart))
				{
					FixedResponseEvent<U>* e = std::get<FixedResponseEvent<U>*>(this->eventSequencePart);
					e->CompileEvent(writer);
				}
				else // This is the brach i REALLY dont wanna implement cuz its painful
				{
					EventDataPair e = std::get<EventDataPair>(this->eventSequencePart);
					rapidjson::Value triggerObject(rapidjson::kObjectType);
					auto targetStr = e.GetTarget();
				    triggerObject.AddMember("target", targetStr, allocator);
					auto eventStr = e.GetEventID();
					triggerObject.AddMember("event", eventStr, allocator);
					data->AddMember("trigger", triggerObject, allocator);
				}
			}

		private:
			CStrWithLength condition;
		};
	public:
		SequencedEvent(CStrWithLength name) : Event(name) {};

		void AddEventSequencePart(SequencedEventPart<T>& part) { m_eventSequence.push_back(part); };
		void AddEventSequencePart(SequencedEventPart<T>* const part) { 
			m_eventSequence.push_back(*part); 
			delete part;
		};

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) override
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
	private:
		std::vector<SequencedEventPart<T>> m_eventSequence;
	};

	template<IsEventComponent T>
	class RandomEvent : public Event
	{
	public:
		template<IsEventComponent U>
		class EventChancePart : private EventPart<U>
		{
		public:
			EventChancePart(
				size_t weight,
				const EventPart<U>& eventPartData)
				: weight(weight), EventPart(eventPartData) {}

			EventChancePart(
				size_t weight,
				FixedResponseEvent<U>* event)
				: weight(weight), EventPart(event) {}


			void CompileEventChancePart(RapidProxy::DefaultValueWriter writer)
			{
				// Pulls down the data and allocator
				DVAP()
					data->AddMember("weight", this->weight, allocator);
				if (std::holds_alternative<FixedResponseEvent<U>*>(this->eventSequencePart))
				{
					FixedResponseEvent* e = std::get<FixedResponseEvent<U>*>(this->eventSequencePart);
					e->CompileEvent(writer);
				}
				else // This is the brach i REALLY dont wanna implement cuz its painful
				{
					EventDataPair e = std::get<EventDataPair>(this->eventSequencePart);
					rapidjson::Value triggerObject(rapidjson::kObjectType);
					auto targetStr = e.GetTarget();
					triggerObject.AddMember("target", targetStr, allocator);
					auto eventStr = e.GetEventID();
					triggerObject.AddMember("event", eventStr, allocator);
					data->AddMember("trigger", triggerObject, allocator);
				}
			}
		private:
			size_t weight;
		};
	public:
		RandomEvent(CStrWithLength name) : Event(name) {};

		void AddEventChancePart(EventChancePart<T>& part) { m_eventChance.push_back(part); };
		void AddEventChancePart(EventChancePart<T>* const part) { m_eventChance.push_back(*part); delete part; };

		// Allows the user to decide whether or not the event should be deleted after being added to the event

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) override
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
	private:
		std::vector<EventChancePart<T>> m_eventChance;
	};
}
#pragma warning(default: 6387)
#pragma warning(default: 4267)