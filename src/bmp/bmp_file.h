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
// File : bmp_file.h
// Contains: bmp image file loader / writer
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

#pragma pack(1)

#ifndef BI_BITFIELDS
#define BI_BITFIELDS 3
#endif

typedef struct bitmap_data_
{
	int16_t xsize;
	int16_t ysize;
	int16_t nb_color;
	uint8_t * palette;
	uint32_t  * data;
}bitmap_data;

typedef struct tagBITMAPFILEHEADER {
	uint16_t   bfType;
	uint32_t   bfSize;
	uint16_t   bfReserved1;
	uint16_t   bfReserved2;
	uint32_t   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	uint32_t    biSize;
	uint32_t    biWidth;
	uint32_t    biHeight;
	uint16_t    biPlanes;
	uint16_t    biBitCount;
	uint32_t    biCompression;
	uint32_t    biSizeImage;
	uint32_t    biXPelsPerMeter;
	uint32_t    biYPelsPerMeter;
	uint32_t    biClrUsed;
	uint32_t    biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
} RGBQUAD;

#pragma pack()

int bmp_load(char * file,bitmap_data * bdata);
int bmpRLE8b_write(char * file,bitmap_data * bdata);
int bmp24b_write(char * file,bitmap_data * bdata);
int bmp16b_write(char * file,bitmap_data * bdata);


