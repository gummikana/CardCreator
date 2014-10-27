#include <sdl.h>

#include <sstream>
#include <vector>

#include <utils/array2d/carray2d.h>
#include <utils/math/cvector2.h>
#include <utils/rect/crect.h>
#include <utils/color/ccolor.h>
#include <utils/imagetoarray/imagetoarray.h>

#include <poro/external/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <poro/external/stb_image_write.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "misc_utils/stb/stb_truetype.h"


struct GridParams
{
	int image_w;
	int image_h;
	int n;		// number of elements
	std::string font;
	float font_size;
	Uint32 background_color;
	Uint32 foreground_color;
	int border_size;
};



struct CharQuad
{
	CharQuad() : rect(), offset(), width( 0 ) { }
	CharQuad( const types::rect& rect, const types::vector2& offset, float width ) : rect( rect ), offset( offset ), width( width ) { }

	types::rect			rect;
	types::vector2		offset;
	float				width;
	float				height;
	
};

unsigned char ttf_buffer[1<<25];

const int bitmap_width = 4096;
const int bitmap_height = 20480;

unsigned char bmap[ bitmap_width * bitmap_height ];
std::vector< CharQuad > char_quads;
	
void CreateFont(
	const std::string& ttf_file,
	float size )
{
	memset( bmap, 0, bitmap_width*bitmap_height );
	memset( ttf_buffer, 0, 1 << 25 );
	FILE* fptr = fopen(ttf_file.c_str(), "rb");

	if( fptr == NULL ) 
	{
		std::cout << "Error reading file: " << ttf_file << std::endl;
		return;
	}

	fread(ttf_buffer, 1, 1<<25, fptr);

	stbtt_bakedchar g_cdata[95]; // ASCII 32..126 is 95 glyphs
	stbtt_BakeFontBitmap( ttf_buffer, 0, size, bmap, bitmap_width, bitmap_height, 32, 95, g_cdata);

	int tsize_x = 0;
	int tsize_y = 0;

	if( char_quads.size() < 95 + 32 )
		char_quads.resize( 95 + 32 );

	for( int i = 0; i < 95; ++i ) 
	{
		char_quads[ i + 32 ] = CharQuad(
				types::rect( 
					(float)g_cdata[ i ].x0, 
					(float)g_cdata[ i ].y0,
					(float)(g_cdata[ i ].x1 - g_cdata[ i ].x0),
					(float)(g_cdata[ i ].y1 - g_cdata[ i ].y0) ),
				types::vector2( 
					g_cdata[ i ].xoff, 
					g_cdata[ i ].yoff ),
				g_cdata[ i ].xadvance );
		
		char_quads[ i + 32 ].height = size;

		if( g_cdata[ i ].x1 > tsize_x ) tsize_x = g_cdata[ i ].x1;
		if( g_cdata[ i ].y1 > tsize_y ) tsize_y = g_cdata[ i ].y1;
	}

	// int result = poro::ImageSave( icon_file.c_str(), w, h, 3, pixels, w * 3 );

	/*tsize_x += 1;
	tsize_y += 1;

	// convert this into a 32bit image
	unsigned char* pixels = (unsigned char*)malloc(width*height*4);

	for( int y = 0; y < tsize_y; ++y )
	{
		for( int x = 0; x < tsize_x; ++x )
		{
			int p = ( 4 * x + 4 * tsize_x * y);
			// int p2 = (x + (pos_x * 3)) + ( 3 * width * ( y + pos_y));
			// int p1 = ( x + w * y )
			pixels[ p + 0 ] = 255;
			pixels[ p + 1 ] = 255;
			pixels[ p + 2 ] = 255;
			pixels[ p + 3 ] = bmap[ x + width * y ];
		}
	}

	std::cout << "Creating file: " << texture_file << " ... ";
	stbi_write_png( texture_file.c_str(), tsize_x, tsize_y, 4, pixels, tsize_x * 4 );
	std::cout << "Done" << std::endl;
	
	std::cout << "Creating file: " << xml_filename << " ... ";
	ceng::XmlSaveToFile( *font, xml_filename, "Font" );
	std::cout << "Done" << std::endl;*/
}

void BlitImage( ceng::CArray2D< Uint32 >& blit_this, ceng::CArray2D< Uint32 >& to_here, int pos_x, int pos_y )
{
	for( int y = 0; y < blit_this.GetHeight(); ++y )
	{
		for( int x = 0; x < blit_this.GetWidth(); ++x )
		{
			if( to_here.IsValid( x + pos_x, y + pos_y ) )
			{
				to_here.At( x + pos_x, y + pos_y ) = blit_this.At( x, y );
			}
		}
	}
}

Uint32 CastToColor( Uint32 c1, Uint32 c2, unsigned char how_much_of_1 )
{
	if( c1 == c2 ) return c1;
	float how_much = (float)(how_much_of_1) / 255.f;

	/*
	unsigned int c = 255 - how_much_of_1;
	types::fcolor color1;
	color1.Set8( c, c, c, 255 );
	return color1.Get32();*/

	types::fcolor color1( c1 );
	types::fcolor color2( c2 );

	types::fcolor result = how_much * color1 + ( 1.f - how_much ) * color2;

	std::swap( result.r, result.a );
	// result.SetR( ( color1.GetR() > color2.GetR() )? color1.GetR() : color2.GetR() );
	return result.Get32();
}

