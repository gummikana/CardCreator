#include "config_sliders.h"

//-----------------------------------------------------------------------------
namespace {

struct UI_Config
{
	UI_Config() : 
		element_spacing( 30.f, 30.f ),
		background_width( 300.f ),
		background_spacing( 30.f, 0.f ),
		background_color(),
		background_sprite( "data/debug/sui_background.png" ),
		background_dropshadow( "data/debug/sui_background_dropshadow.png" ),

		use_background_lines( true ),
		background_line_sprite( "data/debug/sui_background_line.png" ),
		background_line_color(),
		background_line_size( 300.f, 1.f ),
		background_line_position( 0, -1.f ),

		text_color(),
		text_font_file( "data/fonts/ubuntu_condensed_10.xml" ),
		text_extra_position( 0, 7.f ),
		text_use_fadeoff( true ),
		text_fadeoff_sprite( "data/debug/sui_text_gradient.png" ),
		text_fadeoff_position( 120.f - 10.f, 0 ),
		text_fadeoff_size( 10.f, 30.f ),
		text_fadeoff_extra_sprite( "data/debug/sui_text_gradient_extra.png" ),
		text_fadeoff_extra_size( 150.f, 30.f ),
		
		text_value_position( 215.f, 7.f ),

		slider_front_sprite( "data/debug/sui_slider_front.png" ),
		slider_front_stroke_sprite( "data/debug/sui_slider_front_stroke.png" ),
		slider_front_start_pos( 130.f, 0.f ),
		slider_front_end_pos( 200.f, 0.f ),

		slider_back_sprite( "data/debug/sui_slider_back.png" ),
		slider_back_position( 120.f, 0.f ),
		slider_color(),

		checkbox_back_sprite( "data/debug/sui_checkbox.png" ),
		checkbox_back_color(),
		checkbox_front_sprite( "data/debug/sui_checkbox_overlay.png" ),
		checkbox_front_color(),
		checkbox_position( 120.f, 0 ),

		textbox_back_sprite( "data/debug/sui_text_box.png" ),
		textbox_back_sprite_position( 0, 0 ),
		textbox_font( "data/fonts/ubuntu_condensed_10.xml" ),
		textbox_font_color(),
		textbox_font_position( 5, 7 ),
		textbox_position( 120.f, 0 ),

		dropdown_top_sprite( "data/debug/sui_dropdown_box.png" ),
		dropdown_top_sprite_position( 0, 0 ),
		dropdown_font( "data/fonts/ubuntu_condensed_10.xml" ),
		dropdown_font_color(),
		dropdown_font_position( 6, 8 ),
		dropdown_position( 120.f, 0 ),

		dropdown_item_background_sprite( "data/debug/sui_dropdown_item_background.png" ),
		dropdown_item_start_pos( 0, 26 ),
		dropdown_item_position_size( 0, 30 ),
		dropdown_item_font( "data/fonts/ubuntu_condensed_10.xml" ),
		dropdown_item_font_position( 6, 9 ),
		dropdown_item_font_color(),

		header_color(),
		header_font_file( "data/fonts/ubuntu_condensed_18.xml" ),
		header_extra_position( -15, 7.f )
	{
		std::vector< float > primary_color;
		primary_color.resize( 4, 1.f );

		background_color.resize( 4, 57.f / 255.f );
		background_color[ 3 ] = 1.f;

		background_line_color.resize( 4, 38.f / 255.f );
		background_line_color[ 3 ] = 1.f;


		// primaries
		text_color = primary_color;
		slider_color = primary_color;
		checkbox_back_color = primary_color;
		textbox_font_color = primary_color;
		header_color = primary_color;

		// background
		slider_stroke_color = background_color;
		checkbox_front_color = background_color;
		dropdown_font_color = background_color;
		dropdown_item_font_color = background_color;
	}

	types::vector2			element_spacing;

	float					background_width;
	types::vector2			background_spacing;
	std::vector< float >	background_color;
	std::string				background_sprite;
	std::string				background_dropshadow;

