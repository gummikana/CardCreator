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
#include "component_framework/EntitySerializer.h"

#include "misc_utils/debug_layer.h"
#include "misc_utils/config_ui.h"
#include "misc_utils/simple_profiler.h"

#include <utils/imagetoarray/imagetoarray.h>

#include "resize_image.h"
#include "global_data.h"

//---------------------------------------------------------------------------------------
#ifdef WIN32
#include <SDL_syswm.h>

void MoveWindowTo( int x, int y )
{
	SDL_SysWMinfo i;
	SDL_VERSION( &i.version );
	if ( SDL_GetWMInfo ( &i) ) {
		HWND hwnd = i.window;
		SetWindowPos( hwnd, HWND_TOP, x, y,  0, 0,          // Ignores size arguments. 
                 SWP_FRAMECHANGED );
	}
}
#undef LoadImage
#endif // WIN32


template< class T >
inline void SWAP_RED_AND_BLUE( T& color ) 
{
	T red = 0;
	T blue = 0;

	red = 0x00ff0000 & color;
	blue = 0x000000ff & color;

	red = red >> 16;
	blue = blue << 16;


	color = ( color & 0xff00ff00 ) | red | blue;
}

struct Page
{
	Page()
	{
	
	}

	void Init( int pixel_size_x, int pixel_size_y, int card_count_x, int card_count_y, int border_size = 4 )
	{
		card_count = 0;
		types::ivector2 pagesize( 2480, 3508 );
		data.Resize( pagesize.x, pagesize.y );
		data.SetEverythingTo( 0xFFFFFFFF );

		size.Set( pixel_size_x, pixel_size_y );
		border_buffer.Set( border_size, border_size );

		types::ivector2 start_pos =( pagesize - types::ivector2( card_count_x * ( size.x + border_buffer.x ), card_count_y * ( size.y + border_buffer.y ) ) );
		start_pos.x *= 0.5f;
		start_pos.y *= 0.5f;

		types::ivector2 pos = start_pos;
		for( int y = 0; y < card_count_y; ++y )
		{
			for( int x = 0; x < card_count_x; ++x )
			{
				points.push_back( pos );
				pos.x += size.x + border_buffer.x;
			}

			pos.x = start_pos.x;
			pos.y += size.y + border_buffer.y;
		}
	}

	void Clear()
	{
		card_count = 0;
		data.SetEverythingTo( 0xFFFFFFFF );
	}

	ceng::CArray2D< Uint32 > data;
	int card_count;
	std::vector< types::ivector2 > points;
	types::ivector2	 size;
	types::ivector2 border_buffer;

	void AddCardToPage( const ceng::CArray2D< Uint32 >& image )
	{
		types::ivector2 image_pos = ( types::ivector2( image.GetWidth(), image.GetHeight() ) - size ) * 0.5f;
		types::ivector2 page_pos = points[ card_count ];	
		for( int y = -border_buffer.y; y < size.y + border_buffer.y; ++y )
		{
			for( int x = -border_buffer.x; x < size.x + border_buffer.x; ++x )
			{
				if( x >= 0 && y >= 0 && x < size.x && y < size.y )
				{
					data.At( x + page_pos.x, y + page_pos.y ) = image.At( x + image_pos.x, y + image_pos.y );
					SWAP_RED_AND_BLUE( data.At( x + page_pos.x, y + page_pos.y ) );
				}
				else
				{
					data.At( x + page_pos.x, y + page_pos.y ) = 0xFFe8e8e8;
				}
			}
		}

		card_count++;
	}

	bool IsFull() { if( card_count >= (int)points.size() ) return true; else return false; }
};

void LoadCSVFile( const std::string& csv_file, std::vector< std::map< std::string, std::string > >& result );

