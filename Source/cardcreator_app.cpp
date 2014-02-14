#include "cardcreator_app.h"

#include <set>
#include <sdl.h>

#include "misc_utils/debug_layer.h"
#include "misc_utils/config_ui.h"
#include "misc_utils/simple_profiler.h"

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

	mDebugLayer.reset( new DebugLayer );
}

// ----------------------------------------------------------------------------

void CardCreatorApp::Update( float dt )
{
	if( mDebugLayer.get() ) 
		mDebugLayer->Update( dt );

	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_LEFT ) )	camera_offset.x++;
	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_RIGHT ) ) camera_offset.x--;
	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_UP ) )	camera_offset.y++;
	if( Poro()->GetKeyboard()->IsKeyDown( SDLK_DOWN ) )	camera_offset.y--;
}

void CardCreatorApp::Draw( poro::IGraphics* graphics )
{
	if( mDebugLayer.get() ) 
		mDebugLayer->Draw( graphics );

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