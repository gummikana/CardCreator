#ifndef INC_COMPONENT_MACRO_H
#define INC_COMPONENT_MACRO_H

/*
// Examble component
// NOTE: Don't declare these inside ANONYMOUS NAMESPACE

#define COMPONENT_PROC_TREE(list_) \
	list_(float,							rand_seed,						1234.f,			MetaData( 0.f, 10000.f ) ) \
	list_(float,							tree_width,						120.f,			MetaData( 0.f, 512.f ) ) \
	list_(float,							tree_radius_low,				0.3f,			MetaData( 0.f, 1.f ) ) \
	list_(float,							tree_radius_high,				0.7f,			MetaData( 0.f, 1.f ) ) \
	list_(float,							tree_probability,				0.7f,			MetaData( 0.f, 1.f ) ) \
	list_(int,								tree_extra_y,					2,				MetaData( 0, 50 ) ) \
	list_(std::string,						tree_image_file,				"",				NULL ) \
	list_(std::string,						tree_material,					"wood",			NULL ) \
	list_(int,								tree_material_id,				-1,				MetaPrivate() ) \
	list_(ceng::CArray2D< uint32 >*,		mTreeData,						NULL,			MetaPrivate() ) \



DEFINE_COMPONENT( TreeComponent, COMPONENT_PROC_TREE );

#undef COMPONENT_PROC_TREE

*/

#include "../misc_utils/config_ui.h"
#include "ComponentType.h"
#include "Component.h"
#include "ComponentFactory.h"


#define DEFINE_COMPONENT(class_name, list)                 \
class class_name  : public ::SGF::Component { \
	public: \
		class_name() : Component( ::SGF::GetTypeName< class_name >() ) { \
			list(CONFIG_UI_INIT_MEMBERS) \
		} \
		\
		class_name( const class_name& other ) : Component( ::SGF::GetTypeName< class_name >() ) { Set( other ); } \
		\
		const class_name& operator= ( const class_name& other ) { Set( other ); return *this; } \
		\
		const class_name& operator+= ( const class_name& other ) \
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
		std::string GetConfigName() const { return #class_name; } \
		\
		\
		void Serialize( ceng::CXmlFileSys* filesys ) \
		{ \
			::SGF::Component::Serialize( filesys ); \
			list(CONFIG_UI_XML_BIND_DECLARE) \
		} \
		\
		void Set( const class_name& other ) \
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
	}; \
	\
COMPONENT_REGISTER( class_name, ::SGF::GetTypeName< class_name >() )

#endif
