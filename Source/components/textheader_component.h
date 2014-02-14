#ifndef INC_TEXTHEADER_COMPONENT_H
#define INC_TEXTHEADER_COMPONENT_H

#include "../component_framework/ComponentMacro.h"

namespace as { class TextSprite; }


#define COMPONENT_TEXTHEADER(list_) \
	list_(std::string, 			key_name,				"",										MetaComment( "name of key we use to setup the text of this" ) ) \
	list_(std::string, 			default_text,			"Test text",							MetaComment( "what we write here when we do testing" ) ) \
	list_(std::string,			font_file,				"data/fonts/ubuntu_condensed_18.xml",	MetaFile() + MetaComment( "the font file we're going to use for this" ) ) \
	list_(int,					font_size,				18,										MetaData( 4, 128 ) + MetaComment( "the type size of the font" ) ) \
	list_(float,				pos_x,					0,										MetaData( 0.f, 1024.f ) ) \
	list_(float,				pos_y,					0,										MetaData( 0.f, 1024.f ) ) \
	list_(float,				scale_x,				1.f,									MetaData( -5.f, 5.f ) ) \
	list_(float,				scale_y,				1.f,									MetaData( -5.f, 5.f ) ) \
	list_(float,				rotation,				0.f,									MetaData( -3.1415f, 3.1415f ) ) \
	list_(float,				offset_x,				0,										MetaData( -512.f, 512.f ) ) \
	list_(float,				offset_y,				0,										MetaData( -512.f, 512.f ) ) \
	list_(float,				alpha,					1.f,									MetaData( 0.f, 1.f ) ) \
	list_(float,				color_r,				1.f,									MetaData( 0.f, 1.f ) ) \
	list_(float,				color_g,				1.f,									MetaData( 0.f, 1.f ) ) \
	list_(float,				color_b,				1.f,									MetaData( 0.f, 1.f ) ) \
	list_(int,					z_index,				-1,										MetaData( -1, 256 ) ) \
	list_(as::TextSprite*,		mSprite,				NULL,									MetaPrivate() ) \


DEFINE_COMPONENT( TextHeaderComponent, COMPONENT_TEXTHEADER);

#undef COMPONENT_TEXTHEADER

#endif // INC_TEXTHEADER_COMPONENT_H

