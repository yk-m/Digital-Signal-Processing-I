#include "ex2.h"

double generateCosWave( int n, Wave_parameter p ) {
	return p.amplitude * cos( 2 * PI * p.frequency * n/SAMPLING_F + p.phase );
}

double generateSinWave( int n, Wave_parameter p ) {
	p.phase -= 2/PI;
	return generateCosWave( n, p );
}

Array_d* modulateUsingASK( Array_i* input, int bit_rate, Wave_parameter p ) {
	Array_d* output = newArray_d( SAMPLING_F/bit_rate * input->length );

	for( int i = 0, n = 0; i < input->length; i++ ) {
		for( int j = 0; j < SAMPLING_F/bit_rate; j++, n++ ) {
			output->a[n] = input->a[i] * generateCosWave( n, p );
		}
	}
	return output;
}

Array_d* modulateUsingFSK( Array_i* input, int bit_rate, Wave_parameter p1, Wave_parameter p2 ) {
	Array_d* output = newArray_d( SAMPLING_F/bit_rate * input->length );
	Wave_parameter parameter;
	for( int i = 0, n = 0; i < input->length; i++ ) {
		parameter = (input->a[i] == 0 ) ? p1 : p2;
		for( int j = 0; j < SAMPLING_F/bit_rate; j++, n++ ) {
			output->a[n] = generateCosWave( n, parameter );
		}
	}
	return output;
}

Array_d* modulateUsingBPSK( Array_i* input, int bit_rate, Wave_parameter p ) {
	Array_d* output = newArray_d( SAMPLING_F/bit_rate * input->length );
	double phase = p.phase;
	for( int i = 0, n = 0; i < input->length; i++ ) {
		for( int j = 0; j < SAMPLING_F/bit_rate; j++, n++ ) {
			p.phase = ( input->a[i] == 0 ) ? phase : phase + PI;
			output->a[n] = generateCosWave( n, p );
		}
	}
	return output;
}

Array_d* modulateUsingQPSK( Array_i* input, int bit_rate, Wave_parameter p ) {
	Array_d* output = newArray_d( SAMPLING_F/bit_rate * input->length );

	Wave_parameter p2 = p;
	p2.phase -= PI/2;
	for( int i = 0, n = 0; i < input->length; i+=2 ) {
		for( int j = 0; j < SAMPLING_F/bit_rate; j++, n++ ) {
			output->a[n]  = input->a[i]   * generateCosWave( n, p );
			output->a[n] += input->a[i+1] * generateSinWave( n, p2 );
		}
	}
	return output;
}

Array_d* demodulate( Array_d* input, Wave_parameter p ) {
	Array_d* output = newArray_d( input->length );
	for ( int i = 0; i < input->length; ++i ) {
		output->a[i] = input->a[i] * generateCosWave( i, p );
	}
	return output;
}

Array_d* LPF( Array_d* input, double lp_factor ) {
	Array_d* output = newArray_d( input->length );
	output->a[0] = input->a[0];
	for ( int t = 1; t < input->length; ++t ) {
		output->a[t] = input->a[t] + ( output->a[t-1] - input->a[t] ) * lp_factor;
	}
	return output;
}

void printWaveToFile( Array_d* d, const char file_name[] ) {
	FILE *fp = openFile( file_name, "w" );

	for ( int n = 0; n < d->length; ++n ) {
		fprintf( fp, "%f %f\n", n/(double)SAMPLING_F, d->a[n] );
	}

	fclose( fp );
}

void print2WaveToFile( Array_d* d1, Array_d* d2, const char file_name[] ) {
	FILE *fp = openFile( file_name, "w" );

	for ( int n = 0; n < d1->length; ++n ) {
		fprintf( fp, "%f %f %f\n", n/(double)SAMPLING_F, d1->a[n], d2->a[n] );
	}

	fclose( fp );
}
