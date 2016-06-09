#include <stdio.h>
#include <math.h>



double round2(double a, int n)
{
	return (int)( a * pow(10.0, n) + 0.5 ) / pow(10.0, n);
}

int main(int argc, char const *argv[])
{
	printf("%f\n", atan2( 0.00001, 0 ));
	printf("%f\n", atan2( -8.000, 0 ));

	printf("%f\n", round2( 0.7990, 2 ));

	return 0;
}
