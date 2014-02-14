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

	if( ( scomp->mSprite == NULL && scomp->image_file.empty() == false ) || 
		( scomp->mSprite && scomp->mSprite->GetFilename() != scomp->image_file ) ) 
	{
		if( scomp->mSprite )
			delete scomp->mSprite;
		else
			scomp->AddAutoDelete( scomp->mSprite );

		scomp->mSprite = as::LoadSprite( scomp->image_file );
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

void ImageSystem::UpdateComponent( SGF::Entity* e, ImageComponent* scomp ) 
{
	// Initialize if it doesn't exist
	if( scomp->mSprite == NULL )
		RefreshComponent( e, scomp );

	// set the transform
	if( scomp->mSprite ) 
	{
		if( e->GetTransform() )
			scomp->mSprite->SetXForm( *e->GetTransform() );

		// set alpha every frame
		scomp->mSprite->SetAlpha( scomp->alpha );
		scomp->mSprite->SetColor( scomp->color_r, scomp->color_g, scomp->color_b );
	}
	
	// scomp->sprite->MoveTo( types::vector2( scomp->position_x, scomp->position_y ) );
}

//-----------------------------------------------------------------------------
