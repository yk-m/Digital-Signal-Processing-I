#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 2010

void delaymakeFilter( double filter[] );
void echomakeFilter( double filter[] );

void printFilterToFile( const char* fn , double filter[] );
void FileOpenError( const char* fname );

int main(void){

	double filter[NUM];
	// delaymakeFilter( filter );
	// printFilterToFile( "delay_filter.txt" , filter );
	echomakeFilter( filter );
	printFilterToFile( "echo_filter.txt" , filter );
}

void delaymakeFilter( double filter[] ){

	for( int i = 0; i < NUM; i++ ){
		filter[i] = 0.0;
		if( i == 2009 ){
			filter[i] = 1.0;
		}
	}
}
void echomakeFilter( double filter[] ){

	for( int i = 0; i < NUM; i++ ){
		filter[i] = 0.0;
		if( i == 0 ){
			filter[i] = 1.0;
		}
		if( i == 309 ){
			filter[i] = 0.8;
		}
		if( i == 2009 ){
			filter[i] = 0.4;
		}
	}
}


void printFilterToFile( const char* fn , double filter[] ){

	FILE *fp = fopen( fn, "w" );
	if ( fp == NULL )
		FileOpenError( fn );

	for ( int i = 0; i < NUM; i++ ) {
		fprintf( fp, "%f\n", filter[i] );
	}

	fclose( fp );
}

void FileOpenError( const char* fname ) {
	printf("can't open %s\n", fname);
	exit(-1);
}