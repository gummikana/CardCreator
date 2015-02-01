#ifndef INC_PILECREATOR_APP_H
#define INC_PILECREATOR_APP_H

#include <vector>
#include <memory>
#include <poro/default_application.h>

class DebugLayer;
namespace as { class Sprite; }



class PileCreatorApp : public poro::DefaultApplication
{
public:
	PileCreatorApp();

	void Init();
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
	void SaveTheCard( const std::string& filename );

	// -----------
	as::Sprite*	mSpriteContainer;
	std::auto_ptr< DebugLayer >		mDebugLayer;

};

#endif
