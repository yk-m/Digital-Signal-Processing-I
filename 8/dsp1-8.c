//＊＊＊ H27年度・DSP1-8・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FN 1024

typedef struct {
	double *a;
	int length;
} Array;



/* ---------------------------------
	Filter 
---------------------------------- */
Array* filter( Array* input, Array *filter );
double nthFilter( int n, Array* input, Array* filter );



/* ---------------------------------
	I/O functions 
---------------------------------- */
int countLinesOfFile( FILE *fp );
void askForFilename( char file_name[] );
FILE* openFile( const char file_type[], const char* mode );
Array* loadFile ( const char file_type[] );



/* ---------------------------------
	Array
---------------------------------- */
Array* newArray( int length );
void freeArray( Array* d );
void printArrayToFile( Array* d );



/* ---------------------------------
	Error Handler 
---------------------------------- */
void FileOpenError( const char* file_name );
void AllocationError();






int main(void) {
	Array *coefficients
	    , *input
	    , *output
	;
	
	printf(
		"H27年度・DSP1-8・番号33\n"
		"使い方の説明：フィルタ係数と入力信号を受け取り，フィルタリング結果を出力します．\n"
		"  フィルタ係数と入力信号のファイルの名前を入力してください．\n"
		"  最後に，出力したいファイルの名前を入力してください．"
		"必要な結果：\n"
		"    ファイル出力：\n"
		"      1\n"
		"        出力ファイルとして指定されたファイル\n"
		"        内容：フィルタリング結果\n"
		"\n"
	);

	coefficients = loadFile( "フィルタ係数" );
	input        = loadFile( "入力信号" );

	output = filter( input, coefficients );
	printArrayToFile( output );

	freeArray( coefficients );
	freeArray( input );
	freeArray( output );
	return 0;
}



/* ---------------------------------
	Filter 
---------------------------------- */

Array* filter( Array* input, Array *filter ) {
	Array* output = newArray( input->length + filter->length - 1 );

	for( int n = 0; n < output->length; n++ ) {
		output->a[n] = nthFilter( n, input, filter );
	}

	return output;
}

double nthFilter( int n, Array* input, Array* filter ) {
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

Array* loadFile ( const char file_type[] ) {
	FILE *fp = openFile( file_type, "r" );
	Array *data;
	int length;

	length = countLinesOfFile( fp );
	data = newArray( length );

	for ( int i = 0; i < data->length; ++i ) {
		fscanf( fp , "%lf" , &data->a[i] );
	}

	fclose( fp );

	return data;
}



/* ---------------------------------
	Array
---------------------------------- */

Array* newArray( int length ) {
	Array *d;

	if( ( d = (Array*)malloc( sizeof( Array ) ) ) == NULL )
		AllocationError();

	if( ( d->a = (double*)malloc( sizeof(double) * length ) ) == NULL )
		AllocationError();

	d->length = length;

	return d;
}

void freeArray( Array* d ) {
	free( d->a );
	free( d );
}

void printArrayToFile( Array* d ) {
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
