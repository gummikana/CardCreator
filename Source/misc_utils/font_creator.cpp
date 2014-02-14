#include "font_creator.h"

#include <utils/xml/cxml.h>
// #include <vector_utils/vector_utils.h>
#include <utils/filesystem/filesystem.h>
#include <game_utils/font/cfont.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"



unsigned char ttf_buffer[1<<25];

const int width = 4096;
const int height = 20480;

unsigned char bmap[ width*height ];

void CreateFont(
	const std::string& ttf_file,
	float size,
	const std::string& xml_filename,
	const std::string& texture_file )
{
	memset( bmap, 0, width*height );
	memset( ttf_buffer, 0, 1 << 25 );
	FILE* fptr = fopen(ttf_file.c_str(), "rb");

	if( fptr == NULL ) 
	{
		std::cout << "Error reading file: " << ttf_file << std::endl;
		return;
	}

	fread(ttf_buffer, 1, 1<<25, fptr);

	stbtt_bakedchar g_cdata[95]; // ASCII 32..126 is 95 glyphs
	stbtt_BakeFontBitmap( ttf_buffer, 0, size, bmap, width, height, 32, 95, g_cdata);

	CFont* font = new CFont;
	font->SetLineHeight( size );
	font->SetTextureFilename( texture_file );
	
	int tsize_x = 0;
	int tsize_y = 0;

	for( int i = 0; i < 95; ++i ) 
	{
		font->SetCharQuad( i + 32, 
			new CFont::CharQuad( 
				types::rect( 
					(float)g_cdata[ i ].x0, 
					(float)g_cdata[ i ].y0,
					(float)(g_cdata[ i ].x1 - g_cdata[ i ].x0),
					(float)(g_cdata[ i ].y1 - g_cdata[ i ].y0) ),
				types::vector2( 
					g_cdata[ i ].xoff, 
					g_cdata[ i ].yoff ),
				g_cdata[ i ].xadvance ) );

		if( g_cdata[ i ].x1 > tsize_x ) tsize_x = g_cdata[ i ].x1;
		if( g_cdata[ i ].y1 > tsize_y ) tsize_y = g_cdata[ i ].y1;
	}

	// int result = poro::ImageSave( icon_file.c_str(), w, h, 3, pixels, w * 3 );

	tsize_x += 1;
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
	std::cout << "Done" << std::endl;
}
// #endif