#ifndef INC_SIMPLE_PROFILER_H
#define INC_SIMPLE_PROFILER_H

#include <string>
#include <map>

// needed for Poro()->GetTime() function
#include <poro/iplatform.h>

#include <utils/singleton/csingletonptr.h>
#include <utils/math/cstatisticshelper.h>

#define MACRO_JOIN( X, Y ) MACRO_DO_JOIN( X, Y )
#define MACRO_DO_JOIN( X, Y ) MACRO_DO_JOIN2(X,Y)
#define MACRO_DO_JOIN2( X, Y ) X##Y

#define SPROFILE( x ) ::SimpleProfiler MACRO_JOIN( __profiler, __LINE__ ) ( x, __FILE__ )


class SimpleProfiler;
// typedef SimpleProfiler SPROFILE;

//-----------------------------------------------------------------------------

class SimpleProfilerGlobal
{
public:
	typedef ceng::math::CStatisticsHelper< double > TProfilerData;

	TProfilerData* GetData( const std::string& name ) 
	{
		if( mDataMap[ name ] == NULL ) 
			mDataMap[ name ] = new TProfilerData;

		return mDataMap[ name ];
	}

	std::map< std::string, TProfilerData* > mDataMap;
};

//=============================================================================

class SimpleProfiler
{
public:
	SimpleProfiler() : mName(), mMessage(), mStartTime( 0 ) { }

	SimpleProfiler( const std::string& name, const std::string& extra_message = "" )
	{
		mName = name;
		mMessage = extra_message;
		Start();
	}

	~SimpleProfiler()
	{
		End();
	}

	void Start()
	{
		mStartTime = Poro()->GetUpTime();
	}

	void End()
	{
		double dt = Poro()->GetUpTime() - mStartTime;
		ceng::GetSingletonPtr< SimpleProfilerGlobal >()->GetData( mName )->Add( dt * 1000.0 );
	}

	std::string mName;
	std::string mMessage;
	double mStartTime;
};

//-----------------------------------------------------------------------------


#endif
