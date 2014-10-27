#include <sdl.h>

#include <utils/array2d/carray2d.h>

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

void DoAGrid( const GridParams& params )
{
	ceng::CArray2D< Uint32 > image( params.image_w, params.image_h );
	image.SetEverythingTo( params.background_color );


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

	for( int i = 0; i < params.n; ++i )
	{
	}
}

int main(int argc, char *argv[])
{
	return 0;
}
