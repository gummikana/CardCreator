#include "textheader_system.h"
#include "../global_data.h"

#include <game_utils/font/cfont.h>
#include <utils/filesystem/filesystem.h>
#include "../misc_utils/font_creator.h"

namespace 
{

std::string MakeSureFontExists( const std::string& ttf_file, int size )
{
	std::string font_name = ceng::GetFilenameWithoutExtension( ttf_file );

	if( size <= 0 ) size = 16;

	font_name = "data/fonts/" + font_name + "_" + ceng::CastToString( size );

	std::string texture_filename = font_name + ".png";
	std::string xml_filename = font_name + ".xml";

	if( ceng::DoesExist( texture_filename ) == false ||
		ceng::DoesExist( xml_filename ) == false )
	{
		CreateFont( ttf_file, (float)size, xml_filename, texture_filename );
	}

	return xml_filename;
}

}

TextHeaderSystem::TextHeaderSystem( SGF::EventManager* eventManager, SGF::EntityManager* entityManager ) :
	ComponentUpdator( eventManager, entityManager )
{
}

void TextHeaderSystem::RefreshComponent( SGF::Entity* e, TextHeaderComponent* comp )
{
	as::Sprite* mSpriteContainer = GD.mSprite;

	std::string font_file = comp->font_file;
	if( ceng::GetFileExtension( font_file ) == "ttf" ) 
		font_file = MakeSureFontExists( comp->font_file, comp->font_size );
	
	std::string text = comp->default_text;
	if( GD.isCrafting == false )
		text = GD.GetData( comp->key_name );

	if( ( comp->mSprite == NULL && font_file.empty() == false ) || 
		( comp->mSprite && comp->mSprite->GetText() != text ) || 
		( comp->mSprite && comp->mSprite->GetFont()->GetFilename() != font_file ) )
	{
		if( comp->mSprite )
			delete comp->mSprite;
		else
			comp->AddAutoDelete( comp->mSprite );

		comp->mSprite = as::LoadTextSprite( font_file );
		comp->mSprite->SetName( ceng::CastToString( e->GetID() ) );
		comp->mSprite->SetText( text );
		/*comp->mSprite->SetCenterOffset( types::vector2( comp->offset_x, comp->offset_y ) );
		comp->mSprite->SetZ( comp->z_index );*/


		if( mSpriteContainer )
		{
			// if( comp->z_index == -1 )
				mSpriteContainer->addChild( comp->mSprite );
			/*else
				mSpriteContainer->addChildAt( comp->mSprite, comp->mSprite->GetZ() );*/
		}
	}
}

void TextHeaderSystem::UpdateComponent( SGF::Entity* e, TextHeaderComponent* comp )
{
	// Initialize if it doesn't exist
	if( comp->mSprite == NULL || GD.isCrafting == false )
		RefreshComponent( e, comp );

	// set the transform
	if( comp->mSprite ) 
	{
		comp->mSprite->SetScaleX( comp->scale_x );
		comp->mSprite->SetScaleY( comp->scale_y );
		comp->mSprite->SetRotation( comp->rotation );
		comp->mSprite->MoveTo( types::vector2( comp->pos_x, comp->pos_y ) );

		// set alpha every frame
		comp->mSprite->SetAlpha( comp->alpha );
		comp->mSprite->SetColor( comp->color_r, comp->color_g, comp->color_b );
	}	
}

