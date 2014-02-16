#include "image_system.h"
#include "../global_data.h"

//-----------------------------------------------------------------------------

ImageSystem::ImageSystem( SGF::EventManager* eventManager, SGF::EntityManager* entityManager ) : 
	ComponentUpdator( eventManager, entityManager ),
	mSpriteContainer( NULL )
{

}

//-----------------------------------------------------------------------------

void ImageSystem::RefreshComponent( SGF::Entity* e, ImageComponent* scomp )
{
	mSpriteContainer = GD.mSprite;

	std::string image_file = scomp->image_file;
	if( GD.isCrafting == false )
		image_file = GD.GetData( scomp->key_name );
		

	if( ( scomp->mSprite == NULL && image_file.empty() == false ) || 
		( scomp->mSprite && scomp->mSprite->GetFilename() != image_file ) ) 
	{
		if( scomp->mSprite )
			delete scomp->mSprite;
		else
			scomp->AddAutoDelete( scomp->mSprite );

		scomp->mSprite = as::LoadSprite( image_file );
		scomp->mSprite->SetName( ceng::CastToString( e->GetID() ) );
		scomp->mSprite->SetCenterOffset( types::vector2( scomp->offset_x, scomp->offset_y ) );
		scomp->mSprite->SetZ( scomp->z_index );

		if( mSpriteContainer )
		{
			if( scomp->z_index == -1 )
				mSpriteContainer->addChild( scomp->mSprite );
			else
				mSpriteContainer->addChildAt( scomp->mSprite, scomp->mSprite->GetZ() );
		}
	}
}

//-----------------------------------------------------------------------------

void ImageSystem::UpdateComponent( SGF::Entity* e, ImageComponent* comp ) 
{
	// Initialize if it doesn't exist
	if( comp->mSprite == NULL )
		RefreshComponent( e, comp );

	if( GD.isCrafting == false ) 
		RefreshComponent( e, comp );

	// set the transform
	if( comp->mSprite ) 
	{
		// if( e->GetTransform() )
		// comp->mSprite->SetXForm( *e->GetTransform() );
		comp->mSprite->SetScaleX( comp->scale_x );
		comp->mSprite->SetScaleY( comp->scale_y );
		comp->mSprite->SetRotation( comp->rotation );
		comp->mSprite->MoveTo( types::vector2( comp->pos_x, comp->pos_y ) );

		// set alpha every frame
		comp->mSprite->SetAlpha( comp->alpha );
		comp->mSprite->SetColor( comp->color_r, comp->color_g, comp->color_b );
	}
	
	// scomp->sprite->MoveTo( types::vector2( scomp->position_x, scomp->position_y ) );
}

//-----------------------------------------------------------------------------
