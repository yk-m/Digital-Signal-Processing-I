

void dispPointer( riff_chunk riff, fmt_chunk fmt ) {
	printf(
		"RIFF ChunkID   : %p\n"
		"     ChunkSize : %p\n"
		"     Format    : %p\n"
	  , &riff.id, &riff.size, &riff.format
	);

	printf(
		"FMT  ChunkID       : %p\n"
		"     ChunkSize     : %p\n"
		"     AudioFormat   : %p\n"
		"     NumChannels   : %p\n"
		"     SampleRate    : %p\n"
		"     ByteRate      : %p\n"
		"     BlockAlign    : %p\n"
		"     BitsPerSample : %p\n"
	  , &fmt.id, &fmt.size, &fmt.audio_format, &fmt.num_channels, &fmt.sample_rate, &fmt.byte_rate, &fmt.block_align, &fmt.bits_per_sample
	);
}