#ifndef INC_IMAGE_SYSTEM_H
#define INC_IMAGE_SYSTEM_H

#include "../component_framework/ComponentUpdator.h"
#include "../components/image_component.h"

class ImageSystem : public ComponentUpdator< ImageComponent, true >  {
public:

	ImageSystem( SGF::EventManager* eventManager, SGF::EntityManager* entityManager );

	as::Sprite* mSpriteContainer;

	// return order index of 500
	int GetOrderIndex() const { return 500; }

protected:
	void RefreshComponent( SGF::Entity* e, ImageComponent* scomp );
	void UpdateComponent( SGF::Entity* e, ImageComponent* scomp );

};

SYSTEM_REGISTER( ImageSystem );

#endif