void ParseCards( const std::string& filename, const std::string& output_prefix = "output/test_case_",  const types::ivector2& pixel_size = types::ivector2( 550, 550 ), const types::ivector2& card_page_count = types::ivector2( 4, 6 ), int border = 4 )
{
	std::vector< std::map< std::string, std::string > > values;
	LoadCSVFile( filename, values );

	// void	LoadImage( const std::string& filename, ceng::CArray2D< poro::types::Uint32 >& out_array2d, bool include_alpha );
	Page p;
	p.Init( pixel_size.x, pixel_size.y, card_page_count.x, card_page_count.y, border );
	int pagec = 0;
	
	for( std::size_t i = 0; i < values.size(); ++i )
	{
		int count = ceng::CastFromString< int >(values[i]["count"]);
		std::string filename = values[i]["filename"];

		if( filename.empty() ) 
			continue;

		ceng::CArray2D< poro::types::Uint32 > image;
		LoadImage( filename, image, true );

		if( image.GetWidth() <= 0 || image.GetHeight() <= 0 )
		{
			std::cout << "Error couldn't load image: " << filename << std::endl;
		}
		else
		{
			for( int j = 0; j < count; ++j )
			{
				p.AddCardToPage( image );
				if( p.IsFull() ) 
				{
					SaveImage( output_prefix + ceng::CastToString( pagec ) + ".png", p.data );
					pagec++;
					p.Clear();
				}
			}
		}
	}

	// last case
	SaveImage( output_prefix + ceng::CastToString( pagec ) + ".png", p.data );
	pagec++;
	p.Clear();

}

//

void ParseCards( std::vector< std::pair< std::string, int > >& filenames, const std::string& output_prefix = "output/test_case_", 
				const types::ivector2& pixel_size = types::ivector2( 550, 550 ), 
				const types::ivector2& card_page_count = types::ivector2( 4, 6 ),
				const types::ivector2& resize_size = types::ivector2( 550, 550 ),
				int border_size = 3,
				bool rotate = false )
{

	// void	LoadImage( const std::string& filename, ceng::CArray2D< poro::types::Uint32 >& out_array2d, bool include_alpha );

	Page p;
	p.Init( pixel_size.x, pixel_size.y, card_page_count.x, card_page_count.y, border_size );
	int pagec = 0;
	
	for( std::size_t i = 0; i < filenames.size(); ++i )
	{
		int count = filenames[i].second;
		std::string filename = filenames[i].first;

		if( filename.empty() ) 
			continue;

		ceng::CArray2D< poro::types::Uint32 > image;
		LoadImage( filename, image, true );
		if( image.GetWidth() <= 0 || image.GetHeight() <= 0 )
		{
			std::cout << "Error couldn't load image: " << filename << std::endl;
		}
		else
		{
			// if we need to rotate
			if( rotate )
			{
				ceng::CArray2D< poro::types::Uint32 > rotated_image;
				ImageRotate90( image, rotated_image );
				image = rotated_image;
			}

			// if we need to resize
			if( true )
			{
				ceng::CArray2D< poro::types::Uint32 > resized_image;
				// 1095, 958 );
				ImageResize( image, resized_image, resize_size.x, resize_size.y );
				image = resized_image;
			}

			for( int j = 0; j < count; ++j )
			{
				p.AddCardToPage( image );
				if( p.IsFull() ) 
				{
					SaveImage( output_prefix + ceng::CastToString( pagec ) + ".png", p.data );
					pagec++;
					p.Clear();
				}
			}
		}
	}

	// last case
	SaveImage( output_prefix + ceng::CastToString( pagec ) + ".png", p.data );
	pagec++;
	p.Clear();

}


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

void LoadDeckFile( const std::string& csv_file, std::vector< std::pair< std::string, int > >& result )
{
	std::vector< std::map< std::string, std::string > > values;
	LoadCSVFile( csv_file, values );

	for( std::size_t i = 0; i < values.size(); ++i )
	{
		int count = 0;
		if( values[i]["count"].empty() == false ) 
			count = ceng::CastFromString< int >(values[i]["count"]);
		std::string filename = values[i]["filename"];

		if( filename.empty() == false && count > 0 )
			result.push_back( std::make_pair( filename, count ) ) ;
	}
}

//-----------------------------------------------------------------------------

