#ifndef __COMPONENT_TYPE_H__
#define __COMPONENT_TYPE_H__

#include <string> 
#include <vector>
#include <map>

namespace SGF
{
	typedef int ComponentType;

	// ------------------------------------------------------------------------

	template< class T >
	std::string GetTypeName() 
	{
		std::string result = typeid( T ).name();
		if( result.size() > 6 && result.substr( 0, 5 ) == "class" ) result = result.substr( 6 );
		// std::cout << "result: " << result << std::endl;
		return result;
	}

	// ------------------------------------------------------------------------

	class ComponentTypeManager
	{
		ComponentTypeManager() : nextBit( 1 ), nextId( 1 ) { }
	public:

		unsigned int nextBit;
		int nextId;

		static ComponentTypeManager* Instance() 
		{
			static ComponentTypeManager instance;
			return &instance;
		}

		int GetSize() const {
			return nextId + 1;
		}
		
		std::pair< ComponentType, unsigned int > init() 
		{
			std::pair< ComponentType, unsigned int > result;
 			result.second = nextBit;
			nextBit = nextBit << 1;
			result.first = nextId++;
			return result;
		}

		std::pair< ComponentType, unsigned int > GetTypeData( const std::string& name )
		{
			std::map< std::string, std::pair< ComponentType, unsigned int > >::iterator i = mTypes.find( name );

			if( i != mTypes.end() ) return i->second;

			std::pair< ComponentType, unsigned int > result = init();
			mTypes[ name ] = result;
			return result;
		}

		ComponentType GetComponentType( const std::string& name ) const
		{
			std::map< std::string, std::pair< ComponentType, unsigned int > >::const_iterator i = mTypes.find( name );

			if( i != mTypes.end() ) return i->second.first;
			return 0;
		}

		void GetNamesOfAllComponents( std::vector< std::string >& result_names )
		{
			std::map< std::string, std::pair< ComponentType, unsigned int > >::iterator i;
			for( i = mTypes.begin(); i != mTypes.end(); ++i )
				result_names.push_back( i->first );
		}

	private:
		std::map< std::string, std::pair< ComponentType, unsigned int > > mTypes;
	};

} // end of namespace SGF

#endif