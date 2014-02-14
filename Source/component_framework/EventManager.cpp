#include "EventManager.h"
#include <assert.h>

using namespace SGF;

EventManager::EventManager()
{

}

EventManager::~EventManager()
{
	RemoveAllListeners();
}

void EventManager::AddListener( IEventListener *listener, EventType type )
{
	// Add the type to the listeners map if it doesn't exist.
	EventListenerMap::iterator it = _listeners.find( type );
	if( it == _listeners.end() ) 
		it = _listeners.insert( EventListenerMapPair( type, EventListenerList() ) ).first;

	EventListenerList &list = it->second;

#ifdef _DEBUG
	// Check for duplicates.
	for( EventListenerList::const_iterator it=list.begin(); it != list.end(); ++it )
	{
		if( *it == listener )
			assert("Duplicate event listener registered!");
	}
#endif

	// Add the listener to the map.
	list.push_back(listener);
}

void EventManager::RemoveListener( IEventListener *listener, EventType type )
{
	EventListenerMap::iterator it = _listeners.find(type);
	if( it != _listeners.end() )
	{
		EventListenerList &list = it->second;
		for( EventListenerList::iterator it2 = list.begin(); it2 != list.end(); ++it2 )
		{
			if( *it2 == listener )	
			{
				list.erase(it2);
				break;
			}
		}
	}
}

void EventManager::AddGlobalListener( IEventListener *listener )
{
#ifdef _DEBUG
	for( EventListenerList::iterator it = _globals.begin(); it != _globals.end(); ++it )
	{
		if( *it == listener )
			assert("Duplicate global listener registered!");
	}
#endif

	_globals.push_back(listener);
}

void EventManager::RemoveGlobalListener( IEventListener *listener )
{
	EventListenerList &list = _globals;
	for( EventListenerList::iterator it2 = list.begin(); it2 != list.end(); ++it2 )
	{
		if( *it2 == listener )	
		{
			_globals.erase(it2);
			return;
		}
	}
}

void EventManager::RemoveAllListeners()
{
	for( EventListenerMap::iterator it = _listeners.begin(); it != _listeners.end(); ++it )
		it->second.clear();

	_listeners.clear();
	_globals.clear();
}

bool EventManager::FireEvent( IEventData const &evt ) const
{
	// Notify all global listeners.
	for( EventListenerList::const_iterator it = _globals.begin(); it != _globals.end(); ++it )
	{
		if( (*it)->HandleEvent( evt ) )
			return true;
	}

	EventListenerMap::const_iterator it = _listeners.find( evt.GetType() );
	if( it != _listeners.end() )
	{
		const EventListenerList &list = it->second;
		for( EventListenerList::const_iterator it2 = list.begin(); it2 != list.end(); ++it2 )
		{
			if( (*it2)->HandleEvent( evt ) ) 
				return true;
		}
	}

	return false;
}