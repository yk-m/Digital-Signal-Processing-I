//＊＊＊ H27年度・DSP1-3・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FN 1024

int  getNum();
void getData( double data[], int dim );
void loadFile ( char fn[], double vec[], int dim );

double **allocateVectorsMemory( int num, int dim );
double *allocateDoubleArrayMemory( int dim );

void printData( double r[], int dim );
void printDataToFile( double r[], int dim );

void freeVectors( double **vectors, int num );

void calcCorrelationFunction( double r[], double vec1[], double vec2[], int dim );
double calcNthCorrelationFunction( double vec1[], double vec2[], int dim, int n );

int main()
{

	double **data
	     , *r
	;
	int dim
	  , num = 2
	;

	printf(
		"H27年度・DSP1-3（自主課題）・番号33\n"
		"使い方の説明：v0，v1のデータについて，相関関数を求めます．\n"
		"  まず，ベクトルの次元を聞かれますので，一つのデータ系列に含まれるデータ数を入力してください．\n"
		"  例えば，data1・data2は0〜700のデータがあるので，\n"
		"      ベクトルの次元：701\n"
		"  となります．\n"
		"  最後に，v0，v1のデータファイルの（パスを含む）名前を入力してください．\n"
		"必要な結果：\n"
		"  相関関数：\n"
		"    Rxy(0) = 0.025116\n"
		"    Rxy(100) = 0.075794\n"
		"    Rxy(200) = 0.029542\n"
		"    Rxy(300) = 0.007626\n"
		"    Rxy(400) = 0.007195\n"
		"    Rxy(500) = 0.007679\n"
		"    Rxy(600) = 0.001238\n"
		"    Rxy(700) = 0.000000\n"
		"    ファイル出力：\n"
		"      ファイル名：output.txt\n"
		"      内容：Rxy(0)〜Rxy(700)の各値\n"
		"  自己相関関数（v0，v0の相互相関関数）：\n"
		"    Rxy(0) = 0.180281\n"
		"    Rxy(10) = -0.027261\n"
		"    Rxy(20) = -0.056572\n"
		"    Rxy(30) = 0.013220\n"
		"    Rxy(40) = 0.002022\n"
		"    Rxy(50) = -0.012665\n"
		"    Rxy(60) = 0.000095\n"
		"    Rxy(70) = 0.010008\n"
		"    ファイル出力：\n"
		"      ファイル名：output.txt\n"
		"      内容：Rxy(0)〜Rxy(70)の各値\n"
		"\n"
	);

	printf("ベクトルの次元：");
	dim = getNum();

	data = allocateVectorsMemory( num, dim );
	r = allocateDoubleArrayMemory( dim );

	for (int i = 0; i < num; ++i)
	{
		printf("v%dの", i);
		getData( data[i], dim );
	}
	calcCorrelationFunction( r, data[0], data[1], dim );
	printData( r, dim );
	printDataToFile( r, dim );

	freeVectors( data, num );
}

int getNum()
{
	int n;
	scanf( "%d", &n );
	return n;
}

void getData( double data[], int dim )
{
	char fn[FN];

	printf("データファイル名：");
	scanf( "%s", fn );

	loadFile( fn, data, dim );
}

void loadFile ( char fn[], double vec[], int dim )
{
	FILE *fp = fopen( fn, "r" );
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for ( int i = 0; i < dim; ++i )
	{
		fscanf( fp , "%lf" , &vec[i] );
	}

	fclose( fp );
}

double **allocateVectorsMemory( int num, int dim )
{
	double **vectors;
	vectors = ( double ** )malloc( sizeof( double * ) * num );

	for ( int i = 0; i < num; i++ )
	{
		vectors[i] = allocateDoubleArrayMemory( dim );
	}

	return vectors;
}

double *allocateDoubleArrayMemory( int dim )
{
	return ( double* )malloc( sizeof( double ) * dim );
}

void printData( double r[], int dim )
{
	for ( int i = 0; i < dim; i += dim/7 )
	{
		printf("Rxy(%d) = %f\n", i, r[i] );
	}
}

void printDataToFile( double r[], int dim )
{
	FILE *fp = fopen( "output.txt", "w" );
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for ( int i = 0; i < dim; ++i )
	{
		fprintf( fp, "%f\n", r[i] );
	}

	fclose( fp );
}

void freeVectors( double **vectors, int num )
{
	for ( int i = 0; i < num; i++ )
	{
		free( vectors[i] );
	}
	free( vectors );
}

void calcCorrelationFunction( double r[], double vec1[], double vec2[], int dim )
{
	for ( int i = 0; i < dim; ++i )
	{
		r[i] = calcNthCorrelationFunction( vec1, vec2, dim, i );
	}
}

double calcNthCorrelationFunction( double vec1[], double vec2[], int dim, int n )
{
	double sum = 0;

	for ( int i = 0; i + n < dim; ++i )
	{
		sum += vec1[i] * vec2[ i + n ];
	}

	return sum / dim;
}