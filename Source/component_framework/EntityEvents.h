#ifndef __EVENT_TYPE_H__
#define __EVENT_TYPE_H__

#include "ComponentType.h"
#include "Component.h"
#include "Event.h"

namespace SGF
{
	enum
	{
		EVENT_CREATE_ENTITY		= 1,
		EVENT_DESTROY_ENTITY	= 2,
		EVENT_INSERT_COMPONENT	= 3,
		EVENT_REMOVE_COMPONENT	= 4
	};

	struct EntityEvent : public IEventData 
	{
		EntityEvent( EventType _evtType, Entity *_e ) : IEventData(_evtType), e(_e) {}
		Entity *e;
	};

	struct EntityComponentEvent : public IEventData
	{
		EntityComponentEvent( EventType _evtType, Entity *_e, Component* comp, ComponentType _type, unsigned int _typebits ) 
			: IEventData(_evtType), e(_e), type(_type), typebits( _typebits ) 
		{
			mComp = comp;
		}

		Entity *e;
		Component* mComp;
		ComponentType type;
		unsigned int  typebits;
	};
};

#endif