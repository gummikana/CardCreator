#include "screenshotter.h"

#include <SDL.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>

#include <poro/poro.h>
#include <poro/platform_defs.h>

#include <utils/filesystem/filesystem.h>
#include <utils/random/random.h>
#include <utils/string/string.h>
#include <utils/math/math_utils.h>

#include <game_utils/drawlines/drawlines.h>

// 
#ifdef PORO_PLAT_WINDOWS
#include <windows.h>
#endif

// #define SCREENSHOTTER_AUTO_SHOTS

/*#ifdef SCREENSHOTTER_AUTO_SHOTS
#define SCREENSHOTTER_DROPBOX_AUTOS
#endif*/

#define PATH_TO_IMAGEMAGICK "c:\\utils\\ImageMagick-6.8.1-9\\convert.exe"

//-----------------------------------------------------------------------------

namespace 
{

std::vector< std::string > LoadFileToVector( const std::string& filename )
{
	std::vector< std::string > result;

	std::string line;
	std::ifstream file_input( filename.c_str() );

	while ( file_input.good() ) 
	{
		std::getline( file_input, line );
		line = ceng::RemoveWhiteSpaceAndEndings( line );
		if( line.empty() == false )
			result.push_back( line );
	}

	return result;
}


//-----------------------------------------------------------------------------

#ifdef PORO_PLAT_WINDOWS
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// this is taken from here http://goffconcepts.com/techarticles/development/cpp/createprocess.html
size_t ExecuteProcess( const std::string& full_path_to_exe, const std::string& params, size_t SecondsToWait = 500 ) 
{ 

	std::wstring FullPathToExe = s2ws( full_path_to_exe );
	std::wstring Parameters = s2ws( params );

	size_t iMyCounter = 0, iReturnVal = 0, iPos = 0; 
	DWORD dwExitCode = 0; 
	std::wstring sTempStr = L""; 

	/* - NOTE - You should check here to see if the exe even exists */ 

	/* Add a space to the beginning of the Parameters */ 
	if (Parameters.size() != 0) 
	{ 
		if (Parameters[0] != L' ') 
		{ 
			Parameters.insert(0,L" "); 
		} 
	} 

	/* The first parameter needs to be the exe itself */ 
	sTempStr = FullPathToExe; 
	iPos = sTempStr.find_last_of(L"\\"); 
	sTempStr.erase(0, iPos +1); 
	Parameters = sTempStr.append(Parameters); 

	 /* CreateProcessW can modify Parameters thus we allocate needed memory */ 
	static wchar_t pwszParam[ 1024 ];
	// wchar_t * pwszParam = new wchar_t[Parameters.size() + 1]; 
	if (Parameters.size() > 1024 ) 
	{ 
		return 1; 
	} 
	const wchar_t* pchrTemp = Parameters.c_str(); 
	wcscpy_s(pwszParam, Parameters.size() + 1, pchrTemp); 

	/* CreateProcess API initialization */ 
	STARTUPINFOW siStartupInfo; 
	PROCESS_INFORMATION piProcessInfo; 
	memset(&siStartupInfo, 0, sizeof(siStartupInfo)); 
	memset(&piProcessInfo, 0, sizeof(piProcessInfo)); 
	siStartupInfo.cb = sizeof(siStartupInfo); 
	siStartupInfo.wShowWindow = 0;
	siStartupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK;

	if(!CreateProcessW(const_cast<LPCWSTR>(FullPathToExe.c_str()), 
							pwszParam, 0, 0, false, 
							CREATE_DEFAULT_ERROR_MODE, 0, 0, 
							&siStartupInfo, &piProcessInfo ) ) 
	{ 
		 /* Watch the process. */ 
		/*
		dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (SecondsToWait * 1000)); 
	} 
	else 
	{ */
		/* CreateProcess failed */ 
		iReturnVal = GetLastError(); 
	} 

	/* Free memory */ 
	// delete[]pwszParam; 
	// pwszParam = 0; 

	/* Release handles */ 
	// CloseHandle(piProcessInfo.hProcess); 
	// CloseHandle(piProcessInfo.hThread); 

	return iReturnVal; 
} 

#elif 
std::size_t ExecuteProcess( const std::string& full_path_to_exe, const std::string& params, std::size_t SecondsToWait = 500 ) 
{
	return 0;
}

#endif 

//-----------------------------------------------------------------------------

} // end of anonymous namespace

//-----------------------------------------------------------------------------

