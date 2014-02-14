#ifndef __ENTITY_SYSTEM_H__
#define __ENTITY_SYSTEM_H__

#include <vector>
#include <list>
#include <map>

#include "Entity.h"
#include "ComponentType.h"
#include "ComponentMapper.h"
#include "MessageListener.h"

namespace SGF
{
	class Component;
	class EventManager;

	typedef std::vector<Component*> ComponentList;

	class EntityManager : public MessageSender {
	public:
		EntityManager( EventManager* eventSystem );
		virtual ~EntityManager();

		// --- entity creation / destruction ----------------------------------
		// Allocates a new entity in the system and returns its handle.
		Entity* CreateEntity();

		// Destroys an entity in the system.
		void DestroyEntity( Entity* e );

		// --- COMPONENTS -----------------------------------------------------
		// Insert a component.
		void InsertComponent( Entity* e, Component* comp );

		// Remove a component. It does not delete the component.
		void RemoveComponent( Entity* e, Component* comp );

		// returns a list of all the components for entity e
		void						GetAllComponents( Entity* e, std::vector< Component* >& out_components );
		std::vector< Component* >	GetAllComponents( Entity* e );

		// Remove all components of an entity and deletes them.
		void DestroyAllComponents( Entity* e );

		// Remove all components of a given type and deletes them.
		void DestroyAllComponents( Entity* e, ComponentType type );

		// Remove all componets of given type
		template< class CompType >
		void DestroyAllComponents( Entity* e ) 
		{
			ComponentType type = GetComponenType< CompType >();
			DestroyAllComponents( e, type );
		}

		// Get the list of components for an entity by type.
		const ComponentList* GetComponentList( const Entity* e, ComponentType type ) const;
		
		// Get the list of components for an entity defined by CompType
		template< class CompType >
		ComponentMapper< CompType > GetComponentList( const Entity* e ) const 
		{
			ComponentType type = GetComponenType< CompType >();
			return ComponentMapper< CompType >( GetComponentList( e, type ) );
		}

		template< class CompType >
		CompType* GetFirstComponent( const Entity* e ) const
		{
			ComponentType type = GetComponenType< CompType >();
			ComponentMapper< CompType > mapper = ComponentMapper< CompType >( GetComponentList( e, type ) );
			
			if ( mapper.size() == 0)
				return NULL;
			
			return mapper[0];
		}


		// Get Entity with ID (id's are unique and currently we have to go through them all to find the 
		// one you're looking for
		Entity* GetEntityWithId( EntityID id );

		// ---- tags ----------------------------------------------------------

		void InsertTag( Entity* e, const std::string& tag );

		void RemoveTag( Entity* e, const std::string& tag );

		// returns the closest Entity to position that has a transformation
		// although doesn't guarantee that the result has a transformation
		// returns NULL if none found
		// NOTE: Only uses the origin of the entity, so it doesn't include collision volume, 
		// so the result is not guaranteed to be the "closest" by volume
		Entity* GetClosestEntity( const types::vector2& position ) const;

		const std::vector< Entity* >* GetEntitiesWithTag( const std::string &tag ) const;

		// returns the closest Entity that has the tag, excludes itself from the process
		// if the entities don't have transforms, it skips them
		// returns NULL if none found or if the closest_to_who doesn't have a transform
		// NOTE: Only uses the origin of the entity, so it doesn't include collision volume, 
		// so the result is not guaranteed to be the "closest" by volume
		Entity* GetClosestEntityWithTag( Entity* closest_to_who, const std::string& tag ) const;

		// returns the closest Entity to position that has the tag and that has a transformation
		// although doesn't guarantee that the result has a transformation
		// returns NULL if none found
		// NOTE: Only uses the origin of the entity, so it doesn't include collision volume, 
		// so the result is not guaranteed to be the "closest" by volume
		Entity* GetClosestEntityWithTag( const types::vector2& position, const std::string& tag, Entity* exclude_me = NULL ) const;

		// return all the entities found inside the AABB with tag
		// discards the collision volume of entities, so expand your aabb to fit 
		// and figure out your collision volumes by hand
		void GetEntitiesWithTagInsideAABB( const std::string& tag, const types::vector2& aabb_min, const types::vector2& aabb_max, std::vector< Entity* >* entities_out ) const;

		// ---------------- utility to help us a bit --------------------------
		template< class TCompType > 
		ComponentType GetComponenType() const
		{
			ComponentType type = ComponentMapper< TCompType >::component_type;

			// if the mapper doesn't have type, we'll get type from ComponentTypeManager
			if( type == 0 ) 
			{
				type = ComponentTypeManager::Instance()->GetTypeData( GetTypeName< TCompType >() ).first;
				ComponentMapper< TCompType >::component_type = type;
			}

			return type;
		}

	private:

		static ComponentList EmptyList;

		// The next entity id to allocate
		EntityID _nextId;

		std::vector< unsigned int > _removedIndices;
		std::vector< Entity* > _entities;
		typedef std::vector< std::vector< ComponentList* >* > ComponentsType;
		std::vector< std::vector< ComponentList* >* > _components;
		std::map< std::string, std::vector< Entity* > > _entityTags;

		// The event system we are using.
		EventManager* _eventSystem;
	};
};

#endif