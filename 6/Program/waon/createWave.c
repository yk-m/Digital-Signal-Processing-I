#include <stdio.h>
#include <math.h>

#define LENGTH 16384

int main() {
	long double wave
	          , frequencies[3] = { 2349.0, 2960.0, 3520.0 }
	          // , frequency[3] = { 261.63, 329.63, 392 }
	;

	FILE* file = fopen( "waon.txt", "w" );

	for( int i = 0; i < LENGTH; i++ ) {
		wave = 0;
		for ( int j = 0; j < 3; j++ ) {
			wave += powl( 2, 8L ) * sin( 2.0 * M_PI * frequencies[j] * (long double)i / 11025.0 ) / powl( 2, 2-j );
		}
		fprintf( file, "%ld\n", (long)wave );
	}
}