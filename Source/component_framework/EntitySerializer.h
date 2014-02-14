#ifndef INC_ENTITY_SERIALIZER_H
#define INC_ENTITY_SERIALIZER_H

#include <string>

namespace SGF { class Entity; class EntityManager; }
namespace ceng { class CXmlFileSys; }

class EntitySerializer
{
public:
	EntitySerializer( SGF::Entity* entity, SGF::EntityManager* entityMgr ) : entity( entity ), entityMgr( entityMgr ) { }

	void Serialize( ceng::CXmlFileSys* filesys );

	SGF::Entity* entity;
	SGF::EntityManager* entityMgr;
};


SGF::Entity*	LoadEntity( const std::string& xml_file, SGF::EntityManager* entity_manager );
void			SaveEntity( const std::string& xml_file, SGF::Entity* entity,  SGF::EntityManager* entity_manager );

#endif
