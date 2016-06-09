//＊＊＊ H27年度・DSP1-6・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>

#define WAVE_FILE_NAME        "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/6/Data/ara11.wav"
#define SAVE_FILE_NAME        "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/6/Program/amplitudes.txt"
#define OUTPUT_WAVE_FILE_NAME "/Users/yuka/GoogleDrive/School/Subjects/Digital_Signal_Processing_I/6/Program/ara11.wav"

typedef struct{
	char id[4];
	unsigned long size;
	char format[4];
} riff_chunk;

typedef struct{
	char id[4];
	unsigned long  size;
	unsigned short audio_format;
	unsigned short num_channels;
	unsigned long  sample_rate;
	unsigned long  byte_rate;
	unsigned short block_align;
	unsigned short bits_per_sample;
} fmt_chunk;

typedef struct{
	char id[4];
	unsigned long size;
} data_chunk;

typedef struct{
	riff_chunk riff;
	fmt_chunk  fmt;
	data_chunk data;
} wave_file;

int getNum();
void askForFilename( char* name );

void dispWaveHeader( wave_file wave );
void dispCharacters( char* chars, int length );

wave_file generateTextFromWave();
void loadAmplitudeAsText( FILE* wave, FILE* text, long length );
void generateWaveFromText();
void loadAmplitudeAsWave( FILE* wave_fp, FILE* text_fp, long length );
FILE* openFile( const char* file_name, const char* mode );

void loadCharacters( FILE* wave, char* chars, int length );
unsigned long loadLongValue( FILE* wave );
unsigned short loadShortValue( FILE* wave );

void writeCharacters( FILE* wave, const char* chars, int length );
void writeLongValue( FILE* wave, unsigned long d );
void writeShortValue( FILE* wave, unsigned short d );

void FileOpenError( const char* file_name );
void FileReadError( const char* message );
void FileWriteError( const char* value );

int main () {
	int mode;
	wave_file wave;

	printf(
		"H27年度・DSP1-6・番号33\n"
		"使い方の説明：TextをWave（16bit モノラル 11025Hz）に，WaveをTextに変換します．\n"
		"  まず，TextをWaveにするのか，WaveをTextにするのか聞かれるので，答えてください．\n"
		"  次に，インプットファイル名とアウトプットファイル名を入力してください．\n"
		"必要な結果：\n"
		"    WaveをTextにした場合（ara11.wav）\n"
		"        Riff Size      : 79760\n"
		"        NumChannels    : 1\n"
		"        SampleRate     : 11025\n"
		"        BitsPerSample  : 16\n"
		"        Data Size      : 39862\n"
		"        Recording time : 3.615601[s]\n"
		"\n"
	);

	printf(
		"text to wave : 0\n"
		"wave to text : 1\n"
		"? : "
	);
	mode = getNum();

	if ( mode == 0 ) {
		generateWaveFromText();
		return 0;
	}

	dispWaveHeader( generateTextFromWave() );
}

int getNum() {
	int num;
	scanf( "%d", &num );

	if ( num == 0 || num == 1 )
		return num;

	return getNum();
}

void askForFilename( char* name ) {
	printf("ファイル名：");
	scanf( "%s", name );
}

FILE* openFile( const char* file_type, const char* mode ) {
	FILE* file;
	char file_name[1024];

	printf( "%s", file_type );
	askForFilename( file_name );

	if( ( file = fopen( file_name, mode ) ) == NULL ) {
		FileOpenError( file_name );
	}

	return file;
}

void dispWaveHeader( wave_file wave ) {
	printf(
		"Riff Size      : %lu\n"
		"NumChannels    : %u\n"
		"SampleRate     : %lu\n"
		"BitsPerSample  : %u\n"
		"Data Size      : %lu\n"
		"Recording time : %f[s]\n"
	  , wave.riff.size
	  , wave.fmt.num_channels, wave.fmt.sample_rate, wave.fmt.bits_per_sample
	  , wave.data.size, (double)wave.data.size / (double)wave.fmt.sample_rate
	);

	printf("\n");
}

void dispCharacters( char* chars, int length ) {
	for ( int i = 0; i <  length; ++i ) {
		printf( "%c", chars[i] );
	}
	printf("\n");
}

