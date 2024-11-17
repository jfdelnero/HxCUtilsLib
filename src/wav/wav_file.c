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
// File : wav.c
// Contains: wav file helpers
//
// Written by: Jean-François DEL NERO
//
// Copyright (C) 2022 Jean-François DEL NERO
//
// You are free to do what you want with this code.
// A credit is always appreciated if you use it into your product :)
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cache.h"
#include "wav_file.h"

#ifdef __cplusplus
#define NULLPTR 0
#else
#define NULLPTR ((void*)0)
#endif

wav_io * create_wav(char * path,int samplerate, int type)
{
	wav_hdr wavhdr;
	wav_io * w_io;
	int sample_byte_size;

	w_io = malloc(sizeof(wav_io));
	if(w_io)
	{
		memset(w_io,0,sizeof(wav_io));

	//	if(!path)
	//		w_io->file = stdout;//fopen(stdout,"w+b");
	//	else
	//		w_io->file = fopen(path,"w+b");

		if( open_file(&w_io->file, path, sizeof(wav_hdr), 0x00) >= 0 )
		{	
			w_io->total_nb_samples = 0;
			w_io->type = type;
			switch(w_io->type)
			{
				case WAV_FILE_FORMAT_RAW_8BITS_IQ: // Raw / IQ
					w_io->sample_byte_size = 2; // I + Q
				break;

				case WAV_FILE_FORMAT_WAV_8BITS_STEREO:  // Wave 8 bits stereo
				case WAV_FILE_FORMAT_WAV_16BITS_STEREO: // Wave 16 bits stereo
				case WAV_FILE_FORMAT_WAV_16BITS_MONO:   // Wave 16 bits mono
					memset(&wavhdr,0,sizeof(wavhdr));

					memcpy((char*)&wavhdr.RIFF,"RIFF",4);
					memcpy((char*)&wavhdr.WAVE,"WAVE",4);
					memcpy((char*)&wavhdr.fmt,"fmt ",4);
					wavhdr.Subchunk1Size = 16;
					wavhdr.AudioFormat = 1;

					wavhdr.NumOfChan = 2;  // I & Q
					if(w_io->type == WAV_FILE_FORMAT_WAV_16BITS_MONO)
						wavhdr.NumOfChan = 1;

					wavhdr.SamplesPerSec = samplerate;
					w_io->sample_rate = samplerate;

					if(w_io->type == WAV_FILE_FORMAT_WAV_8BITS_STEREO)
						wavhdr.bitsPerSample = 8;
					else
						wavhdr.bitsPerSample = 16;

					sample_byte_size = ((wavhdr.bitsPerSample*wavhdr.NumOfChan)/8);

					w_io->sample_byte_size = sample_byte_size;

					wavhdr.bytesPerSec = ((samplerate*wavhdr.bitsPerSample)/8) * wavhdr.NumOfChan;
					wavhdr.blockAlign = sample_byte_size;
					memcpy((char*)&wavhdr.Subchunk2ID,"data",4);

					wavhdr.ChunkSize = (w_io->total_nb_samples * sample_byte_size) + sizeof(wav_hdr) - 8;
					wavhdr.Subchunk2Size = (w_io->total_nb_samples * sample_byte_size);

					write_buf( &w_io->file, 0, (void*)&wavhdr, sizeof(wav_hdr) );

					// Note about the following raw data format :
					// 8-bit samples are stored as unsigned bytes, ranging from 0 to 255.
					// 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.

				break;
			}
		}
	}

	return w_io;
}

wav_io * open_wav(char * path)
{
	wav_hdr wavhdr;
	wav_io * w_io;
	int sample_byte_size;
	int filesize;

	w_io = malloc(sizeof(wav_io));
	if(w_io)
	{
		memset(w_io,0,sizeof(wav_io));

		if( open_file(&w_io->file, path, -1, 0x00) < 0 )
			goto error;

		filesize = w_io->file.file_size;

		if( filesize < sizeof(wav_hdr) )
			goto error;
		
		read_buf(&w_io->file,0, (void*)&wavhdr, sizeof(wav_hdr));

		if( memcmp((char*)&wavhdr.RIFF,"RIFF",4) )
			goto error;

		if( memcmp((char*)&wavhdr.WAVE,"WAVE",4) )
			goto error;

		if( memcmp((char*)&wavhdr.fmt,"fmt ",4) )
			goto error;

		if( memcmp((char*)&wavhdr.Subchunk2ID,"data ",4) )
			goto error;

		if( wavhdr.AudioFormat != 1 )
			goto error;

		if( wavhdr.Subchunk1Size != 16 )
			goto error;

		if( wavhdr.bitsPerSample != 16 )
			goto error;

		if( wavhdr.NumOfChan == 1 )
			w_io->type = WAV_FILE_FORMAT_WAV_16BITS_MONO;
		else
			goto error;

		w_io->sample_rate = wavhdr.SamplesPerSec;

		sample_byte_size = ((wavhdr.bitsPerSample*wavhdr.NumOfChan)/8);
		w_io->sample_byte_size = sample_byte_size;
		w_io->total_nb_samples = wavhdr.Subchunk2Size / sample_byte_size;
	}

	return w_io;

error:
		printf("open_wav : error ! (%s)\n",path);

		if( w_io )
		{
			close_file( &w_io->file );
		}

		free( w_io );

		w_io = NULL;

	return NULL;
}

void write_wav(wav_io * w_io, void * buffer, int nbsamples)
{
	if(w_io)
	{
		
		//if(!w_io->file)
		//	return;

		write_buf( &w_io->file, w_io->file.file_size, buffer, nbsamples * w_io->sample_byte_size);

		w_io->total_nb_samples += nbsamples;

		w_io->sample_offset += nbsamples;

		w_io->dirty = 1;
	}
}

void read_wav(wav_io * w_io, void * buffer, int nbsamples)
{
	if(w_io)
	{
		//if(!w_io->file)
		//	return;

		read_buf( &w_io->file, sizeof(wav_hdr) + ( w_io->sample_offset * w_io->sample_byte_size ), buffer, nbsamples * w_io->sample_byte_size );

		w_io->sample_offset += nbsamples;
	}
}

void close_wav(wav_io * w_io)
{
	wav_hdr wavhdr;

	if(w_io)
	{
//		if(!w_io->file)
//			return;

		switch(w_io->type)
		{
			case WAV_FILE_FORMAT_RAW_8BITS_IQ: // Raw / IQ
				close_file( &w_io->file );
			break;

			case WAV_FILE_FORMAT_WAV_8BITS_STEREO:  // Wave 8 bits stereo
			case WAV_FILE_FORMAT_WAV_16BITS_STEREO: // Wave 16 bits stereo
			case WAV_FILE_FORMAT_WAV_16BITS_MONO:   // Wave 16 bits mono
				read_buf( &w_io->file, 0, &wavhdr, sizeof(wav_hdr));

				wavhdr.ChunkSize += ( w_io->total_nb_samples * w_io->sample_byte_size );
				wavhdr.Subchunk2Size += ( w_io->total_nb_samples * w_io->sample_byte_size );

				write_buf( &w_io->file, 0, &wavhdr, sizeof(wav_hdr));

				close_file( &w_io->file );
			
			break;
		}

		free(w_io);
	}
}
