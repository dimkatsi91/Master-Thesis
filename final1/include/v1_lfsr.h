/* File   : v1_lfsr.h
 * Author : dimos katsimardos
 * Created on November 29, 2016, 10:58 AM
 *
 * This header file generates ECC ( Error-Correction Codewords for
 * Version 1 QR Codes for every ECL  Error Correction Level )
 * The logic is used for Error Detection Only / For Error Correction
 * Syndromes Logic is needed
 */
#ifndef V1_LFSR_H
#define V1_LFSR_H

#define INPUT_MAX_LEN 19					  // HIGH :: 17 ECC || Q :: 13 ECC || MEDIUM :: 10 ECC || LOW :: 7 ECC
#define ECC_M_L 17						     //  HIGH ::  9 DC  || Q :: 13 DC  || MEDIUM :: 16 DC  || LOW :: 19 DC

#include "for_rev.h"

int V1_LFSR_E[ECC_M_L];				      //     ECC (Decimal Format) for Low || Medium || Q || High ECL
										 //      Above ECC Generated with LFSR must be compared with matrix ECC's for comparison



void make_v1_errors(int input[INPUT_MAX_LEN], int ecl)
{
	const int max_regs = 17;
	int low_alphas[7] = {87,229,146,149,238,102,21};
	int medium_alphas[10] = {251,67,46,61,118,70,64,94,32,45};
	int q_alphas[13] = {74,152,176,100,86,100,106,104,130,218,206,140,78};
	int high_alphas[17] = {43,139,206,78,43,239,123,206,214,147,24,99,150,39,243,163,136};
	int feedback, i=0, hold, b_idx=0, b[max_regs], num_of_eccs;

	for(int k=0;k<max_regs;k++) {
		b[k]=0;
	}

	switch(ecl)
	{
		/* MEDIUM ECL */
		case 0:
			b_idx=9;
			i=15;
			num_of_eccs=10;
			break;
		/* LOW ECL */
		case 1:
			b_idx=6;
			i=18;
			num_of_eccs=7;
			break;
		/* HIGH ECL */
		case 2:
			b_idx=16;
			i=8;
			num_of_eccs=17;
			break;
		/* Q ECL */
		case 3:
			b_idx=12;
			i=12;
			num_of_eccs=13;
			break;
		default:
			break;
	}
	hold=i;

	while(i>=0)
	{
		feedback = input[hold-i]^b[b_idx];
		switch(feedback)
		{
			case 0:
				for(int j=b_idx;j>0;j--) {
					b[j]=b[j-1];
				}
				b[0]=0;
				break;
			default:
				for(int j=b_idx;j>=0;j--)
				{
					switch(ecl)
					{
						/* MEDIUM ECL */
						case 0:
							b[j] = ret_a_power(feedback) + medium_alphas[9-j];
							break;
						/* LOW ECL */
						case 1:
							b[j] = ret_a_power(feedback) + low_alphas[6-j];
							break;
						/* HIGH ECL */
						case 2:
							b[j] = ret_a_power(feedback) + high_alphas[16-j];
							break;
						/* Q ECL */
						case 3:
							b[j] = ret_a_power(feedback) + q_alphas[12-j];
							break;
						default:
							break;
					}
					/* Galois Field Addition Rules Checking */
					b[j] = (b[j]>255) ? b[j]%255 : b[j];
					b[j] = (!j) ? (ret_integer(b[j])) : (ret_integer(b[j])^b[j-1]);
				}
				break;
		}
		i--;
	}

	//cout << "\n\nPRINTING LFSR E[] :: \n";
	for(i=0;i<num_of_eccs;i++) {
		V1_LFSR_E[i] = b[num_of_eccs-i-1];
		//cout << "V1_LFSR_E[" << i << "] = " << V1_LFSR_E[i]; newline;
	}
	//newline; newline;
}
#endif /* v1_lfsr.h */
