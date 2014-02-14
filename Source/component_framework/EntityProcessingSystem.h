#ifndef __ENTITY_PROCESSING_SYSTEM_H__
#define __ENTITY_PROCESSING_SYSTEM_H__

#include "ComponentType.h"
#include "EntitySystem.h"

namespace SGF
{
	class EventManager;
	class EntityManager;
	class IMessage;

	class EntityProcessingSystem : public EntitySystem {
	public:
		EntityProcessingSystem( EventManager *eventManager, EntityManager *entityManager, unsigned int typeBits = 0 );
		virtual ~EntityProcessingSystem();

		// the order systems will be processed		
		// 100 is the default
		// 500 is the SpriteSystem
		// higher index means it will be processed later
		virtual int GetOrderIndex() const { return 100; }

		static int GetProcessedEntityId();

	protected:

		// A preprocessing update call.
		virtual void Begin();

		// Process an entity this system is interested in.
		virtual void ProcessEntity( EntityManager *manager, Entity *e ) = 0;

		// A post processing update call.
		virtual void End();

		// Should we update?
		virtual bool CheckProcessing();

		// Any implementing entity system must implement this method and the logic
		// to process the given entities of the system.
		virtual void ProcessEntities( EntityManager *manager, const std::set<Entity*> &entities );

		static int processedEntityId;
	};
};

#endif