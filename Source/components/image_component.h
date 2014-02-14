#ifndef INC_IMAGE_COMPONENT_H
#define INC_IMAGE_COMPONENT_H

#include <string>
#include "../component_framework/ComponentMacro.h"

// z_index -1 == front
// z_index 0 is back
// z_index 1 is second last from back, and so forth

#define COMPONENT_IMAGE(list_) \
	list_(std::string,						key_name,						"",								MetaComment( "this is the name of the data that we read from the card database" ) ) \
	list_(std::string,						image_file,						"data/temp/temp_gun.png",		MetaFile() ) \
	list_(float,							pos_x,							0,								MetaData( 0.f, 1024.f ) ) \
	list_(float,							pos_y,							0,								MetaData( 0.f, 1024.f ) ) \
	list_(float,							scale_x,						1.f,							MetaData( -5.f, 5.f ) ) \
	list_(float,							scale_y,						1.f,							MetaData( -5.f, 5.f ) ) \
	list_(float,							rotation,						0.f,							MetaData( -3.1415f, 3.1415f ) ) \
	list_(float,							offset_x,						0,								MetaData( -512.f, 512.f ) ) \
	list_(float,							offset_y,						0,								MetaData( -512.f, 512.f ) ) \
	list_(float,							alpha,							1.f,							MetaData( 0.f, 1.f ) ) \
	list_(float,							color_r,						1.f,							MetaData( 0.f, 1.f ) ) \
	list_(float,							color_g,						1.f,							MetaData( 0.f, 1.f ) ) \
	list_(float,							color_b,						1.f,							MetaData( 0.f, 1.f ) ) \
	list_(int,								z_index,						-1,								MetaData( -1, 256 ) ) \
	list_(as::Sprite*,						mSprite,						NULL,							MetaPrivate() ) \


DEFINE_COMPONENT( ImageComponent, COMPONENT_IMAGE );

#undef COMPONENT_IMAGE

#endif
