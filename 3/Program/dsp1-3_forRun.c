//＊＊＊ H27年度・DSP1-3・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 86
#define FN 1024

typedef struct
{
	double population[MAX];
	double normalized_population[MAX];
	double average;
} data_t;

void loadFile       ( char fn[], double population[] );
double calcAverage  ( double population[] );
void normalizeVector( double average, double vec[], double normalizedVec[] );

double calcCorrelationCoefficient ( data_t data1, data_t data2, int normalized );
double calcNorm                   ( double vec[] );
double calcDotProduct             ( double vec1[], double vec2[], int dim );

int main()
{
	char file_names[4][1024] = {
		"/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/3/Source/rdata1.txt"
	  , "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/3/Source/rdata2.txt"
	  , "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/3/Source/rdata3.txt"
	  , "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/3/Source/rdata4.txt"
	};

	data_t data[4];

	printf(
		"H27年度・DSP1-3・番号33\n"
		"使い方の説明：H16男性人口を基準とし，相関係数を求めます．データファイルの名前はそれぞれ\n"
		"    H16男性人口：rdata1\n"
		"    H16女性人口：rdata2\n"
		"    H16総人口：rdata3\n"
		"    S24総人口：rdata4\n"
		"  としてください．\n"
		"必要な結果：\n"
		"  相関係数結果：\n"
		"    H16男性・女性   =0.952\n"
		"    H16男性・総人口 =0.988\n"
		"    H16男性・S24    =0.173\n"
		"  直流成分未除去の場合：\n"
		"    H16男性・女性   =0.995\n"
		"    H16男性・総人口 =0.999\n"
		"    H16男性・S24    =0.821\n"
		"\n"
	);

	for (int i = 0; i < 4; ++i)
	{
		loadFile( file_names[i], data[i].population );
		data[i].average =  calcAverage( data[i].population );

		normalizeVector( data[i].average, data[i].population, data[i].normalized_population );
	}

	printf("実行結果：\n");
	printf("  相関関数結果：\n");
	printf("    H16男性・女性   =%.3f\n", calcCorrelationCoefficient( data[0], data[1], 1 ) );
	printf("    H16男性・総人口 =%.3f\n", calcCorrelationCoefficient( data[0], data[2], 1 ) );
	printf("    H16男性・S24    =%.3f\n", calcCorrelationCoefficient( data[0], data[3], 1 ) );
	printf("\n");
	printf("  直流成分未除去の場合：\n");
	printf("    H16男性・女性   =%.3f\n", calcCorrelationCoefficient( data[0], data[1], 0 ) );
	printf("    H16男性・総人口 =%.3f\n", calcCorrelationCoefficient( data[0], data[2], 0 ) );
	printf("    H16男性・S24    =%.3f\n", calcCorrelationCoefficient( data[0], data[3], 0 ) );

	printf("\n");
}

void loadFile ( char fn[], double population[] )
{
	FILE *fp = fopen(fn, "r");
	if ( fp == NULL )
	{
		printf("can't open a file\n");
		exit(1);
	}

	for (int i = 0; i < MAX; ++i)
	{
		fscanf( fp , "%lf" , &population[i] );
	}
}

double calcAverage( double population[] )
{
	double sum = 0;

	for ( int i = 0; i < MAX; ++i )
	{
		sum += population[i];
	}

	return sum / MAX;
}

void normalizeVector( double average, double vec[], double normalizedVec[] )
{
	for ( int i = 0; i < MAX; ++i )
	{
		normalizedVec[i] = vec[i] - average;
	}
}

double calcCorrelationCoefficient ( data_t data1, data_t data2, int normalized )
{
	double *pop1 = normalized ? data1.normalized_population : data1.population
	     , *pop2 = normalized ? data2.normalized_population : data2.population
	     , norm1 = calcNorm( pop1 )
	     , norm2 = calcNorm( pop2 )
	;

	return calcDotProduct( pop1, pop2, MAX ) / ( norm1 * norm2 );
}

double calcNorm( double vec[] )
{
	// double sum_square = 0;

	// for (int i = 0; i < MAX; ++i)
	// {
	// 	sum_square += vec[i] * vec[i];
	// }
	// return sqrt( sum_square );
	return sqrt( calcDotProduct( vec, vec, MAX ) );
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