void CardCreatorApp::Init()
{
	DefaultApplication::Init();
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 0.15f, 0.15f, 0.15f, 1.f ) );

	/*ParseCards( "data/king/hexes.csv", "output/king_hex_", types::ivector2( 376, 326 ), types::ivector2( 6, 10 ), 0  );
	Poro()->Exit();
	return;*/
	

	// ParseCards( "zombie_game/combat_deck.txt", "zombie_game/output/combat_" );
	/*ParseCards( "data/rebellion.txt", "output/king_", types::ivector2( 712, 1010 ), types::ivector2( 3, 3 ), 3  );

	Poro()->Exit();
	return;*/
	// ParseCards( "space_cards.txt", "output/space_cards_", types::ivector2( 550, 550 ), types::ivector2( 4, 6 ) );
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

	MoveWindowTo( 0, 0 );

	LoadEntity( GD.GetConfig().entity_xml_file, mEntityManager );

	if( GD.GetConfig().parse_automatically )
	{
		std::vector< std::pair< std::string, int > > filenames = DoAllTheCards();

		// write the filenames and card counts out
		if( GD.GetConfig().deck_output.empty() == false )
		{
			// count,filename
			std::fstream fout( GD.GetConfig().deck_output.c_str(), std::ios::out );
			fout << "count,filename" << std::endl;
			for( int i = 0; i < filenames.size(); ++i )
			{
				fout << filenames[i].second << "," << filenames[i].first << std::endl;
			}
		}

		// 1095, 958 );
		if( GD.GetConfig().create_grid )
		{
			ParseCards( filenames, 
				GD.GetConfig().grid_output_prefix, 
				GD.GetConfig().grid_single_image_size,  
				GD.GetConfig().grid_number_on_page,  
				GD.GetConfig().grid_single_image_resize,
				GD.GetConfig().grid_border_size,
				GD.GetConfig().grid_rotate_90 );
		}
	}

	if( GD.GetConfig().create_grid_from_file.empty() == false )
	{
		std::vector< std::pair< std::string, int > > filenames;
		LoadDeckFile( GD.GetConfig().create_grid_from_file, filenames );

		ParseCards( filenames,
				GD.GetConfig().grid_output_prefix, 
				GD.GetConfig().grid_single_image_size,  
				GD.GetConfig().grid_number_on_page,  
				GD.GetConfig().grid_single_image_resize,
				GD.GetConfig().grid_border_size,
				GD.GetConfig().grid_rotate_90 );
	}
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

// returns a list of filenames
std::vector< std::pair< std::string, int > > CardCreatorApp::DoAllTheCards()
{
	std::vector< std::pair< std::string, int > > result;
	std::vector< std::map< std::string, std::string > > data_set;
	LoadCSVFile( GD.GetConfig().csv_file, data_set );

	GD.isCrafting = false;

	for( std::size_t i = 0; i < data_set.size(); ++i )
	{
		std::map< std::string, std::string >::iterator j;
		int number_of_cards = 1;
		for( j = data_set[i].begin(); j != data_set[i].end(); ++j )
		{
			GD.SetData( j->first, j->second );
			if( j->first == "number_of_cards" ) number_of_cards = ceng::CastFromString< int >( j->second );
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


		SaveTheCard( GD.GetConfig().output_folder + name + ".png" );


		result.push_back( std::make_pair( GD.GetConfig().output_folder + name + ".png", number_of_cards ) );

		// put the count and the name into something

		// ---
	}

	GD.isCrafting = true;
	Poro()->GetGraphics()->SetFillColor( poro::GetFColor( 0.15f, 0.15f, 0.15f, 1.f ) );
	return result;
}



void CardCreatorApp::SaveTheCard( const std::string& filename )
{
	// this needs to be somewhere else...
	// Poro()->GetGraphics()->SaveScreenshot( filename, 0, 0, 825, 1125 );
	// hexes
	// Poro()->GetGraphics()->SaveScreenshot( filename, 0, 0, 1200, 1050 );
	Poro()->GetGraphics()->SaveScreenshot( filename, 0, 0, GD.GetConfig().screenshot_size.x, GD.GetConfig().screenshot_size.y );

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

