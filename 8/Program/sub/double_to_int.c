//＊＊＊ H27年度・DSP1-8・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>

#define FN 1024

/* ---------------------------------
	I/O functions 
---------------------------------- */
void askForFilename( char file_name[] );
FILE* openFile( const char file_type[], const char* mode );



/* ---------------------------------
	Error Handler 
---------------------------------- */
void FileOpenError( const char* file_name );



int main(void) {
	FILE *input  = openFile( "double数値のファイル: ", "r" )
	   , *output = openFile( "出力ファイル: ", "w" )
	;
	double tmp;
	
	while( fscanf( input, "%lf", &tmp ) != EOF ) {
		fprintf( output, "%d\n", (int)tmp );
	}

	fclose( input );
	fclose( output );
	return 0;
}



/* ---------------------------------
	I/O functions 
---------------------------------- */

void askForFilename( char file_name[] ) {
	printf("：");
	scanf( "%s", file_name );
}

FILE* openFile( const char file_type[], const char* mode ) {
	FILE* file;
	char file_name[FN];

	printf( "%s", file_type );
	askForFilename( file_name );

	if( ( file = fopen( file_name, mode ) ) == NULL )
		FileOpenError( file_name );

	return file;
}



/* ---------------------------------
	Error Handler 
---------------------------------- */
void FileOpenError( const char* file_name ) {
	printf("Could not Open \"%s\"\n", file_name );
	exit( -1 );
}
