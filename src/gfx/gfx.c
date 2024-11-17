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
// File : gfx.c
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

#include <stdint.h>
#include <stdlib.h>

#include "font8x8.h"
#include "font4x6.h"

#include "gfx.h"

gfx * init_gfx( int xsize, int ysize, int layers_cnt )
{
	int i;
	gfx * ctx;

	ctx = NULL;

	if( layers_cnt <= 0 || layers_cnt > GFX_MAX_LAYERS)
		return NULL;

	ctx = calloc(1, sizeof(gfx));
	if(!ctx)
		goto error;

	ctx->xres = xsize;
	ctx->yres = ysize;

	for(i=0;i<layers_cnt;i++)
	{
		ctx->frame_buffer[i] = calloc(1, xsize*ysize*sizeof(uint32_t) );
		if(!ctx->frame_buffer[i])
			goto error;
	}

	ctx->layers = layers_cnt;

	return ctx;

error:

	for(i=0;i<layers_cnt;i++)
	{
		free(ctx->frame_buffer[i]);
	}

	free(ctx);

	return NULL;
}

void deinit_gfx( gfx * ctx )
{
	int i;

	if( ctx )
	{
		for(i=0;i<ctx->layers;i++)
		{
			free(ctx->frame_buffer[i]);
		}

		free(ctx);
	}
}

void gfx_plot( gfx * ctx, int layer, int x, int y, uint32_t color )
{
	if(!ctx)
		return;

	if(layer >= ctx->layers || layer < 0)
		return;

	if( ( x < 0 || y < 0 ) || ( x >= ctx->xres || y >= ctx->yres ) )
		return;

	ctx->frame_buffer[layer][(y * ctx->xres) + x] = color;
}

void gfx_line( gfx * ctx, int layer, int first_x ,int first_y, int last_x, int last_y, uint32_t color )
{
	int dx;
	int dy;
	int inc1,inc2;
	int sub,remain,error;

	if(!ctx)
		return;

	if(layer >= ctx->layers || layer < 0)
		return;

	dx = last_x - first_x;
	dy = last_y - first_y;

	if ( ( !dx ) && ( !dy ) )
	{
		return; // rien a tracer
	}

	if ( dx < 0 )
	{
		dx = -dx;
		inc2 = -1;
	}
	else
	{
		inc2 = 1;
	}

	if (dy<0)
	{
		dy = -dy;
		inc1 = -1;
	}
	else
	{
		inc1 = 1;
	}

	if ( dx > dy )
	{
		sub = dx - dy;
		error = dy - ( dx >> 1);

		remain = (dx + 1);

		do
		{
			gfx_plot(ctx, layer, first_x, first_y, color);

			if (error>=0)
			{
				gfx_plot(ctx, layer, first_x + inc2, first_y, color);

				first_y += inc1;
				error -= sub;

				gfx_plot(ctx, layer, first_x, first_y, color);
			}
			else
				error += dy;

			first_x += inc2;

		} while ( --remain > 0 );

		if ( !(dx&1) )
			gfx_plot(ctx, layer, first_x, first_y, color);

		return;
	}
	else
	{
		sub = dy - dx;
		error = dx - ( dy >> 1 );

		remain = ( dy + 1 );

		do
		{
			gfx_plot(ctx, layer, first_x, first_y, color);

			if ( error >= 0 )
			{
				gfx_plot(ctx, layer, first_x, first_y + inc1, color);

				first_x += inc2;
				error -= sub;

				gfx_plot(ctx, layer, first_x, first_y, color);
			}
			else
				error += dx;

			first_y += inc1;

		} while ( --remain > 0 );

		if ( !(dy&1) )
			gfx_plot(ctx, layer, first_x, first_y, color);

		return;
	}
}

void gfx_circle( gfx * ctx, int layer, int x_centre, int y_centre, int r, unsigned int color )
{
	int x;
	int y;
	int d;

	if(!ctx)
		return;

	if(layer >= ctx->layers || layer < 0)
		return;

	x = 0;
	y = r;
	d = r - 1;

//    8  1
//  7     2
//  6     3
//    5 4

	while( y >= x )
	{
		gfx_plot(ctx, layer, x+x_centre, -y+y_centre , color);  // 1 -
		gfx_plot(ctx, layer, y+x_centre, -x+y_centre , color);  // 2 +
		gfx_plot(ctx, layer, x_centre + y, x+y_centre, color);  // 3 -
		gfx_plot(ctx, layer, x_centre + x, y+y_centre, color);  // 4 +
		gfx_plot(ctx, layer, -x+x_centre, y+y_centre , color);  // 5 -
		gfx_plot(ctx, layer, -y+x_centre, x+y_centre , color);  // 6 +
		gfx_plot(ctx, layer, -y+x_centre, -x+y_centre, color);  // 7 -
		gfx_plot(ctx, layer, -x+x_centre, -y+y_centre, color);  // 8 +

		if ( d >= 2*x )
		{
			d = d - ( 2 * x ) -1;
			x = x+1;
		}
		else
		{
			if( d <= 2 * ( r - y ) )
			{
				d = d + ( 2 * y ) -1;
				y = y - 1;
			}
			else
			{
				d = d + 2 * (y - x - 1);
				y = y - 1;
				x = x + 1;
			}
		}
	}
}

void gfx_box( gfx * ctx, int layer, int x1, int y1, int x2, int y2, uint32_t color )
{
	int t,i;

	if(!ctx)
		return;

	if(layer >= ctx->layers || layer < 0)
		return;

	if(x1 > x2)
	{
		t = x1;
		x1 = x2;
		x2 = t;
	}

	if(y1 > y2)
	{
		t = y1;
		y1 = y2;
		y2 = t;
	}

	for(i = x1; i < x2 ;i++)
	{
		gfx_plot(ctx, layer, i, y1, color );
		gfx_plot(ctx, layer, i, y2, color );
	}

	for(i = y1; i < y2 ;i++)
	{
		gfx_plot(ctx, layer, x1, i, color );
		gfx_plot(ctx, layer, x2, i, color );
	}
}

void gfx_putc( gfx * ctx, int layer, int x, int y, unsigned int color, unsigned char c )
{
	const unsigned char * ptr;
	int xi,yi;

	if(!ctx)
		return;

	if(layer >= ctx->layers || layer < 0)
		return;

	if( c >= 128 )
		return;

	ptr = &font8x8[c * 8];

	for(yi=0;yi<8;yi++)
	{
		for(xi=0;xi<8;xi++)
		{
			if(*ptr & (0x01 << xi) )
			{
				gfx_plot(ctx, layer, x + xi, y + yi, color );
			}
			else
			{
				gfx_plot(ctx, layer, x + xi, y + yi, 0x000000 );
			}
		}
		ptr++;
	}
}

void gfx_printstr( gfx * ctx, int layer, int x, int y, unsigned int color, char *str )
{
	if(!str)
		return;

	while(*str)
	{
		gfx_putc(ctx, layer, x, y, color, *str);
		str++;
		x += 8;
	}
}
