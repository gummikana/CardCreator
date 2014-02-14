#include "EntitySystem.h"

#include <assert.h>

#include "EntityEvents.h"
#include "EventManager.h"
#include "ComponentType.h"
#include "Component.h"


using namespace SGF;

EntitySystem::EntitySystem( EventManager *eventManager, EntityManager *entityManager, unsigned int typeBits )
{
	dt = 1.f / 60.f;

	_evtManager = eventManager;
	_entManager = entityManager;
	_typeBits = typeBits;

	assert( _evtManager );

	// Register this system for component events.
	_evtManager->AddListener(this, EVENT_INSERT_COMPONENT);
	_evtManager->AddListener(this, EVENT_REMOVE_COMPONENT);
}

EntitySystem::~EntitySystem()
{
	assert( _evtManager );

	_evtManager->RemoveListener(this, EVENT_INSERT_COMPONENT);
	_evtManager->RemoveListener(this, EVENT_REMOVE_COMPONENT);

	_actives.clear();
}

void EntitySystem::Process( float param_dt )
{
	dt = param_dt;
	if( CheckProcessing() )
	{
		Begin();
		ProcessEntities( _entManager, _actives );
		End();
	}
}

bool EntitySystem::HandleEvent( IEventData const &evt )
{
	EventType type = evt.GetType();
	switch( type )
	{
	case EVENT_DESTROY_ENTITY:
		{
			const EntityEvent &data = static_cast<const EntityEvent&>( evt );
			Entity* e = data.e;
			_actives.erase(e);
		}
		break;

	case EVENT_INSERT_COMPONENT:
	case EVENT_REMOVE_COMPONENT:
		{
			const EntityComponentEvent& data = static_cast<const EntityComponentEvent&>( evt );
			Entity* e = data.e;

			
			// Does this entity meet our requirements?
			if( ( data.typebits & _typeBits ) == data.typebits )
			{
				if( type == EVENT_INSERT_COMPONENT )
				{
					_actives.insert(e);
					
					OnComponentAddedToEntityInSystem( e, data.mComp );
				}
				else if ( type == EVENT_REMOVE_COMPONENT )
				{
					OnComponentRemovingFromEntity( e, data.mComp );

					_actives.erase(e);
				}
			}

			break;
		}
	
	}

	return false;
}

void EntitySystem::AddComponent( const std::string& name )
{
	unsigned int bit = ComponentTypeManager::Instance()->GetTypeData( name ).second;
	_typeBits |= bit;
}