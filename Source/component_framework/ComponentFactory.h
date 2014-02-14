#ifndef INC_COMPONENT_FACTORY_H
#define INC_COMPONENT_FACTORY_H

// Standard libraries
#include <map>
#include <string>
#include <utility>

namespace SGF 
{
//-----------------------------------------------------------------------------

// Call this to make sure component types have types...
void InitComponentTypes();

//-----------------------------------------------------------------------------

class Component;

namespace component
{
    namespace detail
    {
        typedef Component* (*CreateComponentFunc)();
        typedef std::map<std::string, CreateComponentFunc> ComponentRegistry;

        inline ComponentRegistry& getComponentRegistry()
        {
            static ComponentRegistry reg;
            return reg;
        }

        template<class T>
        Component* createComponent() {
            return new T;
        }

        template<class T>
        struct RegistryEntry
        {
          public:
            static RegistryEntry<T>& Instance(const std::string& name)
            {
                // Because I use a singleton here, even though `COMPONENT_REGISTER`
                // is expanded in multiple translation units, the constructor
                // will only be executed once. Only this cheap `Instance` function
                // (which most likely gets inlined) is executed multiple times.

                static RegistryEntry<T> inst(name);
                return inst;
            }

          private:
            RegistryEntry(const std::string& name)
            {
                ComponentRegistry& reg = getComponentRegistry();
                CreateComponentFunc func = createComponent<T>;

                std::pair<ComponentRegistry::iterator, bool> ret =
                    reg.insert(ComponentRegistry::value_type(name, func));

                if (ret.second == false) {
                    // This means there already is a component registered to
                    // this name. You should handle this error as you see fit.
                }
            }

            // RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
            // RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
        };

    } // namespace detail

} // namespace component


namespace component
{
    Component* create(const std::string& name);
    void destroy(const Component* comp);
} // end of namespace component
} // end of namespace SGF


#define COMPONENT_REGISTER(TYPE, NAME)											\
	namespace SGF {																\
    namespace component {														\
    namespace detail {															\
    namespace																	\
    {																			\
        template<class T>														\
        class ComponentRegistration;											\
																				\
        template<>																\
        class ComponentRegistration<TYPE>										\
        {																		\
			static const ::SGF::component::detail::RegistryEntry<TYPE>& reg;    \
        };																		\
																				\
        const ::SGF::component::detail::RegistryEntry<TYPE>&					\
            ComponentRegistration<TYPE>::reg =									\
                ::SGF::component::detail::RegistryEntry<TYPE>::Instance(NAME);  \
	}}}}


#endif // COMPONENT_DETAIL_H