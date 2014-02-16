#include "cardcreator_app.h"

#include <set>
#include <sdl.h>

#include <utils/filesystem/filesystem.h>
#include <game_utils/tween/tween_utils.h>

#include "component_framework/EntityManager.h"
#include "component_framework/EventManager.h"
#include "component_framework/gcomponents.h"
#include "component_framework/SystemManager.h"
#include "component_framework/ComponentUpdator.h"

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

//-----------------------------------------------------------------------------

void LoadCSVFile( const std::string& csv_file, std::vector< std::map< std::string, std::string > >& result )
{
	std::vector< std::string > input;
	ceng::ReadFileToVector( csv_file, input );

	if( input.empty() ) 
		return;

	std::vector< std::string > labels = ceng::StringSplit( ",", input[0] );
	result.resize( input.size() - 1 );
	for( std::size_t i = 1; i < input.size(); ++i )
	{
		std::vector< std::string > data = ceng::StringSplit( ",", input[i] );
		for( std::size_t j = 0; j < data.size(); ++j )
		{
			result[ i - 1 ][ labels[j] ] = data[j];
		}
	}
}

//-----------------------------------------------------------------------------

void CardCreatorApp::Init()
{
	DefaultApplication::Init();
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 0.15f, 0.15f, 0.15f, 1.f ) );

	// test
	// LoadCSVFile( "data/nomoremeat.csv" );

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

	mSpriteContainer->SetScale( 1.0f, 1.0f );

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

	if( key == SDLK_SPACE )
		DoAllTheCards();
		// SaveTheCard( "output/test_card.png" );
}

void CardCreatorApp::OnKeyUp( int key, poro::types::charset unicode )
{
}

//=============================================================================

void CardCreatorApp::DoAllTheCards()
{
	std::vector< std::map< std::string, std::string > > data_set;
	LoadCSVFile( "data/nomoremeat.csv", data_set );

	GD.isCrafting = false;

	for( std::size_t i = 0; i < data_set.size(); ++i )
	{
		std::map< std::string, std::string >::iterator j;
		for( j = data_set[i].begin(); j != data_set[i].end(); ++j )
		{
			GD.SetData( j->first, j->second );
		}

		// --- card data has been set
		// disable overlay, set color to white
		mOverlay->SetVisibility( 0 );
		mOverlay->SetAlpha( 0 );
		Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 1, 1, 1, 1 ) );

		UpdateStep();

		// --- save the card

		std::string name = data_set[i]["name"];
		if( name.empty() ) 
			name = ceng::CastToString( i );


		SaveTheCard( "output/" + name + ".png" );

		// put the count and the name into something

		// ---
	}

	GD.isCrafting = true;
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 0.15f, 0.15f, 0.15f, 1.f ) );
}



void CardCreatorApp::SaveTheCard( const std::string& filename )
{
	Poro()->GetGraphics()->SaveScreenshot( filename, 0, 0, 825, 1125 );
}

//=============================================================================

void CardCreatorApp::UpdateStep()
{
	for( int i = 0; i < 2; ++i )
	{
		float dt = 1.f / 60.f;

		SetComponentRefresh( true );
		SGF::SystemManager::GetSingletonPtr()->Process( dt );

		Poro()->GetApplication()->Update( dt );

		Poro()->GetGraphics()->BeginRendering();
		Poro()->GetApplication()->Draw(Poro()->GetGraphics());
		Poro()->GetGraphics()->EndRendering();
	}
}

//=============================================================================

