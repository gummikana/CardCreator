import sys, datetime, os, os.path

# helper for writing indented text
def WriteIndent(theFile, indentCount, text):
    while (indentCount > 0):
        text = '\t' + text
        indentCount -= 1
    theFile.write(text)

# we need at least the name
if (len(sys.argv) < 2):
    sys.exit()
 
# component name
component_name = sys.argv[1]

components_dir = 'Source/components/'
systems_dir = 'Source/component_systems/'

# get our file paths
component_header = os.path.join(components_dir, component_name.lower() + '_component.h')
component_macroname = 'COMPONENT_' + component_name.upper()

#define guard
define_guard = 'INC_' + component_name.upper() + '_COMPONENT_H'
component_class = component_name + 'Component'
system_class = component_name + 'System'

#print 'Creating file: ' + component_header

with open(component_header, 'w') as component_file:
    # write the copyright header
    # WriteCopyright(headerFile, itemName + '.hpp', creator)
 
    # write the include guards
    component_file.write('#ifndef ' + define_guard + '\n#define ' + define_guard + '\n\n')
    
    
    # write includes
    WriteIndent(component_file, 0, '#include "../component_framework/ComponentMacro.h"\n\n\n')
    
    # write the basic class definition
    WriteIndent(component_file, 0, '#define ' + component_macroname + '(list_) \\\n')
    WriteIndent(component_file, 1, 'list_(float, 			TEMP_TEMPY,					0.0f,			MetaData( 0.f, 	3.5f ) ) \\\n')
    WriteIndent(component_file, 1, 'list_(float, 			TEMP_TEMP_TEMP,				0.0f,			MetaData( 0.f, 	3.5f ) ) \\\n\n\n')
    WriteIndent(component_file, 0, 'DEFINE_COMPONENT( ' + component_class + ', ' + component_macroname + ');\n\n')
    WriteIndent(component_file, 0, '#undef '  + component_macroname + '\n')

    component_file.write('\n#endif // ' + define_guard + '\n\n')


# Now the ComponentSystems.h and .cpp files

# get our file paths
header_path = os.path.join(systems_dir, component_name.lower() + '_system.h')
impl_path = os.path.join(systems_dir, component_name.lower() + '_system.cpp')

#define guard
define_guard = 'INC_' + component_name.upper() + '_SYSTEM_H'


#print 'Creating file: ' + header_path;

with open(header_path, 'w') as file_handle:
    # write the copyright header
    # WriteCopyright(headerFile, itemName + '.hpp', creator)
 
    # write the include guards
    file_handle.write('#ifndef ' + define_guard + '\n#define ' + define_guard + '\n\n')
    
    
    # write includes
    WriteIndent(file_handle, 0, '#include "../component_framework/ComponentUpdator.h"\n')
    WriteIndent(file_handle, 0, '#include "../components/' + component_name.lower() + '_component.h"\n\n\n')
 
    # messages comment section
    WriteIndent(file_handle, 0, "// if you're interested in messages\n" )
    WriteIndent(file_handle, 0, '// #include "../component_framework/MessageListener.h"\n' )
    WriteIndent(file_handle, 0, '// #include "../messages/message_test.h"\n' )
    WriteIndent(file_handle, 0, "//\n" )
    WriteIndent(file_handle, 0, "// also remember to inherit \n" )
    WriteIndent(file_handle, 0, "// public MessageListener< TestMessagesComponent, Message_Test >\n" )
    WriteIndent(file_handle, 0, "// and implement\n" )
    WriteIndent(file_handle, 0, "// void HandleMessage( SGF::Entity* e, TestMessagesComponent* comp, Message_Test* message );\n" )

    WriteIndent(file_handle, 0, "\n\n")

    # write the basic class definition
    WriteIndent(file_handle, 0, 'class ' + system_class + ' : public ComponentUpdator< ' + component_class + ', false >\n')
    WriteIndent(file_handle, 0, '{\npublic:\n')
    WriteIndent(file_handle, 1, '' + system_class + '( SGF::EventManager* eventManager, SGF::EntityManager* entityManager );\n\n')
    WriteIndent(file_handle, 0, 'protected:\n')
    WriteIndent(file_handle, 1, 'void RefreshComponent( SGF::Entity* e, ' + component_class + '* comp );\n')
    WriteIndent(file_handle, 1, 'void UpdateComponent( SGF::Entity* e, ' + component_class + '* comp );\n\n')
    WriteIndent(file_handle, 0, '};\n\n')
    WriteIndent(file_handle, 0, 'SYSTEM_REGISTER(' + system_class + ');\n\n')

    file_handle.write('\n#endif // ' + define_guard + '\n\n')


# now lets do the system.cpp

#print 'Creating file: ' + impl_path

with open(impl_path, 'w') as file_handle:
    # write the copyright header
    # WriteCopyright(headerFile, itemName + '.hpp', creator)
 
    # write the include guards
    file_handle.write('#include "' + component_name.lower() + '_system.h"\n\n')
    
    
    # write includes
    # no need for this either, included in the header
    # WriteIndent(file_handle, 0, '#include "../component_framework/ComponentUpdator.h"\n')
    # no need for this include since it's already required in the header
    # WriteIndent(file_handle, 0, '#include "../components/' + component_name.lower() + '_component.h"\n\n\n')
 
    # write the basic class definition
    WriteIndent(file_handle, 0, system_class + '::' + system_class + '( SGF::EventManager* eventManager, SGF::EntityManager* entityManager ) :\n')
    WriteIndent(file_handle, 1, 'ComponentUpdator( eventManager, entityManager )\n')
    WriteIndent(file_handle, 0, '{\n}\n\n')
    WriteIndent(file_handle, 0, 'void ' + system_class + '::RefreshComponent( SGF::Entity* e, ' + component_class + '* comp )\n')
    WriteIndent(file_handle, 0, '{\n}\n\n')
    WriteIndent(file_handle, 0, 'void ' + system_class + '::UpdateComponent( SGF::Entity* e, ' + component_class + '* comp )\n')
    WriteIndent(file_handle, 0, '{\n}\n\n')


