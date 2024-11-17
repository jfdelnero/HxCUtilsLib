///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------H----H--X----X-----CCCCC----22222----0000-----0000------11----------//
//----------H----H----X-X-----C--------------2---0----0---0----0--1--1-----------//
//---------HHHHHH-----X------C----------22222---0----0---0----0-----1------------//
//--------H----H----X--X----C----------2-------0----0---0----0-----1-------------//
//-------H----H---X-----X---CCCCC-----222222----0000-----0000----1111------------//
//-------------------------------------------------------------------------------//
//----------------------------------------------------- http://hxc2001.free.fr --//
///////////////////////////////////////////////////////////////////////////////////
// File : gfx.h
// Contains: Graphics functions helpers
//
// Written by: Jean-François DEL NERO
//
// Copyright (C) 2024 Jean-François DEL NERO
//
// You are free to do what you want with this code.
// A credit is always appreciated if you use it into your product :)
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#define GFX_MAX_LAYERS 16

typedef struct _gfx
{
	unsigned int * frame_buffer[GFX_MAX_LAYERS];
	int xres;
	int yres;
	int layers;
}gfx;

gfx * init_gfx( int xsize, int ysize, int layers_cnt );
void deinit_gfx( gfx * ctx );

void gfx_plot( gfx * ctx, int layer, int x, int y, uint32_t color );
void gfx_line( gfx * ctx, int layer, int first_x, int first_y, int last_x, int last_y, uint32_t color );
void gfx_circle( gfx * ctx, int layer, int x_centre, int y_centre, int r, unsigned int color );
void gfx_box( gfx * ctx, int layer, int x1, int y1, int x2, int y2, uint32_t color );
void gfx_putc( gfx * ctx, int layer, int x, int y, unsigned int color, unsigned char c );
void gfx_printstr( gfx * ctx, int layer, int x, int y, unsigned int color, char *str );
