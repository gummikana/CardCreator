#include "metadata.h"

//-----------------------------------------------------------------------------

const IMetaData& MetaData( int variable )
{
	static IMetaData mMetaData;
	mMetaData.Init();
	if( variable == NULL ) {
		mMetaData.type = IMetaData::TYPE_NULL;
	}
	return mMetaData;
}

const IMetaData& MetaData( int min_v, int max_v )
{
	static IMetaData mMetaData;
	mMetaData.Init();
	mMetaData.type = IMetaData::TYPE_MIN_MAX;
	mMetaData.min_value = (double)min_v;
	mMetaData.max_value = (double)max_v;

	return mMetaData;
}

const IMetaData& MetaData( float min_v, float max_v )
{
	static IMetaData mMetaData;
	mMetaData.Init();
	mMetaData.type = IMetaData::TYPE_MIN_MAX;
	mMetaData.min_value = (double)min_v;
	mMetaData.max_value = (double)max_v;
	return mMetaData;
}

const IMetaData& MetaData( double min_v, double max_v )
{
	static IMetaData mMetaData;
	mMetaData.Init();
	mMetaData.type = IMetaData::TYPE_MIN_MAX;
	mMetaData.min_value = (double)min_v;
	mMetaData.max_value = (double)max_v;
	return mMetaData;
}

const IMetaData& MetaFile( const std::string& path, const std::string& extension  )
{
	static IMetaData mMetaData;
	mMetaData.Init();
	mMetaData.type = IMetaData::TYPE_FILE;
	mMetaData.path = path;
	mMetaData.extension = extension;
	return mMetaData;
}

const IMetaComment& MetaComment( const std::string& comment )
{
	static IMetaComment mMetaData;
	mMetaData.Init();
	mMetaData.type = IMetaData::TYPE_COMMENT;
	mMetaData.comment = comment;
	return mMetaData;
}

const IMetaDataNoSerialize&  MetaPrivate()
{
	static IMetaDataNoSerialize mMetaData;
	mMetaData.Init();
	return mMetaData;
}

//-----------------------------------------------------------------------------

const IMetaData& operator+ (const IMetaData& data, const IMetaComment& comment)
{
	static IMetaData mMetaData;
	mMetaData.Init();

	if( (&data) != NULL ) 
	{
		mMetaData.type = data.type;

		// min - max value
		mMetaData.min_value = data.min_value;
		mMetaData.max_value = data.max_value;

		// file
		mMetaData.path = data.path;
		mMetaData.extension = data.extension;
	}
	
	// comment
	if( (&comment) != NULL )
	{
		mMetaData.comment = comment.comment;
	}

	return mMetaData;
}

//-----------------------------------------------------------------------------
IMetaData::IMetaData() { Init(); }

IMetaData::IMetaData( int variable ) 
{
	Init();
	if( variable == NULL ) {
		type = TYPE_NULL;
	}
}

IMetaData::IMetaData( const std::string& comment ) 
{
	Init();
	// this is a comment meta data
	type = TYPE_COMMENT;
}

IMetaData::IMetaData( float min_v, float max_v ) 
{
	Init();
	// this is a slider min, max data structure
	type = TYPE_MIN_MAX;
	min_value = (double)min_v;
	max_value = (double)max_v;
}

IMetaData::IMetaData( int min_v, int max_v )
{
	Init();
	// this is a slider min, max data structure
	type = TYPE_MIN_MAX;
	min_value = (double)min_v;
	max_value = (double)max_v;
}

IMetaData::IMetaData( double min_v, double max_v )
{
	Init();
	// this is a slider min, max data structure
	type = TYPE_MIN_MAX;
	min_value = (double)min_v;
	max_value = (double)max_v;
}


void IMetaData::Init()
{
	type = TYPE_NULL;
	min_value = 0;
	max_value = 1;
}

//-----------------------------------------------------------------------------