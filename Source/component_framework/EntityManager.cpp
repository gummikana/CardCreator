#include "EntityManager.h"

#include <utils/vector_utils/vector_utils.h>
#include <utils/math/point_inside.h>

#include "EventManager.h"
#include "EntityEvents.h"
#include "Component.h"
#include "ComponentFactory.h"


using namespace SGF;

ComponentList EntityManager::EmptyList;

// ----------------------------------------------------------------------------

namespace
{
	Entity* GetClosestEntity_Private(const std::vector< Entity* >* entities, const types::vector2& position, Entity* exclude_me )
	{
		// find the closest
		float closest_squared = FLT_MAX;
		Entity* result = NULL;

		for( std::size_t i = 0; i < entities->size(); ++i ) 
		{
			Entity* current =  entities->at( i );
			assert( current );
			if( current == exclude_me ) 
				continue;

			if( current->IsDead() || current->HasTransform() == false )
				continue;
			
			float dist_squared = ceng::math::DistanceSquared( position, current->GetTransform()->position );
			if( dist_squared < closest_squared ) 
			{
				closest_squared = dist_squared;
				result = current;
			}
		}

		return result;
	}

} // End of anonymous namespace



EntityManager::EntityManager( EventManager* eventSystem ) : 
	_nextId( 0 ),
	_removedIndices(),
	_entities(),
	_components(),
	_eventSystem(eventSystem) 
{
	_components.resize( ComponentTypeManager::Instance()->GetSize() );
	InitComponentTypes();
}

EntityManager::~EntityManager()
{
	for( std::vector< Entity* >::iterator it = _entities.begin(); it != _entities.end(); ++it )
		if( *it )
			DestroyAllComponents( *it );

	// Clear the list of entities.
	_entities.clear();

	// Clear the component arrays.
	for( std::size_t i = 0; i < _components.size(); i++ )
	{
		if( _components[i] != NULL )
			ceng::VectorClearPointers( *(_components[i]) );
	}

	// Clear the tag arrays
	// unnessary unless we start using pointers to vectors which we should do
	/*
	std::map< std::string, std::vector< Entity* > >::iterator iter;
    for (iter = _entityTags.begin(); iter != _entityTags.end(); ++iter)
		iter->second.clear();
	*/

	_entityTags.clear();
	_components.clear();
}

// ----------------------------------------------------------------------------

Entity* EntityManager::CreateEntity()
{
	Entity* e = new Entity(++_nextId);

	// Do we have previously removed slot indices that we can reuse?
	if( _removedIndices.empty() == false )
	{
		e->_index = _removedIndices.back();
		_removedIndices.pop_back();
		
		// Resize the vector if we need .
		if( e->_index >= _entities.size() ) 
			_entities.resize(e->_index+1, NULL);

		_entities[e->_index] = e;
	}
	else 
	{
		e->_index = _entities.size();
		_entities.push_back(e);
	}

	// Inform the world that we just created an entity.
	_eventSystem->FireEvent( EntityEvent( EVENT_CREATE_ENTITY, e ) );

	// Return a reference to the element itself.
	return e;
}

void EntityManager::DestroyEntity( Entity* e )
{
	_entities[e->_index] = NULL;
	_removedIndices.push_back( e->_index );
	DestroyAllComponents(e);

	// Delete the tags
	if (e->_tags != NULL)
	{
		std::vector< std::string >* tags = e->GetTags();
		for( std::size_t i = 0; i < tags->size(); ++i ) 
		{
			std::string tag = tags->at(i);
			
			// Get list of all entities with this tag and remove given entity from the list;
			std::map< std::string, std::vector< Entity* > >::iterator iter = _entityTags.find(tag);
			if( iter != _entityTags.end() )
			{
				std::vector< Entity* > &entitiesWithTag = iter->second;
				ceng::VectorRemove( entitiesWithTag, e );
			}
			else
			{
				// This shouldn't happen
				assert(false);
			}
		}
	}

	// Inform the world that we just destroyed an entity.
	_eventSystem->FireEvent( EntityEvent( EVENT_DESTROY_ENTITY, e ) );

	delete e;
}

// ----------------------------------------------------------------------------

Entity* EntityManager::GetEntityWithId( EntityID id )
{
	for( std::size_t i = 0; i < _entities.size(); ++i ) 
	{
		if( _entities[ i ]->GetID() == id ) return _entities[ i ];
	}

	return NULL;
}

