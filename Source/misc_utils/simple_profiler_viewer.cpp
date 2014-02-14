#include "simple_profiler_viewer.h"

#include <game_utils/actionscript/actionscript.h>

//-----------------------------------------------------------------------------

SimpleProfilerViewer::SimpleProfilerViewer() : 
	as::Sprite(),
	mLabels( NULL ),
	mEntries()
{
	mLabels = new SimpleProfilerViewerEntry;
	mLabels->DoLabels();
	addChild( mLabels );
}
	
void SimpleProfilerViewer::Update( float dt )
{
	as::Sprite::Update( dt );

	SimpleProfilerGlobal* global = ceng::GetSingletonPtr< SimpleProfilerGlobal >();
	for( std::map< std::string, SimpleProfilerGlobal::TProfilerData* >::const_iterator i = global->mDataMap.begin(); i != global->mDataMap.end(); ++i )
	{
		SimpleProfilerViewerEntry* entry = GetEntry( i->first );
		if( entry ) entry->ParseStats( i->first, i->second );
	}
}

SimpleProfilerViewerEntry* SimpleProfilerViewer::GetEntry( const std::string& name )
{
	if( mEntries[ name ] == NULL ) 
	{
		SimpleProfilerViewerEntry* entry = new SimpleProfilerViewerEntry;
		entry->MoveTo( types::vector2( 0, ( mEntries.size() ) * 20.f ) );
		mEntries[ name ] = entry;
		addChild( entry );
	}

	return mEntries[ name ];
}
	
//-----------------------------------------------------------------------------

SimpleProfilerViewerEntry::SimpleProfilerViewerEntry() :
	mTextSprites()
{
	std::string font_file = "data/fonts/font_arial.xml";
	mTextSprites.resize( _SPRITE_COUNT );
	const float task_name_length = 260.f;
	float x_step = ( 800.f - task_name_length ) / ( _SPRITE_COUNT );
	for( int i = 0; i < _SPRITE_COUNT; ++i ) 
	{
		as::TextSprite* text_sprite = as::LoadTextSprite( font_file );
		text_sprite->SetColor( 1, 1, 1 );
		float x_position = task_name_length + ( i - 1 ) * x_step;
		if( i == 0 ) x_position = 10.f;
		text_sprite->MoveTo( types::vector2( x_position, 0 ) );

		addChild( text_sprite );
		mTextSprites[ i ] = text_sprite;
	}
	
}

namespace {
	
	template< class T >
	std::string Profiler_ToString( T value )
	{
		if( value == 0 ) return "0";
		std::stringstream ss;
		ss << std::fixed << std::setprecision(4);
		ss << value;
		return ss.str();
	}
} // end of anonymous namespace

void SimpleProfilerViewerEntry::ParseStats( const std::string& name, const SimpleProfilerGlobal::TProfilerData* data )
{
	mTextSprites[ SPRITE_NAME		]->SetText( name );
	mTextSprites[ SPRITE_CURRENT	]->SetText(		Profiler_ToString( data->GetCurrent() ) );
	mTextSprites[ SPRITE_AVERAGE	]->SetText(		Profiler_ToString( data->GetAverage() ) );
	// mTextSprites[ SPRITE_MIN_VALUE	]->SetText(		Profiler_ToString( data->GetMin() ) );
	mTextSprites[ SPRITE_MAX_VALUE	]->SetText(		Profiler_ToString( data->GetMax() ) );
	mTextSprites[ SPRITE_TOTAL_VALUE ]->SetText(	Profiler_ToString( data->GetTotal() ) );
}

void SimpleProfilerViewerEntry::DoLabels()
{
	mTextSprites[ SPRITE_NAME		]->SetText( "NAME" );
	mTextSprites[ SPRITE_CURRENT	]->SetText( "LATEST" );
	mTextSprites[ SPRITE_AVERAGE	]->SetText(	"AVERAGE" );
	// mTextSprites[ SPRITE_MIN_VALUE	]->SetText(	"MINIMUM" );
	mTextSprites[ SPRITE_MAX_VALUE	]->SetText(	"MAXIMUM" );
	mTextSprites[ SPRITE_TOTAL_VALUE ]->SetText( "TOTAL TIME" );
}

