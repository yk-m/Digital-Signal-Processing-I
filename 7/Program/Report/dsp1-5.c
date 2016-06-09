//＊＊＊ H27年度・DSP1-5・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define FN 1024
#define DATA_FILE "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/7/data.txt"
#define DATA_DIM 16384

// #define DATA_FILE "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/7/data_2**7.txt"
// #define DATA_DIM 128

#define DFT 1
#define IDFT 0

typedef struct
{
	double re;
	double im;
} complex_t;

complex_t *allocateComplexMemory( int num );
double *allocateDoubleMemory( int num );

int getNum();
void getData( complex_t data[], int dim );
void loadFile ( const char fn[], complex_t vec[], int dim );

void printComplexToFile( char fn[], complex_t vec[], int dim );
void printDoubleToFile( char fn[], double vec[], int dim );
int isEqualZero( double a );

void calcDiscreteFourierTransform( complex_t xn[], complex_t Xk[], int N, int dft );
complex_t calcNthDiscreteFourierTransform( complex_t xn[], int N, int a, int b, int k );
void calcAmplitudeSpectrum( complex_t Xk[], double spectrum[], int dim );
void calcPhaseSpectrum( complex_t Xk[], double spectrum[], int dim );

double round2(double dIn, int nLen);

void hammingWindow( complex_t xn[], complex_t xn_hammed[], int N );

int main()
{
	int dim;
	complex_t *xn, *xn_windowed, *Xk;
	double *ampSpectrum, *phaseSpectrum;
	double dft_time  = 0
	     , idft_time = 0
	;
	clock_t t_start, t_end;

	dim = DATA_DIM;
	xn = allocateComplexMemory( dim );
	xn_windowed = allocateComplexMemory( dim );
	Xk = allocateComplexMemory( dim );
	ampSpectrum   = allocateDoubleMemory( dim );
	phaseSpectrum = allocateDoubleMemory( dim );

	for ( int i = 0; i < 100; i++ ) {
		loadFile( DATA_FILE, xn, dim );

		time( &t_start );
		calcDiscreteFourierTransform( xn, Xk, dim, DFT );
		time( &t_end );
		dft_time += difftime( t_end, t_start );

		printf("%d回目：%f ", i+1, difftime( t_end, t_start ));

		time( &t_start );
		calcDiscreteFourierTransform( Xk, xn, dim, IDFT );
		time( &t_end );
		dft_time += difftime( t_end, t_start );

		printf("%f\n", difftime( t_end, t_start ));
	}

	printf("DFT: %f\n", dft_time/100);
	printf("IDFT: %f\n", idft_time/100);

	free( xn );
	free( xn_windowed );
	free( Xk );
	free( ampSpectrum );
	free( phaseSpectrum );
}

int getNum()
{
	int n;
	scanf( "%d", &n );
	return n;
}

complex_t *allocateComplexMemory( int num )
{
	return (complex_t *) malloc( num * sizeof( complex_t ) );
}

double *allocateDoubleMemory( int num )
{
	return (double *) malloc( num * sizeof( double ) );
}

void getData( complex_t data[], int dim )
{
	char fn[FN];

	printf("データファイル名：");
	scanf( "%s", fn );

	loadFile( fn, data, dim );
}

void loadFile ( const char fn[], complex_t vec[], int dim )
{
	FILE *fp = fopen( fn, "r" );
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for ( int i = 0; i < dim; ++i )
	{
		fscanf( fp , "%lf" , &vec[i].re );
		vec[i].im = 0;
	}

	fclose( fp );
}

void printComplexToFile( char fn[], complex_t vec[], int dim )
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

		if ( isEqualZero( vec[i].im ) ){
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

void printDoubleToFile( char fn[], double vec[], int dim )
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

void calcDiscreteFourierTransform( complex_t xn[], complex_t Xk[], int N, int dft )
{
	double a = ( dft )? 1.0 : -1.0
	     , b = ( dft )? 1.0 : N
	;

	for (int k = 0; k < N; ++k)
	{
		Xk[k] = calcNthDiscreteFourierTransform( xn, N, a, b, k );
	}
}

complex_t calcNthDiscreteFourierTransform( complex_t xn[], int N, int a, int b, int k )
{
	complex_t Xk = { 0, 0 };

	for (int n = 0; n < N; ++n)
	{
		Xk.re += xn[n].re * cos( 2*M_PI/N * n*k ) + a * xn[n].im * sin( 2*M_PI/N * n*k );
		Xk.im += xn[n].im * cos( 2*M_PI/N * n*k ) - a * xn[n].re * sin( 2*M_PI/N * n*k );
	}

	Xk.re /= b;
	Xk.im /= b;

	Xk.re = round2( Xk.re, 3 );
	Xk.im = round2( Xk.im, 3 );

	return Xk;
}

int isEqualZero( double a )
{
	return a < 0.0001 && a > -0.0001;
}

double round2(double a, int n)
{
	int operand = ( a < 0 )? -1 : 1;
	return (int)( a * pow(10.0, n) + operand*0.5 ) / pow(10.0, n);
}

void calcAmplitudeSpectrum( complex_t Xk[], double spectrum[] , int dim ) {
	for (int k = 0; k < dim; ++k)
	{
		spectrum[k] = sqrt( pow( Xk[k].re, 2 ) + pow( Xk[k].im, 2 ) );
	}
}

void calcPhaseSpectrum( complex_t Xk[], double spectrum[], int dim ) {
	for (int k = 0; k < dim; ++k)
	{
		spectrum[k] = atan2( Xk[k].im, Xk[k].re );
	}
}

void hammingWindow( complex_t xn[], complex_t xn_windowed[], int N ) {
	for (int n = 0; n < N; ++n)
	{
		xn_windowed[ n ].re = xn[n].re * ( 0.54 - 0.46*cos( 2*M_PI*n / N ) );
	}
}