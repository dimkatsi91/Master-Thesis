/* File   : V1_data_retrieval.h
 * Author : dimos katsimardos
 * Created on November 11, 2016, 12:22 AM
 *
 * This header file generates all data and error correction codewords
 * and fills the bit stream which will be used based on mode indicator
 * (numeric-->0001 , alphanumeric-->0010 and byte-->0100) and CCI (how
 * many characters are encoded/ length of encoded characters)
 */
#ifndef V1_DATA_RETRIEVAL_H
#define V1_DATA_RETRIEVAL_H

#include "V1_unmasking.h"


class V1_DATA_RETRIEVAL : public V1_UNMASKING {
public:
	Codeword vD1, vD2, vD3, vD4, vD5, vD6, vD7, vD8, vD9, vD10, vD11, 							   // Error Correction Codewords :: LOW --> 7 || MEDIUM --> 10
	         vD12, vD13, vD14, vD15, vD16, vD17, vD18, vD19, vD20,             					  //  Q --> 13 || HIGH --> 17
	         vD21, vD22, vD23, vD24, vD25, vD26;                              					 //   LOW -> bin2dec_2(v1_ec_level)=1 || MEDIUM -> bin2dec_2(v1_ec_level)=0
	          												                 					//    Q -> bin2dec_2(v1_ec_level)=3 || HIGH -> bin2dec(v1_ec_level)=2
									/* MEMBER FUNCTIONS */
	void get_codeword_down(int v1[V1_SIZE][V1_SIZE], Codeword D, int j, int i_first, int i_last);
	void get_codeword_up(int v1[V1_SIZE][V1_SIZE], Codeword D, int j, int i_first, int i_last);
	void get_D16(int v1[V1_SIZE][V1_SIZE]);
	void get_D19(int v1[V1_SIZE][V1_SIZE]);
	void get_all_codewords(int v1[V1_SIZE][V1_SIZE]);
	void set(int bit_stream[], Codeword C, int start_idx);
	void set_v1_bit_stream();
	void set_v1_MI();
	void set_v1_CCI();
	void v1_data_retrieval_packet(int v1[V1_SIZE][V1_SIZE]);
};


/* ------------------------------------------------------------------------------------------------------------- */
/* First, let's define #2 functions that get bits from 8-bit codewords in QR Version1  Matrix with same pattern */
/*                              Codeword LSBit is in lower position like D1 Codeword                           */
/* ---------------------------------------------------------------------------------------------------------- */
void V1_DATA_RETRIEVAL::get_codeword_down(int v1[V1_SIZE][V1_SIZE], Codeword D, int j, int i_first, int i_last)
{
	int idx=7, tmp=j;
	for(int i=i_first;i<i_last;i++) {
		j=tmp;
		for(int k=0;k<2;k++) {
			D[idx--] = v1[i][j++];
		}
	}
}

/* Codeword LSBit is in Upper Position like D4 Codeword */
void V1_DATA_RETRIEVAL::get_codeword_up(int v1[V1_SIZE][V1_SIZE], Codeword D, int j, int i_first, int i_last)
{
	int idx=0, tmp=j;
	for(int i=i_first;i<i_last;i++) {
		j=tmp;
		for(int k=0;k<2;k++) {
			D[idx++] = v1[i][j--];
		}
	}
}

/* Get Codeword D16 & D19 which are exceptions due to horizontal timing patterns */
void V1_DATA_RETRIEVAL::get_D16(int v1[V1_SIZE][V1_SIZE])
{
	int idx=0, col=11, row=4;
	for(int counter=0;counter<4;counter++) {
		col=11;
		for(int counter2=0;counter2<2;counter2++) {
			vD16[7-idx] = v1[row][col++];
			idx++;
		}
		row += (counter==1) ? 2 : 1;
	}
}

void V1_DATA_RETRIEVAL::get_D19(int v1[V1_SIZE][V1_SIZE])
{
	int idx=0, row=4, col=10;
	for(int c1=0;c1<4;c1++) {
		col=10;
		for(int c2=0;c2<2;c2++) {
			vD19[idx++] = v1[row][col--];
		}
		row += (c1==1) ? 2 : 1;
	}
}