	bool					use_background_lines;
	std::string				background_line_sprite;
	std::vector< float >	background_line_color;
	types::vector2			background_line_size;
	types::vector2			background_line_position;

	std::vector< float >	text_color;
	std::string				text_font_file;
	types::vector2			text_extra_position;

	bool					text_use_fadeoff;
	std::string				text_fadeoff_sprite;
	types::vector2			text_fadeoff_position;
	types::vector2			text_fadeoff_size;
	std::string				text_fadeoff_extra_sprite;
	types::vector2			text_fadeoff_extra_size;

	types::vector2			text_value_position;


	std::string				slider_front_sprite;
	std::string				slider_front_stroke_sprite;
	std::vector< float >	slider_stroke_color;
	types::vector2			slider_front_start_pos;
	types::vector2 			slider_front_end_pos;

	std::string				slider_back_sprite;
	types::vector2			slider_back_position;
	std::vector< float >	slider_color;

	std::string				checkbox_back_sprite;
	std::vector< float >	checkbox_back_color;
	std::string				checkbox_front_sprite;
	std::vector< float >	checkbox_front_color;
	types::vector2			checkbox_position;

	std::string				textbox_back_sprite;
	types::vector2			textbox_back_sprite_position;
	std::string				textbox_font;
	std::vector< float >	textbox_font_color;
	types::vector2			textbox_font_position;
	types::vector2			textbox_position;

	std::string				dropdown_top_sprite;
	types::vector2			dropdown_top_sprite_position;
	std::string				dropdown_font;
	std::vector< float >	dropdown_font_color;
	types::vector2			dropdown_font_position;
	types::vector2			dropdown_position;

	std::string				dropdown_item_background_sprite;
	types::vector2			dropdown_item_start_pos;
	types::vector2			dropdown_item_position_size;
	std::string				dropdown_item_font;
	types::vector2			dropdown_item_font_position;
	std::vector< float >	dropdown_item_font_color;

	std::vector< float >	header_color;
	std::string				header_font_file;
	types::vector2			header_extra_position;
};

UI_Config g_config;

} // end of anonymous namespace
//-----------------------------------------------------------------------------

