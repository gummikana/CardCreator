#include <limits>
#include <sdl.h>

#include <poro/poro.h>
#include <utils/random/random.h>
#include <tester/tester_console.h>
#include "cardcreator_app.h"

int main(int argc, char *argv[])
{
	RunTests();

	{
		poro::AppConfig appconf;
		appconf.title = "CardCreator App";
		/*
		appconf.internal_size_w = 2048;
		appconf.internal_size_h = 1536;
		appconf.window_w = 2048;
		appconf.window_h = 1536;
		*/

		appconf.internal_size_w = 1536;
		appconf.internal_size_h = 960;
		appconf.window_w = 1536;
		appconf.window_h = 960;

		appconf.report_fps = 1;
		appconf.framerate = 60;

		appconf.SetRandomSeed = ceng::SetRandomSeeds;
		appconf.record_events = true;
		
		appconf.do_a_playback = true;
		appconf.playback_file = "playbacks/140214-175946-playback-8384.poro_plbk";

		poro::RunPoro< CardCreatorApp >( appconf );
	}

	return 0;
}
