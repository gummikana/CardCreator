#include <limits>
#include <sdl.h>

#include <poro/poro.h>
#include <utils/random/random.h>
#include <tester/tester_console.h>
#include <utils/vector_utils/vector_utils.h>
#include <utils/string/string.h>
#include "deckviewer_app.h"
#include "pilecreator_app.h"

//-----------------------------------------------------------------------------

bool HasArgument( const std::string& argument_name, const std::vector< std::string >& args )
{
	for( std::size_t i = 0; i < args.size(); ++i ) 
		if( ceng::RemoveWhiteSpaceAndEndings( args[ i ] ) == argument_name ) return true;

	return false;
}
//-----------------------------------------------------------------------------

std::string GetArgumentParam( const std::string& argument_name, const std::vector< std::string >& args, const std::string& default_value = "" )
{
	for( std::size_t i = 0; i < args.size(); ++i ) 
	{
		if( ceng::RemoveWhiteSpaceAndEndings( args[ i ] ) == argument_name && i + 1 < args.size() )
		{
			if( args[ i + 1 ].empty() == false &&
				args[ i + 1 ][0] != '-' ) 
				return args[ i + 1 ];
		}	
	}

	return default_value;
}
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{
	std::vector< std::string > args = ceng::ArgsToVector( argc, argv );
	RunTests();
	// no need to save anything...
	// ceng::XmlSaveToFile( GD.mConfigDo, config_file, "Config" );


	{
		poro::AppConfig appconf;
		appconf.title = "Deck viewer App";
		
		appconf.internal_size_w = 1024;
		appconf.internal_size_h = 768;
		appconf.window_w = 1024;
		appconf.window_h = 768;
		

		/*appconf.internal_size_w = 1536;
		appconf.internal_size_h = 960;
		appconf.window_w = 1536;
		appconf.window_h = 960;*/

		/*appconf.internal_size_w = 1920 - 100;
		appconf.internal_size_h = 1200 - 50;
		appconf.window_w = 1920 - 100;
		appconf.window_h = 1200 - 50;
		*/

		appconf.report_fps = 1;
		appconf.framerate = 60;

		appconf.SetRandomSeed = ceng::SetRandomSeeds;
		appconf.record_events = true;
		
		/*appconf.do_a_playback = true;
		appconf.playback_file = "playbacks/140214-175946-playback-8384.poro_plbk";*/

		// poro::RunPoro< DeckViewerApp >( appconf );
		poro::RunPoro< PileCreatorApp >( appconf );
	}

	return 0;
}
