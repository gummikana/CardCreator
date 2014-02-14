#include "GComponents.h"

#include "EventManager.h"
#include "EntityManager.h"
#include "SystemManager.h"


//----------------------------------------------------------------------------

SGF::EventManager* GComponents::eventManager = 0;
SGF::EntityManager* GComponents::entityManager = 0;

//-----------------------------------------------------------------------------

void GComponents::Init()
{
	using namespace SGF;

	if( eventManager == NULL )
	{
		eventManager = new EventManager;
		entityManager = new EntityManager( eventManager  );

		SystemManager::GetSingletonPtr()->InitSystems( eventManager, entityManager );
	}
}

//-----------------------------------------------------------------------------
