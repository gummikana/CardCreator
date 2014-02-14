#include "Component.h"

#include <utils/xml/cxml.h>

using namespace SGF;

//-----------------------------------------------------------------------------

Component::Component( const std::string& name ) : 
	_name( name ), 
	_type(0), 
	_bit(0),
	_id(0),
	_enabled( true )
{ 
	// does this cause static initialization  problems?
	InitType();
} 

Component::~Component()
{
	if( _auto_deletes.empty() == false ) 
	{
		for( std::list< AutoDeleter* >::iterator i = _auto_deletes.begin(); i != _auto_deletes.end(); ++i ) 
		{
			delete (*i);
		}
	}

}
//-----------------------------------------------------------------------------

// Get this components type.
ComponentType Component::GetType()
{
	if( _type != 0 ) return _type;
	InitType();
	return _type;
}

// Get this component's bit.
unsigned int Component::GetTypeBit()
{
	if( _bit != 0 ) return _bit;
	InitType();
	return _bit;
}

void Component::InitType()
{
	std::pair< ComponentType, unsigned int > type = ComponentTypeManager::Instance()->GetTypeData( _name );
	_type = type.first;
	_bit = type.second;
}

//-----------------------------------------------------------------------------

void Component::Serialize( ceng::CXmlFileSys* filesys )
{
	XML_BindAttributeAlias( filesys, _enabled, "_enabled" );
}

//-----------------------------------------------------------------------------
