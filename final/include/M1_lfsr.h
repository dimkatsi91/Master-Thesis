/* File   : M1_lfsr.h ( M1 Micro QR Codes )
 * Author : diman77
 * Created on November 7, 2016, 10:33 AM
 * Edited on November 13, 2016, 2:27 PM
 *
 * This file contains function for M1 Micro QR Codes ECC
 * (Error-COrrection Codewords) generation for message m[3]
 * input, i.e. m(x) || DD1, DD2, DD3
 */
#ifndef M1_lfsr_H
#define M1_lfsr_H

#include "for_rev.h"
int LFSR_E1, LFSR_E2;

void make_m1_errors(int input[3])
{
	int i=2, feedback, b[2] = {0,0};
	int alphas[2] = {25,1};
	
	while(i>=0)
	{
		feedback = input[i]^b[1];
		switch(feedback)
		{
			case 0:
				b[1]=b[0];
				b[0]=0;
				break;
			default:
				for(int j=1;j>=0;j--) {
					b[j] = ret_a_power(feedback) + alphas[1-j];
					b[j] = (b[j]>255) ? b[j]%255 : b[j];
					b[j] = (!j) ? ret_integer(b[j]) : (ret_integer(b[j])^b[j-1]);
				}
				break;
		}
		i--;
	}
	LFSR_E1 = b[1];
	LFSR_E2 = b[0];
	cout << "LFSR E1 = " << LFSR_E1 << "\n";
	cout << "LFSR E2 = " << LFSR_E2 << "\n";
}
#endif /* M1_lfsr.h */