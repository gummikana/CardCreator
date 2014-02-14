#ifndef __EVENT_SYSTEM_H__
#define __EVENT_SYSTEM_H__

#include "Event.h"
#include <map>
#include <list>

namespace SGF
{
	class EventManager {
	public:
		EventManager();
		virtual ~EventManager();

		// Add a listener for a given event type.
		void AddListener( IEventListener *listener, EventType type );
		
		// Remove a listener for a given event type.
		void RemoveListener( IEventListener *listener, EventType type );

		// Add a global listener. This type of listener receives all types of events.
		// A listener that is registered as global as well as for event types will receive
		// duplicate events.
		void AddGlobalListener( IEventListener *listener );

		// Remove a global listener.
		void RemoveGlobalListener( IEventListener *listener );

		// Remove all listeners for all event types.
		void RemoveAllListeners();

		// Dispatch an event.
		bool FireEvent( IEventData const &evt ) const;

	protected:

		typedef std::list<IEventListener*> EventListenerList;
		typedef std::map< EventType, EventListenerList > EventListenerMap;
		typedef std::pair< EventType, EventListenerList > EventListenerMapPair;

		// A map of all the listeners connected to specific events.
		std::map< EventType, EventListenerList > _listeners;

		// A list of global event listeners. These listeners receive all events regardless of type.
		EventListenerList _globals;
	};

};

#endif