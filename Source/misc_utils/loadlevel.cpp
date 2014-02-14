#include "loadlevel.h"
#include "array2d_utils.h"


#define STBI_HEADER_FILE_ONLY
#include <poro/external/stb_image.h>
#undef STBI_HEADER_FILE_ONLY
#include <sdl.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <poro/iplatform.h>
#include <poro/igraphics.h>
#include <utils/string/string.h>
#include "../game_types.h"


using namespace imagetoarray;

//=============================================================================

bool IsInRectVector( const RectData& r, const std::vector< RectData >& rects )
{
	for( std::size_t i = 0; i < rects.size(); ++i )
	{
		if( r.x >= rects[ i ].x &&
			r.x <= rects[ i ].x + rects[ i ].w &&
			r.y >= rects[ i ].y &&
			r.y <= rects[ i ].y + rects[ i ].h &&
			r.x + r.w >= rects[ i ].x &&
			r.x + r.w <= rects[ i ].x + rects[ i ].w &&
			r.y + r.h >= rects[ i ].y &&
			r.y + r.h <= rects[ i ].y + rects[ i ].h )
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
struct SortRectsBy
{
	
	bool operator()( const RectData& a, const RectData& b ) const
	{ 
		return (a.w > b.w);
	}
};





//-----------------------------------------------------------------------------
std::vector< RectData > LoadImageRect( const std::string& filename )
{
	std::vector< RectData > first_pass;

	ceng::CArray2D< Uint32 > data = LoadDataFromImage( filename, NULL );

	for( int y = 0; y < data.GetHeight(); ++y )
	{
		for( int x = 0; x < data.GetWidth(); ++x )
		{
			if( data.At( x, y ) != 0 )
			{
				RectData new_rect( 0, 0, 0, 0, 0 );
				Array2D_FindColorArea< Uint32, Array2D_PixelCompare >( &data, x, y, new_rect, false );

				if( IsInRectVector( new_rect, first_pass ) == false )
					first_pass.push_back( new_rect );
			}
		}
	}

	// what to we look for, horizontal or vertical...
	SortRectsBy s;
	std::sort( first_pass.begin(), first_pass.end(), s );

	std::vector< RectData > result;

	for( std::size_t i = 0; i < first_pass.size(); ++i )
	{
		RectData r = first_pass[ i ];
		if( Array2D_AreaIsOf< Uint32, Array2D_PixelCompare >( r.x, r.x + r.w - 1, r.y, r.y + r.h - 1, &data, r.color ) )
		{
			result.push_back( r );
			Array2D_PaintItBlack( r.x, r.x + r.w, r.y, r.y + r.h, &data );
		}
	}

	// then we salvage what's left of the image
	
	for( int y = 0; y < data.GetHeight(); ++y )
	{
		for( int x = 0; x < data.GetWidth(); ++x )
		{
			if( data.At( x, y ) != 0 )
			{
				RectData new_rect;
				Array2D_FindColorArea< Uint32, Array2D_PixelCompare >( &data, x, y, new_rect );
				result.push_back( new_rect );
			}
		}
	}
	
	return result;
}

//=============================================================================
std::vector< RectData > LoadImageRectSimple( const std::string& filename )
{
	std::vector< RectData > result;

	ceng::CArray2D< Uint32 > data = LoadDataFromImage( filename, NULL );

	for( int y = 0; y < data.GetHeight(); ++y )
	{
		for( int x = 0; x < data.GetWidth(); ++x )
		{
			if( data.At( x, y ) != 0 )
			{
				RectData new_rect;
				Array2D_FindColorArea< Uint32, Array2D_PixelCompare >( &data, x, y, new_rect );
				result.push_back( new_rect );
			}
		}
	}

	return result;
}




//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

// template< class TileType, class Converter >
ceng::CArray2D< TileType > LoadGameDataFromImage( const std::string& filename, TileConverter* converter )
{
	TempTexture* surface = GetTexture( filename );
	// SDL_Surface* temp = SDL_LoadBMP( filename.c_str() );

	if( surface == NULL || surface->data == NULL )
	{
		std::cout << "GenerateMapFromImage() - Failed to load image: " << filename << std::endl;
		return ceng::CArray2D< TileType >();
	}
	

	ceng::CArray2D< TileType > wall_grid( surface->w, surface->h );

	if( surface )
	{
		for( int y = 0; y < surface->h; ++y )
		{
			for( int x = 0; x < surface->w; ++x )
			{
				Uint32 p = GetPixel( surface, x, y );
				
				wall_grid[ x ][ y ] = TileType();

				if( p == 0 ) 
					continue;

				TileType type = p;
				if( converter ) 
					type = converter->GetTypeForColor( p );
				wall_grid[ x ][ y ] = type;
			}
		}
	}

	delete surface;
	// SDL_FreeSurface( surface );
	return wall_grid;
}


//-----------------------------------------------------------------------------

// this is just a copy of the above thing, templates, should work with headers!!!
ceng::CArray2D< Uint32 > LoadDataFromImage( const std::string& filename, ImageLevelReader< Uint32, Uint32Cmpr >* converter )
{
	TempTexture* surface = GetTexture( filename );

	if( surface == NULL || surface->data == NULL )
	{
		std::cout << "GenerateMapFromImage() - Failed to load image: " << filename << std::endl;
		return ceng::CArray2D< Uint32 >();
	}

	ceng::CArray2D< Uint32 > wall_grid( surface->w, surface->h );

	if( surface )
	{
		for( int y = 0; y < surface->h; ++y )
		{
			for( int x = 0; x < surface->w; ++x )
			{
				Uint32 p = GetPixel( surface, x, y );
				
				wall_grid[ x ][ y ] = Uint32();

				if( p == 0 ) 
					continue;

				Uint32 type = p;
				if( converter )
					type = converter->GetTypeForColor( p );

				wall_grid[ x ][ y ] = type;
			}
		}
	}

	delete surface;
	// SDL_FreeSurface( surface );

	return wall_grid;
}

void SaveGameDataToImage( const std::string& filename, const ceng::CArray2D< TileType >& level_data, TileConverter* converter )
{
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		const Uint32 RMASK = 0xff000000;
		const Uint32 GMASK = 0x00ff0000;
		const Uint32 BMASK = 0x0000ff00;
		const Uint32 AMASK = 0x000000ff;
#else
		const Uint32 RMASK = 0x000000ff;
		const Uint32 GMASK = 0x0000ff00;
		const Uint32 BMASK = 0x00ff0000;
		const Uint32 AMASK = 0xff000000;
#endif

	int width = level_data.GetWidth();
	int height = level_data.GetHeight();

	SDL_Surface* t = SDL_CreateRGBSurface( SDL_SWSURFACE, width, height, 32, RMASK, GMASK, BMASK, AMASK);
	SDL_Surface* surface = SDL_DisplayFormat( t );
	SDL_FreeSurface( t );

	if( surface )
	{
		for( int y = 0; y < height; ++y )
		{
			for( int x = 0; x < width; ++x )
			{
				const TileType& type = level_data[ x ][ y ];
				Uint32 p = type;
				if( converter ) p = converter->GetColorForType( type );
				Uint32* target_p = (Uint32*)((Uint8*)surface->pixels + (y)  * surface->pitch + x * surface->format->BytesPerPixel );
				*target_p = p;
			}
		}
	}

	SDL_SaveBMP( surface, filename.c_str() );

	SDL_FreeSurface( surface );
}


////////////////////////////////////////////////////////////////////////////////
//
//******************************************************************************

//=============================================================================

std::string CastToString( const TileType& tile )
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(8) << ( tile );
	return ss.str();
}

TileType CastFromString( const std::string& str )
{
	Uint32 result;
	std::stringstream ss( str );
	ss >> std::hex >> ( result );
	
	return TileType( result );
}

//=============================================================================

ceng::CArray2D< TileType >	LoadGameDataFromTxt( const std::string& filename )
{
	ceng::CArray2D< TileType > result;

	std::fstream file_input;
   	std::string line;

	if( filename.empty() == false )
	{
		file_input.open( filename.c_str(), std::ios::in );

		if( !file_input.good() ) {
			std::cout << "Unable to open txt-file: " << filename.c_str() << "\n";
		}
		else
		{
			std::getline( file_input, line );
			std::vector< std::string > temp_v = ceng::Split( ",", line );
			cassert( temp_v.size() > 1 );

			int width = ceng::CastFromString< int >( temp_v[ 0 ] );
			int height = ceng::CastFromString< int >( temp_v[ 1 ] );
		
			result.Resize( width, height );
		}
		
		int x = 0;
		int y = 0;
		// read array data
		while ( file_input.good() ) 
		{
			std::getline( file_input, line );
			std::vector< std::string > temp_v = ceng::Split( ",", line );
			for( x = 0; x < (int)temp_v.size(); ++x )
			{
				if( x < result.GetWidth() &&
					y < result.GetHeight() )
				{
					result[x][y] = CastFromString( temp_v[x] );
				}
			}

			y++;
		}

		file_input.close();
	}


	return result;
}
//=============================================================================

void SaveGameDataToTxt( const std::string& filename, const ceng::CArray2D< TileType >& level_data )
{
	std::fstream output( filename.c_str(), std::ios::out );

	output << level_data.GetWidth() << ", " << level_data.GetHeight() << std::endl;
	for( int y = 0; y < level_data.GetHeight(); ++y )
	{
		for( int x = 0; x < level_data.GetWidth(); ++x )
		{
			output << CastToString( level_data[x][y] );
			if( ( x + 1 ) < level_data.GetWidth() )
				output << ", ";
		}

		output << std::endl;
	}

	output.close();
}