void ConfigSliders::Update( float dt )
{
	// std::cout << "ConfigSliders::Update()" << std::endl;
	std::map< as::Sprite*, impl::IConfigElement* >::iterator i;
	for( i = mElements.begin(); i != mElements.end(); ++i ) 
	{
		impl::IConfigElement* config = i->second;
		if( config ) 
		{
			std::string str_value = config->GetValueStr();
			std::string str_prev_value = str_value;
			
			// sliders value update
			SimpleSlider* slider = dynamic_cast< SimpleSlider* >( i->first );
			if( slider && slider->mUserData )
			{
				str_prev_value = ((as::TextSprite*)(slider->mUserData))->GetText();

				SimpleTextbox* tbox = dynamic_cast< SimpleTextbox* >( ((as::TextSprite*)(slider->mUserData)) );
				// std::cout << "tbox: " << tbox << ", HasFocus()" << tbox->HasFocus() << std::endl;
				if( tbox && tbox->HasFocus() ) {
					// std::cout << "Getting text value: " << tbox->GetText() << std::endl;
					float slider_value = slider->GetSliderValue();
					slider_value = ceng::CastFromString< float >( tbox->GetText() );
					slider->SetSliderValue( slider_value);
				}

				if( ( tbox == NULL || tbox->HasFocus() == false ) && str_prev_value != str_value ) {
					((as::TextSprite*)(slider->mUserData))->SetText( str_value );
					slider->SetSliderValue( config->GetValue() );
				}
			}

			// checkbox value update
			SimpleCheckbox* checkbox = dynamic_cast< SimpleCheckbox* >( i->first );
			if( checkbox && checkbox->mUserData )
			{
				str_prev_value = ((as::TextSprite*)(checkbox->mUserData))->GetText();
				if( str_prev_value != str_value ) {
					((as::TextSprite*)(checkbox->mUserData))->SetText( str_value );
					checkbox->SetValue( config->GetValue()>0.5f?true:false );
				}
			}

			// textbox value update
			SimpleTextbox* textbox = dynamic_cast< SimpleTextbox* >( i->first );
			if( textbox && textbox->HasFocus() == false )
			{
				str_prev_value = textbox->GetValue();
				if( str_prev_value != str_value ) {
					textbox->SetValue( str_value );
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------

void ConfigSliders::OnSlideTo( as::Sprite* sprite, float value ) 
{
	std::map< as::Sprite*, impl::IConfigElement* >::iterator i = mElements.find( sprite );
	impl::IConfigElement* config = i->second;
	if( config ) 
	{
		std::string str_value_before = config->GetValueStr();
		config->SetValue( value );
		std::string str_value = config->GetValueStr();

		if( str_value != str_value_before )
		{
			SimpleSlider* slider = dynamic_cast< SimpleSlider* >( sprite );
			if( slider && slider->mUserData )
				((as::TextSprite*)(slider->mUserData))->SetText( str_value );

			if( mListener && slider ) mListener->OnSliderChangeTiny( slider->GetName() );
		}
	}
}
//-----------------------------------------------------------------------------

void ConfigSliders::OnSlideEnd( as::Sprite* sprite, float value )
{
	std::map< as::Sprite*, impl::IConfigElement* >::iterator i = mElements.find( sprite );

	if( i == mElements.end() ) 
	{
		logger << "Error - couldn't find config element for sprite: " << sprite;
		if( sprite ) 
			logger << " - " << sprite->GetName();

		logger << std::endl;
		return;
	}

	impl::IConfigElement* config = i->second;
	if( config ) 
		config->SetValue( value );

	if( mListener ) mListener->OnSliderChange();
}
//-----------------------------------------------------------------------------

void ConfigSliders::OnClick( as::Sprite* sprite, const std::string& name_of_sprite )
{
	SimpleButton* simple_button = dynamic_cast< SimpleButton* >( sprite );
	// Removing a component
	if( simple_button && name_of_sprite == "remove_component" )
	{
		if( mListener ) mListener->OnRemoveComponent( simple_button->mUserData );
	}

	if( sprite == mAddComponentTextbox )
	{
		std::cout << "Component name: " << name_of_sprite << std::endl;
		if( mListener )
			mListener->OnAddComponent( name_of_sprite );
	}
}
//-----------------------------------------------------------------------------

void ConfigSliders::OnCheckboxChange( as::Sprite* sprite, bool value )
{
	std::map< as::Sprite*, impl::IConfigElement* >::iterator i = mElements.find( sprite );

	if( i == mElements.end() ) 
	{
		logger << "Error - couldn't find config element for sprite: " << sprite;
		if( sprite ) 
			logger << " - " << sprite->GetName();

		logger << std::endl;
		return;
	}

	impl::IConfigElement* config = i->second;
	if( config ) 
		config->SetValue( value?1.f:0.f );

	if( mListener ) mListener->OnSliderChange();
}
//-----------------------------------------------------------------------------

void ConfigSliders::OnTextboxChange( as::Sprite* sprite, const std::string& value )
{
	// -- check if it's the thing
	std::map< as::Sprite*, impl::IConfigElement* >::iterator i = mElements.find( sprite );

	if( i == mElements.end() ) 
	{
		logger << "Error - couldn't find config element for sprite: " << sprite;
		if( sprite ) 
			logger << " - " << sprite->GetName();

		logger << std::endl;
		return;
	}

	impl::IConfigElement* config = i->second;
	if( config ) 
		config->SetValue( value );

	
	std::vector< as::Sprite* > elements;
	std::map< as::Sprite*, impl::IConfigElement* >::iterator j;
	for( j = mElements.begin(); j != mElements.end(); ++j ) 
	{
		if( j->second == config && j->first != sprite )  
			elements.push_back( j->first );
	}

	for( std::size_t k = 0; k < elements.size(); ++k )
	{
		// sliders value update
		SimpleSlider* slider = dynamic_cast< SimpleSlider* >( elements[ k ] );
		if( slider && slider->mUserData == sprite )
			slider->SetSliderValue( config->GetValue() );
	}


	if( mListener ) mListener->OnSliderChange();
}


void ConfigSliders::OnTextboxFocusChange( as::Sprite* sprite, bool has_focus )
{
	if( has_focus ) mFocusedSprite = sprite;
	if( has_focus == false && sprite == mFocusedSprite ) mFocusedSprite = NULL;
}

bool ConfigSliders::HasFocus() const 
{
	return ( mFocusedSprite != NULL );
}

//-----------------------------------------------------------------------------

ConfigSliders::ConfigSliders() :
	mListener( NULL ),
	mFocusedSprite( NULL ),
	mAddComponentTextbox( NULL )
{
	Init();
}

ConfigSliders::ConfigSliders( IConfigSlidersListener* listener ) :
	mListener( listener ),
	mFocusedSprite( NULL ),
	mAddComponentTextbox( NULL )
{
	Init();
}
//-----------------------------------------------------------------------------

void ConfigSliders::Init()
{
	mCanCreateLines = true;
	mBoxSize.Set( g_config.background_width, 400 );
	
	// do we even need this?!
	mYPosition = g_config.background_spacing.y;

	// creating background
	{
		as::Sprite* background = new as::Sprite;
		as::Sprite* background_solid = as::LoadSprite( g_config.background_sprite );
		background_solid->SetName( "background_solid" );
		background_solid->SetColor( g_config.background_color );
		as::Sprite* background_dropshadow = as::LoadSprite( g_config.background_dropshadow );
		background_dropshadow->SetCenterOffset( 0.5f * ( background_dropshadow->GetTextureSize() - background_solid->GetTextureSize() ) );
		background_dropshadow->SetName( "background_dropshadow" );

		background->addChild( background_dropshadow );
		background->addChild( background_solid );
		background->SetSize( (int)background_solid->GetTextureSize().x, (int)background_solid->GetTextureSize().y );
		mBackground = background;
		addChild( mBackground );
	}

	MoveTo( types::vector2( Poro()->GetGraphics()->GetInternalSize().x - mBoxSize.x, 0 ) );
	ResizeBackground( mBoxSize.y );
	CreateLine();
	
	// resize the width to boxsize as well
	cassert( mBackground );
	mBackground->SetScaleX( mBoxSize.x / mBackground->GetTextureSize().x );
}

void ConfigSliders::ResizeBackground( float height )
{
	cassert( mBackground );
	float scale_y = height / mBackground->GetTextureSize().y;
	mBackground->SetScaleY( scale_y );
	mBoxSize.y = height;
}

void ConfigSliders::RecomputeBackgroundSize()
{
	int element_count = GetElementCount();
	ResizeBackground( (float)element_count * g_config.element_spacing.y + 2.f * g_config.background_spacing.y );
}
//-----------------------------------------------------------------------------

as::Sprite* ConfigSliders::CreateLine()
{
	if( g_config.use_background_lines == false ) return NULL;
	if( mCanCreateLines == false ) return NULL;

	const types::vector2 position( 
		0,
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	as::Sprite* line = as::LoadSprite( g_config.background_line_sprite );
	line->MoveTo( position + g_config.background_line_position );
	line->Resize( g_config.background_line_size.x, g_config.background_line_size.y );
	line->SetColor( g_config.background_line_color );
	
	this->addChild( line );
	return line;
}
//-----------------------------------------------------------------------------

void ConfigSliders::AddComponentHeader( const std::string& component_name, bool remove_me_button, void* extra_data )
{

	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	// --- 

	as::Sprite* text_sprite = CreateHeader( component_name );
	if( remove_me_button == false ) mSliders.push_back( text_sprite );
	
	// ---------
	
	if( remove_me_button )
	{
		as::Sprite* button_sprite = as::LoadSprite( "data/debug/sui_close_button.png" );
		button_sprite->SetColor( g_config.checkbox_back_color );
		button_sprite->SetName( "remove_component" );

		SimpleButton* simple_button = new SimpleButton( this );
		simple_button->addChild( button_sprite );

		simple_button->MoveTo( position + types::vector2( 242 - 15.f, 0 )   );
		simple_button->mUserData = extra_data;

		this->addChild( simple_button );
		mSliders.push_back( simple_button );
	}
	//----------

	RecomputeBackgroundSize();
	CreateLine();
}

void ConfigSliders::AddHeaderText( const std::string& name )
{
	AddComponentHeader( name, false );
}

void ConfigSliders::AddAddComponent( const std::string& text, std::vector< std::string >& component_names )
{
	// --- 

	as::Sprite* text_sprite = CreateHeader( text );

	SimpleDropDown* dropdown = CreateDropDown( "", "Component", component_names );
	// SimpleTextbox* textbox = CreateTextbox( "", "" );
	mAddComponentTextbox = dropdown;

	//----------

	RecomputeBackgroundSize();
	CreateLine();
	// config_sliders->CreateAddComponentButton( "Add Component", "Default Component" );
}
//-----------------------------------------------------------------------------

as::Sprite*	ConfigSliders::CreateLabel( const std::string& name )
{
	if( name.empty() ) return NULL;

	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	// create text element first
	as::Sprite* text_element = new as::Sprite;
	text_element->MoveTo( types::vector2( position.x, position.y ) );


	as::TextSprite* t = as::LoadTextSprite( g_config.text_font_file );
	t->MoveTo( g_config.text_extra_position );
	t->SetColor( g_config.text_color );
	t->SetText( name );
	t->SetName( "text_name" );
	text_element->addChild( t );

	// text_element fade off part
	if( g_config.text_use_fadeoff ) 
	{
		as::Sprite* fadeoff_part = as::LoadSprite( g_config.text_fadeoff_sprite );
		fadeoff_part->SetColor( g_config.background_color );
		fadeoff_part->MoveTo( g_config.text_fadeoff_position );
		text_element->addChild( fadeoff_part );

		as::Sprite* fadeoff_extra = as::LoadSprite( g_config.text_fadeoff_extra_sprite );
		fadeoff_extra->SetColor( g_config.background_color  );
		fadeoff_extra->MoveTo( g_config.text_fadeoff_position + types::vector2( fadeoff_part->GetTextureSize().x, 0 ) );
		fadeoff_extra->Resize( g_config.text_fadeoff_extra_size.x, g_config.text_fadeoff_extra_size.y );
		text_element->addChild( fadeoff_extra );
	}

	this->addChild( text_element );
	return text_element;
}
//-----------------------------------------------------------------------------

as::Sprite*	ConfigSliders::CreateHeader( const std::string& name )
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	// create text element first
	as::Sprite* text_element = new as::Sprite;
	text_element->MoveTo( types::vector2( position.x, position.y ) );


	as::TextSprite* t = as::LoadTextSprite( g_config.header_font_file );
	t->MoveTo( g_config.header_extra_position );
	t->SetColor( g_config.header_color );
	t->SetText( name );
	t->SetName( "text_name" );
	text_element->addChild( t );

	this->addChild( text_element );
	return text_element;
}

//=============================================================================

as::Sprite* ConfigSliders::CreateTextValue( const std::string& text )
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	as::TextSprite* text_value = as::LoadTextSprite( "data/fonts/ubuntu_condensed_10.xml" );
	text_value->SetColor( 1, 1, 1 );
	text_value->SetText( text );
	text_value->SetAlign( as::TextSprite::FONT_ALIGN_LEFT );
	text_value->SetTextBox( types::rect( 0, 0, 30, 30 ) );
	text_value->MoveTo( position + g_config.text_value_position );
	text_value->SetName( "text_value" );

	this->addChild( text_value );
	return text_value;
}
// ----------------------------------------------------------------------------

SimpleTextbox*	ConfigSliders::CreateTextboxValue( const std::string& name, const std::string& value )
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	// ---------------------

	as::TextSprite* text_sprite = as::LoadTextSprite( "data/fonts/ubuntu_condensed_10.xml" );
	text_sprite->SetColor( 1, 1, 1 );
	text_sprite->SetText( value );
	text_sprite->SetAlign( as::TextSprite::FONT_ALIGN_LEFT );
	text_sprite->SetTextBox( types::rect( 0, 0, 30, 30 ) );
	// text_sprite->MoveTo( position + g_config.text_value_position );
	text_sprite->SetName( "text_element" );
	
	// "text_element"

	as::Sprite* background = as::LoadSprite( "data/debug/sui_spacer.png" );
	background->Resize( 32, 32 );


	SimpleTextbox* simple_textbox = new SimpleTextbox( this );
	simple_textbox->addChild( background );
	simple_textbox->addChild( text_sprite );
	simple_textbox->SetValue( value );


	simple_textbox->MoveTo( position + g_config.text_value_position );
	this->addChild( simple_textbox );
	// mSliders.push_back( simple_textbox );

	return simple_textbox;
}
// ----------------------------------------------------------------------------

SimpleSlider* ConfigSliders::CreateSlider( const std::string& name ) 
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	// label
	CreateLabel( name );

	// --- create slider backdrop
	{
		as::Sprite* slider_back = as::LoadSprite( g_config.slider_back_sprite );
		slider_back->MoveTo( position + g_config.slider_back_position );
		slider_back->SetColor( g_config.slider_color );
		addChild( slider_back );
	}
	
	// text value (the right hand element)
	SimpleTextbox* text_value = CreateTextboxValue( "humppa", "12" );
	
	//------ actual slider sprite element

	as::Sprite* slider_image = new as::Sprite;
	
	as::Sprite* slider_front = as::LoadSprite( g_config.slider_front_sprite );
	as::Sprite* slider_front_stroke = as::LoadSprite( g_config.slider_front_stroke_sprite );

	slider_front->SetCenterOffset( types::vector2( 0.5f * slider_front->GetTextureSize().x, 0 ) );
	slider_front->SetColor( g_config.slider_color );
	slider_front_stroke->SetCenterOffset( types::vector2( 0.5f * slider_front_stroke->GetTextureSize().x, 0 ) );
	slider_front_stroke->SetColor( g_config.slider_stroke_color );
	slider_image->addChild( slider_front_stroke );
	slider_image->addChild( slider_front );
	slider_image->SetSize( (int)slider_front->GetTextureSize().x, (int)slider_front->GetTextureSize().y );
	slider_image->SetCenterOffset( types::vector2( 0.5f * slider_front->GetTextureSize().x, 0 ) );
	slider_image->MoveTo( types::vector2( 0, 0 ) );
	slider_image->SetName( "slider_image" );

	// ---- simple slider gui element based on the sprite

	SimpleSlider* slider = new SimpleSlider( this );
	slider->addChild( slider_image );
	slider->SetName( name );
	slider->mStartPos = position + g_config.slider_front_start_pos;
	slider->mEndPos = position + g_config.slider_front_end_pos;

	slider->mUserData = ((void*)text_value);

	this->addChild( slider );
	mSliders.push_back( slider );

	RecomputeBackgroundSize();
	CreateLine();
	return slider;
}
//-----------------------------------------------------------------------------

SimpleCheckbox* ConfigSliders::CreateCheckbox( const std::string& name, bool value )
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	CreateLabel( name );

	// text value (the right hand element)
	as::Sprite* text_value = CreateTextValue( ceng::CastToString( value ) );

	// ---------------------
	

	as::Sprite* checkbox_sprite = as::LoadSprite( g_config.checkbox_back_sprite );
	as::Sprite* checkbox_overlay = as::LoadSprite( g_config.checkbox_front_sprite );
	checkbox_sprite->SetColor( g_config.checkbox_back_color );
	checkbox_overlay->SetColor( g_config.checkbox_front_color );
	checkbox_overlay->SetName( "checkbox_overlay" );

	SimpleCheckbox* simple_checkbox = new SimpleCheckbox( this );
	simple_checkbox->mValue = true;
	simple_checkbox->mCheckboxSpriteName = "checkbox_overlay";
	simple_checkbox->addChild( checkbox_sprite );
	simple_checkbox->addChild( checkbox_overlay );
	simple_checkbox->SetValue( value );
	simple_checkbox->mUserData = (void*)text_value;

	simple_checkbox->MoveTo( position + g_config.checkbox_position );
	this->addChild( simple_checkbox );
	mSliders.push_back( simple_checkbox );

	RecomputeBackgroundSize();
	CreateLine();
	return simple_checkbox;
}
//-----------------------------------------------------------------------------

SimpleButton* ConfigSliders::CreateButton( const std::string& name, const std::string& image_file )
{
	return NULL;
}

SimpleTextbox*	ConfigSliders::CreateTextbox( const std::string& name, const std::string& value )
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	
	CreateLabel( name );

	// ---------------------
	as::Sprite* back_textbox = as::LoadSprite( g_config.textbox_back_sprite );
	back_textbox->MoveTo( g_config.textbox_back_sprite_position );
	

	as::TextSprite* text_sprite = as::LoadTextSprite( g_config.textbox_font );
	text_sprite->MoveTo( g_config.textbox_font_position );
	text_sprite->SetName( "text_element" );
	text_sprite->SetColor( g_config.textbox_font_color );
	
	// "text_element"

	SimpleTextbox* simple_textbox = new SimpleTextbox( this );
	simple_textbox->addChild( back_textbox );
	simple_textbox->addChild( text_sprite );
	simple_textbox->SetValue( value );


	simple_textbox->MoveTo( position + g_config.checkbox_position );
	this->addChild( simple_textbox );
	mSliders.push_back( simple_textbox );

	RecomputeBackgroundSize();
	CreateLine();
	return simple_textbox;
}
//-----------------------------------------------------------------------------

