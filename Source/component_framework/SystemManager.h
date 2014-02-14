#ifndef INC_SYSTEM_MANAGER_H
#define INC_SYSTEM_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <utils/singleton/csingleton.h>

namespace SGF 
{

class EventManager;
class EntityManager;
class EntityProcessingSystem;
class Entity;
class IMessage;

// ----------------------------------------------------------------------------

class SystemManager
{
public:

	static SystemManager* GetSingletonPtr() {
		static SystemManager instance;
		return &instance;
	}

	void InitSystems( SGF::EventManager* event_manager, 
						SGF::EntityManager* entity_manager );

	void Process( float dt );


	// -- for macros --

	template< class T >
	void RegisterSystem() {
		TSystemAutoCreator< T >* creator = new TSystemAutoCreator< T >();
		mSystemsCreators.push_back( creator );
		std::cout << "Registered system: " << creator->GetName() << ", size: " << mSystemsCreators.size() << std::endl;
	}


	template< class T >
	T* GetSystem()
	{
		std::string name = typeid( T ).name();
		for( std::size_t i = 0; i < mSystemsCreators.size(); ++i ) 
		{
			if( mSystemsCreators[ i ]->GetName() == name ) 
				return dynamic_cast< T* >( mSystemsCreators[ i ]->GetSystem() );
		}

		std::cout << "CRASH, couldn't find system: " << name << std::endl;

		return NULL;
	}
	
private:
	SystemManager() : mSystemsCreators(), mSystems() { std::cout << "SystemManager::SystemManager() - called " << std::endl; }

	void ClearSystems();

	SGF::EntityManager* mEntityManager;
	SGF::EventManager* mEventManager;

	class ISystemAutoCreator;

	std::vector< ISystemAutoCreator* > mSystemsCreators;
	std::vector< SGF::EntityProcessingSystem* > mSystems;

	//-------------------------------------------------------------------------
			
	class ISystemAutoCreator
	{
	public:
		ISystemAutoCreator() : 
		  mEventManager( NULL ),
		  mEntityManager( NULL ),
		  mName( "UNKNOWN" )
		{
		}
		
		virtual ~ISystemAutoCreator() { }
		
		virtual const std::string& GetName() const { return mName; } 

		virtual SGF::EntityProcessingSystem* GetSystem() = 0;

		SGF::EventManager* mEventManager;
		SGF::EntityManager* mEntityManager;
		std::string mName;
	};

	// ----------------------------------------------------------------------------

	template< class SystemName >
	class TSystemAutoCreator : public ISystemAutoCreator
	{
	public:
		TSystemAutoCreator() : 
			ISystemAutoCreator() 
		{ 
			mName = typeid( SystemName ).name();
		}

		virtual SGF::EntityProcessingSystem* GetSystem()
		{
			static SystemName system( mEventManager, mEntityManager );
			return &system;
		}

	};

	//-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
} // end of namespace SGF

#endif
