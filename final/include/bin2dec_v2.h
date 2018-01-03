/* File   : bin2dec_v2.h
 * Author : dimos katsimardos
 * Created on November 14, 2016, 9:14 AM
 *
 * Header file with function for conversion from a binary
 * number to a decimal one
 */
#ifndef BIN2DEC_V2_H
#define BIN2DEC_V2_H

#include <math.h>
#include <bitset>

int bin2dec(int bin[], int len)
{
	int dec = 0;
	for(int i=len-1;i>=0;i--) {
		dec +=  bin[i]*pow(2.,len-i-1);
	}
	return dec;
}
#endif /* bin2dec_v2.h */
