#ifndef INC_CARD_CREATOR_APP_H
#define INC_CARD_CREATOR_APP_H

#include <vector>
#include <memory>
#include <poro/default_application.h>

class DebugLayer;
namespace as { class Sprite; }



class CardCreatorApp : public poro::DefaultApplication
{
public:
	CardCreatorApp();

	void Init();

	//-------

	void Update( float dt );
	void Draw( poro::IGraphics* graphics );

	// -----

	virtual void MouseButtonDown(const poro::types::vec2& pos, int button);
	virtual void MouseButtonUp(const poro::types::vec2& pos, int button);
	virtual void MouseMove(const poro::types::vec2& p);

	virtual void OnKeyDown( int key, poro::types::charset unicode );
	virtual void OnKeyUp( int key, poro::types::charset unicode );

	// -----------

	std::vector< std::string > DoAllTheCards();
	void SaveTheCard( const std::string& filename );

	// -----------

	void UpdateStep();

	// -----------

	as::Sprite*	mSpriteContainer;
	as::Sprite* mCardContainer;
	as::Sprite* mOverlay;
	std::auto_ptr< DebugLayer >		mDebugLayer;
};

#endif
