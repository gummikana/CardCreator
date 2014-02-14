#ifndef INC_SIMPLE_UI_H
#define INC_SIMPLE_UI_H

#include <vector>
#include <poro/poro.h>
#include <game_utils/actionscript/actionscript.h>

namespace as { class Sprite; }

//-----------------------------------------------------------------------------

class SimpleUIListener
{
public:
	virtual  ~SimpleUIListener() { }

	virtual void OnClick( as::Sprite* sprite, const std::string& name_of_sprite ) 
	{
	}

	virtual void OnSlideTo( as::Sprite* sprite, float value ) 
	{
	}

	virtual void OnSlideEnd( as::Sprite* sprite, float value )
	{
	}

	virtual void OnCheckboxChange( as::Sprite* sprite, bool value )
	{
	}

	virtual void OnTextboxChange( as::Sprite* sprite, const std::string& value )
	{
	}

	virtual void OnTextboxFocusChange( as::Sprite* sprite, bool has_focus ) 
	{
	}
};

//-----------------------------------------------------------------------------

class SimpleButton : public as::Sprite, public poro::IMouseListener
{
public:
	SimpleButton( SimpleUIListener* listener = NULL );
	virtual ~SimpleButton();

	virtual void				MouseButtonDown(const poro::types::vec2& pos, int button);
	virtual types::vector2		TransformMousePos( const types::vector2& mouse_pos );

	void* mUserData;
	SimpleUIListener* mListener;
};

//-----------------------------------------------------------------------------

class SimpleSlider : public as::Sprite, public poro::IMouseListener
{
public:
	SimpleSlider( SimpleUIListener* listener = NULL );
	~SimpleSlider();

	void MouseButtonDown(const poro::types::vec2& pos, int button);
	void MouseButtonUp(const poro::types::vec2& pos, int button);

	void MouseMove(const poro::types::vec2& pos );

	types::vector2 TransformMousePos( const types::vector2& mouse_pos );

	
	float RelativeDistance( const types::vector2& start, const types::vector2& end, const types::vector2& current );

	void SetSliderValue( float value );
	float GetSliderValue() const;
	
	void* mUserData;

	bool mMouseDown;
	types::vector2 mMouseDownOffset;
	types::vector2 mMouseDownPos;
	SimpleUIListener* mListener;

	types::vector2 mStartPos;
	types::vector2 mEndPos;
	float mValue;
};

//-----------------------------------------------------------------------------

class SimpleCheckbox : public as::Sprite, public poro::IMouseListener
{
public:
	SimpleCheckbox( SimpleUIListener* listener = NULL );
	~SimpleCheckbox();

	void MouseButtonDown(const poro::types::vec2& pos, int button);
	
	types::vector2 TransformMousePos( const types::vector2& mouse_pos );

	void SetValue( bool value );

	SimpleUIListener* mListener;
	bool mValue;
	std::string mCheckboxSpriteName;
	
	void* mUserData;
};

//-----------------------------------------------------------------------------

class SimpleTextbox : public as::TextSprite, public poro::IMouseListener, public poro::IKeyboardListener
{
public:
	SimpleTextbox( SimpleUIListener* listener = NULL );
	~SimpleTextbox();

	void MouseButtonDown(const poro::types::vec2& pos, int button);

	types::vector2 TransformMousePos( const types::vector2& mouse_pos );

	void SetValue( const std::string& value );

	std::string GetValue() const;
	bool HasFocus() const;

	virtual void OnLoseFocus();
	virtual void OnEnter();

	virtual void OnKeyDown( int key, poro::types::charset unicode );
	virtual void OnKeyUp( int key, poro::types::charset unicode );

	virtual void SetFocus( bool value );

	void SetAsFileDialog( bool is_file_dialog, const std::string& path = "data\\" );

	virtual void		SetText( const std::string& text );
	virtual std::string	GetText() const;


	bool mFocus;
	SimpleUIListener* mListener;
	std::string mValue;
	bool mClear;
	bool mIsFileDialog;
	std::string mFileDialogPath;
	std::string mTextboxName;
};

//-----------------------------------------------------------------------------

class SimpleDropDown : public SimpleButton
{
public:
	SimpleDropDown( SimpleUIListener* listener = NULL );
	~SimpleDropDown();

	void MouseButtonDown(const poro::types::vec2& pos, int button);

	void ShowDropDowns();
	void HideDropDowns();

	void AddDropDown( as::Sprite* drop_down );

protected:
	std::vector< as::Sprite* > mDropDowns;	
	bool mDropDownsVisible;
};

//-----------------------------------------------------------------------------

#endif
