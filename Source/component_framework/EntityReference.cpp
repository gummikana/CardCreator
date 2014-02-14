#include "EntityReference.h"

#include <utils/debug.h>
#include "Entity.h"


SGF::Entity* EntityReference::GetEntity( EntityReference::EntityID id ) const
{
	EntityMap::const_iterator i = mEntities.find( id );
	if( i == mEntities.end() ) return NULL;

	return i->second;
}

// returns the id of the entity
EntityReference::EntityID EntityReference::AddEntity( SGF::Entity* entity )
{
	cassert( entity );
	
	// don't add if it already exists

	if( GetEntity( entity->GetID() ) != NULL )
		return entity->GetID();

	mEntities.insert( std::make_pair( entity->GetID(), entity ) );

	return entity->GetID();
}

void EntityReference::RemoveEntity( SGF::Entity* entity )
{
	cassert( entity );

	EntityID id = entity->GetID();
	RemoveEntity( id );
}

void EntityReference::RemoveEntity( EntityReference::EntityID id )
{
	EntityMap::iterator i = mEntities.find( id );
	if( i == mEntities.end() ) 
		return;

	mEntities.erase( i );
}