void V1_DATA_RETRIEVAL::get_all_codewords(int v1[V1_SIZE][V1_SIZE])
{
	/* First get all same pattern Codewords */
	get_codeword_down(v1, vD1,  19, 17, 21);
	get_codeword_down(v1, vD2,  19, 13, 17);
	get_codeword_down(v1, vD3,  19,  9, 13);
	get_codeword_down(v1, vD7,  15, 17, 21);
	get_codeword_down(v1, vD8,  15, 13, 17);
	get_codeword_down(v1, vD9,  15,  9, 13);
	get_codeword_down(v1, vD13, 11, 17, 21);
	get_codeword_down(v1, vD14, 11, 13, 17);
	get_codeword_down(v1, vD15, 11,  9, 13);
	get_codeword_down(v1, vD17, 11,  0,  4);
	get_codeword_down(v1, vD23,  7,  9, 13);
	get_codeword_down(v1, vD25,  2,  9, 13);
	/* Second the other pattern style Codewords */
	get_codeword_up(v1, vD4,  18,  9, 13);
	get_codeword_up(v1, vD5,  18, 13, 17);
	get_codeword_up(v1, vD6,  18, 17, 21);
	get_codeword_up(v1, vD10, 14,  9, 13);
	get_codeword_up(v1, vD11, 14, 13, 17);
	get_codeword_up(v1, vD12, 14, 17, 21);
	get_codeword_up(v1, vD18, 10,  0,  4);
	get_codeword_up(v1, vD20, 10,  9, 13);
	get_codeword_up(v1, vD21, 10, 13, 17);
	get_codeword_up(v1, vD22, 10, 17, 21);
	get_codeword_up(v1, vD24,  5,  9, 13);
	get_codeword_up(v1, vD26,  1,  9, 13);
	/* Last get exception Codewords D16 & D19 */
	get_D16(v1);
	get_D19(v1);

}

/* --------------------------------------------------------------------------------------  */
/* Next let's fill my v1_bit_stream[208] with codewords D1...D26 using set() sub-function */
/* ------------------------------------------------------------------------------------- */
void V1_DATA_RETRIEVAL::set(int bit_stream[], Codeword C, int start_idx)
{
	for(int i=0;i<8;i++) {
		bit_stream[start_idx++] = C[i];
	}
}

void V1_DATA_RETRIEVAL::set_v1_bit_stream(void)
{
	/* First Initialize v1_bit_stream[CML] matrix */
	for(int i=0;i<CML;i++) {
		v1_bit_stream[i] = 0;
	}
	/* Now set v1_bit_stream matrix with codeword's bits */
	set(v1_bit_stream,  vD1,    0);
	set(v1_bit_stream,  vD2,    8);
	set(v1_bit_stream,  vD3,   16);
	set(v1_bit_stream,  vD4,   24);
	set(v1_bit_stream,  vD5,   32);
	set(v1_bit_stream,  vD6,   40);
	set(v1_bit_stream,  vD7,   48);
	set(v1_bit_stream,  vD8,   56);
	set(v1_bit_stream,  vD9,   64);
	set(v1_bit_stream, vD10,   72);
	set(v1_bit_stream, vD11,   80);
	set(v1_bit_stream, vD12,   88);
	set(v1_bit_stream, vD13,   96);
	set(v1_bit_stream, vD14,  104);
	set(v1_bit_stream, vD15,  112);
	set(v1_bit_stream, vD16,  120);
	set(v1_bit_stream, vD17,  128);
	set(v1_bit_stream, vD18,  136);
	set(v1_bit_stream, vD19,  144);
	set(v1_bit_stream, vD20,  152);
	set(v1_bit_stream, vD21,  160);
	set(v1_bit_stream, vD22,  168);
	set(v1_bit_stream, vD23,  176);
	set(v1_bit_stream, vD24,  184);
	set(v1_bit_stream, vD25,  192);
	set(v1_bit_stream, vD26,  200);
}

/* ---------------------------------------------------------------------------------------  */
/* Next let's set Character Count Indicator && Mode Indicator for Version 1 QR Code Matrix */
/* -------------------------------------------------------------------------------------- */
void V1_DATA_RETRIEVAL::set_v1_MI(void)
{
	for(int i=0;i<4;i++) {
		v1_MI[i] = v1_bit_stream[i];
	}
}

void V1_DATA_RETRIEVAL::set_v1_CCI(void)
{
	for(int i=4;i<14;i++) {
		v1_CCI[i-4] = v1_bit_stream[i];
	}
}


/* --------------------------------------------------------  */
/* Packet for Version 1 QR Codes Data Retrieval Header File */
/* ------------------------------------------------------- */
void V1_DATA_RETRIEVAL::v1_data_retrieval_packet(int v1[V1_SIZE][V1_SIZE])
{
	/* Call Functions from format string & unmasking header files for start */
	v1_unmask(v1);
	/* Now fill all Data & Error Correction Codewords */
	get_all_codewords(v1);
	/* Now set v1_bit_stream[208] matrix using bits from D1...D26 Codewords */
	set_v1_bit_stream();
	/* Now set CCI and MI for Version 1 QR Code */
	set_v1_CCI();
	set_v1_MI();
	//cout << "Encoding Mode Check :: " << Modes[bin2dec(v1_MI, 4)]; newline;
}
#endif /* v1_data_retrieval.h */
