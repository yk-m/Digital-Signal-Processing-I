#include <array.h>

Array_d* newArray_d( int length ) {
	Array_d *d;

	if( ( d = (Array_d*)malloc( sizeof( Array_d ) ) ) == NULL )
		AllocationError();

	if( ( d->a = (double*)malloc( sizeof(double) * length ) ) == NULL )
		AllocationError();

	d->length = length;

	return d;
}

void freeArray_d( Array_d* d ) {
	free( d->a );
	free( d );
}

Array_d* loadFileAsArray_d( const char file[], bool ask_filename ) {
	FILE *fp;
	Array_d *data;
	char file_name[FN]
	   , line[FN]
	;
	int length;

	if ( ask_filename ) {
		printf( "%s", file );
		askForFilename( file_name );
	}
	else
		strcpy( file_name, file );

	length = countLinesOfFile( file_name );
	data = newArray_d( length );

	fp = openFile( file_name, "r" );
	for ( int i = 0; i < data->length; ++i ) {
		fgets( line, sizeof(line), fp );
		sscanf( line , "%lf" , &data->a[i] );
	}
	fclose( fp );
	return data;
}

void printArray_d( Array_d* a ) {
	printf("length: %d\n", a->length);
	for ( int i = 0; i < a->length; i++ ) {
		printf("%f\n", a->a[i]);
	}
	printf("\n");
}

void printArray_dToFile( Array_d* d, const char file_name[] ) {
	FILE *fp = openFile( file_name, "w" );

	for ( int i = 0; i < d->length; ++i ) {
		fprintf( fp, "%f\n", d->a[i] );
	}

	fclose( fp );
}

Array_i* newArray_i( int length ) {
	Array_i *d;

	if( ( d = (Array_i*)malloc( sizeof( Array_i ) ) ) == NULL )
		AllocationError();

	if( ( d->a = (int*)malloc( sizeof(int) * length ) ) == NULL )
		AllocationError();

	d->length = length;

	return d;
}

void freeArray_i( Array_i* d ) {
	free( d->a );
	free( d );
}

Array_i* loadFileAsArray_i( const char file[], bool ask_filename ) {
	FILE *fp;
	Array_i *data;
	char file_name[FN]
	   , line[FN]
	;
	int length;

	if ( ask_filename ) {
		printf( "%s", file );
		askForFilename( file_name );
	}
	else
		strcpy( file_name, file );

	length = countLinesOfFile( file_name );
	data = newArray_i( length );

	fp = openFile( file_name, "r" );
	for ( int i = 0; i < data->length; ++i ) {
		fgets( line, sizeof(line), fp );
		sscanf( line , "%lf" , &data->a[i] );
	}
	fclose( fp );
	return data;
}

void printArray_i( Array_i* a ) {
	printf("length: %d\n", a->length);
	for ( int i = 0; i < a->length; i++ ) {
		printf("%d\n", a->a[i]);
	}
	printf("\n");
}

void printArray_iToFile( Array_i* d, const char file_name[] ) {
	FILE *fp = openFile( file_name, "w" );

	for ( int i = 0; i < d->length; ++i ) {
		fprintf( fp, "%d\n", d->a[i] );
	}

	fclose( fp );
}