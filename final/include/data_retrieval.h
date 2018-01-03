/* File   : data_retrieval.h
 * Author : dimos katsimardos
 *  Created November 7, 2016, 5:27 PM
 *
 * Header file for M1 Micro QR Code Decoding Procedure from filled matrix.
 * Here exist the functions for data retreival from unmasked
 * m1 matrix for D1 -- D2 -- D3 -- E1 -- E2 data codewords and
 * error correction codewords respectively.
 */
#ifndef DATA_RETRIEVAL_H
#define DATA_RETRIEVAL_H

#include "unmasking.h"
#include "M1_lfsr.h"								     // With this header file I calculate E1 && E2

class M1_DECODE : public M1_UNMASKING{
public:
	int D1[8], D2[8], D3[4], E1[8], E2[8];	                //   Data and error correction codewords
	int CCI_bin[3], bit_stream[maximum];				   //    Character Count Indicator = 3bits long
	                                                      //     Max numbers = 5

	int GLOBAL_CHECK;    							    //       Global Check variable to check my whole Decoding Algorithm | Initializing to False
	/* MEMBER FUNCTIONS */
	void get_D1(int m1[M1_SIZE][M1_SIZE]);
	void get_D2(int m1[M1_SIZE][M1_SIZE]);
	void get_D3(int m1[M1_SIZE][M1_SIZE]);
	void get_E1(int m1[M1_SIZE][M1_SIZE]);
	void get_E2(int m1[M1_SIZE][M1_SIZE]);
	void get_all_codewords(int m1[M1_SIZE][M1_SIZE]);
	void print_codewords();
	void print_decoded_nums(int param, int num1, int num2);
	void create_bit_stream();
	void set_CCI();
	void cut_numbers();
	void check_diman_decode();
	void m1_decode(int m1[M1_SIZE][M1_SIZE]);
};


void M1_DECODE::get_D1(int m1[M1_SIZE][M1_SIZE])
{
	int j=9, idx=0;
	for(int i=7;i<M1_SIZE;i++) {
		j=9;	// 9 again to control the loop sequence ::
		for(int k=0;k<2;k++) {
			D1[7-idx] = m1[i][j];		// index to fill my D1 codeword correctly
			j++;
			idx++;
		}
	}
}

void M1_DECODE::get_D2(int m1[M1_SIZE][M1_SIZE])
{
	int j=9, idx=0;
	for(int i=3;i<7;i++) {
		j=9;
		for(int k=0;k<2;k++) {
			D2[7-idx] = m1[i][j];
			j++;
			idx++;
		}
	}
}

void M1_DECODE::get_D3(int m1[M1_SIZE][M1_SIZE])
{
	int j=9, idx=0;
	for(int i=1;i<3;i++) {
		j=9;
		for(int k=0;k<2;k++) {
			D3[3-idx] = m1[i][j];
			j++;
			idx++;
		}
	}
}

void M1_DECODE::get_E1(int m1[M1_SIZE][M1_SIZE])
{
	int j=8, idx=0;
	for(int i=9;i<M1_SIZE;i++) {
		j=8;
		for(int k=0;k<2;k++) {
			E1[idx] = m1[i][j];
			idx++;
			j--;
		}
	}

	for(int i=10;i>=9;i--) {
		j=6;
		for(int k=0;k<2;k++) {
			E1[idx] = m1[i][j];
			idx++;
			j--;
		}
	}
}

void M1_DECODE::get_E2(int m1[M1_SIZE][M1_SIZE])
{
	int j=4, idx=0;
	for(int i=9;i<M1_SIZE;i++) {
		j=4;
		for(int k=0;k<2;k++) {
			E2[idx] = m1[i][j];
			idx++;
			j--;
		}
	}

	for(int i=10;i>=9;i--) {
		j=2;
		for(int k=0;k<2;k++) {
			E2[idx] = m1[i][j];
			idx++;
			j--;
		}
	}
}

