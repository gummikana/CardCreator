#ifndef INC_TEXTHEADER_SYSTEM_H
#define INC_TEXTHEADER_SYSTEM_H

#include "../component_framework/ComponentUpdator.h"
#include "../components/textheader_component.h"


// if you're interested in messages
// #include "../component_framework/MessageListener.h"
// #include "../messages/message_test.h"
//
// also remember to inherit 
// public MessageListener< TestMessagesComponent, Message_Test >
// and implement
// void HandleMessage( SGF::Entity* e, TestMessagesComponent* comp, Message_Test* message );


class TextHeaderSystem : public ComponentUpdator< TextHeaderComponent, true >
{
public:
	TextHeaderSystem( SGF::EventManager* eventManager, SGF::EntityManager* entityManager );

protected:
	void RefreshComponent( SGF::Entity* e, TextHeaderComponent* comp );
	void UpdateComponent( SGF::Entity* e, TextHeaderComponent* comp );

};

SYSTEM_REGISTER(TextHeaderSystem);


#endif // INC_TEXTHEADER_SYSTEM_H

