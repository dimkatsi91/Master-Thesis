/* File   : format_string.h
 * Author : dimos katsimardos
 * Created on November 7, 2016, 1:55 PM
 *
 * Header with functions for M1 Micro QR Code Format String Manipulation.
 * Each function takes a M1 Micro QR Code Matrix as input and generates
 * binary symbol(3 bits) -- mask used(2 bits) -- and 10 error correction
 * bits (BCH) and puts these bits in the correct matrix for each one.
 */
#ifndef FORMAT_STRING_H
#define FORMAT_STRING_H

#include "M1_definitions.h"

class M1_FORMAT_STRING {
public:
	int m1[M1_SIZE][M1_SIZE];
	int format_string[15];		 // All 15 bits for Format Info String
	int binary_symbol[3];		//  Table 13 -- Binary Symbol (000 always for M1 Micro QR before the masking procedure)
	int mask_info[2];		   //   Mask that was used -- Critical
	int err_corr[10];	      //    The 10-bit error correction  sequence
	/* MEMBER FUNCTIONS */
	void get_binary_symbol(int m1[M1_SIZE][M1_SIZE]);
	void get_mask(int m1[M1_SIZE][M1_SIZE]);
	void get_error_seq(int m1[M1_SIZE][M1_SIZE]);
	void get_format_string(int m1[M1_SIZE][M1_SIZE]);
	void unxor_format_string();
	void set_correct_format_info();
	void format_packet(int m1[M1_SIZE][M1_SIZE]);
};

/* 1st :: The 3 first bits */
void M1_FORMAT_STRING::get_binary_symbol(int m1[M1_SIZE][M1_SIZE])
{
	for(int j=1;j<4;j++) {
		binary_symbol[j-1] = m1[8][j];
	}
}

/* 2nd :: The mask that is used */
void M1_FORMAT_STRING::get_mask(int m1[M1_SIZE][M1_SIZE])
{
	for(int j=4;j<6;j++) {
		mask_info[j-4] = m1[8][j];
	}
}

/* 3rd :: The error correction bits, 10 bits sequence */
void M1_FORMAT_STRING::get_error_seq(int m1[M1_SIZE][M1_SIZE])
{
	for(int j=6;j<9;j++) {
		err_corr[j-6] = m1[8][j];
	}

	int ii=3;
	for(int i=7;i>=1;i--) {
		err_corr[ii] = m1[i][8];
		ii++;
	}
}

/* Fill the format string array with all bits from the three above functions */
void M1_FORMAT_STRING::get_format_string(int m1[M1_SIZE][M1_SIZE])
{
	for(int i=0;i<3;i++) {
		format_string[i] = binary_symbol[i];
	}
	for(int i=3;i<5;i++) {
		format_string[i] = mask_info[i-3];
	}
	for(int i=5;i<15;i++) {
		format_string[i] = err_corr[i-5];
	}
	//cout << "FORMAT STRING CHECK ::";
	//for(int i=0;i<15;i++) 	cout << format_string[i];
	//newline;
}

/* 4th :: Now, I have the 15-bit format string sequence, but it has been applied with XOR
 * mask for M1 MICRO QR CODE, so I must go back to correct form sequence for format string
*/
void M1_FORMAT_STRING::unxor_format_string(void)
{
	const int mask_size = 15;
	int m1_xor_mask[mask_size] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,1};
	int tmp_format_string[mask_size];
	for(int i=0;i<mask_size;i++) {
		tmp_format_string[i] = m1_xor_mask[i]^format_string[i];
	}
	for(int i=0;i<mask_size;i++)	format_string[i]=tmp_format_string[i];
}

/* Last, let's set my binary_symbol[] -- mask_info[] -- err_corr(] arrays with correct bits */
void M1_FORMAT_STRING::set_correct_format_info(void)
{
	for(int i=0;i<3;i++)	binary_symbol[i]=format_string[i];
	int ii=3;
	for(int i=0;i<2;i++) {
		mask_info[i]=format_string[ii];
		ii++;
	} // Now ii=5 :: checked
	for(int i=0;i<10;i++) {
		err_corr[i]=format_string[ii];
		ii++;
	}
}

/* Making my life simpler --> A format string packet */
void M1_FORMAT_STRING::format_packet(int m1[M1_SIZE][M1_SIZE])
{
	get_binary_symbol(m1);
	get_mask(m1);
	get_error_seq(m1);
	get_format_string(m1);
	unxor_format_string();	// This was missing in 1st try for decoding M1 QR Code || debug step #0
	set_correct_format_info();
}
#endif /* format_string.h */
