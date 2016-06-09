void FileOpenError( const char* file_name ) {
	printf("Could not Open \"%s\"\n", file_name );
	exit( -1 );
}

void AllocationError() {
	printf( "Allocation Error\n" );
	exit(1);
}
