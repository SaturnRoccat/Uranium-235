#pragma once
#include "../Utils/rapidProxy.hpp" // The fact that this file has to exist is a crime against humanity
#include <string>
#include "../Component/EventComponent/EventComponent.hpp"
#include <vector>
#include <variant>
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
namespace Uranium::Events
{

	class Event
	{
	public:
		Event(const std::string& name) : m_eventIdentifier(name) {};

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) = 0;
	public:
		std::string m_eventIdentifier;
	};

	class FixedResponseEvent : public Event
	{
		public:
		FixedResponseEvent(const std::string& name) : Event(name) {};

		void AddEventComponent(Components::Events::EventComponent* const component) { m_eventComponents.push_back(component); };

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) override; 
	private:
		std::vector<Components::Events::EventComponent*> m_eventComponents;
	};

	class EventDataPair {
	public:
		EventDataPair(const std::string& target, const std::string& eventID)
			: target(target), eventID(eventID) {}

		std::string GetTarget() const { return target; }
		std::string GetEventID() const { return eventID; }

	private:
		std::string target;
		std::string eventID;
	};

	class EventPart
	{
	public:
		EventPart(EventDataPair& const dataPair) : eventSequencePart(dataPair) {}
		EventPart(
			FixedResponseEvent* event)
			: eventSequencePart(event) {}
	public:
		std::variant<EventDataPair, FixedResponseEvent*> eventSequencePart;
	};

	class SequencedEvent : public Event
	{
	public:
		class SequencedEventPart : protected EventPart {
		public:
			SequencedEventPart(EventDataPair* dataPair, const char* const condition = "")
				: EventPart(*dataPair), condition(condition) {
				delete dataPair;
			}
			SequencedEventPart(EventDataPair& dataPair, const char* const condition = "")
				: EventPart(dataPair), condition(condition) {
			}

			SequencedEventPart(FixedResponseEvent* const event, const char* condition)
				: EventPart(event), condition(condition) {}

			void CompileEventSequencePart(RapidProxy::DefaultValueWriter writer);

		private:
			std::string condition;
		};
	public:
		SequencedEvent(const std::string& name) : Event(name) {};

		void AddEventSequencePart(SequencedEventPart& const part) { m_eventSequence.push_back(part); };
		void AddEventSequencePart(SequencedEventPart* const part) { 
			m_eventSequence.push_back(*part); 
			delete part;
		};

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) override;
	private:
		std::vector<SequencedEventPart> m_eventSequence;
	};

	class RandomEvent : public Event
	{
	public:
		class EventChancePart : private EventPart
		{
		public:
			EventChancePart(
				size_t weight,
				const EventPart& eventPartData)
				: weight(weight), EventPart(eventPartData) {}

			EventChancePart(
				size_t weight,
				FixedResponseEvent* event)
				: weight(weight), EventPart(event) {}


			void CompileEventChancePart(RapidProxy::DefaultValueWriter writer);
		private:
			size_t weight;
		};
	public:
		RandomEvent(const std::string& name) : Event(name) {};

		void AddEventChancePart(EventChancePart& part) { m_eventChance.push_back(part); };
		void AddEventChancePart(EventChancePart* const part) { m_eventChance.push_back(*part); delete part; };

		// Allows the user to decide whether or not the event should be deleted after being added to the event

		virtual void CompileEvent(RapidProxy::DefaultValueWriter writer) override;
	private:
		std::vector<EventChancePart> m_eventChance;
	};
}