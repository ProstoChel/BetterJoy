#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


struct {
	uint32_t start;
	uint32_t end;
} font_offsets;

FILE *fwfile;

uint32_t *fptrs;
uint32_t npics;

int main( int argc, char* argv[] )
{
	uint32_t offset;

	offset = 0;
	npics = 0;

	if (( argc != 2 ) && ( argc != 4))
	{
		fprintf( stderr, "usage: expfont <fwfile> [<offset> <npics>]\n" );
		return 1;
	}

	if ( argc == 4 )
	{
		offset = strtoul( argv[2], 0, 0 );
		npics  = strtoul( argv[3], 0, 0 );

		if ( !offset || !npics )
		{
			fprintf( stderr, "invalid parameters\n" );
			return 1;
		}
	}

	fwfile = fopen( argv[1], "rb" );
	if ( !fwfile )
		return 2;

	if ( offset )
	{
		font_offsets.start = offset;
		font_offsets.end = offset + npics * 4;
	}
	else
	{
		fseek( fwfile, 0x144, SEEK_SET );
		fread( &font_offsets, sizeof(font_offsets), 1, fwfile );

		npics = ( font_offsets.end - font_offsets.start ) / 4;
	}

	fptrs = malloc( npics * 4 );

	fseek( fwfile, font_offsets.start, SEEK_SET );
	fread( fptrs, npics * 4, 1, fwfile );

	for ( int i = 0 ; i < npics ; ++i )
	{
		fseek( fwfile, fptrs[i], SEEK_SET );

		int w = fgetc( fwfile );
		int h = fgetc( fwfile );
		fprintf( stdout, "const image_t img1306_%04X = { %d, %d, { ", i+1, w, h );

		int l = w * (( h >> 3 ) + (( h & 7 ) ? 1 : 0 ));
		for ( int j = 1 ; j <= l ; ++j )
		{
			int b = fgetc( fwfile );
			fprintf( stdout, "%d%s ", b, (j==l)?"":"," );
		}
		
		fprintf( stdout, "} };\n" );
	}

	fprintf( stdout, "\n" );

	if ( !offset )
	{
		fseek( fwfile, 0x14C, SEEK_SET );
		fread( &font_offsets, sizeof(font_offsets), 1, fwfile );

		npics = ( font_offsets.end - font_offsets.start ) / 4;
		fptrs = realloc( fptrs, npics * 4 );
		
		fseek( fwfile, font_offsets.start, SEEK_SET );
		fread( fptrs, npics * 4, 1, fwfile );

		for ( int i = 0 ; i < npics ; ++i )
		{
			fseek( fwfile, fptrs[i], SEEK_SET );

			int w = fgetc( fwfile );
			int h = fgetc( fwfile );
			fprintf( stdout, "const image_t img1327_%04X = { %d, %d, { ", i+1, w, h );

			int l = h * (( w >> 3 ) + (( w & 7 ) ? 1 : 0 ));
			for ( int j = 1 ; j <= l ; ++j )
			{
				int b = fgetc( fwfile );
				fprintf( stdout, "%d%s ", b, (j==l)?"":"," );
			}
			
			fprintf( stdout, "} };\n" );
		}
	}
	
	free( fptrs );

	return 0;
}