// returns a list of all the components for entity e
void EntityManager::GetAllComponents( Entity* e, std::vector< Component* >& out_components )
{
	assert( e );
	unsigned int index = e->_index;

	for( std::size_t i = 0; i < _components.size(); ++i ) 
	{
		std::vector<ComponentList* >* entities = _components[ i ];
		if( entities == NULL ) continue;
		if( index >= entities->size() ) continue;
	
		ComponentList* list = entities->at( index );
		if( list != NULL && list->empty() == false )
			out_components.insert( out_components.end(), list->begin(), list->end() );
	}
}

std::vector< Component* > EntityManager::GetAllComponents( Entity* e )
{
	std::vector< Component* > result;

	GetAllComponents( e, result );

	return result;
}



void EntityManager::InsertComponent( Entity* e, Component* comp )
{
	if( comp->GetType() >= (int)_components.size() ) 
	{
		std::cout << "EntityManager::InsertComponent - had to resize, performance killer?" << std::endl;
		_components.resize( ComponentTypeManager::Instance()->GetSize() );
	}

	std::vector<ComponentList* >* entities = _components[comp->GetType()];
	if( entities == NULL ) 
	{
		_components[comp->GetType()] = new std::vector<ComponentList* >();
		entities = _components[comp->GetType()];
	}
	
	cassert( entities );

	// Resize the vector if we need to.
	if( e->_index >= entities->size() )
		entities->resize( e->_index + 1 );

	// Grab the component list for this entity and add the component.
	ComponentList* list = (*entities)[e->_index];
	if( list == NULL ) 
	{
		list = new ComponentList();
		entities->at( e->_index ) = list;
	}

	cassert( list );
	list->push_back( comp );

	// init the component
	

	// Set the entity's bit for this component.
	e->_typeBits |= comp->GetTypeBit();

	// Inform the world that we just inserted a component.
	_eventSystem->FireEvent( EntityComponentEvent( EVENT_INSERT_COMPONENT, e, comp, comp->GetType(), comp->GetTypeBit() ) );
}

void EntityManager::RemoveComponent( Entity* e, Component* comp )
{
	if( comp->GetType() >= (int)_components.size() ) 
	{
		std::cout << "EntityManager::RemoveComponent - had to resize, performance killer?" << std::endl;
		_components.resize( comp->GetType() + 1 );
	}

	std::vector<ComponentList*>* entities = _components[ comp->GetType() ];
	if( entities == NULL ) return;

	// Skip if we don't even have the space allocated.
	if( e->_index >= entities->size() ) return;

	// Grab the component list for this entity and remove the component.
	ComponentList* list = entities->at( e->_index );
	if( list == NULL ) return;

	// should this be replaced with VectorRemove(?)
	for( ComponentList::iterator it = list->begin(); it != list->end();  )
	{
		if( *it == comp ) {
			it = list->erase(it);
		} else {
			++it;
		}
	}

	// Remove the component bit if we don't have any more of this component type.
	if( list->empty() ) 
	{ 
		e->_typeBits &= ~comp->GetTypeBit(); 
		delete list;
		entities->at( e->_index ) = NULL;
	}

	// Inform the world that we just removed a component.
	_eventSystem->FireEvent( EntityComponentEvent( EVENT_REMOVE_COMPONENT, e, comp, comp->GetType(), comp->GetTypeBit() ) );
}

void EntityManager::DestroyAllComponents( Entity* e )
{
	for( int type = 0; type < (int)_components.size(); type++ )
		DestroyAllComponents(e, (ComponentType)type);
}

void EntityManager::DestroyAllComponents( Entity* e, ComponentType type )
{
	if( type >= (int)_components.size() ) 
	{
		// std::cout << "EntityManager::DestroyAllComponents - had to resize, performance killer?" << std::endl;
		// _components.resize( (int)type + 1 );
		// can't have any components of type, if _components doesn't have any reference to type
		return;
	}

	// old implementation
	std::vector<ComponentList* >* entities = _components[type];
	if( entities == NULL ) return;

	// Skip if we don't even have the space allocated.
	if( e->_index >= entities->size() ) return;

	// Grab the component list for this entity and remove the component.
	ComponentList* list = entities->at( e->_index );

	if( list == NULL ) return;
	
	// Delete the components
	while( list->empty() == false )
	{
		unsigned int type_bits = (list->back()->GetTypeBit());

		// Inform the world that we just removed a component.
		_eventSystem->FireEvent( EntityComponentEvent( EVENT_REMOVE_COMPONENT, e, list->back(), (ComponentType)type, type_bits ) );

		cassert( list->empty() == false );

		delete list->back();
		list->pop_back();

		// Remove the bit precisely when we get to the last component, prior to firing the event.
		// the bug was in the use of e->_typeBits &= ~(1 << type);
		// this made the typebits be off
		if( list->empty() ) e->_typeBits &= ~(type_bits);
	}	

	// Clear the list.
	list->clear();
	delete list;
	entities->at( e->_index ) = NULL;
}

