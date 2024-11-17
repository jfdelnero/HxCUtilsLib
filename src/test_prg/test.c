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
// File : test.c
// Contains: test purpose
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

#include "gfx.h"
#include "bmp_file.h"

#include "cache.h"
#include "wav_file.h"

#include "rand_gen.h"

int main (int argc, char ** argv)
{
/////////////////////////////////////////////
	gfx * graph;

	graph = init_gfx(1024,768, 1);
	if(graph)
	{
		gfx_circle( graph, 0, graph->xres/2, graph->yres/2, graph->yres/6, 0xFFFFFF);

		gfx_line( graph, 0, 0, 0, graph->xres - 1, graph->yres - 1, 0xFFFFFF );
		gfx_line( graph, 0, 0, graph->yres - 1, graph->xres - 1, 0, 0xFFFFFF );

		gfx_box( graph, 0, graph->xres/4,graph->yres/4,graph->xres/4 + graph->xres/2,graph->yres/4 + graph->yres/2, 0xFFFFFF );

		gfx_plot( graph, 0, graph->xres/2, graph->yres/2 - graph->yres/3, 0xFFFFFF);
		gfx_plot( graph, 0, graph->xres/2, graph->yres/2 + (graph->yres/3), 0xFFFFFF);
		gfx_plot( graph, 0, graph->xres/2 - (graph->xres/3), graph->yres/2, 0xFFFFFF);
		gfx_plot( graph, 0, graph->xres/2 + (graph->xres/3), graph->yres/2, 0xFFFFFF);

		gfx_printstr( graph, 0, graph->xres/3,graph->xres/8, 0x3377FF, "Hello !");

		bitmap_data bmp;

		bmp.xsize = graph->xres;
		bmp.ysize = graph->yres;
		bmp.data =  graph->frame_buffer[0];
		bmp.palette = NULL;

		bmp24b_write("test.bmp",&bmp);

		deinit_gfx(graph);
	}

/////////////////////////////////////////////

	rand_gen_state rand_state;
	wav_io * wav;

	rand_gen_init(&rand_state,0x6728308);

	wav = create_wav("test.wav",44100,WAV_FILE_FORMAT_WAV_16BITS_MONO);
	if(wav)
	{
		int16_t smp;

		for(int i=0;i<44100*10;i++)
		{
			smp = rand_gen_get_next_word(&rand_state);
			write_wav(wav, &smp, 1);
		}

		close_wav(wav);
	}

	return 0;
}
