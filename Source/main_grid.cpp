#include <sdl.h>

#include <utils/array2d/carray2d.h>
#include <utils/math/cvector2.h>
#include <utils/imagetoarray/imagetoarray.h>

#include <poro/external/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <poro/external/stb_image_write.h>

struct GridParams
{
	int image_w;
	int image_h;
	int n;		// number of elements
	std::string font;
	Uint32 background_color;
	Uint32 foreground_color;
	int border_size;
};

void BlitImage( ceng::CArray2D< Uint32 >& blit_this, ceng::CArray2D< Uint32 >& to_here, int pos_x, int pos_y )
{
	for( int y = 0; y < blit_this.GetHeight(); ++y )
	{
		for( int x = 0; x < blit_this.GetWidth(); ++x )
		{
			if( to_here.IsValid( x + pos_x, y + pos_y ) )
			{
				to_here.At( x + pos_x, y + pos_y ) = blit_this.At( x, y );
			}
		}
	}
}

void DoAGrid( const GridParams& params, const std::string& output_filename )
{
	ceng::CArray2D< Uint32 > image( params.image_w, params.image_h );
	image.SetEverythingTo( params.background_color );

	// SaveImage( output_filename, image );

	float square_size = (float)( params.image_w * 2 + params.image_h * 2 ) / (float)( params.n + 4 );
	ceng::CArray2D< Uint32 > border( (int)(square_size + 0.5f), (int)(square_size + 0.5f) );
	
	for( int y = 0; y < border.GetHeight(); ++y )
	{
		for( int x = 0; x < border.GetWidth(); ++x )
		{
			Uint32 c = params.foreground_color;		
			if( x >= params.border_size && x < border.GetWidth() - params.border_size &&
				y >= params.border_size && y < border.GetHeight() - params.border_size )
			{
				c = params.background_color;
			}
			border.At( x, y ) = c;
		}
	}

	{
		types::ivector2 pos( 0, 0 );
		types::ivector2 vel( 1, 0 );
		int x_width = params.image_w / border.GetWidth();
		int y_height = params.image_h / border.GetHeight();
		int n = x_width;
		for( int i = 0; i < params.n + 1; ++i )
		{
			BlitImage( border, image, pos.x, pos.y );
			types::ivector2 actual_vel = types::ivector2( vel.x * border.GetWidth(), vel.y * border.GetHeight() );
			types::ivector2 new_pos = pos + actual_vel;
			n--;
			if( n <= 0 )
			{
			
			/*if( new_pos.x + actual_vel.x > image.GetWidth() + 2 || 
				new_pos.x + actual_vel.x < -200 ||
				new_pos.y + actual_vel.y > image.GetHeight() + 2 || 
				new_pos.y + actual_vel.y < -200 )
			{*/
				// rotate vel
				if( vel.x > 0 ) { vel.Set( 0, 1 ); n = y_height - 1; }
				else if( vel.y > 0 ) { vel.Set( -1, 0 ); n = x_width; }
				else if( vel.x < 0 ) { vel.Set( 0, -1 ); n = y_height; pos.x = 0; }
				else if( vel.y < 0 ) { vel.Set( 1, 0 ); n = x_width; }
				std::cout << "n, vel: " << n << ", " << vel.x << ", " << vel.y << std::endl;
				actual_vel = types::ivector2( vel.x * border.GetWidth(), vel.y * border.GetHeight() );
				new_pos = pos + actual_vel;
			}
			
			pos = new_pos;
		}
	}

	SaveImage( output_filename, image );
}

int main(int argc, char *argv[])
{
	
	//struct GridParams
	//{
	//	int image_w;
	//	int image_h;
	//	int n;		// number of elements
	//	std::string font;
	//	Uint32 background_color;
	//	Uint32 foreground_color;
	//	int border_size;
	//};

	GridParams gridparams;
	gridparams.image_w = 1024;
	gridparams.image_h = 1280;
	gridparams.n = 25;
	gridparams.font = "";
	gridparams.background_color = 0xFFFFFFFF;
	gridparams.foreground_color = 0x000000FF;
	gridparams.border_size = 5;
	DoAGrid( gridparams, "grid_test.png" );
	return 0;
}
