#ifndef __COMPONENT_MAPPER_H__
#define __COMPONENT_MAPPER_H__

// #include <vector>
#include <utils/debug.h>
#include "ComponentType.h"

namespace SGF
{
	class Component;

	template<class T>
	class ComponentMapper {
	public:
		// typedef T ComponentType;
		//
		ComponentMapper() : _list( NULL ) { }
		ComponentMapper(const std::vector< Component* >* list) : _list( list ) { }
		ComponentMapper(const ComponentMapper< T >& other ) : _list( other.list ) { }
		virtual ~ComponentMapper() {}

		void SetList( const std::vector< Component* >* list ) { _list = list; }

		// Get the size of the component list
		int size() const { if( _list == NULL ) return 0; return _list->size(); }

		// Get a specific element of the component list.
		T* operator[](int idx) 
		{
			cassert( _list );
			cassert( idx < size() );
			return static_cast<T*>( (*_list)[idx] );
		}

		static ComponentType component_type;
	protected:
		const std::vector<Component*>* _list;
	};
	
	template<class T>
	ComponentType ComponentMapper< T >::component_type = 0;
};

#endif