SimpleDropDown* ConfigSliders::CreateDropDown( const std::string& name, const std::string& box_name, const std::vector< std::string >& values )
{
	const types::vector2 position( 
		g_config.background_spacing.x, 
		GetElementCount() * g_config.element_spacing.y + g_config.background_spacing.y );

	CreateLabel( name );

	// ----
	as::Sprite* dropdown_top = as::LoadSprite( g_config.dropdown_top_sprite );
	dropdown_top->MoveTo( g_config.dropdown_top_sprite_position );
	

	as::TextSprite* text_sprite = as::LoadTextSprite( g_config.dropdown_font );
	text_sprite->MoveTo( g_config.dropdown_font_position );
	text_sprite->SetName( "text_element" );
	text_sprite->SetColor( g_config.dropdown_font_color );
	text_sprite->SetText( box_name );
	
	// "text_element"

	SimpleDropDown* simple_dropdown = new SimpleDropDown( this );
	simple_dropdown->addChild( dropdown_top );
	simple_dropdown->addChild( text_sprite );

	// ---- values ---

	for( std::size_t i = 0; i < values.size(); ++i )
	{
		as::Sprite* dropdown_item = as::LoadSprite( g_config.dropdown_item_background_sprite );
		dropdown_item->MoveTo( g_config.dropdown_item_start_pos + ( g_config.dropdown_item_position_size * (float)i ) );
		dropdown_item->SetName( values[ i ] );

		as::TextSprite* text_item = as::LoadTextSprite( g_config.dropdown_font );
		text_item->MoveTo( g_config.dropdown_item_font_position );
		text_item->SetName( "text_element" );
		text_item->SetColor( g_config.dropdown_item_font_color );
		text_item->SetText( values[ i ] );
		text_item->SetName( values[ i ] );
		
		dropdown_item->addChild( text_item );
		simple_dropdown->AddDropDown( dropdown_item );
	}

	// /values


	simple_dropdown->MoveTo( position + g_config.dropdown_position );
	addChild( simple_dropdown );
	mSliders.push_back( simple_dropdown );

	RecomputeBackgroundSize();
	CreateLine();
	return simple_dropdown;
}
//-----------------------------------------------------------------------------
