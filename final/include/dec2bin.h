/* File   : dec2bin.h
 * Author : dimos katsimardos
 * Created on January 29, 2017, 4:06 AM
 *
 * Header file with function for conversion from a decimal
 * number to an array with the equal binaty number | only 8 bits needed
 */
#ifndef DEC2BIN_H
#define DEC2BIN_H

#include <bitset>


void dec2bin(int dec, int bin[8]) {
	std::string binary = std::bitset<8>(dec).to_string();
	for(int i=0;i<8;i++) {
		/* Zero value = 48 || One value = 49 */
		bin[i] = (binary[i]==48) ? 0 : 1;
	}
}
#endif /* dec2bin.h */
