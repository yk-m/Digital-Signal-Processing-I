
// void loadChunkID( FILE* wave, riff_chunk riff );
// void loadChunkSize( FILE* wave, riff_chunk riff );
// void loadNumChannels( FILE* wave, fmt_chunk fmt );
// void loadSampleRate( FILE* wave, fmt_chunk fmt );
// void loadBitsPerSample( FILE* wave, fmt_chunk fmt );




// void loadWave( riff_chunk riff, fmt_chunk fmt ) {
// 	FILE* wave = openBinaryFile();

// 	fseek( wave, 0, SEEK_SET );
// 	fread( &riff, sizeof( riff ), 1, wave );
// 	fread( &fmt,  sizeof( fmt ),  1, wave );

// 	dispWaveHeader( riff, fmt );

// 	// loadChunkID( wave, riff );
// 	// loadChunkSize( wave, riff );
// 	// loadNumChannels( wave, fmt );
// 	// loadSampleRate( wave, fmt );
// 	// loadBitsPerSample( wave, fmt );
// }

// void loadChunkID( FILE* wave, riff_chunk riff ) {
// 	fseek( wave, 0, SEEK_SET );
// 	fread( riff.id, sizeof( char ), 4, wave );
// }

// void loadChunkSize( FILE* wave, riff_chunk riff ) {
// 	char size[4];
// 	fseek( wave, 4, SEEK_SET );
// 	fread( size, sizeof( char ), 4, wave );

// 	riff.size = atol( size );
// }

// void loadNumChannels( FILE* wave, fmt_chunk fmt ) {
// 	char num[2];
// 	fseek( wave, 22, SEEK_SET );
// 	fread( num, sizeof( char ), 2, wave );

// 	fmt.size = atol( num );
// }

// void loadSampleRate( FILE* wave, fmt_chunk fmt ) {
// 	char rate[4];
// 	fseek( wave, 28, SEEK_SET );
// 	fread( rate, sizeof( char ), 4, wave );

// 	fmt.sample_rate = atol( rate );
// }

// void loadBitsPerSample( FILE* wave, fmt_chunk fmt ) {
// 	char bit[2];
// 	fseek( wave, 34, SEEK_SET );
// 	fread( bit, sizeof( char ), 2, wave );

// 	fmt.bits_per_sample = atol( bit );
// }