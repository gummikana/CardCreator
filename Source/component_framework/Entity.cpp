#include <string>

#include <utils/vector_utils/vector_utils.h>

#include "Entity.h"
#include "EntityReference.h"

namespace SGF {

Entity::Entity(EntityID id) : 
	_id( id ), 
	_index( 0 ), 
	_typeBits( 0 ), 
	_dead( false ),
	mName( "unknown" ),
	mTransform( NULL ),
	_tags( NULL )
{

}

Entity::~Entity()
{
	EntityReference::GetSingletonPtr()->RemoveEntity( _id );

	delete mTransform;
	mTransform = NULL;

	delete _tags;
	_tags = NULL;
}

// transform...
types::xform* Entity::GetTransform()
{
	if( mTransform == NULL ) mTransform = new types::xform;
	return mTransform;
}


// tags
std::vector< std::string>* Entity::GetTags()
{
	if ( _tags == NULL ) _tags = new std::vector< std::string>();
	return _tags;
}

bool Entity::HasTag( const std::string& tag )
{
	GetTags();

	std::vector< std::string >::iterator iter;
	for (iter = _tags->begin(); iter != _tags->end(); ++iter)
	{
		if ( *iter == tag )
			return true;
	}
		
	return false;
}

void Entity::RemoveTag( const std::string& tag )
{
	if( _tags == NULL ) return;

	ceng::VectorRemove( *_tags, tag );
}


} // end of namespace SGF