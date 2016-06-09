

// unsigned int reverseBits2( unsigned int v, int bit );
// unsigned int reverseBits2( unsigned int num, int number_of_bits ) {
// 	unsigned int reverse_num = 0
// 			   , temp
// 	;

// 	for ( int i = 0; i < number_of_bits; i++ ) {
// 		temp = (num & (1 << i));
// 		if( !temp )
// 			continue;
// 		reverse_num |= (1 << ((number_of_bits - 1) - i));
// 	}

// 	return reverse_num;
// }