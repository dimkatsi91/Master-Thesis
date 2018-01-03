/* File   : V1_format_string.h
 * Author : dimos katsimardos
 * Created on November 9, 2016, 2:35 AM
 *
 * Format String Header File || 2nd Approach || Simple One
 * Functions for quarry bits from exact positions in the
 * V1 QR Code Matrix and place them in specific matrices,
 * v1_ec_level[], v1_mask_info[] & v1_err_corr[]
 */
#ifndef V1_FORMAT_STRING_H
#define V1_FORMAT_STRING_H

#include "bin2dec_v2.h"
#include "V1_definitions.h"


class V1_FORMAT_STRING {
protected:
	int v1[V1_SIZE][V1_SIZE];
public:
	int v1_format_string[FSL];	                //  15-bit format string sequence
	int v1_ec_level[ECLL];                    //  2-bit error correction level
	int v1_mask_info[MIL];                  //   3-bit sequence with used mask (000...111)
	int v1_err_corr[ECSL];	              //    10-bit error correction sequence (BCH)
	int v1_fs_check[FSL];	      	    //     15-bit format string sequence || the second
							     	   //     one placed in matrix for checking format string

	bool f_s_check;		            //      format strings check boolean
					/* MEMBER FUNCTIONS */
	void get_v1_format_string(int v1[V1_SIZE][V1_SIZE]);
	void unxor_v1_format_string();
	void set_ec_level();
	void set_mask_info();
	void set_err_corr();
	void set_second_format(int v1[V1_SIZE][V1_SIZE]);
	void compare_format_strings();
	void calc_hamming();
	void v1_format_string_packet(int v1[V1_SIZE][V1_SIZE]);
};



/* -------------------------------------------------- */
/* Get bits for format_string[15] matrix fullillment */
/* ------------------------------------------------ */
void V1_FORMAT_STRING::get_v1_format_string(int v1[V1_SIZE][V1_SIZE])
{
	int idx=5, row=8, col=5;
	/* First 5 bits are error correction level(2 bits) & mask(3 bits) */
	for(int j=0;j<5;j++) {
		v1_format_string[j] = v1[8][j];
	}
	/* Next 10 bits are error correction bits (BCH) */
	for(int i=0;i<10;i++) {
		v1_format_string[idx++] = v1[row][col];
		//cout << "v1[" << row << "][" << col << "]= " << v1[row][col] << "\n";
		row = (i==0 || i==1) ? 8 : row-=1;
		row = (i==3) ? row-=1 : row;
		col = (i==0) ? 7 : 8;
	}
}


/* ------------------------------------------------------------------------------------------------- */
/* Format_string[15] matrix must be unxored with Version 1 QR Code Mask to go back to correct value */
/* ----------------------------------------------------------------------------------------------- */
void V1_FORMAT_STRING::unxor_v1_format_string(void)
{
	const int v1_mask_size = 15;
	int v1_xor_mask[v1_mask_size] = {1,0,1,0,1,0,0,0,0,0,1,0,0,1,0};
	int tmp[v1_mask_size], i;
	for(i=0;i<v1_mask_size;i++) {
		tmp[i] = v1_format_string[i]^v1_xor_mask[i];
	}
	for(i=0;i<v1_mask_size;i++) {
		v1_format_string[i] = tmp[i];
	}
}


/* ------------------------------------------------------------------ */
/* Set error correction level matrix bits using format string matrix */
/* ---------------------------------------------------------------- */
void V1_FORMAT_STRING::set_ec_level(void)
{
	for(int i=0;i<2;i++) {
		v1_ec_level[i] = v1_format_string[i];
	}
	//cout << "Error Correction Level Check :: " << ec_level[bin2dec(v1_ec_level, 2)]; newline;
}


/* Set mask matrix using format string matrix */
void V1_FORMAT_STRING::set_mask_info(void)
{
	for(int i=2;i<5;i++)	v1_mask_info[i-2] = v1_format_string[i];
}


/* ------------------------------------------------------- */
/* Set error correction matrix using format string matrix */
/* ----------------------------------------------------- */
void V1_FORMAT_STRING::set_err_corr(void)
{
	for(int i=5;i<15;i++)	v1_err_corr[i-5] = v1_format_string[i];
}