const ComponentList* EntityManager::GetComponentList( const Entity* e, ComponentType type ) const
{
	if( type >= (int)_components.size() ) 
	{
		std::cout << "EntityManager::GetComponentList - performance killer?" << std::endl;
		return NULL;
	}

	// old implementation
	const std::vector<ComponentList*>* entities = _components[type];
	if( entities == NULL ) return NULL;

	// Return a blank list if we don't even have the space allocated.
	if( e->_index >= entities->size() ) return NULL;

	// Grab the component list for this entity and remove the component.
	return entities->at( e->_index );
}

// ----------------------------------------------------------------------------

void EntityManager::InsertTag( Entity* e, const std::string& tag )
{
	if( e == NULL ) return;
	if ( e->HasTag(tag) ) return;

	_entityTags[ tag ].push_back( e );
	e->GetTags()->push_back( tag );
}

void EntityManager::RemoveTag( Entity* e, const std::string& tag )
{
	if( e == NULL ) return;
	if( e->HasTag(tag) == false ) return;

	e->RemoveTag(tag);
	std::map< std::string, std::vector< Entity* > >::iterator iter = _entityTags.find(tag);
	if( iter != _entityTags.end() )
	{
		ceng::VectorRemove( iter->second, e );
	}
}

//.............................................................................

Entity* EntityManager::GetClosestEntity( const types::vector2& position ) const
{
	return GetClosestEntity_Private( &_entities, position, NULL );
}

//.............................................................................

const std::vector< Entity* >* EntityManager::GetEntitiesWithTag( const std::string &tag ) const
{
	const std::vector< Entity* >* entitiesWithTag = NULL;

	std::map< std::string, std::vector< Entity* > >::const_iterator iter = _entityTags.find( tag );
	if( iter != _entityTags.end() )
		entitiesWithTag = &iter->second;

	return entitiesWithTag;
}
Entity* EntityManager::GetClosestEntityWithTag( Entity* closest_to_who, const std::string& tag ) const
{
	if( closest_to_who->HasTransform() == false ) 
		return NULL;

	return GetClosestEntityWithTag( closest_to_who->GetTransform()->position, tag, closest_to_who );
}

Entity* EntityManager::GetClosestEntityWithTag( const types::vector2& position, const std::string& tag, Entity* exclude_me ) const
{
	const std::vector< Entity* >* entities_with_tag = GetEntitiesWithTag( tag );
	if( entities_with_tag == NULL || entities_with_tag->empty() ) 
		return NULL;

	if( entities_with_tag->size() <= 1 ) 
		return entities_with_tag->at( 0 );

	return GetClosestEntity_Private( entities_with_tag, position, exclude_me );
}

//.............................................................................

void EntityManager::GetEntitiesWithTagInsideAABB( const std::string& tag, const types::vector2& aabb_min, const types::vector2& aabb_max, std::vector< Entity* >* entities_out ) const
{
	const std::vector< Entity* >* entities_with_tag = GetEntitiesWithTag( tag );
	if( entities_with_tag == NULL || entities_with_tag->empty() ) 
		return;

	cassert( entities_out );
	cassert( aabb_min.x <= aabb_max.x );
	cassert( aabb_min.y <= aabb_max.y );

	for( std::size_t i = 0; i < entities_with_tag->size(); ++i ) 
	{
		Entity* current = entities_with_tag->at( i );
		assert( current );

		if( current->IsDead() || current->HasTransform() == false )
			continue;

		if( ceng::math::IsPointInsideAABB( current->GetTransform()->position, aabb_min, aabb_max ) )
			entities_out->push_back( current );
	}
}

// ----------------------------------------------------------------------------