wave_file generateTextFromWave() {
	wave_file wave;
	FILE *wave_fp = openFile( "入力するWAVE", "rb" )
	   , *text_fp = openFile( "出力するテキスト", "w" )
	;

	fseek( wave_fp, 0, SEEK_SET );

	loadCharacters( wave_fp, wave.riff.id, 4 );
	wave.riff.size           = loadLongValue( wave_fp );
	loadCharacters( wave_fp, wave.riff.format, 4 );

	loadCharacters( wave_fp, wave.fmt.id, 4 );
	wave.fmt.size            = loadLongValue( wave_fp );
	wave.fmt.audio_format    = loadShortValue( wave_fp );
	wave.fmt.num_channels    = loadShortValue( wave_fp );
	wave.fmt.sample_rate     = loadLongValue( wave_fp );
	wave.fmt.byte_rate       = loadLongValue( wave_fp );
	wave.fmt.block_align     = loadShortValue( wave_fp );
	wave.fmt.bits_per_sample = loadShortValue( wave_fp );

	loadCharacters( wave_fp, wave.data.id, 4 );
	wave.data.size           = loadLongValue( wave_fp ) / 2;
	loadAmplitudeAsText( wave_fp, text_fp, wave.data.size );

	fclose( wave_fp );
	fclose( text_fp );
	return wave;
}

void loadAmplitudeAsText( FILE* wave, FILE* text, long length ) {
	for ( int i = 0; i < length; i++ ) {
		fprintf( text, "%u\n", loadShortValue( wave ) );
	}
}

void generateWaveFromText() {
	FILE *text_fp = openFile( "入力するテキスト", "r" )
	   , *wave_fp = openFile( "出力するWAVE", "wb" )
	;

	writeCharacters( wave_fp, "RIFF", 4 );
	writeLongValue(  wave_fp, 22086 );
	writeCharacters( wave_fp, "WAVE", 4 );
	writeCharacters( wave_fp, "fmt ", 4 );
	writeLongValue(  wave_fp, 16 );
	writeShortValue( wave_fp, 1 );
	writeShortValue( wave_fp, 1 );
	writeLongValue(  wave_fp, 11025 );
	writeLongValue(  wave_fp, 22050 );
	writeShortValue( wave_fp, 2 );
	writeShortValue( wave_fp, 16 );
	writeCharacters( wave_fp, "data", 4 );
	writeLongValue(  wave_fp, 11025*2 );

	loadAmplitudeAsWave( wave_fp, text_fp, 11025L );

	fclose( wave_fp );
	fclose( text_fp );
}

void loadAmplitudeAsWave( FILE* wave_fp, FILE* text_fp, long length ) {
	unsigned short amplitude;
	for ( int i = 0; i < length; i++ ) {
		fscanf( text_fp, "%hu", &amplitude );
		writeShortValue( wave_fp, amplitude );
	}
}

void loadCharacters( FILE* wave, char* chars, int length ) {
	for ( int i = 0; i < length; i++ ) {
		if ( fread( &chars[i], sizeof( char ), 1, wave ) != 1 )
			FileReadError( "Characters" );
	}
}

unsigned long loadLongValue( FILE* wave ) {
	unsigned char s[4];
	if ( fread( s, 4, 1, wave ) != 1 )
		FileReadError( "Long int" );
	return ( s[0] + 256L * ( s[1] + 256L * ( s[2] + 256L * s[3] ) ) );
}

unsigned short loadShortValue( FILE* wave ) {
	unsigned char s[2];
	if ( fread( s, 2, 1, wave ) != 1 )
		FileReadError( "Short int" );
	return ( s[0] + 256U * s[1] );
}

void writeCharacters( FILE* wave, const char* chars, int length ) {
	for ( int i = 0; i < length; i++ ) {
		if ( fwrite( &chars[i], sizeof( char ), 1, wave ) != 1 )
			FileWriteError( "Characters" );
	}
}

void writeLongValue( FILE* wave, unsigned long d ) {
	if ( fwrite( &d, 4, 1, wave ) != 1 )
		FileWriteError( "Long int" );
}

void writeShortValue( FILE* wave, unsigned short d ) {
	if ( fwrite( &d, 2, 1, wave ) != 1 )
		FileWriteError( "Short int" );
}


void FileOpenError( const char* file_name ) {
	printf("Could not Open \"%s\"\n", file_name );
	exit( -1 );
}

void FileReadError( const char* value ) {
	printf( "Could not Read %s\n", value );
	exit( -1 );
}

void FileWriteError( const char* value ) {
	printf( "Could not Write %s\n", value );
	exit( -1 );
}