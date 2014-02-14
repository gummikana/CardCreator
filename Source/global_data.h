#ifndef INC_GLOBAL_DATA_H
#define INC_GLOBAL_DATA_H

#define GD GlobalData::GetSingleton()


#include <map>
#include <string>
#include <utils/singleton/csingleton.h>

namespace as { class Sprite; }

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

private:
	GlobalData();
	
	std::map< std::string, std::string > mData;
	
	friend class ceng::CSingleton< GlobalData >;
};

//-----------------------------------------------------------------------------

#endif
