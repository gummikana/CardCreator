#include "EntityProcessingSystem.h"
#include "EntityManager.h"

#include <list>
#include <iostream>

using namespace SGF;

EntityProcessingSystem::EntityProcessingSystem( EventManager *eventManager, EntityManager *entityManager, unsigned int typeBits ) 
												: EntitySystem(eventManager, entityManager, typeBits) {}

EntityProcessingSystem::~EntityProcessingSystem() {}

bool EntityProcessingSystem::CheckProcessing() { return true; }

void EntityProcessingSystem::Begin() {}

void EntityProcessingSystem::ProcessEntities( EntityManager *manager, const std::set<Entity*> &entities )
{
	if( entities.empty() ) return;

	std::list< Entity* > dead_entities;
	Entity* current_entity = NULL;
	for( std::set<Entity*>::const_iterator it = entities.begin(); it != entities.end(); )
	{
		current_entity = *it;
		++it;

		processedEntityId = (int)current_entity->GetID();
		ProcessEntity(manager, current_entity);
		if( current_entity->IsDead() ) dead_entities.push_back( current_entity );
	}

	processedEntityId = -1;

	// remove the dead
	if( dead_entities.empty() == false ) 
	{
		// std::cout << "EntityProcessingSystem: with dead_entities(" << dead_entities.size() << ") " << std::endl;
		for( std::list< Entity* >::iterator i = dead_entities.begin(); i != dead_entities.end(); ++i ) 
		{
			manager->DestroyEntity( *i );
		}
	}
}

void EntityProcessingSystem::End() {}

int EntityProcessingSystem::GetProcessedEntityId()
{
	return EntityProcessingSystem::processedEntityId;
}

int EntityProcessingSystem::processedEntityId = -1;