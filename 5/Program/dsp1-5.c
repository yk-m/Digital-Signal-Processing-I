//＊＊＊ H27年度・DSP1-5・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FN 1024

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
void loadFile ( char fn[], complex_t vec[], int dim );

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

	printf(
		"H27年度・DSP1-5・番号33\n"
		"使い方の説明：与えられたデータについて，ハミング窓をかけた場合とそうでない場合のDFT，IDFT，振幅スペクトル，位相スペクトルを求めます．\n"
		"  まず，データ数を聞かれますので，入力してください．\n"
		"  例えば，500点DFTをしたい場合は，\n"
		"      ベクトルの次元：500\n"
		"  となります．\n"
		"  最後に，波形のデータファイルの（パスを含む）名前を入力してください．\n"
		"必要な結果：\n"
		"    ファイル出力：\n"
		"      1\n"
		"        ファイル名：DFT.txt\n"
		"        内容：DFT後のデータ\n"
		"      2\n"
		"        ファイル名：AmpSpectrum.txt\n"
		"        内容：DFT後の振幅スペクトル\n"
		"      3\n"
		"        ファイル名：PhaseSpectrum.txt\n"
		"        内容：DFT後の位相スペクトル\n"
		"      4\n"
		"        ファイル名：IDFT.txt\n"
		"        内容：DFT後のデータをIDFTしたもの\n"
		"      5\n"
		"        ファイル名：windowedData.txt\n"
		"        内容：ハミング窓をかけたデータ\n"
		"      6\n"
		"        ファイル名：windowedDFT.txt\n"
		"        内容：ハミング窓をかけたデータをDFTしたもの\n"
		"      7\n"
		"        ファイル名：windowedIDFT.txt\n"
		"        内容：ハミング窓をかけたデータをDFTしたものをIDFTしたデータ\n"
		"      8\n"
		"        ファイル名：windowedAmpSpectrum.txt\n"
		"        内容：ハミング窓をかけたデータをDFTしたものの振幅スペクトル\n"
		"      9\n"
		"        ファイル名：windowedPhaseSpectrum.txt\n"
		"        内容：ハミング窓をかけたデータをDFTしたものの位相スペクトル\n"
		"\n"
	);

	printf("データ数：");
	dim = getNum();
	xn = allocateComplexMemory( dim );
	xn_windowed = allocateComplexMemory( dim );
	Xk = allocateComplexMemory( dim );
	ampSpectrum   = allocateDoubleMemory( dim );
	phaseSpectrum = allocateDoubleMemory( dim );

	getData( xn, dim );
	hammingWindow( xn, xn_windowed, dim );

	calcDiscreteFourierTransform( xn, Xk, dim, DFT );
	calcDiscreteFourierTransform( Xk, xn, dim, IDFT );
	calcAmplitudeSpectrum( Xk, ampSpectrum, dim );
	calcPhaseSpectrum( Xk, phaseSpectrum, dim );

	printComplexToFile( "DFT.txt",  Xk, dim );
	printComplexToFile( "IDFT.txt", xn, dim );
	printDoubleToFile( "AmpSpectrum.txt", ampSpectrum, dim );
	printDoubleToFile( "PhaseSpectrum.txt", phaseSpectrum, dim );

	printComplexToFile( "windowedData.txt",  xn_windowed, dim );

	calcDiscreteFourierTransform( xn_windowed, Xk, dim, DFT );
	calcDiscreteFourierTransform( Xk, xn_windowed, dim, IDFT );
	calcAmplitudeSpectrum( Xk, ampSpectrum, dim );
	calcPhaseSpectrum( Xk, phaseSpectrum, dim );

	printComplexToFile( "windowedDFT.txt",  Xk, dim );
	printComplexToFile( "windowedIDFT.txt", xn, dim );
	printDoubleToFile( "windowedAmpSpectrum.txt", ampSpectrum, dim );
	printDoubleToFile( "windowedPhaseSpectrum.txt", phaseSpectrum, dim );

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

void loadFile ( char fn[], complex_t vec[], int dim )
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