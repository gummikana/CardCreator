#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <map>
#include <list>

#include "ComponentType.h"


class ConfigSliders;
class IConfigSlidersListener;
struct IMetaData;
namespace ceng { class CXmlFileSys; class CAnyContainer; }


namespace SGF {

//-----------------------------------------------------------------------------

class AutoDeleter
{
public:
	virtual ~AutoDeleter() { }
};
//-----------------------------------------------------------------------------

template< class T >
class TAutoDeleter : public AutoDeleter
{
public:
	TAutoDeleter( T*& ptr ) : mPtr( ptr ) { }
	~TAutoDeleter() 
	{
		delete mPtr;
		mPtr = NULL;
	}
	
	T*& mPtr;
};

//=============================================================================

class Component 
{
public:
	Component( const std::string& name );
	virtual	~Component();

	//------- types & names -------
	// Type is unique id, typebits is used to figure out the combinations, name is the # of the name
	ComponentType		GetType();
	unsigned int		GetTypeBit();
	const std::string&	GetName() const { return _name; }
	
	// ----- enabled? ---------
	bool IsEnabled() const			{ return _enabled; }
	void SetEnabled( bool enabled ) { _enabled = enabled; }

	//------- id stuff ---------
	// this is an id that has been given, outside to this component...
	unsigned int		GetID() { return _id; }
	void				SetID( unsigned int id ) { _id = id; }

	//---- serialize ----
	virtual void			Serialize( ceng::CXmlFileSys* filesys );
	virtual ConfigSliders*  GetConfigSliders( ConfigSliders* config_sliders = NULL, IConfigSlidersListener* listener = NULL ) = 0;

	//----- property accessors -------
	virtual const std::map< std::string, bool >&	GetListOfVariables() const = 0;
	virtual void									SetValue( const std::string& n, const ceng::CAnyContainer& new_value ) = 0;
	virtual ceng::CAnyContainer						GetValue( const std::string& n ) const = 0;
	virtual IMetaData								GetMetaData( const std::string& n ) const = 0;
	
	template< class T >
	T GetValueAs( const std::string& n ) const {
		return ceng::CAnyContainerCast< T >( GetValue( n ) );
	}

	// ------------------------------------
	// auto deletors, if the component creates a new pointer to something
	// it can be destroyed by adding a auto delete function to it
	// this is because, sometimes there's no updators
	template< class T >
	void AddAutoDelete( T*& pointer ) {
		_auto_deletes.push_back( new TAutoDeleter< T >( pointer ) );
	}

	void AddAutoDeleter( AutoDeleter* deleter ) { 
		_auto_deletes.push_back( deleter ); 
	}

private:
	
	void InitType();

	std::string		_name;
	ComponentType	_type;
	unsigned int	_bit;
	unsigned int	_id;
	bool			_enabled;

	std::list< AutoDeleter* > _auto_deletes;
};

//-----------------------------------------------------------------------------

} // end of namespace SGF

#endif
