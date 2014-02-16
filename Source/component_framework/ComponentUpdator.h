#ifndef INC_COMPONENT_UPDATOR_H
#define INC_COMPONENT_UPDATOR_H

#include <iostream>
#include <string>
#include <typeinfo>
#include "EntityProcessingSystem.h"
#include "EntityManager.h"
#include "ComponentMapper.h"
#include "SystemManager.h"
#include "../misc_utils/simple_profiler.h"

namespace SGF { class EventManager; class EntityManager; class IMessage; class IEntity; }

// #define SYSTEM_REGISTER(TYPE) is below there
/*
Example System...
-----------------

class ExampleSystem : public ComponentUpdator< ControlsComponent, false >
{
public:

	ExampleSystem( SGF::EventManager* eventManager, SGF::EntityManager* entityManager ) : 
		ComponentUpdator( eventManager, entityManager )
	{ }
	
	void UpdateComponent( SGF::Entity* e, ControlsComponent* controls ) 
	{ }

	void RefreshComponent( SGF::Entity* e, ControlsComponent* scomp )
	{ }
};

SYSTEM_REGISTER( ExampleSystem );

// ------------
*/
void SetComponentRefresh( bool refresh_always );
bool GetComponentRefresh();


template< class TComponentType, bool RefreshElements = false > 
class ComponentUpdator : public SGF::EntityProcessingSystem {
public:

	std::string			_profilename;
	SimpleProfiler		_simpleprofiler;

	ComponentUpdator( 
		SGF::EventManager* eventManager, 
		SGF::EntityManager* entityManager
		) : 
		EntityProcessingSystem(eventManager, entityManager )
	{
		AddComponentType< TComponentType >();

		_profilename = typeid( TComponentType ).name();
		_simpleprofiler.mName = _profilename;

		// you can call AddComponentType< TComponentType >() 
		// to make sure the entities updated have that component as well
		
	}
	
	virtual ~ComponentUpdator() 
	{ 
	}

	//-------------------------------------------------------------------------

	// A preprocessing update call.
	virtual void Begin() { _simpleprofiler.Start(); }

	// A post processing update call.
	virtual void End() { _simpleprofiler.End(); }


	virtual void ProcessEntity( SGF::EntityManager* manager, SGF::Entity* e )
	{
		using namespace SGF;
		ComponentMapper< TComponentType > cmap = manager->GetComponentList< TComponentType >( e );

		bool refresh = GetComponentRefresh();

		// only update graphics every 64th frame ~ 1 per second
		if( RefreshElements == true ) {
			// used to ( Poro()->GetFrameNum() % 64 )
			refresh = ( ( Poro()->GetFrameNum() & 0x3F ) == 0 );
		}

		for( int i = 0; i < cmap.size(); ++i ) 
		{
			if( refresh ) RefreshComponent( e, cmap[ i ] );
			if( cmap[i]->IsEnabled() )
				UpdateComponent( e, cmap[ i ] );
		}
	}

	//-------------------------------------------------------------------------
	// The actual functions defined in the updators that do the actual magic
	//
	// LISTENERS 
	// ---------

	virtual void RefreshComponent( SGF::Entity* e, TComponentType* scomp )
	{
		
	}

	virtual void UpdateComponent( SGF::Entity* e, TComponentType* scomp ) 
	{
		std::cout << "UpdateComponent(...) - missing implementation" << std::endl;
	}
	
	virtual void OnComponentAddedToEntity( SGF::Entity* e, TComponentType* comp )
	{

	}

	virtual void OnComponentRemovingFromEntity( SGF::Entity* e, TComponentType* scomp  )
	{

	}

	// ---------
private:
	void OnComponentAddedToEntityInSystem( SGF::Entity* e, SGF::Component* scomp )
	{
		TComponentType* comp = static_cast< TComponentType* >( scomp );
		OnComponentAddedToEntity( e, comp );
	}

	void OnComponentRemovingFromEntity( SGF::Entity* e, SGF::Component* scomp  )
	{
		TComponentType* comp = static_cast< TComponentType* >( scomp );
		OnComponentRemovingFromEntity( e, comp );
	}
};
//-----------------------------------------------------------------------------


template< class SystemType >
class RegisterSystem
{
public:

	RegisterSystem() { SGF::SystemManager::GetSingletonPtr()->RegisterSystem< SystemType >(); }

	static RegisterSystem< SystemType >& Instance() { static RegisterSystem< SystemType > instance; return instance; }
};

//-----------------------------------------------------------------------------

#define SYSTEM_REGISTER(TYPE)													\
namespace SGF {																	\
namespace detail {																\
namespace {																		\
        template<class T>														\
        class SystemRegistration;												\
																				\
        template<>																\
        class SystemRegistration<TYPE>											\
        {																		\
			static const ::RegisterSystem<TYPE>& reg;							\
        };																		\
																				\
		const ::RegisterSystem<TYPE>&											\
             SystemRegistration<TYPE>::reg =									\
                ::RegisterSystem<TYPE>::Instance();								\
}}}

//-----------------------------------------------------------------------------

#endif
