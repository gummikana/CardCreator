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

	// -- create card crafting ---
	as::Sprite* new_card = new as::Sprite;
	mSpriteContainer->addChild( new_card );
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

void CardCreatorApp::MouseMove(const poro::types::vec2& p)
{
}

void CardCreatorApp::MouseButtonDown(const poro::types::vec2& p, int button)
{
}

void CardCreatorApp::MouseButtonUp(const poro::types::vec2& pos, int button)
{
}

//=============================================================================