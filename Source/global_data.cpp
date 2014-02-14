#include "global_data.h"

//-----------------------------------------------------------------------------

GlobalData::GlobalData()
{
	mSprite = NULL;
	isCrafting = true;
}

GlobalData::~GlobalData()
{
}

//-----------------------------------------------------------------------------

bool GlobalData::HasData( const std::string& key_name ) const
{
	std::map< std::string, std::string >::const_iterator i = mData.find( key_name );
	return ( i != mData.end() );
}

std::string GlobalData::GetData( const std::string& key_name )
{
	return mData[ key_name ];
}

void GlobalData::SetData( const std::string& key_name, const std::string& value )
{
	mData[ key_name ] = value;
}

void GlobalData::ClearData()
{
	mData.clear();
}

//-----------------------------------------------------------------------------
