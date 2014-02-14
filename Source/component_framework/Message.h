#ifndef INC_CF_MESSAGE_H
#define INC_CF_MESSAGE_H

#include "../misc_utils/config_ui.h"

// A macro to create messages easily
/*
// -- example message creation, creates message Message_PhysicsCollision

	#define MESSAGE_PHYSICS_COLLISION(list_) \
		list_(float, 			position_x,				-1.0f,							MetaPrivate() ) \
		list_(float, 			position_y,				2.0f,							MetaPrivate() ) \

	DEFINE_MESSAGE( Message_PhysicsCollision, MESSAGE_PHYSICS_COLLISION );

*/

// TODO - register messages automatically and use some kind of factory to create them
// 
// All messages implement the IMessage and GenericMessage interfaces
// the data can be accessed directly or dynamically
// e.g.
// 
/* 
	if( message->GetName() == "Message_PhysicsCollision" ) 
	{
			float x = message->GetValueAs< float >( "position_x" );
			float y = message->GetValueAs< float >( "position_y" );
	}
*/
// or directly as 
/*
	void HandleMessage( SGF::Entity* e, SpriteComponent* comp, Message_PhysicsCollision* message )
	{
		float x = message->position_x;
		float y = message->position_y;
	}
*/

// -------- params parsing ----------

class IMessage
{
public:
	IMessage() : _done( false ) { }
	virtual ~IMessage() { }

	virtual std::string GetName() const { return ""; }
	virtual int GetType() const { return -1; }

	bool _done;	
};

//--------- generic message ---------

class GenericMessage : public IMessage
{
public:
		
	virtual const std::map< std::string, bool >& GetListOfVariables() const 
	{ 
		return _m_variables; 
	}

	virtual std::string GetName() const { return "GenericMessage"; }

	virtual void Serialize( ceng::CXmlFileSys* filesys ) 
	{
	}
	
	virtual void SetValue( const std::string& n, const ceng::CAnyContainer& new_value )
	{
	}

	virtual ceng::CAnyContainer GetValue( const std::string& n ) const 
	{ 
		return ceng::CAnyContainer();
	} 
	
	virtual IMetaData GetMetaData( const std::string& n ) const
	{ 
		return IMetaData( NULL ); 
	} 
	
	virtual bool HasValue( const std::string& name ) const
	{
		std::map< std::string, bool >::const_iterator i = _m_variables.find( name );
		if( i == _m_variables.end() ) return false;
		return true;
	}

	template< class T >
	T GetValueAs( const std::string& name ) const
	{
		return ceng::CAnyContainerCast< T >( GetValue( name ) );
	}

	std::map< std::string, bool > _m_variables; 
};

//  so now we could do one from lua, in which you just create the type and
//  the contents dynamically
//
// the only problem is that it has to be dealt with "dynamically" as well
// through the GetValue() methods

#define DEFINE_MESSAGE(class_name, list)                 \
class class_name  : public GenericMessage { \
	public: \
		class_name() : GenericMessage() { \
			list(CONFIG_UI_INIT_MEMBERS) \
		} \
		\
        list(CONFIG_UI_HEADER_DECLARE)   \
		std::string GetName() const { return #class_name; } \
		\
		void Serialize( ceng::CXmlFileSys* filesys ) \
		{ \
			list(CONFIG_UI_XML_BIND_DECLARE) \
		} \
		\
		void SetValue( const std::string& n, const ceng::CAnyContainer& new_value ) \
		{ \
			list(CONFIG_UI_SET_VALUE) \
		} \
		\
		ceng::CAnyContainer GetValue( const std::string& n ) const \
		{ \
			list(CONFIG_UI_GET_VALUE) \
			return ceng::CAnyContainer(); \
		} \
		\
		IMetaData GetMetaData( const std::string& n ) const \
		{ \
			list(CONFIG_UI_GET_META_DATA) \
			return IMetaData( NULL ); \
		} \
		\
	}; \

//	\
// COMPONENT_REGISTER( class_name, ::SGF::GetTypeName< class_name >() )


//---- --------------------

#endif