/*H27年度・DSP1-5・番号01*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define dft  0
#define idft 1
#define	normal 0
#define window 1

typedef struct 
{
	double re;
	double im;
} complex_t;

double DFT( int num, int p, complex_t xn[], complex_t Xk[] ){

	int n;
	int k;
	double a, b;

	if( p == 0 ){
		a = 1.0;
		b = 1.0;
	} else {
		a = -1.0;
		b = num;
	}
	for( k = 0; k < num; k++ ){

		Xk[k].re=0;
		Xk[k].im=0;

		for( n = 0; n < num; n++ ){
			Xk[k].re += xn[n].re * cos(2 * M_PI / num * k * n) + a * xn[n].im * sin(2 * M_PI / num * k * n );
			Xk[k].im += xn[n].im * cos(2 * M_PI / num * k * n) - a * xn[n].re * sin(2 * M_PI / num * k * n );
		}
		Xk[k].re /= b;
		Xk[k].im /= b;
	}
}
double amp_calc( int num, complex_t Xk[], double X[] ){

	int i;

	for( i = 0; i < num; i++ ){
		X[i] = sqrt( Xk[i].re * Xk[i].re + Xk[i].im * Xk[i].im );
	}
}
double phase_calc( int num, complex_t Xk[], double arg[] ){
	
	int i;

	for( i = 0; i < num; i++ ){

		arg[i] = atan2( Xk[i].im , Xk[i].re );

		if( Xk[i].im <= 0.0001 && Xk[i].im >= -0.0001 ){
			arg[i] = 0;
		}
	}
}
void hamming( int num, complex_t xn[] ){

	int i;
	FILE *fp;

	for( i = 0; i < num; i++){
		xn[i].re *= (0.54 - 0.46 * cos(2 * M_PI * i / num));
	}
	fp = fopen( "window.txt", "w" );
	for( i = 0; i < num; i++){
		fprintf( fp, "%5.2lf\n", xn[i].re);
	}
	fclose( fp );

}

void amp_and_phase_export( char fn[], double X[], double arg[], int num ){

	int i;
	FILE *fp = fopen( fn, "w" );


	fprintf( fp, " 振幅　　位相\n");
	for( i = 0; i < num; i++){
		fprintf( fp, "%5.2lf %5.2lf\n", X[i], arg[i] );
	}
	fclose( fp );

}
void DFT_export( char fn[], complex_t Xk[], int num ){

	int i;

	FILE *fp = fopen( fn, "w" );

	for( i = 0; i < num; i++){
		fprintf( fp, "%5.2lf+j(%5.2lf)\n", Xk[i].re, Xk[i].im );
	}
	fclose( fp );

}

void get_file( complex_t xn[], int num ){
	
	FILE *fp;
	char file1[1024];
	int i;

	printf( "ファイル名：" );
	scanf( "%s" ,file1 );
	fp = fopen( file1, "r" );

	for( i = 0; i < num; i++ ){
		fscanf( fp, "%lf", &xn[i].re );
		xn[i].im = 0;
	}
	fclose( fp );
}

void explanation(){

	printf("\nH27年度・DSP1-5・番号01\n");
	printf("\n使い方の説明\n");
	printf("出力するもの：\n"
			"DFTの結果（DFT1.txt）\n"
			"振幅スペクトル・位相スペクトル（amp_and_phase1.txt）\n"
			"IDFTの結果（IDFT1.txt）\n"
			"窓関数実行後のサンプル（window.txt）\n"
			"窓関数実行後のDFTの結果（DFT2.txt）\n"
			"窓関数実行後の振幅スペクトル・位相スペクトル（amp_and_phase2.txt）\n"
			"窓関数実行後のIDFTの結果（IDFT2.txt）\n");	
}
void execute( int w, int num, complex_t xn[], complex_t Xk[], double X[], double arg[] ){

	if( w == 0 ){
		DFT( num, dft, xn, Xk );
		DFT_export( "DFT1.txt", Xk, num );

		amp_calc( num, Xk, X );
		phase_calc( num, Xk, arg );
		amp_and_phase_export("amp_and_phase1.txt", X, arg, num);

		DFT( num, idft, Xk, xn );
		DFT_export( "IDFT1.txt", xn, num );
	}
	if( w == 1 ){
		DFT( num, dft, xn, Xk );
		DFT_export( "DFT2.txt", Xk, num );

		amp_calc( num, Xk, X );
		phase_calc( num, Xk, arg );
		amp_and_phase_export("amp_and_phase2.txt", X, arg, num);

		DFT( num, idft, Xk, xn );
		DFT_export( "IDFT2.txt", xn, num );
	}
}
int main(void)
{
	complex_t xn[501];
	complex_t Xk[501];
	double X[501],arg[501];
	int num = 0;
	
	explanation();

	printf( "\nデータ数：" );
	scanf( "%d" , &num );

	get_file( xn, num );

	execute( normal, num, xn, Xk, X, arg );
	
	hamming( num, xn );
	execute( window, num, xn, Xk, X, arg );

	return 0;
}