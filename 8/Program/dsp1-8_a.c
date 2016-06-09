//＊＊＊ H27年度・DSP1-7・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FN 1024

typedef struct
{
	double re;
	double im;
} complex_t;

typedef struct
{
	double radius;
	double angle;
} polar_t;

typedef struct {
	complex_t *a;
	int length;
} Array;



/* ---------------------------------
	FFT 
---------------------------------- */
void fft( complex_t *data, complex_t *twiddles, int fft_points );
void ifft( complex_t *data, complex_t *twiddles, int fft_points );
void sortData( complex_t *data, complex_t *sorted, int fft_points );
void reverseBits( int *bits, int fft_points, int number_of_bits );
void conjugateComplexes( complex_t *comp, int num );
void generateTwiddleFactors( complex_t *twiddles, int fft_points );
void calcAmplitudeSpectrum( complex_t Xk[], double spectrum[] , int dim );


/* ---------------------------------
	General 
---------------------------------- */
int isCloseToZero( double value );
int isPowerOfTwo( int value );

complex_t *allocateComplexMemory( int num );
double *allocateDoubleMemory( int num );

int intlog2( int n );


/* ---------------------------------
	I/O functions 
---------------------------------- */
int countLinesOfFile( FILE *fp );
void askForFilename( char file_name[] );
FILE* openFile( const char file_type[], const char* mode );
Array* loadFile ( const char file_type[] );
void printComplexToFile( const char fn[], complex_t vec[], int dim );
void printDoubleToFile( const char fn[], double vec[], int dim );


/* ---------------------------------
	complex calculation 
---------------------------------- */
complex_t addComplex( complex_t a, complex_t b );
complex_t subtractComplex( complex_t minuend, complex_t subtrahend );
complex_t multiplyComplex( complex_t a, complex_t b );
complex_t divideComplex( complex_t dividend, complex_t divisor );
complex_t conjugateComplex( complex_t c );
double calcRadius( complex_t c );
double calcAngle( complex_t c );
polar_t convertRectangularToPolar( complex_t c );
complex_t convertPolarToRectangular( polar_t p );
complex_t generateTwiddleFactor( int dimension, int nth );



/* ---------------------------------
	Error Handler 
---------------------------------- */
void FileOpenError( const char* file_name );
void AllocationError();



int main()
{
	int dim;
	complex_t *sorted, *twiddles, *tmp;
	double *ampSpectrum, *phaseSpectrum;
	Array *coefficients = loadFile( "フィルタ係数" )
	    , *input        = loadFile( "入力信号" )
	    , *output
	;

	generateTwiddleFactors( twiddles, coefficients->length );
	sortData( coefficients->a, coefficients->length );
		tmp = coefficients->a;
		coefficients->a = sorted;
		sorted = tmp;
	fft( coefficients->a, twiddles, coefficients->length );

	generateTwiddleFactors( twiddles, input->length );
	sortData( input->a, input->length );
		tmp = input->a;
		input->a = sorted;
		sorted = tmp;
	fft( coefficients->a, twiddles, input->length );




	sortData( xn, xn_sorted, dim );
	tmp = xn;
	xn = xn_sorted;
	xn_sorted = tmp;
	ifft( xn, twiddles, dim ); 
	printComplexToFile( "IFFT.txt", xn, dim );

	return 0;
}





/* ---------------------------------
	FFT 
---------------------------------- */

void fft( complex_t *data, complex_t *twiddles, int fft_points ) {
	int number_of_stage = intlog2( fft_points )
	  , distance = 1
	  , resolution = fft_points/2
	  , upper, lower
	;

	complex_t tmp;

	for ( int stage = 0; stage < number_of_stage; stage++ ) {
		for ( int i = 0; i < distance; i++ ) {
			for ( int j = 0; j < resolution; j++ ) {
				upper = distance * 2 * j + i;
				lower = upper + distance;

				tmp = multiplyComplex( data[ lower ], twiddles[ i * resolution ] );
				data[ lower ] = subtractComplex( data[ upper ], tmp );
				data[ upper ] = addComplex( data[ upper ], tmp );
			}
		}
		distance   *= 2;
		resolution /= 2;
	}
}

void ifft( complex_t *data, complex_t *twiddles, int fft_points ) {
	complex_t n = { fft_points, 0 };
	conjugateComplexes( twiddles, fft_points );
	fft( data, twiddles, fft_points );

	for( int i = 0; i < fft_points; i++ ) {
		data[i] = divideComplex( data[i], n );
	}
}

void sortData( complex_t *data, complex_t *sorted, int fft_points )
{
	int order[fft_points]
	  , number_of_bits = intlog2( fft_points )
	;

	reverseBits( order, fft_points, number_of_bits );
	for ( int i = 0; i < fft_points; i++ ) {
		sorted[i] = data[ order[i] ];
	}
}

