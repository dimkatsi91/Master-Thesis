/* File   : V1_definitions.h
 * Author : dimos katsimardos
 * Created on April 5, 2017, 1:35 PM
 *
 * This header file includes all sub-matrices
 * and their lengths that will be used from all
 * other header files and their functions
 */

#ifndef V1_DEFINITIONS_H
#define V1_DEFINITIONS_H

/* ---------------------------------------------- */
/*              V1_format_string.h              */
/* ------------------------------------------- */
#include <stdlib.h>
#include <iostream>
using std::cout;

#define V1_SIZE 21
#define FSL 15							         //  Format String Length (15 bits)
#define ECLL 2							       //    Error Correction Level Length (2 bits)
#define MIL 3                                //      Mask Info Length (3 bits)
#define ECSL 10                            //        Error Correction Sequence Length (10 bits)
#define HLL 32

#define newline { cout << "\n"; }
#define arrange { cout << "\t\t\t"; }

char ec_level[4][ECSL] = {"Medium", "Low", "High", "Q"};
int HAM_LUT[HLL][FSL] = {
					   1,0,1,0,1,0,0,0,0,0,1,0,0,1,0,				// Table C.1 - Valid Format Information Bit Sequences
					   1,0,1,0,0,0,1,0,0,1,0,0,1,0,1,              //  After Encoding Masking (QR Code Symbols)
					   1,0,1,1,1,1,0,0,1,1,1,1,1,0,0,
					   1,0,1,1,0,1,1,0,1,0,1,1,1,0,0,
					   1,0,0,0,1,0,1,1,1,1,1,1,0,0,1,
					   1,0,0,0,0,0,0,1,1,0,0,1,1,1,0,
					   1,0,0,1,1,1,1,1,0,0,1,0,1,1,1,
					   1,0,0,1,0,1,0,1,0,1,0,0,0,0,0,
					   1,1,1,0,1,1,1,1,1,0,0,0,1,0,0,
					   1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,
					   1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,
					   1,1,1,1,0,0,0,1,0,0,1,1,1,0,1,
					   1,1,0,0,1,1,0,0,0,1,0,1,1,1,1,
					   1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,
					   1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,
					   1,1,0,1,0,0,1,0,1,1,1,0,1,1,0,
					   0,0,1,0,1,1,0,1,0,0,0,1,0,0,1,
					   0,0,1,0,0,1,1,1,0,1,1,1,1,1,0,
					   0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,
					   0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,
					   0,0,0,0,1,1,1,0,1,1,0,0,0,1,0,
					   0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,
					   0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,
					   0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,
					   0,1,1,0,1,0,1,0,1,0,1,1,1,1,1,
					   0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,
					   0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,
					   0,1,1,1,0,1,0,0,0,0,0,0,1,1,0,
					   0,1,0,0,1,0,0,1,0,1,1,0,1,0,0,
					   0,1,0,0,0,0,1,1,0,0,0,0,0,1,1,
					   0,1,0,1,1,1,0,1,1,0,1,1,0,1,0,
					   0,1,0,1,0,1,1,1,1,1,0,1,1,0,1 };


/* ---------------------------------------------- */
/*                V1_Unmasking.h                */
/* ------------------------------------------- */
char v1_masks[8][4] = {"000", "001", "010", "011", "100", "101", "110", "111"};


/* ---------------------------------------------- */
/*              V1_data_retrieval.h              */
/* ------------------------------------------- */
#define CML 208					                                                 				   // Codeword Maximum Bits Length
#define printWord(word,N) { for(int i=0;i<N;i++) { cout << word[i]; } }

typedef int Codeword[8];

int v1_bit_stream[CML];											                		   //     Version 1 Bit Stream with all Data and Error Correction Bits in one matrix
int v1_CCI[10], v1_MI[4];																  //      CCI --> character count indicator with 10 bits for numeric || 9 bits for Alpha Numeric Mode
																						 //       MI --> Mode Indicator with 4 bits :: Numeric -> 0001(1) || Alphanumeric -> 0010(2) || Byte -> 0100(4)

char Modes[5][15] = {" ", "Numeric", "Alphanumeric", " ", "Byte"};
int v1_CCI_dec = bin2dec(v1_CCI, 10);				               					  //          How many characters/ digits are encoded



/* ---------------------------------------------- */
/*                 V1_decode.h                  */
/* ------------------------------------------- */
#define MAX_DC 19													  // Maximum Data Codewords are 19 for LOW Error Correction Level
#define MAX_ECC 17                                                   //  Maximum Error Correction Codewords are 17 for HIGH Error Correction Level
#define ALPHA_MAX 41                                                //   Maximum encoded characters for Alphanumeric Encoding Mode

#define MSL 16													  // Maximum Syndromes Length -> 16 for High ECL
#define MSIGL 8													 //  Maximum Error - Locator - Polynomial sigma(x) Length
															    //   LOW -> 2 || MEDIUM -> 4 || Q -> 6 || HIGH -> 8
#define RX_LEN 26

#include <string.h>											  //    strcpy  function
#include <iomanip>											 //     setfill function
#include <vector>

using std::vector;
using std::setfill;
using std::setw;

char LUT[45][2] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J",
			       "K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," ","$","%","*",
			       "+","-",".","/",":"};							  // Table #5: Encoding / Decoding table for Alphanumeric mode



#endif /* definitions.h */
