#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FN 1024

typedef struct
{
	double *vector;
	double *normalized_vector;
	double average;
} data_t;

int  getNum();
void getData( data_t *data, int dim );
void loadFile ( char fn[], double vec[], int dim );
data_t *allocateDataMemory( int num );
double *allocateVectorMemory( int dim );
void freeData( data_t *data, int num );

void printCorrelationCoefficient  ( data_t data[], int num, int dim );

double calcAverage  ( double vector[], int dim );
void normalizeVector( double average, double vec[], double normalizedVec[], int dim );

double calcCorrelationCoefficient ( double vec1[], double vec2[], int dim );
double calcNorm                   ( double vec[], int dim );
double calcDotProduct             ( double vec1[], double vec2[], int dim );

int main()
{

	data_t *data;
	int dim, num;

	printf(
		"H27年度・DSP1-3（自主課題）・番号33\n"
		"使い方の説明：v0〜vnのデータについて，v0を基準とした相関係数を求めます．\n"
		"  まず，ベクトルの本数を聞かれますので，相関を調べたいデータ系列の数を入力してください．\n"
		"  例では，H16男性人口，H16女性人口，H16総人口，S24総人口の4データ系列について相関を求めるので，\n"
		"      ベクトルの本数：4\n"
		"  となります．\n"
		"  次に，ベクトルの次元を聞かれますので，一つのデータ系列に含まれるデータ数を入力してください．\n"
		"  例では，0〜85歳のデータがあるので，\n"
		"      ベクトルの次元：86\n"
		"  となります．\n"
		"  最後に，それぞれのデータファイルの（パスを含む）名前を入力してください．\n"
		"  例では，同じディレクトリにあるとすると\n"
		"    v0のデータファイル名：rdata1\n"
		"    v1のデータファイル名：rdata2\n"
		"    v2のデータファイル名：rdata3\n"
		"    v3のデータファイル名：rdata4\n"
		"  のようになります．\n"
		"必要な結果：\n"
		"  相関係数：\n"
		"    v0・v1 : 0.952\n"
		"    v0・v2 : 0.988\n"
		"    v0・v3 : 0.173\n"
		"  直流成分未除去：\n"
		"    v0・v1 : 0.995\n"
		"    v0・v2 : 0.999\n"
		"    v0・v3 : 0.821\n"
		"\n"
	);

	printf("ベクトルの本数：");
	num = getNum();

	printf("ベクトルの次元：");
	dim = getNum();

	data = allocateDataMemory( num );

	for (int i = 0; i < num; ++i)
	{
		data[i].vector = allocateVectorMemory( dim );
		data[i].normalized_vector = allocateVectorMemory( dim );
	}

	for (int i = 0; i < num; ++i)
	{
		printf("v%dの", i);
		getData( &data[i], dim );
		data[i].average =  calcAverage( data[i].vector, dim );
		normalizeVector( data[i].average, data[i].vector, data[i].normalized_vector, dim );
	}

	printCorrelationCoefficient(data, num, dim);
}

int getNum()
{
	int n;
	scanf( "%d", &n );
	return n;
}

data_t *allocateDataMemory( int num )
{
	data_t *data;
	data = (data_t *) malloc( num * sizeof( data_t ) );

	return data;
}

double *allocateVectorMemory( int dim )
{
	double *vec;
	vec = (double *) malloc( dim * sizeof( double ) );

	return vec;
}

void freeData( data_t *data, int num )
{
	for (int i = 0; i < num; ++i)
	{
		free( data[i].vector );
		free( data[i].normalized_vector );
	}

	free( data );
}

void getData( data_t *data, int dim )
{
	char fn[FN];

	printf("データファイル名：");
	scanf( "%s", fn );

	loadFile( fn, data->vector, dim );
}

void loadFile ( char fn[], double vec[], int dim )
{
	FILE *fp = fopen(fn, "r");
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for (int i = 0; i < dim; ++i)
	{
		fscanf( fp , "%lf" , &vec[i] );
	}
}

void printCorrelationCoefficient( data_t data[], int num, int dim ) {
	printf("相関係数：\n");
	for (int i = 1; i < num; ++i)
	{
		printf("v0・v%d : %0.3f\n", i, calcCorrelationCoefficient( data[0].normalized_vector, data[i].normalized_vector, dim ) );
	}

	printf("\n");

	printf("直流成分未除去：\n");
	for (int i = 1; i < num; ++i)
	{
		printf("v0・v%d : %0.3f\n", i, calcCorrelationCoefficient( data[0].vector, data[i].vector, dim ) );
	}
}

double calcAverage( double vec[], int dim )
{
	double sum = 0;

	for ( int i = 0; i < dim; ++i )
	{
		sum += vec[i];
	}

	return sum / dim;
}

void normalizeVector( double average, double vec[], double normalizedVec[], int dim )
{
	for ( int i = 0; i < dim; ++i )
	{
		normalizedVec[i] = vec[i] - average;
	}
}

double calcCorrelationCoefficient ( double vec1[], double vec2[], int dim )
{
	double norm1 = calcNorm( vec1, dim )
	     , norm2 = calcNorm( vec2, dim )
	;

	return calcDotProduct( vec1, vec2, dim ) / ( norm1 * norm2 );
}

double calcNorm( double vec[], int dim )
{
	return sqrt( calcDotProduct( vec, vec, dim ) );
}

double calcDotProduct( double vec1[], double vec2[], int dim )
{
	double sum = 0;
	for (int d = 0; d < dim; ++d)
	{
		sum += vec1[d] * vec2[d];
	}
	return sum;
}