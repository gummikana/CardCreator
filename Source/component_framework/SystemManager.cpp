#include "SystemManager.h"

#include <algorithm>
#include <utils/debug.h>

#include "EntityProcessingSystem.h"

namespace SGF 
{


namespace {
	struct SystemPointerCompare 
	{
		bool operator()(const SGF::EntityProcessingSystem* l, const SGF::EntityProcessingSystem* r) 
		{
			cassert( l );
			cassert( r );
			return l->GetOrderIndex() < r->GetOrderIndex();
		}
	};
} // end of anonymous namespace
//-----------------------------------------------------------------------------

void SystemManager::InitSystems( SGF::EventManager* event_manager, 
									SGF::EntityManager* entity_manager )

{
	ClearSystems();

	mEntityManager = entity_manager;
	mEventManager = event_manager;

	for( std::size_t i = 0; i < mSystemsCreators.size(); ++i ) 
	{
		ISystemAutoCreator* creator = mSystemsCreators[ i ];
		creator->mEntityManager = mEntityManager;
		creator->mEventManager = mEventManager;

		SGF::EntityProcessingSystem* system = creator->GetSystem();
		system->SetName( creator->GetName() );
		mSystems.push_back( system );
	}

	// order up system
	std::sort( mSystems.begin(), mSystems.end(), SystemPointerCompare() );
}
//-----------------------------------------------------------------------------

void SystemManager::ClearSystems()
{
	// don't delete the system, since they're are "singletons" that are 
	// static element in TSystemAutoCreators and it's returned from there
	mSystems.clear();
}

//-----------------------------------------------------------------------------

void SystemManager::Process( float dt )
{
	for( std::size_t i = 0; i < mSystems.size(); ++i ) 
	{
		mSystems[ i ]->Process( dt );
	}
}
//-----------------------------------------------------------------------------

} // end of namespace SGF
