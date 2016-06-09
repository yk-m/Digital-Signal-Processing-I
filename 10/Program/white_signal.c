//＊＊＊ H27年度・DSP1-9・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979
#define FN 1024

typedef struct {
	double *a;
	int length;
} Array_d;



/* ---------------------------------
	Random
---------------------------------- */
double gauss( void );



/* ---------------------------------
	Filter
---------------------------------- */
Array_d* filter( Array_d* input, Array_d *filter );
double nthFilter( int n, Array_d* input, Array_d* filter );



/* ---------------------------------
	I/O functions
---------------------------------- */
int countLinesOfFile( FILE *fp );
void askForFilename( char file_name[] );
FILE* openFile( const char file_type[], const char* mode );
Array_d* loadFile ( const char file_type[] );



/* ---------------------------------
	Array_d
---------------------------------- */
Array_d* newArray_d( int length );
void freeArray_d( Array_d* d );
void printArray_dToFile( Array_d* d );



/* ---------------------------------
	Error Handler
---------------------------------- */
void FileOpenError( const char* file_name );
void AllocationError();






int main(void) {
	int num = 11025;
	Array_d *noise = newArray_d( num );
	srand( (unsigned int)time( NULL ) );

	for ( int i = 0; i < num; ++i) {
		noise->a[i] = powl( 2, 9L ) * gauss();
	}

	printArray_dToFile( noise );

	freeArray_d( noise );
	return 0;
}



/* ---------------------------------
	Random
---------------------------------- */
double gauss( void )//正規乱数（ボックスミュラー法を利用）
{
	double x1, x2
	     , standard_deviation = 1.0
	;

	x1 = (double)rand() / (RAND_MAX);
	while ( x1 == 0.0 ) {
		x1 = (double)rand() / (RAND_MAX);
	}
	x2 = (double)rand() / (RAND_MAX);

	return sqrt( -2.0 * log(x1) ) * cos( 2.0 * PI * x2 ) * sqrt( standard_deviation );
}


/* ---------------------------------
	Filter
---------------------------------- */
Array_d* filter( Array_d* input, Array_d *filter ) {
	Array_d* output = newArray_d( input->length + filter->length - 1 );

	for( int n = 0; n < output->length; n++ ) {
		output->a[n] = nthFilter( n, input, filter );
	}

	return output;
}

double nthFilter( int n, Array_d* input, Array_d* filter ) {
	double sum = 0;

	for( int i = 0; i < filter->length && i <= n; i++ ) {
		sum += input->a[ n - i ] * filter->a[i];
	}

	return sum;
}



/* ---------------------------------
	I/O functions
---------------------------------- */
int countLinesOfFile( FILE *fp ) {
	int num = 0;
	char line[24];

	while( fgets( line , sizeof( line ) , fp ) != NULL ) {
		if ( line[0] == '\n' )
			continue;
		num++;
	}
	rewind( fp );

	return num;
}

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

Array_d* loadFile ( const char file_type[] ) {
	FILE *fp = openFile( file_type, "r" );
	Array_d *data;
	int length;

	length = countLinesOfFile( fp );
	data = newArray_d( length );

	for ( int i = 0; i < data->length; ++i ) {
		fscanf( fp , "%lf" , &data->a[i] );
	}

	fclose( fp );

	return data;
}



/* ---------------------------------
	Array_d
---------------------------------- */
Array_d* newArray_d( int length ) {
	Array_d *d;

	if( ( d = (Array_d*)malloc( sizeof( Array_d ) ) ) == NULL )
		AllocationError();

	if( ( d->a = (double*)malloc( sizeof(double) * length ) ) == NULL )
		AllocationError();

	d->length = length;

	return d;
}

void freeArray_d( Array_d* d ) {
	free( d->a );
	free( d );
}

void printArray_dToFile( Array_d* d ) {
	FILE *fp = openFile( "出力信号", "w" );

	for ( int i = 0; i < d->length; ++i ) {
		fprintf( fp, "%f\n", d->a[i] );
	}

	fclose( fp );
}



/* ---------------------------------
	Error Handler
---------------------------------- */
void FileOpenError( const char* file_name ) {
	printf("Could not Open \"%s\"\n", file_name );
	exit( -1 );
}

void AllocationError() {
	printf( "Allocation Error\n" );
	exit(1);
}