void M1_DECODE::get_all_codewords(int m1[M1_SIZE][M1_SIZE])
{
	get_D1(m1);
	get_D2(m1);
	get_D3(m1);
	get_E1(m1);
	get_E2(m1);
}

void M1_DECODE::print_codewords(void)
{
	cout << "D1 = "; tipose(D1); newline;
	cout << "D1 = " << bin2dec(D1, 8); newline;
	cout << "D2 = "; tipose(D2); newline;
	cout << "D2 = " << bin2dec(D2, 8); newline;
	cout << "D3 = "; tipose2(D3); newline;
	cout << "D3 = " << bin2dec(D3, 8); newline;
	cout << "E1 = "; tipose(E1); newline;
	cout << "E1 = " << bin2dec(E1, 8); newline;
	cout << "E2 = "; tipose(E2); newline;
	cout << "E2 = " << bin2dec(E2, 8); newline;
}

void M1_DECODE::print_decoded_nums(int param, int num1, int num2)
{
	switch(param)
	{
		// 1 encoded number ::
		case 1:
			cout << "Encoded number is: " << num1; newline;
			break;
		// 2 encoded numbers ::
		case 2:
			if(num1<10) {
				cout << "Encoded numbers are: " << num2 << num1; newline;
			} else {
				cout << "Encoded numbers are: " << num1; newline;
			}
			break;
		// 3 encoded numbers ::
		case 3:
			if(num1<10) {
				cout << "Encoded numbers are: " << num2 << num2 << num1; newline;
			} else if(num1>10 && num1<100) {
				cout << "Encoded numbers are: " << num2 << num1; newline;
			} else {
				cout << "Encoded numbers are: " << num1; newline;
			}
			break;
		// 4 encoded numbers ::
		case 4:
			if(num1<10) {
				cout << "Encoded numbers are: 00" << num1 << num2; newline;
			} else if(num1>10 && num1<100) {
				cout << "Encoded numbers are: 0" << num1 << num2; newline;
			} else {
				cout << "Encoded numbers are: " << num1 << num2; newline;
			}
			break;
		// 5 encoded numbers ::
		case 5:
			if(num1<10) {
				if(num2<10) {
					cout << "Encoded numbers are: 00" << num1 << "0" << num2; newline;
				} else {
					cout << "Encoded numbers are: 00" << num1 << num2; newline;
				}
			}
			else if(num1>10 && num1<100) {
				if(num2<10) {
					cout << "Encoded numbers are: 0" << num1 << "0" << num2; newline;
				} else {
					cout << "Encoded numbers are: 0" << num1 << num2; newline;
				}
			}
			else {
				if(num2<10) {
					cout << "Encoded numbers are: " << num1 << "0" << num2; newline;
				} else {
					cout << "Encoded numbers are: " << num1 << num2; newline;
				}
			}
			break;
		default:
			break;
	}
}


void M1_DECODE::create_bit_stream(void)
{
	// Initialization of bit stream array
	for(int i=0;i<maximum;i++)	bit_stream[i]=0;
	int idx=0;
	for(int i=0;i<8;i++) {
		bit_stream[idx] = D1[i];
		idx++;
	}
	for(int i=0;i<8;i++) {
		bit_stream[idx] = D2[i];
		idx++;
	}
	for(int i=0;i<4;i++) {
		bit_stream[idx] = D3[i];
		idx++;
	}
	for(int i=0;i<8;i++) {
		bit_stream[idx] = E1[i];
		idx++;
	}
	for(int i=0;i<8;i++) {
		bit_stream[idx] = E2[i];
		idx++;
	}
	cout << "Bit Stream is:\n|   D1     |    D2    |  D3  |    E1    |    E2    |\n| ";
	for(int i=0;i<maximum;i++) {
		cout << bit_stream[i];
		cout << ( (i==7 || i==15 || i==19 || i==27 || i==maximum-1) ? " | " : "" );
	}
	newline;
}

void M1_DECODE::set_CCI(void)
{
	for(int i=0;i<3;i++)	CCI_bin[i] =  bit_stream[i];
	//cout << "CCI = ";
	//for(int i=0;i<3;i++) { cout << CCI_bin[i]; }
	//newline;
}

