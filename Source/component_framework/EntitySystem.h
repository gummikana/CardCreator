#ifndef __COMPONENT_SYSTEM_H__
#define __COMPONENT_SYSTEM_H__

#include <set>
#include <iostream>

#include "Event.h"
#include "Entity.h"
#include "ComponentType.h"

namespace SGF
{
class Entity;
class EntityManager;
class Component;
class EventManager;

class EntitySystem : public IEventListener {
public:
	EntitySystem( EventManager *eventManager, EntityManager *entityManager, unsigned int typeBits = 0 );
	virtual ~EntitySystem();

	virtual void SetName( const std::string& name ) { mSystemName = name; }
	virtual const std::string& GetName() const		{ return mSystemName; }

	// Event handler for the system.
	virtual bool HandleEvent( IEventData const &evt );

	// The processing function that handles calling the pure virtual methods.
	void Process( float dt );

	// this was added so that systems could be used to collect certain types of entities
	// this is used e.g. in collision detection, when we need to check bullets agains hitboxes

	const std::set< Entity* >&	GetActivesConst() const;

	EntityManager*				GetEntityManager() { return _entManager; }

protected:

	// Should we update?
	virtual bool CheckProcessing() = 0;

	// A preprocessing update call.
	virtual void Begin() = 0;

	// A post processing update call.
	virtual void End() = 0;

	// Any implementing entity system must implement this method and the logic
	// to process the given entities of the system.
	virtual void ProcessEntities( EntityManager *manager, const std::set<Entity*> &entities ) = 0;

	template< class T >
	void AddComponentType() {
		AddComponent( GetTypeName< T >() );
	}
	virtual void AddComponent( const std::string& name );

	virtual void OnComponentAddedToEntityInSystem( SGF::Entity* e, Component* comp )
	{

	}

	virtual void OnComponentRemovingFromEntity( SGF::Entity* e, Component* scomp  )
	{

	}

	float dt;

private:

	// The bits representing the component types that we care about.
	unsigned int _typeBits;

	// The set of entities that this system cares about.
	std::set< Entity* > _actives;

	// The entity manager that we pull our information from.
	EntityManager *_entManager;

	// The event manager pointer.
	EventManager *_evtManager;

	// name of the system
	std::string mSystemName;
};


inline const std::set< Entity* >& EntitySystem::GetActivesConst() const { 
	return _actives;
}

} // end of namespace SGF

#endif