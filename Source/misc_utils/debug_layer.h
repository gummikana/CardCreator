#ifndef INC_DEBUG_LAYER_H
#define INC_DEBUG_LAYER_H

#include <memory>
#include <string>
#include <poro/default_application.h>

#include "config_ui.h"

class ConfigSliders;
class Screenshotter;
class SimpleProfilerViewer;

namespace as { class Sprite; }
namespace SGF { class Entity; class EventManager; class EntityManager; }

//-----------------------------------------------------------------------------

class DebugLayer : public poro::DefaultApplication, public SimpleUIListener, public IConfigSlidersListener 
{
public:
	DebugLayer();
	DebugLayer( SGF::EventManager* eventMgr, SGF::EntityManager* entityMgr );
	~DebugLayer();

	void SetManagers( SGF::EventManager* eventMgr, SGF::EntityManager* entityMgr );
	
	void SetLoadEntity( bool enabled );

	//-------------------------------------------------------------------------

	void Init();
	void Exit();

	//-------------------------------------------------------------------------

	void Update( float dt );

	void Draw( poro::IGraphics* graphics );

	//-------------------------------------------------------------------------

	virtual void OnKeyDown( int key, poro::types::charset unicode );
	virtual void MouseMove( const poro::types::vec2& pos );
	virtual void MouseButtonDown( const poro::types::vec2& pos, int button );


	//-------------------------------------------------------------------------
	// Config stuff
	template< class TType >
	ConfigSliders* OpenConfig( TType& config )
	{
		ConfigSliders* result = config.GetConfigSliders( this );
		SetConfig( result );
		return result;
	}

	// ........................................................................

	void			SetConfig( ConfigSliders* config );
	ConfigSliders*	GetConfig();

	//-------------------------------------------------------------------------

	void			SetEntity( SGF::Entity* entity );
	SGF::Entity*	GetEntity() const; 


	//-------------------------------------------------------------------------
	// component related things
	virtual void OnSlideEnd( as::Sprite* sprite, float value );
	virtual void OnSlideTo( as::Sprite* sprite, float value );
	virtual void OnSliderChange();
	virtual void OnRemoveComponent( void* extra_data );
	virtual void OnAddComponent( const std::string& component_name );


	//-------------------------------------------------------------------------

	void OpenProfilerViewer();
	void CloseProfilerViewer();

	types::vector2 mMousePositionInWorld;
private:

	// all this for debugging?
	void	RecreateInspector();

	// component managers
	SGF::EventManager* mEventMgr;
	SGF::EntityManager* mEntityMgr;

	// ----------
	as::Sprite*		mDebugSprite;

	bool mLoadEntityEnabled;
	
	ConfigSliders*	mConfigSliders;
	float			mConfigSlidersSizeY;
	SimpleSlider*	mConfigScrollbar;

	SGF::Entity*	mInspectorEntity;
	float			mInspectorTransformRotation;
	float			mExInspectorTransformRotation;

	// mouse cursor
	bool			mExCursorVisibility;

	std::auto_ptr< SimpleProfilerViewer >	mProfilerViewer;
	std::auto_ptr< Screenshotter >			mScreenshotter;
};

//-----------------------------------------------------------------------------

inline void DebugLayer::SetLoadEntity( bool enabled ) {
	mLoadEntityEnabled = enabled;
}

#endif
