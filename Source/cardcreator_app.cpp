#include "cardcreator_app.h"

#include <set>
#include <sdl.h>

#include <game_utils/tween/tween_utils.h>

#include "component_framework/EntityManager.h"
#include "component_framework/EventManager.h"
#include "component_framework/gcomponents.h"
#include "component_framework/SystemManager.h"

#include "misc_utils/debug_layer.h"
#include "misc_utils/config_ui.h"
#include "misc_utils/simple_profiler.h"

#include "global_data.h"

//---------------------------------------------------------------------------------------

#define CONFIG_MARCHING_HOLES(list_) \
	list_(bool,			display_libtess2,					true,		NULL ) \
	list_(bool,			display_poly2tri,					false,		NULL ) \
	list_(float,		camera_zoom,						6.0f,		MetaData( 1.f, 20.f ) ) \
	list_(int,			stat_fps_count,						0,			NULL ) \


DEFINE_CONFIG_UI( ConfigMarchingHoles, CONFIG_MARCHING_HOLES )

namespace { 

	ConfigMarchingHoles g_config;
	types::vector2 camera_offset( -114, 14 );
}
// ----------------------------------------------------------------------------

CardCreatorApp::CardCreatorApp()
{
}

void CardCreatorApp::Init()
{
	DefaultApplication::Init();
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 0.15f, 0.15f, 0.15f, 1.f ) );


	// --- components ----
	GComponents::Init();
	SGF::EventManager* mEventManager = GComponents::eventManager;
	SGF::EntityManager* mEntityManager = GComponents::entityManager;

	// -- debug stuff ---

	mDebugLayer.reset( new DebugLayer );
	mDebugLayer->SetManagers( mEventManager, mEntityManager );

	// ----- sprite containers ----

	mSpriteContainer = new as::Sprite;

	mCardContainer = new as::Sprite;
	mOverlay = new as::Sprite;

	// -- create card crafting ---
	mSpriteContainer->addChild( mCardContainer );
	mSpriteContainer->addChild( mOverlay );

	mSpriteContainer->SetScale( 0.75f, 0.75f );

	// -- overlay sprite
	as::Sprite* overlay = as::LoadSprite( "data/templates/poker_overlay.png" );
	mOverlay->addChild( overlay );


	as::Sprite* new_card = new as::Sprite;
	mCardContainer->addChild( new_card );
	GD.mSprite = new_card;
}

// ----------------------------------------------------------------------------

void CardCreatorApp::Update( float dt )
{
	UpdateGTweens( dt );

	if( mDebugLayer.get() ) 
		mDebugLayer->Update( dt );

	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_LEFT ) )	camera_offset.x++;
	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_RIGHT ) ) camera_offset.x--;
	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_UP ) )	camera_offset.y++;
	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_DOWN ) )	camera_offset.y--;

	if( mSpriteContainer )
		mSpriteContainer->Update( dt );

	SGF::SystemManager::GetSingletonPtr()->Process( dt );
}

// ----------------------------------------------------------------------------

void CardCreatorApp::Draw( poro::IGraphics* graphics )
{
	if( mDebugLayer.get() ) 
		mDebugLayer->Draw( graphics );

	if( mSpriteContainer )
		as::DrawSprite( mSpriteContainer, graphics );

}

// ----------------------------------------------------------------------------

types::vector2 TransformMouseForSprite( as::Sprite* sprite, const types::vector2& mouse_pos )
{
	using namespace as;
	std::vector< const DisplayObjectContainer* > parents;
	sprite->getParentTree( parents );

	as::types::xform xform;
	for( int i = (int)parents.size() - 1; i > 0; --i )
	{
		cassert( parents[ i ] );
		const Sprite* parent = dynamic_cast< const Sprite* >( parents[ i ] );
		if( parent )
			xform = ceng::math::Mul( xform, parent->GetXForm() );
	}

	// xform = ceng::math::Mul( xform, this->GetXForm() );

	return ceng::math::MulT( xform, mouse_pos );
}

void CardCreatorApp::MouseMove(const poro::types::vec2& p)
{
}

void CardCreatorApp::MouseButtonDown(const poro::types::vec2& p, int button)
{
	if( button == poro::Mouse::MOUSE_BUTTON_LEFT ) 
	{
		types::vector2 mouse_pos = TransformMouseForSprite( mCardContainer, types::vector2( p ) );
		std::vector< as::Sprite* > sprites = mCardContainer->FindSpritesAtPoint( mouse_pos );
		std::cout << "sprites.size()" << sprites.size() << std::endl;

		/*SGF::Entity* e = mEntityManager->GetClosestEntity(types::vector2( p ));

		if ( e != NULL )
			mDebugLayer->SetEntity(e);
			*/
	}
}

void CardCreatorApp::MouseButtonUp(const poro::types::vec2& pos, int button)
{
}

//=============================================================================

void CardCreatorApp::OnKeyDown( int key, poro::types::charset unicode )
{
	if( key == SDLK_0 ) 
		mOverlay->SetAlpha( 0.f );
	else if( key >= SDLK_1 && key <= SDLK_9 )
	{
		float value = ( ( key - SDLK_1 ) + 1 ) / 10.f;
		mOverlay->SetAlpha( value );
	}
}

void CardCreatorApp::OnKeyUp( int key, poro::types::charset unicode )
{
}

//=============================================================================

