#include "pilecreator_app.h"

#include <sdl.h>

#include <utils/filesystem/filesystem.h>
#include <utils/string/string.h>
#include <utils/vector_utils/vector_utils.h>
#include <game_utils/tween/tween_utils.h>

#include "misc_utils/file_dialog.h"
#include "misc_utils/debug_layer.h"
#include "misc_utils/config_ui.h"
#include "misc_utils/simple_profiler.h"


//-----------------
// TODO:
// [ ] Shuffle the deck
// [ ] Back of the cards

//---------------------------------------------------------------------------------------
namespace {

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
//----------


} // end of anonymous namespace
// ----------------------------------------------------------------------------

PileCreatorApp::PileCreatorApp()
{
}


//-----------------------------------------------------------------------------

void PileCreatorApp::Init()
{
	DefaultApplication::Init();
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 248.f / 255.f, 245.f / 255.f, 236.f / 255.f, 1.f ) );

	// -- debug stuff ---

	mDebugLayer.reset( new DebugLayer );
	mDebugLayer->SetLoadEntity( false );
	// mDebugLayer->SetManagers( mEventManager, mEntityManager );

	// ----- sprite containers ----

	mSpriteContainer = new as::Sprite;
}

// ----------------------------------------------------------------------------

void PileCreatorApp::Clear()
{
}

// ----------------------------------------------------------------------------

void PileCreatorApp::Update( float dt )
{
	UpdateGTweens( dt );

	if( mDebugLayer.get() ) 
		mDebugLayer->Update( dt );

	if( mSpriteContainer )
		mSpriteContainer->Update( dt );

}

// ----------------------------------------------------------------------------

void PileCreatorApp::Draw( poro::IGraphics* graphics )
{
	if( mDebugLayer.get() ) 
		mDebugLayer->Draw( graphics );

	if( mSpriteContainer )
		as::DrawSprite( mSpriteContainer, graphics );

}

// ----------------------------------------------------------------------------

void PileCreatorApp::MouseMove(const poro::types::vec2& p)
{
}

void PileCreatorApp::MouseButtonDown(const poro::types::vec2& p, int button)
{
	if( button == 1 ) 
	{
		std::vector< std::string > files;
		files.push_back( "data/coins/coin001.png" );
		files.push_back( "data/coins/coin002.png" );
		files.push_back( "data/coins/coin003.png" );
		files.push_back( "data/coins/coin004.png" );
		files.push_back( "data/coins/coin005.png" );

		std::string file = files[ ceng::Random( 0, files.size() - 1 ) ];
		as::Sprite* c = as::LoadSprite( file );
		c->SetCenterOffset( 0.5f * c->GetTextureSize() );
		c->MoveTo( types::vector2( p ) );
		c->SetRotation( ceng::Randomf( -3.141596f, 3.14145f ) );
		mSpriteContainer->addChild( c );
	}
}

void PileCreatorApp::MouseButtonUp(const poro::types::vec2& pos, int button)
{
}

//=============================================================================

void PileCreatorApp::OnKeyDown( int key, poro::types::charset unicode )
{
	if( key == SDLK_s ) 
	{
		SaveTheCard( "coin_pile.png" );
	}
}

//-----------------------------------------------------------------------------


void PileCreatorApp::OnKeyUp( int key, poro::types::charset unicode )
{
}


void PileCreatorApp::SaveTheCard( const std::string& filename )
{
	Poro()->GetGraphics()->SaveScreenshot( filename );
	// Poro()->GetGraphics()->SaveScreenshot( filename, 0, 0, GD.GetConfig().screenshot_size.x, GD.GetConfig().screenshot_size.y );
}
//=============================================================================

