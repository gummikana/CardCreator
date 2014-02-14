#ifndef INC_CARD_CREATOR_APP_H
#define INC_CARD_CREATOR_APP_H

#include <memory>
#include <poro/default_application.h>

class DebugLayer;

class CardCreatorApp : public poro::DefaultApplication
{
public:
	CardCreatorApp();

	void Init();

	void Update( float dt );
	void Draw( poro::IGraphics* graphics );

	virtual void MouseButtonDown(const poro::types::vec2& pos, int button);
	virtual void MouseButtonUp(const poro::types::vec2& pos, int button);
	virtual void MouseMove(const poro::types::vec2& p);

	std::auto_ptr< DebugLayer >		mDebugLayer;
};

#endif