/* ------------------------------------------------------------------------------------ */
/* Now a function that quarrys bits from the other QR Code Matrix Modules and compares */
/* them with the v1_format_string[15] matrix to check if modules placement is correct */
/* --------------------------------------------------------------------------------- */
void V1_FORMAT_STRING::set_second_format(int v1[V1_SIZE][V1_SIZE])
{
	int idx=0, i, j;
	const int v1_mask_len = 15;
	int v1_mask[v1_mask_len] = {1,0,1,0,1,0,0,0,0,0,1,0,0,1,0};
	int tmp[v1_mask_len];
	for(i=20;i>13;i--) {
		v1_fs_check[idx++] = v1[i][8];
	}
	for(j=13;j<V1_SIZE;j++) {
		v1_fs_check[idx++] = v1[8][j];
	}
	/* Now unxor format string matrix with v1 mask */
	for(i=0;i<v1_mask_len;i++)	tmp[i] = v1_fs_check[i]^v1_mask[i];
	for(i=0;i<v1_mask_len;i++)	v1_fs_check[i] = tmp[i];
}


/* ------------------------------------------------------------------------------------------------------------------------------------------ */
/* Now a last function that compares the format string with all possible format strings and if there are less than 3 bits difference keep it */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
void V1_FORMAT_STRING::compare_format_strings(void)
{
	/* f_s_diff = Hamming distance of 2 format strings from QR code symbol */
	/* If Hamming distance > 3 no format string correction from Lookup table is available */
	int i, j, fs_diff1=0, fs_diff2=0, ham_dist1[32], ham_dist2[32], ham1=0, ham2=0, min1, min2, min;
	f_s_check = true;
	for(i=0;i<32;i++)	ham_dist1[i]=ham_dist2[i]=0;
	for(i=0;i<15;i++) {
		f_s_check = (v1_format_string[i]!=v1_fs_check[i]) ? false : f_s_check;
	}
	if(!f_s_check) 
	{
		for(i=0;i<32;i++) {
			for(j=0;j<15;j++) {
				fs_diff1 += (v1_format_string[j]!=HAM_LUT[i][j]) ? 1 : 0;
				fs_diff2 += (v1_fs_check[j]!=HAM_LUT[i][j]) ? 1 : 0;
			}
		}
		if(fs_diff1>3 && fs_diff2>3) {
			cout << "Hamming distance > 3. Cannot correct format string. Aborting...\n";
			exit(1);
		} else {
			for(i=0;i<32;i++) {
				ham1=0;
				ham2=0;
				for(j=0;j<15;j++) {
					ham1 += (v1_format_string[j]!=HAM_LUT[i][j]) ? 1 : 0;
					ham2 += (v1_fs_check[j]!=HAM_LUT[i][j]) ? 1 : 0;
				}
				ham_dist1[i] = ham1;
				ham_dist2[i] = ham2;
			}
			/* Now sort ham_dist1[] && ham_dist2[] matrices and keep this with the smallest Hamming Distance */
			min=ham_dist1[0];
			for(i=0;i<32-1;i++) {
				if(ham_dist1[i]<ham_dist1[i+1]) {
					min1	= ham_dist1[i];
				}
				if(ham_dist2[i]<ham_dist2[i+1]) {
					min2 = ham_dist2[i];
				}
			}
			min = (min1<min2) ? min1 : min2;
			/* Replace Format String */
			for(i=0;i<15;i++) {
				v1_format_string[i] = HAM_LUT[min][i];
			}
		}
		newline;
	}
}


/* -------------------------------------------------------- */
/*            Format String Header File Packet            */
/* ---------------------------------------------------- */
void V1_FORMAT_STRING::v1_format_string_packet(int v1[V1_SIZE][V1_SIZE])
{
	get_v1_format_string(v1);
	unxor_v1_format_string();
	set_second_format(v1);
	compare_format_strings();
	set_ec_level();
	set_mask_info();
	set_err_corr();
}
#endif /* v1_unmasking.h */
