#include "deckviewer_app.h"

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

class TwoSidesSprite : public as::Sprite
{
public:
	TwoSidesSprite() : 
		as::Sprite(),
		mFrontTexture( NULL ), 
		mBackTexture( NULL ) 
	{ 
	}

	virtual void Update( float dt )
	{
		as::Sprite::Update( dt );

		if( GetScaleX() < 0 ) 
			mTexture = mBackTexture;
		else
			mTexture = mFrontTexture;
	}

	Image* mFrontTexture;
	Image* mBackTexture;
};


TwoSidesSprite* LoadTwoSidedSprite( const std::string& filename_front, const std::string& filename_back )
{
	TwoSidesSprite* result = new TwoSidesSprite;

	result->mFrontTexture = as::GetTexture( filename_front );
	result->mBackTexture = as::GetTexture( filename_back );

	result->SetSize( (int)result->mFrontTexture->GetWidth(), (int)result->mFrontTexture->GetHeight() );
	result->SetFilename( filename_front );

	return result;
}

//----------
/*
class SpriteToFront : public GTween
{
public:
	SpriteToFront( float duration, as::Sprite* sprite ) : GTween( duration, true ), mSprite( sprite ) { }

	~SpriteToFront()
	{
		if( mSprite && mSprite->GetParent() ) 
		{
			as::Sprite* parent = mSprite->GetParent();
			parent->BringChildToFront( mSprite );
		}
	}

	as::Sprite* mSprite;
};*/

types::vector2 pos_facedown_deck( 270, 400 ); 
types::vector2 pos_faceup_deck( 300 + 220 * 2, 400 );

float sprite_scale = 0.5f;

} // end of anonymous namespace
// ----------------------------------------------------------------------------

DeckViewerApp::DeckViewerApp()
{
}


//-----------------------------------------------------------------------------

void DeckViewerApp::Init()
{
	DefaultApplication::Init();
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 0.15f, 0.15f, 0.15f, 1.f ) );

	// -- debug stuff ---

	mDebugLayer.reset( new DebugLayer );
	mDebugLayer->SetLoadEntity( false );
	// mDebugLayer->SetManagers( mEventManager, mEntityManager );

	// ----- sprite containers ----

	mSpriteContainer = new as::Sprite;

	// Load temp
	LoadDeck( "decks/monster_1_deck.txt" );
}

// ----------------------------------------------------------------------------

void DeckViewerApp::Clear()
{
	ceng::VectorClearPointers( mDeckFaceDown );
	ceng::VectorClearPointers( mDeckFaceUp );
}

// ----------------------------------------------------------------------------

void DeckViewerApp::LoadDeck( const std::string& filename )
{
	Clear();
	mDeckLoaded = filename;

	// 
	std::string default_back =  "data/card_back.jpg";
	std::vector< std::pair< std::string, std::string > > m_deck;

	// load the file to m_deck
	{
		std::vector< std::map< std::string, std::string > > temp;
		LoadCSVFile( filename, temp );
		for( std::size_t i = 0; i < temp.size(); ++i )
		{
			std::string filename = temp[i]["filename"];
			std::string count = temp[i]["count"];
			std::string back = temp[i]["back"];
			if( back.empty() ) back = default_back;

			if( filename.empty() == false && count.empty() == false )
			{
				int count_i = ceng::CastFromString< int >( count );
				for( int j = 0; j < count_i; ++j )
				{
					m_deck.push_back( std::make_pair( filename, back ) );
				}
			}
		}

		std::random_shuffle( m_deck.begin(), m_deck.end() );
		std::random_shuffle( m_deck.begin(), m_deck.end() );
	}

	// std::vector< as::Sprite* > mDeckFaceDown;
	// std::vector< as::Sprite* > mDeckFaceUp;
	
	for( std::size_t i = 0; i < m_deck.size(); ++i )
	{
		as::Sprite* card_sprite = LoadTwoSidedSprite( m_deck[i].first, m_deck[i].second );
		card_sprite->SetCenterOffset( 0.5f * card_sprite->GetTextureSize() );
		card_sprite->SetScale( sprite_scale, sprite_scale );
		card_sprite->SetScaleX( -card_sprite->GetScaleX() );
		card_sprite->MoveTo( pos_facedown_deck + types::vector2( ceng::Randomf( -10, 10 ), ceng::Randomf( -10, 10 ) )  );
		mSpriteContainer->addChild( card_sprite );
		mDeckFaceDown.push_back( card_sprite );
	}
}

