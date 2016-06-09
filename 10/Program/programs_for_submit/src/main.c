//＊＊＊ H27年度・DSP1-10・番号33 ＊＊＊

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "array.h"
#include "dsp.h"

int main( int argc, char *argv[] ) {
	Array_d *input, *system, *output, *desired_signal, *error, *error_log;

	printf(
		"H27年度・DSP1-10・番号33\n"
		"使い方の説明：音声信号，白色信号，有色信号について，エコーキャンセルします．\n"
		"  ara11_s.txtとw_imp.txtを同じディレクトリに格納しておいてください．\n"
		"必要な結果：\n"
		"    ファイル出力：\n"
		"      1\n"
		"        output.txt\n"
		"        内容：擬似エコー\n"
		"      2\n"
		"        desired.txt\n"
		"        内容：エコー\n"
		"      3\n"
		"        error.txt\n"
		"        内容：誤差信号\n"
		"      4\n"
		"        error_log.txt\n"
		"        内容：誤差信号(db)\n"
		"\n"
		"      1~4について，\n"
		"        音声信号：voise_\n"
		"        白色信号：noise_\n"
		"        白色信号：colored_noise_\n"
		"      がファイル名の先頭につき，それぞれの結果が出力される．"
		"\n"
	);

	input  = loadFileAsArray_d( "ara11_s.txt", false );
	desired_signal = newArray_d( input->length );
	error          = newArray_d( input->length );
	error_log      = newArray_d( input->length );
	system = loadFileAsArray_d( "./w_imp.txt", false );

	output = LMS( input, system, desired_signal, error, error_log );
	printArray_dToFile( output,         "voise_output.txt" );
	printArray_dToFile( desired_signal, "voise_desired.txt" );
	printArray_dToFile( error,          "voise_error.txt" );
	printArray_dToFile( error_log,      "voise_error_log.txt" );

	freeArray_d( input );
	freeArray_d( system );
	freeArray_d( output );
	freeArray_d( desired_signal );
	freeArray_d( error );
	freeArray_d( error_log );



	input = generateGaussSignal( 12163 );
	printArray_dToFile( input, "noise.txt" );
	desired_signal = newArray_d( input->length );
	error          = newArray_d( input->length );
	error_log      = newArray_d( input->length );
	system = loadFileAsArray_d( "./w_imp.txt", false );

	output = LMS( input, system, desired_signal, error, error_log );
	printArray_dToFile( output,         "noise_output.txt" );
	printArray_dToFile( desired_signal, "noise_desired.txt" );
	printArray_dToFile( error,          "noise_error.txt" );
	printArray_dToFile( error_log,      "noise_error_log.txt" );

	freeArray_d( input );
	freeArray_d( system );
	freeArray_d( output );
	freeArray_d( desired_signal );
	freeArray_d( error );
	freeArray_d( error_log );



	input = generateGaussSignal( 12163 );
	color( input, 0.95 );
	printArray_dToFile( input, "colored_noise.txt" );
	desired_signal = newArray_d( input->length );
	error          = newArray_d( input->length );
	error_log      = newArray_d( input->length );
	system = loadFileAsArray_d( "./w_imp.txt", false );

	output = LMS( input, system, desired_signal, error, error_log );
	printArray_dToFile( output,         "colored_noise_output.txt" );
	printArray_dToFile( desired_signal, "colored_noise_desired.txt" );
	printArray_dToFile( error,          "colored_noise_error.txt" );
	printArray_dToFile( error_log,      "colored_noise_error_log.txt" );

	freeArray_d( input );
	freeArray_d( system );
	freeArray_d( output );
	freeArray_d( desired_signal );
	freeArray_d( error );
	freeArray_d( error_log );
}


