#ifndef INC_CONFIG_SLIDERS_H
#define INC_CONFIG_SLIDERS_H

#include <map>
#include <string>
#include <utils/xml/canycontainer.h>

#include "metadata.h"
#include "simple_ui.h"

//-----------------------------------------------------------------------------


class IConfigSlidersListener 
{
public:
	virtual void OnSliderChange() { cassert( false ); }
	virtual void OnSliderChangeTiny( const std::string& variable_name ) { }
	virtual void OnRemoveComponent( void* extra_data ) { }
	virtual void OnAddComponent( const std::string& component_name ) { }
};

//-------------------------------------------------------------------------

namespace impl {

class IConfigElement
{
public:
	virtual ~IConfigElement() { }

	virtual void SetValue( float v ) { }
	virtual void SetValue( const std::string& value ) { }
	virtual float GetValue() { return 0; }
	virtual std::string GetValueStr() { return ""; }
	virtual void* GetPtr() const { return NULL; }
}; 
//-------------------------------------------------------------------------

template< class T >
struct ConfigTemplated : public IConfigElement
{
	ConfigTemplated( T& reference, const T& min_value, const T& max_value ) :
		mReferenceToVariable( reference ), mMinValue( min_value ), mMaxValue( max_value )
	{ }

	virtual void SetValue( const std::string& value ) {
		T fvalue = ceng::CastFromString< T >( value );
		mReferenceToVariable = fvalue;
	}

	virtual void SetValue( float v ) { 
		const T diff = mMaxValue - mMinValue;
		mReferenceToVariable = ((T)( v * diff )) + mMinValue;
	}

	virtual float GetValue() { 
		const T diff = mMaxValue - mMinValue;
		if( mMaxValue == 0 ) return 0;
		return ((float)( (mReferenceToVariable - mMinValue) / diff ));
	}

	virtual void* GetPtr() const { 
		return (void*)( &mReferenceToVariable );
	}

	virtual std::string GetValueStr() {
		std::stringstream ss;
		ss.precision(4);
		ss << mReferenceToVariable;
		return ss.str();
	}

	T& mReferenceToVariable;
	const T mMinValue;
	const T mMaxValue;
};
//-------------------------------------------------------------------------

template <>
struct ConfigTemplated<int> : public IConfigElement
{
	ConfigTemplated( int& reference, const int& min_value, const int& max_value ) :
		mReferenceToVariable( reference ), mMinValue( min_value ), mMaxValue( max_value )
	{ }

	virtual void SetValue( float v ) { 
		mReferenceToVariable = ((int)( v * (float)(mMaxValue - mMinValue) + 0.5f )) + mMinValue;
	}

	virtual float GetValue() { 
		if( mMaxValue == 0 ) return 0;
		return ((float)( (mReferenceToVariable - mMinValue) / (float)( mMaxValue - mMinValue ) ));
	}

	virtual void* GetPtr() const { 
		return (void*)( &mReferenceToVariable );
	}

	virtual std::string GetValueStr() {
		std::stringstream ss;
		ss << mReferenceToVariable;
		return ss.str();
	}

	int& mReferenceToVariable;
	const int mMinValue;
	const int mMaxValue;
};
//-------------------------------------------------------------------------

template <>
struct ConfigTemplated<bool> : public IConfigElement
{
	ConfigTemplated( bool& reference, const bool& min_value, const bool& max_value ) :
		mReferenceToVariable( reference )
	{ }

	virtual void SetValue( float v ) { 
		if( v > 0.5f ) mReferenceToVariable = true;
		else mReferenceToVariable = false;
	}

	virtual float GetValue()  { 
		return mReferenceToVariable? 1.f : 0.f;
	}

	virtual void* GetPtr() const { 
		return (void*)( &mReferenceToVariable );
	}

	virtual std::string GetValueStr() {
		std::stringstream ss;
		ss << mReferenceToVariable;
		return ss.str();
	}

	bool& mReferenceToVariable;
};
//-------------------------------------------------------------------------

template <>
struct ConfigTemplated<std::string> : public IConfigElement
{
	ConfigTemplated( std::string& reference, const std::string& min_value, const std::string& max_value ) :
		mReferenceToVariable( reference )
	{
	}

	virtual void SetValue( float v ) { 
		cassert( false && "No implementation for setting float value to string..." );
	}

	virtual void SetValue( const std::string& value ) {
		mReferenceToVariable = value;
	}

	virtual float GetValue() { 
		return 0;
	}

	virtual void* GetPtr() const { 
		return (void*)( &mReferenceToVariable );
	}

	virtual std::string GetValueStr() {
		return mReferenceToVariable;
	}

	std::string& mReferenceToVariable;
};
//-----------------------------------------------------------------------------
} // end of namespace impl
//=============================================================================


class ConfigSliders : public as::Sprite, public SimpleUIListener
{
public:
	
	//-------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////
	
	template< class T >
	void AddVariable(  T* variable, const std::string& name, const IMetaData& metadata )
	{
		// do nothing for pointers
	}
	