void BlitText( const std::string& text, ceng::CArray2D< Uint32 >& to_here, int center_x, int center_y, Uint32 fcolor )
{
	float width = 0;
	float height = 0;
	for( std::size_t i = 0; i < text.size(); ++i ) 
	{
		char c = text[i];
		if( c > char_quads.size() ) continue;
		width += char_quads[ c ].width;
		height = std::max( char_quads[ c ].rect.h, height );
	}
	
	int pos_x = (int)( center_x - 0.5f * width + 0.5f); 
	int pos_y = (int)( center_y + 0.5f * height + 0.5f ); 
	
	for( std::size_t i = 0; i < text.size(); ++i ) 
	{
		char c = text[i];
		if( c > char_quads.size() ) continue;

		int px =(int)( pos_x + char_quads[c].offset.x );
		int py =(int)( pos_y + char_quads[c].offset.y );

		for( int y = 0; y < char_quads[c].rect.h; ++y )
		{
			for( int x = 0; x < char_quads[c].rect.w; ++x )
			{
				int bitx = x + char_quads[c].rect.x;
				int bity = y + char_quads[c].rect.y;
			
				unsigned char color = bmap[ bitx + bitmap_width * bity ];
				// if( color > 0 )
				{
					to_here.At( px + x, py + y ) = CastToColor( fcolor, to_here.At( px + x, py + y ), color );
				}
			
			}
		}

		pos_x += char_quads[c].width;
	}
}

void DoAGrid( const GridParams& params, const std::string& output_filename )
{
	ceng::CArray2D< Uint32 > image( params.image_w, params.image_h );
	image.SetEverythingTo( params.background_color );
	CreateFont( params.font, params.font_size );

	// SaveImage( output_filename, image );

	float square_size = (float)( params.image_w * 2 + params.image_h * 2 ) / (float)( params.n + 4 );
	ceng::CArray2D< Uint32 > border( (int)(square_size + 0.5f), (int)(square_size + 0.5f) );
	
	for( int y = 0; y < border.GetHeight(); ++y )
	{
		for( int x = 0; x < border.GetWidth(); ++x )
		{
			Uint32 c = params.foreground_color;		
			if( x >= params.border_size && x < border.GetWidth() - params.border_size &&
				y >= params.border_size && y < border.GetHeight() - params.border_size )
			{
				c = params.background_color;
			}
			border.At( x, y ) = c;
		}
	}

	{
		types::ivector2 pos( 0, 0 );
		types::ivector2 vel( 1, 0 );
		int x_width = params.image_w / border.GetWidth();
		int y_height = params.image_h / border.GetHeight();
		int n = x_width;
		for( int i = 0; i < params.n + 1; ++i )
		{
			BlitImage( border, image, pos.x, pos.y );
			std::stringstream ss;
			ss << ( i+1);
			BlitText( ss.str(), image, pos.x + border.GetWidth() / 2, pos.y + border.GetHeight() / 2, params.foreground_color );

			types::ivector2 actual_vel = types::ivector2( vel.x * border.GetWidth(), vel.y * border.GetHeight() );
			types::ivector2 new_pos = pos + actual_vel;
			n--;
			if( n <= 0 )
			{
			
			/*if( new_pos.x + actual_vel.x > image.GetWidth() + 2 || 
				new_pos.x + actual_vel.x < -200 ||
				new_pos.y + actual_vel.y > image.GetHeight() + 2 || 
				new_pos.y + actual_vel.y < -200 )
			{*/
				// rotate vel
				if( vel.x > 0 ) { vel.Set( 0, 1 ); n = y_height - 1; }
				else if( vel.y > 0 ) { vel.Set( -1, 0 ); n = x_width; }
				else if( vel.x < 0 ) { vel.Set( 0, -1 ); n = y_height; pos.x = 0; }
				else if( vel.y < 0 ) { vel.Set( 1, 0 ); n = x_width; }
				std::cout << "n, vel: " << n << ", " << vel.x << ", " << vel.y << std::endl;
				actual_vel = types::ivector2( vel.x * border.GetWidth(), vel.y * border.GetHeight() );
				new_pos = pos + actual_vel;
			}
			
			pos = new_pos;
		}
	}

	SaveImage( output_filename, image );
}

int main(int argc, char *argv[])
{
	
	//struct GridParams
	//{
	//	int image_w;
	//	int image_h;
	//	int n;		// number of elements
	//	std::string font;
	//	Uint32 background_color;
	//	Uint32 foreground_color;
	//	int border_size;
	//};

	GridParams gridparams;
	gridparams.image_w = 2480;
	gridparams.image_h = 3508;
	gridparams.n = 100;
	gridparams.font = "data/fonts/arial.ttf";
	gridparams.font_size = 64;
	gridparams.background_color = 0xFFFFFFFF;
	gridparams.foreground_color = 0x000000FF;
	gridparams.border_size = 5;
	DoAGrid( gridparams, "grid_test.png" );
	return 0;
}
