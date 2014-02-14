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
		appconf.window_w = 1024;
		appconf.window_h = 768;

		appconf.report_fps = 1;
		appconf.framerate = 60;

		appconf.SetRandomSeed = ceng::SetRandomSeeds;
		appconf.record_events = true;
		
		// appconf.do_a_playback = true;
		// appconf.playback_file = "playbacks/120628-175222-playback-1492.poro_plbk";

		poro::RunPoro< CardCreatorApp >( appconf );
	}

	return 0;
}
