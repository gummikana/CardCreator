//-----------------------------------------------------------------------------
// Screenshotter
// =============
// 
// A simple utility that automatically takes a screenshot around start up
// (it's the 3rd frame). And then another shot at a random time into the game
//
// The utility saves these into screenshots/ folder.
//
// These functions only happen if the #define SCREENSHOTTER_AUTO_SHOTS is 
// defined.
//
// Screenshotter utility also makes it available that if you press F2 it
// creates a screenshot
//-----------------------------------------------------------------------------
#ifndef INC_SCREENSHOTTER_H
#define INC_SCREENSHOTTER_H

#include <vector>
#include <string>
#include <poro/ikeyboard_listener.h>

#include <utils/math/cvector2.h>
#include <utils/rect/crect.h>

namespace poro { class IGraphics; }

class Screenshotter : public poro::IKeyboardListener
{
public:
	Screenshotter();
	~Screenshotter();

	void Update( float dt );

	void Draw( poro::IGraphics* graphics );

	// ----
	virtual void OnKeyDown( int key, poro::types::charset unicode );
	virtual void OnKeyUp( int key, poro::types::charset unicode );
	// ----

	// returns the filename and path of the saved screenshot
	std::string DoScreenshot( const std::string& prefix = "shot_", bool add_path_before = true, const types::irect* rect = NULL );
	void		SaveScreenshot( const std::string& filename, const types::irect* rect = NULL );

private:

	void CopyScreenshotTo( const std::string& filename, const std::string& path_or_filename );

	// -- auto shots related thing
	int		mFrameCount;
	int		mTakeshotAtFrame;
	int		mFirstShotAtFrame;
	bool	mAutoshots;
	
	// --- gif recording related things
	bool				mDoingGifRect;
	types::ivector2		mGifRectStartPos;
	types::ivector2		mGifRectEndPos;
	types::ivector2		mGifExRectEndPos;

	bool		mDoingGifRecording;
	std::string mGifFilePath;
	
	std::vector< std::string > mCopyHere;
};

#endif
