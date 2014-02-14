#ifndef INC_LOAD_LEVEL_H
#define INC_LOAD_LEVEL_H

#include <map>
#include <poro/poro_types.h>
#include <utils/color/ccolor.h>
#include <utils/array2d/carray2d.h>
#include <utils/imagetoarray/imagetoarray.h>

//-----------------------------------------------------------------------------

struct Uint32Cmpr
{
	bool operator()( poro::types::Uint32 a, poro::types::Uint32 b ) { return (a == b); }
};

struct Uint32AlphaTest
{
	Uint32AlphaTest() { ceng::CColorUint8::InitMasks(); }

	bool operator()( poro::types::Uint32 c ) const 
	{
		if( c == 0 ) return false;
		// alpha mask check...
		if( ( c & ceng::CColorUint8::AMask ) == 0 ) return false;

		return true;
	}
};

struct RectData
{
	RectData() : color( 0 ), x(0), y(0), w(0), h(0) { }
	RectData( int x, int y, int w, int h, poro::types::Uint32 c ) :
		color( c ), x( x ), y( y ), w( w ), h( h ) { }

	poro::types::Uint32 color;
	int x;
	int y;
	int w;
	int h;
};


std::vector< RectData > LoadImageRect( const std::string& filename );
std::vector< RectData > LoadImageRectSimple( const std::string& filename );


template< class TileType, class TileTypeCompare >
class ImageLevelReader
{
public:

	void AddType( poro::types::Uint32 color, const TileType& type )
	{
		myConverts.insert( std::pair< poro::types::Uint32, TileType >( color, type ) );
	}

	TileType GetTypeForColor( poro::types::Uint32 p )
	{
		typename std::map< poro::types::Uint32, TileType >::iterator i;
		i = myConverts.find( p );
		if( i == myConverts.end() )
			return TileType();
		else
			return i->second;
	}

	poro::types::Uint32 GetColorForType( const TileType& type )
	{
		TileTypeCompare compare_func;
		typename std::map< poro::types::Uint32, TileType >::iterator i;
		for( i = myConverts.begin(); i != myConverts.end(); ++i )
		{
			if( compare_func( i->second, type ) )
				return i->first;
		}

		return 0;
	}

	std::map< poro::types::Uint32, TileType > myConverts;
};


//-----------------------------------------------------------------------------

typedef poro::types::Uint32								TileType;
typedef ImageLevelReader< poro::types::Uint32, Uint32Cmpr >				TileConverter;

//-----------------------------------------------------------------------------

ceng::CArray2D< TileType >				LoadGameDataFromImage( const std::string& filename, TileConverter* converter );
ceng::CArray2D< poro::types::Uint32 >	LoadDataFromImage( const std::string& filename, ImageLevelReader< poro::types::Uint32, Uint32Cmpr >* converter );
void									SaveGameDataToImage( const std::string& filename, const ceng::CArray2D< TileType >& level_data, TileConverter* converter );

ceng::CArray2D< TileType >				LoadGameDataFromTxt( const std::string& filename );
void									SaveGameDataToTxt( const std::string& filename, const ceng::CArray2D< TileType >& level_data );

//-----------------------------------------------------------------------------

#endif
