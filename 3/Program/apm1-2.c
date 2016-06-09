#include <stdio.h>
#include <math.h>

#define DIM 3

void dispMatrix( double vec[][DIM], int col, int row )
{
	for (int r = 0; r < row; ++r)
	{
		for (int c = 0; c < col; ++c)
		{
			printf(" | %4.1f | ", vec[c][r] );

			if ( c == col -1 )
				continue;

			printf("%c", (r == row/2)? ',' : ' ' );
		}
		printf("\n");
	}
	printf("\n");
}

void dispAlpha( double vec1[], double vec2[], int num, double alpha )
{
	printf(
			"                          　 | %4.1f | \n"
		"α(%d) = | %4.1f %4.1f %4.1f | ・ | %4.1f | = %.2f\n"
			"                          　 | %4.1f | \n"
	  , vec2[0]
	  , num, vec1[0], vec1[1], vec1[2], vec2[1], alpha
	  , vec2[2]
	);
	printf("\n");
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

void restoreXn( double vec[][3], double alpha[], double Xn[], int col, int dim ) {
	for (int c = 0; c < col; ++c)
	{
		Xn[c] = 0;

		for (int d = 0; d < dim; ++d)
		{
			Xn[c] += alpha[d] * vec[c][d];
		}
	}
}

int main()
{
	double vec[][DIM] = {
		{ 1,           0,            0 }
	  , { 0, 1/sqrt( 2 ),  1/sqrt( 2 ) }
	  , { 0, 1/sqrt( 2 ), -1/sqrt( 2 ) }
	};
	double Xn[DIM] = {
		2, -1, 1
	};

	double alpha[DIM]
	     , restoredXn[DIM]
	;

	printf("A, B, C\n");
	dispMatrix( vec, 3, DIM );

	printf("A･B = %.0f\n", calcDotProduct( vec[0], vec[1], DIM ) );
	printf("B･C = %.0f\n", calcDotProduct( vec[1], vec[2], DIM ) );
	printf("A･C = %.0f\n", calcDotProduct( vec[0], vec[2], DIM ) );

	for (int i = 0; i < DIM; ++i)
	{
		alpha[i] = calcDotProduct( Xn, vec[i], DIM );
		dispAlpha( Xn, vec[i], i, alpha[i] );
	}

	restoreXn( vec, alpha, restoredXn, 3, DIM );
	printf(
		"     | %2.0f |\n"
		"Xn = | %2.0f |\n"
		"     | %2.0f |\n"
	  , restoredXn[0], restoredXn[1], restoredXn[2]
	);
}