void M1_DECODE::cut_numbers(void)
{
	int num1=0, num2=0;
	int tmp1[10], tmp2[7], idx=0, input[3];	         // for case 5
	int tmp3[4]; 				            //  for case 4
	int CCI_dec = bin2dec(CCI_bin, 3);	   //   convert my binary number to decimal for use
	switch(CCI_dec)
	{
		// When I have encoded #5 numbers: CCI = 101 = 5
		case 5:
			for(int i=3;i<13;i++) {
				tmp1[idx] = bit_stream[i];
				idx++;
			}
			num1 = bin2dec(tmp1, 10);
			idx=0;
			for(int i=13;i<20;i++) {
				tmp2[idx] = bit_stream[i];
				idx++;
			}
			num2 = bin2dec(tmp2, 7);
			print_decoded_nums(CCI_dec, num1, num2);
			break;
		// When I have #4 encoded numbers: CCI = 100 = 4
		case 4:
			for(int i=3;i<13;i++) {
				tmp1[idx] = bit_stream[i];
				idx++;
			}
			num1 = bin2dec(tmp1, 10);
			idx=0;
			for(int i=13;i<17;i++) {
				tmp3[idx] = bit_stream[i];
				idx++;
			}
			num2 = bin2dec(tmp3, 4);
			print_decoded_nums(CCI_dec, num1, num2);
			break;
		// When I have #3 encoded numbers: CCI = 011 = 3
		case 3:
			for(int i=3;i<13;i++) {
				tmp1[idx] = bit_stream[i];
				idx++;
			}
			num1 = bin2dec(tmp1, 10);
			print_decoded_nums(CCI_dec, num1, num2);
			break;
		// When I have #2 encoded numbers: CCI = 010 = 2
		case 2:
			for(int i=3;i<10;i++) {
				tmp1[idx] = bit_stream[i];
				idx++;
			}
			num1 = bin2dec(tmp1, 7);
			print_decoded_nums(CCI_dec, num1, num2);
			break;
		// When I have #1 encoded number: CCI = 001 = 1
		case 1:
			for(int i=3;i<7;i++) {
				tmp3[idx] = bit_stream[i];
				idx++;
			}
			num1 = bin2dec(tmp3, 4);
			print_decoded_nums(CCI_dec, num1, num2);
			break;
		case 0:
			cout << "No numbers are encoded!"; newline;
			break;
		default:
			break;
		}
		input[2] = bin2dec(D1, 8);
		input[1] = bin2dec(D2, 8);
		input[0] = bin2dec(D3, 4);
		make_m1_errors(input);
}

void M1_DECODE::check_diman_decode(void)
{
	int SUM=0;
	cout << "E1 from matrix = " << bin2dec(E1, 8) << "\t\tE2 from matrix = " << bin2dec(E2, 8); newline;
	GLOBAL_CHECK = (LFSR_E1!=bin2dec(E1, 8) || LFSR_E2!=bin2dec(E2, 8)) ? 0 : 1;
	if(!GLOBAL_CHECK) {
		cout << "Your decoding is wrong for Codewords:\nD1 = " << bin2dec(D1, 8) << "\nD2 = " << bin2dec(D2, 8) << "\nD3 = " << bin2dec(D3, 4);
		newline;
		SUM++;
	} else {
		cout << "Decoding is correct."; newline;
	}
	cout << "Errors: " << SUM;
	newline;
}

void M1_DECODE::m1_decode(int m1[M1_SIZE][M1_SIZE])
{
	cout << setfill('-') << setw(80); newline; newline;
	cout << "\t\t\tM1 Micro QR Codes Decoding\n\n";
	cout << setfill('-') << setw(80); newline; newline;
	unmask(m1);						// call unmask() which calls previous functions from format_string.h file
	get_all_codewords(m1);
	//print_codewords();
	create_bit_stream();
	set_CCI();
	cut_numbers();
	check_diman_decode();
}
#endif /* data_retrieval.h */
