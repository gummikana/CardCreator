#ifndef INC_SIMPLE_PROFILER_VIEWER_H
#define INC_SIMPLE_PROFILER_VIEWER_H

#include <map>
#include <string>

#include <game_utils/actionscript/sprite.h>
#include "simple_profiler.h"

class SimpleProfilerViewerEntry;
namespace as { class TextSprite; }

//-----------------------------------------------------------------------------

class SimpleProfilerViewer : public as::Sprite
{
public:
	
	SimpleProfilerViewer();
	
	virtual void				Update( float dt );
	SimpleProfilerViewerEntry*	GetEntry( const std::string& name );

	
	SimpleProfilerViewerEntry* mLabels;
	std::map< std::string, SimpleProfilerViewerEntry* > mEntries;
};

//-----------------------------------------------------------------------------

class SimpleProfilerViewerEntry : public as::Sprite
{
public:
	enum {
		SPRITE_NAME,
		SPRITE_CURRENT,
		SPRITE_AVERAGE,
		// SPRITE_MIN_VALUE,
		SPRITE_MAX_VALUE,
		SPRITE_TOTAL_VALUE,
		_SPRITE_COUNT 
	};

	SimpleProfilerViewerEntry();


	void ParseStats( const std::string& name, const SimpleProfilerGlobal::TProfilerData* data );
	void DoLabels();


	std::vector< as::TextSprite* > mTextSprites;
};

//-----------------------------------------------------------------------------

#endif
