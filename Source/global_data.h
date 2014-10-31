#ifndef INC_GLOBAL_DATA_H
#define INC_GLOBAL_DATA_H

#define GD GlobalData::GetSingleton()


#include <map>
#include <string>
#include <utils/singleton/csingleton.h>
#include <utils/xml/cxml.h>
#include <utils/math/cvector2.h>

namespace as { class Sprite; }

//-----------------------------------------------------------------------------

struct ConfigDo
{
	ConfigDo() : parse_automatically( false ), output_folder("output/"), screenshot_size( 825, 1125 ) { }

	bool parse_automatically;
	std::string entity_xml_file;
	std::string csv_file;
	std::string output_folder;
	types::ivector2 screenshot_size;
	
	void Serialize( ceng::CXmlFileSys* filesys )
	{
		XML_BindAttribute( filesys, parse_automatically );
		XML_BindAttribute( filesys, entity_xml_file );
		XML_BindAttribute( filesys, csv_file );
		XML_BindAttribute( filesys, output_folder );
		XML_BindAttributeAlias( filesys, screenshot_size.x, "screenshot_size_width" );
		XML_BindAttributeAlias( filesys, screenshot_size.y, "screenshot_size_height" );
	}
};

//-----------------------------------------------------------------------------

class GlobalData : public ceng::CSingleton< GlobalData >
{
public:
	~GlobalData();

	as::Sprite* mSprite;

	// --
	bool isCrafting;

	// -- data related things ---
	bool		HasData( const std::string& key_name ) const;
	std::string GetData( const std::string& key_name );
	void		SetData( const std::string& key_name, const std::string& value );
	void		ClearData();

	//---
	const ConfigDo& GetConfig() const { return mConfigDo; }

	ConfigDo mConfigDo;

private:
	GlobalData();
	
	std::map< std::string, std::string > mData;
	
	friend class ceng::CSingleton< GlobalData >;
};

//-----------------------------------------------------------------------------

#endif