Screenshotter::Screenshotter() :
	mFrameCount( 0 ),
	mTakeshotAtFrame( 10000 ),
	mFirstShotAtFrame( 13 ),
	mAutoshots( false ),

	mDoingGifRect( false ),
	mGifRectStartPos( 0, 0 ),
	mGifRectEndPos( 1024, 768 ),

	mDoingGifRecording( false ),
	mGifFilePath(),

	mCopyHere()
{
	if( Poro() && Poro()->GetKeyboard() )
		Poro()->GetKeyboard()->AddKeyboardListener( this );


/*#ifdef SCREENSHOTTER_AUTO_SHOTS
	mAutoshots = true;
#endif */

	// how many seconds into we should take the autoshot?
	// 1.5 - 2 mins into the game, which translates to
	// 90 - 120
	mTakeshotAtFrame = 60 * ceng::Random( 90, 120 );

	mFirstShotAtFrame = ceng::Random( 10, 240 );

	mCopyHere = LoadFileToVector( "screenshot_paths.txt" );

	if( mCopyHere.empty() == false && mCopyHere[0] == "1" ) 
	{
		mAutoshots = true;
		mCopyHere.erase( mCopyHere.begin() );

		for( std::size_t i = 0; i < mCopyHere.size(); ++i ) std::cout << mCopyHere[ i ] << std::endl;
	}

	// we should probably create a temp directory for these animations
	// mGifFilePath = "screenshots_animated/anim_";
}

Screenshotter::~Screenshotter()
{
	if( Poro() && Poro()->GetKeyboard() )
		Poro()->GetKeyboard()->RemoveKeyboardListener( this );

	// this is we've not taken the shot we do it here
	if( mFrameCount < mFirstShotAtFrame )
	{
		DoScreenshot( "autoshot_" );
	}
}

//-----------------------------------------------------------------------------
namespace { 

template< class Vector2 >
types::irect GetIRect( const Vector2& p1, const Vector2& p2 )
{
	types::irect result;

	result.x = (int)ceng::math::Min( p1.x, p2.x );
	result.y = (int)ceng::math::Min( p1.y, p2.y );
	result.w = (int)(ceng::math::Max( p1.x, p2.x )) - result.x;
	result.h = (int)(ceng::math::Max( p1.y, p2.y )) - result.y;

	return result;
}

}
//-----------------------------------------------------------------------------

void Screenshotter::Draw( poro::IGraphics* graphics )
{
	// draw a rect to show which area is going to be recorded by gif animation
	if( mDoingGifRect ) 
	{
		DrawBox( graphics, types::vector2( mGifRectStartPos ), types::vector2( mGifRectEndPos ), poro::GetFColor( 1, 0, 0, 1 ) );
		DrawHersheyText( graphics, 
			ceng::CastToString( mGifRectEndPos.x - mGifRectStartPos.x ) + ", " + ceng::CastToString( mGifRectEndPos.y - mGifRectStartPos.y ),
			types::vector2( mGifRectEndPos.x - 80.f, mGifRectEndPos.y + 10.f ), 12.f, poro::GetFColor( 1, 0, 0, 1 ) );
	}

	// flash a red outline outside of gif capture area
	if( mDoingGifRecording ) 
	{
		if( mFrameCount % 32 < 16 ) 
			DrawBox( graphics, types::vector2( mGifRectStartPos ) + types::vector2( -2, -2 ), types::vector2( mGifRectEndPos ) + types::vector2( 2, 2 ), poro::GetFColor( 1, 0, 0, 1 ) );
	}
}

//-----------------------------------------------------------------------------

void Screenshotter::Update( float dt )
{
	mFrameCount++;

	// --------- gif -----------
	// save every other frame
	if( mDoingGifRecording && mFrameCount % 2 == 0 ) 
	{
		types::irect temp_rect = GetIRect( mGifRectStartPos, mGifRectEndPos );
		std::cout << "rect size: " << temp_rect.x << ", " << temp_rect.y << " - " << temp_rect.w << ", " << temp_rect.h << std::endl;
		DoScreenshot( mGifFilePath, false, &temp_rect );
	}

	if( mDoingGifRect ) 
		mGifRectEndPos = types::ivector2( Poro()->GetMouse()->GetMousePos() );
	

	// ---------- if we're autoshooting

	if( mAutoshots ) 
	{
		if( mFrameCount == mFirstShotAtFrame )
		{
			DoScreenshot( "autoshot_" );
		}
		else if( mFrameCount == mTakeshotAtFrame ) 
		{
			std::string filename = DoScreenshot( "autoshot_" );
			// check if we need to dropbox this screenshot

//#ifdef SCREENSHOTTER_DROPBOX_AUTOS
			std::string dropbox_file = "";
			if( mCopyHere.empty() == false ) dropbox_file = mCopyHere[ 0 ];

			if( dropbox_file.empty() == false )
			{
				std::string filedate_dropbox = ceng::GetDateForFile( dropbox_file );
				std::string filedate_screenshot = ceng::GetDateForFile( filename );

				if( filedate_dropbox.substr( 0, 6 ) != filedate_screenshot.substr( 0, 6 ) ) 
				{
					std::cout << "DropBoxxing the screenshot ... ";
					CopyScreenshotTo( filename, dropbox_file );
					std::cout << "DONE" << std::endl;
				}
			}
			// std::cout << ceng::GetDateForFile( filename ) << std::endl;
			// 1301041413
//#endif
		}
	}
}

//-----------------------------------------------------------------------------

