#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FN 1024

typedef struct
{
	double *vector;
	int first;
	int length;
} data_t;

int  getNum();
void getData( data_t *data, int dim );
void loadFile ( char fn[], double vec[], int dim );
data_t *allocateDataMemory( int num );
double *allocateVectorMemory( int dim );
void freeData( data_t *data, int num );

int main()
{

	data_t data[2];

	int dim
	  , num = 2
	;

	printf("ベクトルの次元：");
	dim = getNum();

	for (int i = 0; i < num; ++i)
	{
		data[i].vector = allocateVectorMemory( dim );

		printf("v%dの", i);
		getData( &data[i], dim );

	}
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
	}

	free( data );
}

void getData( data_t *data, int dim )
{
	char fn[FN];

	printf("データファイル名：");
	scanf( "%s", fn );

	loadFile( fn, data->vector, dim );
	locateStart( data->vector );
	calcLength ( data->vector );
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

int locateStart( double vector[] )
{
	int i;
	while ( vector[i] != 0 ) i++;
	return i;
}

int calcLength( double vector[], int dim, int first )
{
	int i = dim;
	while ( vector[i] != 0 ) i--;
	return start - i;
}