#ifndef INC_METADATA_H
#define INC_METADATA_H

#include <string>

//-----------------------------------------------------------------------------

struct IMetaData;
struct IMetaComment;
struct IMetaDataNoSerialize;

const IMetaData&			MetaData( int variable );
const IMetaData&			MetaData( int min_value, int max_value );
const IMetaData&			MetaData( float min_value, float max_value );
const IMetaData&			MetaData( double min_value, double max_value );

const IMetaData&			MetaFile( const std::string& path = "data/", const std::string& extension = ""  );

const IMetaComment&			MetaComment( const std::string& comment );

const IMetaDataNoSerialize&	MetaPrivate();

//-----------------------------------------------------------------------------

struct IMetaData
{
	enum MetaDataSerialize
	{
		META_SERIALIZE = 1,
		META_CONFIG_SLIDERS = 1
	};

	enum MetaDataTypes
	{
		TYPE_NULL = 0,
		TYPE_COMMENT = 1,
		TYPE_MIN_MAX = 2,
		TYPE_FILE = 3,
	};

	IMetaData();
	IMetaData( int variable );
	IMetaData( const std::string& comment );
	IMetaData( int min_value, int max_value );
	IMetaData( float min_value, float max_value );
	IMetaData( double min_value, double max_value );

	virtual ~IMetaData() { }

	void Init();

	int type;

	// min - max value
	double min_value;
	double max_value;

	// file
	std::string path;
	std::string extension;

	// comment
	std::string comment;

	virtual bool Serialize() const { return true; }
};

//-----------------------------------------------------------------------------

struct IMetaComment : public IMetaData
{
};

//-----------------------------------------------------------------------------

struct IMetaDataNoSerialize : public IMetaData
{
	enum MetaDataSerialize
	{
		META_SERIALIZE = 0,
		META_CONFIG_SLIDERS = 0
	};

	virtual bool Serialize() const { return false; }
};

//-----------------------------------------------------------------------------

const IMetaData& operator+ (const IMetaData& data, const IMetaComment& comment);

//-----------------------------------------------------------------------------

#endif
