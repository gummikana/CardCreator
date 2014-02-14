#ifndef INC_ENTITY_REFERENCE_H
#define INC_ENTITY_REFERENCE_H

#include <map>
#include <utils/singleton/csingleton.h>

namespace SGF { class Entity; }

class EntityReference : public ceng::CSingleton< EntityReference >
{
public:
	typedef unsigned int EntityID;
	typedef std::map< EntityID, SGF::Entity* > EntityMap;


	~EntityReference() { }

	SGF::Entity* GetEntity( EntityID id ) const;

	// returns the id of the entity
	EntityID AddEntity( SGF::Entity* entity );

	void RemoveEntity( SGF::Entity* entity );
	void RemoveEntity( EntityID entity_id );

private:
	EntityReference() { }
	EntityMap mEntities;

	friend class ceng::CSingleton< EntityReference >;
};

#endif
