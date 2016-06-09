#include "dsp.h"

Array_d* filter( Array_d* input, Array_d *filter ) {
	Array_d* output = newArray_d( input->length + filter->length - 1 );

	for( int n = 0; n < output->length; n++ ) {
		output->a[n] = nthFilter( n, input, filter );
	}

	return output;
}

double nthFilter( int n, Array_d* input, Array_d* filter ) {
	double sum = 0;
	int i;

	for(  i = 0; i < filter->length && i <= n; i++ ) {
		sum += input->a[ n - i ] * filter->a[i];
	}

	return sum;
}

Array_d* generateGaussSignal( int length ) {
	Array_d *signal = newArray_d( length );

	for ( int i = 0; i < signal->length; ++i) {
		signal->a[i] = powl( 2, 9L ) * gauss();
	}
	return signal;
}

double gauss( void ) {
	double x1, x2
	     , standard_deviation = 1.0
	;

	x1 = (double)rand() / (RAND_MAX);
	while ( x1 == 0.0 ) {
		x1 = (double)rand() / (RAND_MAX);
	}
	x2 = (double)rand() / (RAND_MAX);

	return sqrt( -2.0 * log(x1) ) * cos( 2.0 * PI * x2 ) * sqrt( standard_deviation );
}

void color( Array_d *noise, double coefficient ) {
	for ( int i = 1; i < noise->length; i++ ) {
		noise->a[i] += coefficient * noise->a[i-1];
	}
}

double gain( int n, int length, Array_d *a ) {
	const int alpha = 1;
	const double gamma = 1e-7;
	return alpha / ( dot( n, length, a, a ) + gamma );
}

double dot( int n, int length, Array_d *a, Array_d *b ) {
	double sum = 0;
	for ( int i = 0; i < length && i <= n; ++i ) {
		sum += a->a[n-i] * b->a[n-i];
	}
	return sum;
}

void initFilter( Array_d *filter ) {
	for ( int i = 0; i < filter->length; ++i ) {
		filter->a[i] = 0;
	}
}

Array_d* LMS( Array_d* input, Array_d* unknown_system
            , Array_d* desired_signal, Array_d* error, Array_d* error_log ) {
	int filter_length = unknown_system->length
	  , data_length   = input->length
	;
	double myu;

	Array_d *system         = newArray_d( filter_length )
	      , *output_signal  = newArray_d( data_length )
	;

	initFilter( system );

	for( int t = 0; t < data_length; t++ ) {
		desired_signal->a[t] = nthFilter( t, input, unknown_system );
		output_signal->a[t]  = nthFilter( t, input, system );
		error->a[t] = desired_signal->a[t] - output_signal->a[t];

		myu = gain( t, filter_length, input );
		for ( int i = 0; i < filter_length && i <= t; i++ ) {
			system->a[i] = system->a[i] + myu * error->a[t] * input->a[ t - i ];
		}

		error_log->a[t] = 10*log10( error->a[t] * error->a[t] );
	}

	freeArray_d( system );

	return output_signal;
}