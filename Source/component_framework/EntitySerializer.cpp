#include "EntitySerializer.h"

#include <vector>
#include <utils/xml/cxml.h>
#include <utils/filesystem/filesystem.h>

#include "Component.h"
#include "ComponentFactory.h"
#include "EntityManager.h"

// --------------
namespace {
class XFormSerializer
{
public:
	types::xform* mTransform;
	XFormSerializer( types::xform* transform ) : mTransform( transform ) { }

	void Serialize( ceng::CXmlFileSys* filesys ) 
	{
		if( mTransform ) 
		{
			XML_BindAttributeAlias( filesys, mTransform->position.x, "position.x" );
			XML_BindAttributeAlias( filesys, mTransform->position.y, "position.y" );

			XML_BindAttributeAlias( filesys, mTransform->scale.x, "scale.x" );
			XML_BindAttributeAlias( filesys, mTransform->scale.y, "scale.y" );

			float rotation = mTransform->R.GetAngle();
			XML_BindAttributeAlias( filesys, rotation, "rotation" );
			mTransform->R.Set( rotation );
		}
	}
};
} // end of anonymous namespace
// --------------

void EntitySerializer::Serialize( ceng::CXmlFileSys* filesys )
{
	cassert( filesys );
	cassert( entity );
	cassert( entityMgr );

	std::string name = entity->GetName();
	XML_BindAttribute( filesys, name );
	entity->SetName( name );

	//TODO: tags serialization
	// tags saving
	std::string tags;
	if( filesys->IsWriting() && entity->_tags != NULL )
	{
		std::stringstream ss;
		for( std::size_t i = 0; i < entity->GetTags()->size(); ++i ) 
		{
			ss << entity->GetTags()->at(i);
			if( i != ( entity->GetTags()->size() - 1 ) ) 
				ss << ", ";
		}

		tags = ss.str();
	}

	XML_BindAttributeAlias( filesys, tags, "tags" );

	// tags loading
	if( filesys->IsReading() && tags.empty() == false )
	{
		std::vector< std::string > split_tags = ceng::Split( ",", tags );
		for( std::size_t i = 0; i < split_tags.size(); ++i )
		{
			split_tags[ i ] = ceng::RemoveWhiteSpaceAndEndings( split_tags[ i ] );
			entityMgr->InsertTag( entity, split_tags[ i ] );
		}
	}
		

	// transform serialization
	if( ( filesys->IsWriting() && entity->HasTransform() ) ||
		( filesys->IsReading() && filesys->HasChildNamed( "_Transform" ) ) )
	{
		XFormSerializer xform_ser( entity->GetTransform() );
		XML_BindAlias( filesys, xform_ser, "_Transform" );
	}


	// saving
	if( filesys->IsWriting() ) 
	{
		std::vector< SGF::Component* > components;
		entityMgr->GetAllComponents( entity, components );
		for( std::size_t i = 0; i < components.size(); ++i ) 
		{
			if( components[ i ] )
			{
				XML_BindAlias( filesys, *components[ i ], components[ i ]->GetName() );
			}
		}
	}
	// loading
	else if( filesys->IsReading() ) 
	{
		cassert( filesys->GetNode() );
		for( int i = 0; i < filesys->GetNode()->GetChildCount(); i++ )
		{
			std::string component_name = filesys->GetNode()->GetChild( i )->GetName();
			if( component_name == "_Transform" ) continue;
			// hax hax hax before proper factory
			SGF::Component* new_component = SGF::component::create( component_name );

			if( new_component ) 
			{
				ceng::XmlConvertTo( filesys->GetNode()->GetChild( i ), *new_component );
				entityMgr->InsertComponent( entity, new_component );
			}
			else
			{
				std::cout << "Warning! Couldn't create component by the name of: " << component_name << std::endl;
			}
		}
	}
}

// -----------------------

SGF::Entity* LoadEntity( const std::string& xml_file, SGF::EntityManager* entity_manager )
{
	cassert( entity_manager );
	SGF::Entity* e = entity_manager->CreateEntity();

	if( e == NULL ) {
		std::cout << "Error! Couldn't create a new entity, out of memory?" << std::endl;
		return NULL;
	}

	if( ceng::DoesExist( xml_file ) == false ) {
		std::cout << "Error! Couldn't load entity: " << xml_file << " - because the file doesn't exist" << std::endl;
		return NULL;
	}

	// --- load the data ------
	EntitySerializer serializer( e, entity_manager );
	ceng::XmlLoadFromFile( serializer, xml_file, "Entity" );

	return e;
}
// -----------------------

void SaveEntity( const std::string& xml_file, SGF::Entity* entity,  SGF::EntityManager* entity_manager )
{
	// --- load the data ------
	EntitySerializer serializer( entity, entity_manager );
	ceng::XmlSaveToFile( serializer, xml_file, "Entity", true );

}