	template< class T >
	void AddVariable(  T& variable, const std::string& name, const IMetaData& metadata )
	{
		SimpleSlider* slider = CreateSlider( name );

		//-------
		
		impl::IConfigElement* element = CreateElement( variable, ceng::math::Min( variable, (T)metadata.min_value ), ceng::math::Max( variable, (T)metadata.max_value ) );
		mElements.insert( std::pair< as::Sprite*, impl::IConfigElement* >( slider, element ) );

		if( slider->mUserData ) mElements.insert( std::pair< as::Sprite*, impl::IConfigElement* >( (as::Sprite*)slider->mUserData, element ) );
		
		float v = element->GetValue();
		
		if( slider && slider->mUserData ) {
			((as::TextSprite*)(slider->mUserData))->SetText( element->GetValueStr() );
			slider->SetSliderValue( v );
		}
	}

	template<>
	void AddVariable(  bool& variable,  const std::string& name, const IMetaData& metadata   )
	{
		SimpleCheckbox* simple_checkbox = CreateCheckbox( name, variable );
		impl::IConfigElement* element = CreateElement( variable, false, true );
		mElements.insert( std::pair< as::Sprite*, impl::IConfigElement* >( simple_checkbox, element ) );

		float v = element->GetValue();
		simple_checkbox->SetValue( v > 0.5f );
	}

	template<>
	void AddVariable( std::string& variable, const std::string& name, const IMetaData& metadata )
	{
		SimpleTextbox* textbox = CreateTextbox( name, variable );
		
		if( metadata.type == IMetaData::TYPE_FILE ) {
			textbox->SetAsFileDialog( true, metadata.path );
		}

		//-------
		
		impl::IConfigElement* element = CreateElement< std::string >( variable, "", "" );
		mElements.insert( std::pair< as::Sprite*, impl::IConfigElement* >( textbox, element ) );
	}
	
	//-------------------------------------------------------------------------

	void AddHeaderText( const std::string& name );
	void AddComponentHeader( const std::string& component_name, bool remove_me_button = true, void* extra_data = NULL );
	void AddAddComponent( const std::string& text, std::vector< std::string >& component_names );

	//-------------------------------------------------------------------------
	
	template< class T >
	void RemoveVariable( const T& variable )  {
		RemoveVariable( (void*)(&variable) );
	}

	void RemoveVariable( void* value )
	{
	}

	//-------------------------------------------------------------------------

	ConfigSliders();
	ConfigSliders( IConfigSlidersListener* listener );

	void Init();
	void Update( float dt );

	//-------------------------------------------------------------------------

	void OnSlideTo( as::Sprite* sprite, float value );
	void OnSlideEnd( as::Sprite* sprite, float value );

	void OnClick( as::Sprite* sprite, const std::string& name_of_sprite );
	void OnCheckboxChange( as::Sprite* sprite, bool value );
	void OnTextboxChange( as::Sprite* sprite, const std::string& value );
	void OnTextboxFocusChange( as::Sprite* sprite, bool has_focus );

	bool HasFocus() const;

	//-------------------------------------------------------------------------
	// creation of ui elements
	SimpleSlider*	CreateSlider( const std::string& name );
	SimpleCheckbox* CreateCheckbox( const std::string& name, bool value );
	SimpleTextbox*	CreateTextbox( const std::string& name, const std::string& value );
	SimpleButton*	CreateButton( const std::string& name, const std::string& image_file = "" );
	SimpleDropDown* CreateDropDown( const std::string& name, const std::string& box_name, const std::vector< std::string >& values );

	as::Sprite*		CreateLine();
	as::Sprite*		CreateLabel( const std::string& name );
	as::Sprite*		CreateTextValue( const std::string& text );
	SimpleTextbox*	CreateTextboxValue( const std::string& name, const std::string& value );
	as::Sprite*		CreateHeader( const std::string& title_name );

	//-------------------------------------------------------------------------

	template< class T > 
	impl::IConfigElement* CreateElement( T* variable, const T* variable_min_value, const T* variable_max_value ) {
		return NULL;
	}

	template< class T > 
	impl::IConfigElement* CreateElement( T& variable, const T& variable_min_value, const T& variable_max_value ) {
		return new impl::ConfigTemplated< T >( variable, variable_min_value, variable_max_value );
	}

	//-------------------------------------------------------------------------

	types::vector2 GetBoxSize() const { return mBoxSize; }
	types::rect GetCollisionRect() const { return types::rect( GetPos().x, GetPos().y, mBoxSize.x, mBoxSize.y ); }

	void ResizeBackground( float height );
	void RecomputeBackgroundSize();

	int GetElementCount() const { return (int)mSliders.size(); }

	as::Sprite* mFocusedSprite;

	types::vector2 mStartPos;
	types::vector2 mBoxSize;
	types::vector2 mSliderSize;
	bool mCanCreateLines;

	float mYPosition;

	std::vector< as::Sprite* > mSliders;
	std::map< as::Sprite*, impl::IConfigElement* > mElements;
	IConfigSlidersListener* mListener;
	SimpleButton* mAddComponentTextbox;
	
	as::Sprite* mBackground;
};
//-----------------------------------------------------------------------------

#endif
