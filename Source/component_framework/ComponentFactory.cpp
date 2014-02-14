#include "ComponentFactory.h"

// Standard libraries
#include <string>

#include "Component.h"

namespace SGF {

void InitComponentTypes()
{
	SGF::component::detail::ComponentRegistry& reg = SGF::component::detail::getComponentRegistry();
	SGF::component::detail::ComponentRegistry::iterator it;
	for( it = reg.begin(); it != reg.end(); ++it )
	{
		std::pair< SGF::ComponentType, unsigned int > type = SGF::ComponentTypeManager::Instance()->GetTypeData( it->first );
	}
}

Component* component::create(const std::string& name)
{
    detail::ComponentRegistry& reg = detail::getComponentRegistry();
    detail::ComponentRegistry::iterator it = reg.find(name);

    if (it == reg.end()) {
        // This happens when there is no component registered to this
        // name. Here I return a null pointer, but you can handle this
        // error differently if it suits you better.
        return NULL;
    }

    detail::CreateComponentFunc func = it->second;
    return func();
}

void component::destroy(const Component* comp)
{
    delete comp;
}

} // end of namespace SGF