// ----------------------------------------------------------------------------

void DeckViewerApp::Update( float dt )
{
	UpdateGTweens( dt );

	if( mDebugLayer.get() ) 
		mDebugLayer->Update( dt );

	if( mSpriteContainer )
		mSpriteContainer->Update( dt );

}

// ----------------------------------------------------------------------------

void DeckViewerApp::Draw( poro::IGraphics* graphics )
{
	if( mDebugLayer.get() ) 
		mDebugLayer->Draw( graphics );

	if( mSpriteContainer )
		as::DrawSprite( mSpriteContainer, graphics );

}

// ----------------------------------------------------------------------------

void DeckViewerApp::MouseMove(const poro::types::vec2& p)
{
}

void DeckViewerApp::MouseButtonDown(const poro::types::vec2& p, int button)
{
}

void DeckViewerApp::MouseButtonUp(const poro::types::vec2& pos, int button)
{
}

//=============================================================================

void DeckViewerApp::OnKeyDown( int key, poro::types::charset unicode )
{
	if( key == SDLK_o && Poro()->GetKeyboard()->IsCtrlDown() ) 
	{
		std::string filename = LoadFileDialog( "data\\" );
		if( filename.empty() == false ) 
			LoadDeck( filename );
	}

	if( key == SDLK_RIGHT || key == SDLK_SPACE )
		NextCard();

	if( key == SDLK_LEFT )
		PrevCard();

	if( key == SDLK_r ) 
		LoadDeck( mDeckLoaded );
}

//-----------------------------------------------------------------------------

void DeckViewerApp::NextCard()
{
	// tween test
	if( mDeckFaceDown.empty() == false )
	{
		as::Sprite* s = mDeckFaceDown.back();
		if( s ) 
		{
			GTweenClearSpriteOfTweens( s );
			as::DisplayObjectContainer* parent = s->getParent();
			parent->BringChildToFront( s );
			
			GTweenSpriteTo( s, pos_faceup_deck + types::vector2( ceng::Randomf( -10, 10 ), ceng::Randomf( -10, 10 ) ), 0.3f, ceng::easing::Quad::easeInOut );
			GTweenSpriteScaleTo( s, types::vector2( sprite_scale, sprite_scale ), 0.3f, ceng::easing::Sine::easeInOut );
		}

		mDeckFaceDown.pop_back();
		mDeckFaceUp.push_back( s );
	}
}

void DeckViewerApp::PrevCard()
{
	// tween test
	if( mDeckFaceUp.empty() == false )
	{
		as::Sprite* s = mDeckFaceUp.back();
		if( s ) 
		{
			GTweenClearSpriteOfTweens( s );

			as::DisplayObjectContainer* parent = s->getParent();
			parent->BringChildToFront( s );
			
			GTweenSpriteTo( s, pos_facedown_deck + types::vector2( ceng::Randomf( -10, 10 ), ceng::Randomf( -10, 10 ) ), 0.3f, ceng::easing::Quad::easeInOut );
			GTweenSpriteScaleTo( s, types::vector2( -sprite_scale, sprite_scale ), 0.3f, ceng::easing::Sine::easeInOut );
		}

		mDeckFaceUp.pop_back();
		mDeckFaceDown.push_back( s );
	}
}

void DeckViewerApp::OnKeyUp( int key, poro::types::charset unicode )
{
}


//=============================================================================

