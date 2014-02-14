#ifndef INC_GCOMPONENTS_H
#define INC_GCOMPONENTS_H

namespace SGF { class EventManager; class EntityManager; }


struct GComponents 
{
	static SGF::EventManager*				eventManager;
	static SGF::EntityManager*				entityManager;

	static void Init();
};


#endif