void Screenshotter::OnKeyDown( int key, poro::types::charset unicode )
{
	if( key == SDLK_F2 ) {
		std::string filename = DoScreenshot();

		// if taking a screenshot always replace the daily dropbox screenshot with it
		// since it's probably a better shot than any of the auto shots
		// if no user created screenshot is provided that day, overwrite it with
		// an automatic screenshot


		// copies the file to places
		// if auto copy is enabled
		if( true )
		{
			for( std::size_t i = 0; i < mCopyHere.size(); ++i )
				CopyScreenshotTo( filename, mCopyHere[ i ] );
		}
	}
	else if( key == SDLK_F3 ) 
	{
		if( mDoingGifRecording == false ) 
		{
			mDoingGifRect = true;
			mGifRectStartPos = types::ivector2( Poro()->GetMouse()->GetMousePos() );
			mGifExRectEndPos = mGifRectEndPos;
			mGifRectEndPos = types::ivector2( Poro()->GetMouse()->GetMousePos() );
		}
	}
	else if( key == SDLK_F4 ) 
	{
		mDoingGifRecording = !mDoingGifRecording;
		
		// create the folder for the png files
		if( mDoingGifRecording == true ) 
		{
			time_t now = time(0);
			tm *ltm = localtime(&now);

			std::stringstream ss;
			ss << "screenshots_animated/anim_" << 1900 + ltm->tm_year 
				<< std::setfill( '0' ) << std::setw( 2 ) << 1 + ltm->tm_mon
				<< std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_mday 
				<< "-" 
				<< std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_hour
				<< std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_min
				<< std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_sec
				<< "-" << mFrameCount;

			ceng::CreateDir( ss.str() );

			mGifFilePath = ss.str() + "/frame_";
		}
		else // the end of a gif recording
		{
			std::stringstream ss;
			ss << " -delay 1x30 " << mGifFilePath << "*.png " << ceng::GetParentPath( mGifFilePath ) << ".gif";
			ExecuteProcess( PATH_TO_IMAGEMAGICK, ss.str() );
			std::cout << ss.str() << std::endl;
		}
	}

	// should we check if dropbox is setup, then we copy the screenshot to 
	// dropbox over writing the auto shot
}

void Screenshotter::OnKeyUp( int key, poro::types::charset unicode )
{
	if( key == SDLK_F3 ) 
	{
		mDoingGifRect = false;

		// check for just tapping f3, if the rect is none existing, don't overwrite the gif rect
		if( mGifRectEndPos == mGifRectStartPos ) {
			mGifRectEndPos = mGifExRectEndPos;
		}

		const int min_x = ceng::math::Min( mGifRectStartPos.x, mGifRectEndPos.x );
		const int max_x = ceng::math::Max( mGifRectStartPos.x, mGifRectEndPos.x );
		const int min_y = ceng::math::Min( mGifRectStartPos.y, mGifRectEndPos.y );
		const int max_y = ceng::math::Max( mGifRectStartPos.y, mGifRectEndPos.y );
		mGifRectStartPos.Set( min_x, min_y );
		mGifRectEndPos.Set( max_x, max_y );
	}
}

//-----------------------------------------------------------------------------

std::string Screenshotter::DoScreenshot( const std::string& prefix, bool add_path_before, const types::irect* rect  )
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << "Random number: " << ceng::Randomf( 0.f, 1.f ) << std::endl;

	std::stringstream ss;
	if( add_path_before ) {
		ss << "screenshots/";
		ceng::CreateDir( "screenshots" );
	}

	ss << prefix << 1900 + ltm->tm_year 
	   << std::setfill( '0' ) << std::setw( 2 ) << 1 + ltm->tm_mon
	   << std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_mday 
	   << "-" 
	   << std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_hour
	   << std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_min
	   << std::setfill( '0' ) << std::setw( 2 ) << ltm->tm_sec
	   << "-";
	if( rect ) ss << std::setfill( '0' ) << std::setw( 10 );
	
	ss << mFrameCount << ".png";

	std::string result = ss.str();
	SaveScreenshot( result, rect );
	return result;
}

void Screenshotter::SaveScreenshot( const std::string& filename, const types::irect* rect  )
{
	std::cout << Poro()->GetFrameNum() << " ";
	std::cout << "Saving a screenshot to: " << filename << " ... ";
	if( rect ) 
		Poro()->GetGraphics()->SaveScreenshot( filename, rect->x, rect->y, rect->w, rect->h );
	else
		Poro()->GetGraphics()->SaveScreenshot( filename );


	std::cout << "DONE" << std::endl;
}

//-----------------------------------------------------------------------------

void Screenshotter::CopyScreenshotTo( const std::string& filename, const std::string& path_or_filename )
{
	if( path_or_filename.empty() ) return;

	// if provided a backslash ending path copies it to that folder, otherwise overwrites the file
	if( (*path_or_filename.rbegin()) == '/' ) 
		ceng::CopyFileCeng( filename, path_or_filename + ceng::GetFilename( filename ) );
	else
		ceng::CopyFileCeng( filename, path_or_filename );
}

//-----------------------------------------------------------------------------
