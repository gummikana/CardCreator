#ifndef INC_DECKVIEWER_APP_H
#define INC_DECKVIEWER_APP_H

#include <vector>
#include <memory>
#include <poro/default_application.h>

class DebugLayer;
namespace as { class Sprite; }



class DeckViewerApp : public poro::DefaultApplication
{
public:
	DeckViewerApp();

	void Init();

	void LoadDeck( const std::string& filename );
	void Clear();
	
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

	void NextCard();
	void PrevCard();

	void ShuffleTheDeck();

	// -----------
	as::Sprite*	mSpriteContainer;
	std::auto_ptr< DebugLayer >		mDebugLayer;

	std::string mDeckLoaded;
	std::vector< as::Sprite* > mDeckFaceDown;
	std::vector< as::Sprite* > mDeckFaceUp;
};

#endif
