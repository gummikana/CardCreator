#ifndef INC_CONFIG_UI_H
#define INC_CONFIG_UI_H

/*
#define CONFIG_BIOME(list_) \
	list_(float,			mPerlinScale_x,					1.f,			MetaData( 0.f, 512.f ) ) \
	list_(float,			mPerlinScale_y,					1.f,			MetaData( 0.f, 512.f ) ) \
	list_(int,				mGradientType,					1,				MetaData( 0, 2 ) ) \
	list_(std::string,		mInsidePerlinFunc,				"",				NULL ) \
	list_(bool,				mGradientAddNoise,				1,				NULL ) \

DEFINE_CONFIG_UI( Biome, CONFIG_BIOME );

#undef CONFIG_BIOME
*/

#include <map>
#include <string>
#include <utils/xml/canycontainer.h>

#include "metadata.h"
#include "config_sliders.h"


//-----------------------------------------------------------------------------

namespace ceng {

template< class T >
struct META_IsPointer
{
	enum { value = 0 };
};

template< class T >
struct META_IsPointer< T* >
{
	enum { value = 1 };
};

template< class T >
bool IsPointer( const T& value ) 
{
	if( META_IsPointer< T >::value == 0 ) return false;
	else return true;
}

template< class T > 
bool SerializeThis( const T& value, const int& null_reference )
{
	if( META_IsPointer< T >::value == 1 ) return false;
	return true;
}

template< class T, class MData > 
bool SerializeThis( const T& value, const MData& mt )
{
	if( META_IsPointer< T >::value == 1 ) return false;
	if( mt.Serialize() == false ) return false;
	return true;
}

#define CONFIG_UI_HEADER_DECLARE(type, name, value, meta_data) \
    type name; 
	
#define CONFIG_UI_XML_BIND_DECLARE(type, name, value, meta_data) \
	if( ceng::SerializeThis( name, meta_data ) ) { \
		XML_BindAttribute( filesys, name ); \
		if( filesys->GetNode()->HasAttribute( #name ) ) _m_variables[ #name ] = true;  \
	}

#define CONFIG_UI_INIT_MEMBERS(type, name, value, meta_data) \
	name = value; \
	_m_variables[ #name ] = true;

#define CONFIG_UI_SET_OTHER(type, name, value, meta_data) \
	name = other.name; 

#define CONFIG_UI_SET_VALUE(type, name, value, meta_data) \
	if( n == #name ) { name = ceng::CAnyContainerCast< type >( new_value ); return; }

#define CONFIG_UI_GET_VALUE(type, name, value, meta_data) \
	if( n == #name ) return name;

#define CONFIG_UI_GET_META_DATA(type, name, value, meta_data) \
	if( n == #name ) return meta_data;

#define CONFIG_UI_GET_CONFIGSLIDERS(type, name, value, meta_data) \
	AddToConfigSlidersImpl( _temp_configsliders_result, name, #name, meta_data );


//-----------------------------------------------------------------------------

#define DEFINE_CONFIG_UI(name, list)                 \
    class name { \
	public: \
		name() { \
			list(CONFIG_UI_INIT_MEMBERS) \
		} \
		\
		name( const name& other ) { Set( other ); } \
		\
		const name& operator= ( const name& other ) { Set( other ); return *this; } \
		\
		const name& operator+= ( const name& other ) \
		{ \
			for( std::map< std::string, bool >::const_iterator i = other._m_variables.begin(); i != other._m_variables.end(); ++i ) { \
				if( i->second ) SetValue( i->first, other.GetValue( i->first ) ); \
			} \
			return *this; \
		} \
		\
        list(CONFIG_UI_HEADER_DECLARE)   \
		std::map< std::string, bool > _m_variables; \
		\
		const std::map< std::string, bool >& GetListOfVariables() const { return _m_variables; } \
		std::string GetName() const { return #name; } \
		\
		\
		void Serialize( ceng::CXmlFileSys* filesys ) \
		{ \
			list(CONFIG_UI_XML_BIND_DECLARE) \
		} \
		\
		void Set( const name& other ) \
		{ \
			list(CONFIG_UI_SET_OTHER) \
			_m_variables = other._m_variables; \
		} \
		\
		void SetValue( const std::string& n, const ceng::CAnyContainer& new_value ) \
		{ \
			list(CONFIG_UI_SET_VALUE) \
		} \
		template< class T > void AddToConfigSlidersImpl( ConfigSliders* _temp_configsliders_result, T& variable, const std::string& name, const IMetaDataNoSerialize& meta_data ) { } \
		\
		template< class T > void AddToConfigSlidersImpl( ConfigSliders* _temp_configsliders_result, T& variable, const std::string& name, const IMetaData& meta_data ) \
		{ \
			_temp_configsliders_result->AddVariable(variable, name, meta_data ); \
		} \
		\
		ConfigSliders* GetConfigSliders( IConfigSlidersListener* listener = NULL ) \
		{ \
			ConfigSliders* _temp_configsliders_result = new ConfigSliders( listener ); \
			return GetConfigSliders( _temp_configsliders_result, listener ); \
		} \
		ConfigSliders* GetConfigSliders( ConfigSliders* _temp_configsliders_result = NULL, IConfigSlidersListener* listener = NULL ) \
		{ \
			if( _temp_configsliders_result == NULL ) _temp_configsliders_result = new ConfigSliders( listener ); \
			list(CONFIG_UI_GET_CONFIGSLIDERS) \
			return _temp_configsliders_result; \
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
	};


} // end of namespace ceng
//-----------------------------------------------------------------------------

#endif