void reverseBits( int *bits, int fft_points, int number_of_bits )
{
	int max = 1;
	bits[0] = 0;
	for ( int i = 0; i < number_of_bits; i++ ) {
		for ( int j = 0; j < max; j++ )
			bits[ j + max ] = bits[j] + fft_points / ( 2 * max );
		max *= 2;
	}
	for ( int i = fft_points/2; i < fft_points; i++ ) {
		bits[i] = bits[ i - fft_points/2 ] + 1;
	}
}

void generateTwiddleFactors( complex_t *twiddles, int fft_points )
{
	for ( int i = 0; i < fft_points/2; i++ ) {
		twiddles[i] =  generateTwiddleFactor(  fft_points, i );
	}
	for ( int i = fft_points/2; i < fft_points; i++ ) {
		twiddles[i] =  conjugateComplex( twiddles[ i - fft_points/2 ] );
	}
}

void conjugateComplexes( complex_t *comp, int num )
{
	for ( int i = 0; i < num; i++ ) {
		comp[i] = conjugateComplex( comp[i] );
	}
}
void calcAmplitudeSpectrum( complex_t Xk[], double spectrum[] , int dim ) 
{
	for ( int k = 0; k < dim; k++ ) {
		spectrum[k] = calcRadius( Xk[k] );
	}
}





/* ---------------------------------
	General 
---------------------------------- */

int isCloseToZero( double value )
{
	return value < 0.0001 && value > -0.0001;
}

int isPowerOfTwo( int value )
{
	return !( value & ( value - 1 ) );
}

complex_t *allocateComplexMemory( int num )
{
	return (complex_t *) malloc( num * sizeof( complex_t ) );
}

double *allocateDoubleMemory( int num )
{
	return (double *) malloc( num * sizeof( double ) );
}

int intlog2( int n )
{
	int count = 0;
	while ( n > 1 ) {
		count++;
		n >>= 1;
	}
	return count;
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
		fscanf( fp , "%lf" , &data->a[i].re );
		data->a[i].im = 0;
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

	if( ( d->a = (complex_t*)malloc( sizeof(complex_t) * length ) ) == NULL )
		AllocationError();

	d->length = length;

	return d;
}

void freeArray( Array* d ) {
	free( d->a );
	free( d );
}

void printComplexToFile( const char fn[], complex_t vec[], int dim )
{
	FILE *fp = fopen( fn, "w" );
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for ( int i = 0; i < dim; ++i )
	{
		fprintf( fp, "%.2f", vec[i].re );

		if ( isCloseToZero( vec[i].im ) ){
			fprintf( fp, "\n");
			continue;
		}

		if ( vec[i].im < 0 )
			fprintf( fp, "-j%.2f\n", -vec[i].im );
		else
			fprintf( fp, "+j%.2f\n", vec[i].im );
	}

	fclose( fp );
}

void printDoubleToFile( const char fn[], double vec[], int dim )
{
	FILE *fp = fopen( fn, "w" );
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for ( int i = 0; i < dim; ++i )
	{
		fprintf( fp, "%.2f\n", vec[i] );
	}

	fclose( fp );
}




/* ---------------------------------
	complex calculation 
---------------------------------- */

complex_t addComplex( complex_t a, complex_t b )
{
	complex_t sum;

	sum.re = a.re + b.re;
	sum.im = a.im + b.im;

	return sum;
}

complex_t subtractComplex( complex_t minuend, complex_t subtrahend )
{
	complex_t difference;

	difference.re = minuend.re - subtrahend.re;
	difference.im = minuend.im - subtrahend.im;

	return difference;
}

complex_t multiplyComplex( complex_t a, complex_t b )
{
	complex_t product;

	product.re = a.re * b.re - a.im * b.im;
	product.im = a.im * b.re + a.re * b.im;

	return product;
}

complex_t divideComplex( complex_t dividend, complex_t divisor )
{
	complex_t quotient;

	quotient.re = ( dividend.re * divisor.re + dividend.im * divisor.im ) / ( pow ( divisor.re, 2 ) + pow ( divisor.im, 2 ) );
	quotient.im = ( dividend.im * divisor.re - dividend.re * divisor.im ) / ( pow ( divisor.re, 2 ) + pow ( divisor.im, 2 ) );

	return quotient;
}

complex_t conjugateComplex( complex_t c )
{
	c.im *= -1;
	return c;
}

double calcRadius( complex_t c )
{
	return sqrt( pow( c.re, 2 ) + pow( c.im, 2 ) );
}

double calcAngle( complex_t c )
{
	return atan2( c.im, c.re );
}

polar_t convertRectangularToPolar( complex_t c )
{
	polar_t p;

	p.radius = calcRadius( c );
	p.angle = calcAngle( c );

	return p;
}

complex_t convertPolarToRectangular( polar_t p )
{
	complex_t c;

	c.re = p.radius * ( cos( p.angle ) );
	c.im = p.radius * ( sin( p.angle ) );

	return c;
}

complex_t generateTwiddleFactor( int dimension, int nth )
{
	polar_t p = { 1, -2.0 * M_PI * nth / dimension };
	return convertPolarToRectangular( p );
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
