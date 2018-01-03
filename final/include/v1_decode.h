/* File   : v1_decode.h
 * Author : dimos katsimardos
 * Created on November 12, 2016, 4:13 PM
 * Edited on March 25, 2017, 11:20 AM (RS Logic Added)
 * Edited on March 29, 2017, 5:18 PM (LOW -- MEDIUM -- Q -- HIGH Error Correction Added)
 *
 * This header file uses all previous header files for Version 1 QR Code Matrix
 * Decoding Procedure and based on MI(Mode Indicator) & CCI(Character Count Indicator)
 * chooses the appropriate decoding procedure for Numeric || AlphaNumeric || Byte Mode
 * MI(Numeric) = 0001 || MI(AlphaNumeric) = 0010 || MI(Byte) = 0100
 *
 * Also, Reed - Solomon Error Correction Algorithm is applied when Syndromes != 0
 * When Syndromes are zero no R-S Correction is needed cause no error has occurred.
 * If Syndromes != 0 errors have occurred. So, we folllow 3 step approach .
 * Step 1: Calc Error Locator Polynomial Lambda(x) using calculated Syndromes.
 * Step 2: Calc Error Magnitude Polynomial Omega(x) with Extended Euclid's Algorithm.
 *
 * Lambda(x) && Omega(x) are calculated using Extended Euclid's Algorithm for polynomials.
 * Also, Berlekamp - Massey can be used for Lambda(x) & Omega(x) calculation.
 *
 * Step 3: Correct rx/ R(x) polynomial using Lambda & Omega and known formula for error calc, i.e.
 * Yj = Xj * Omega(Xj^-1) / Lambda'(Xj^-1) , where Yj = e[] && Lambda' = d(Lambda)/dx following
 * derivatives rules for Galois Fields.
 */
#ifndef V1_DECODE_H
#define V1_DECODE_H

#ifdef __unix__
	#define unix 1
#elif defined(_WIN32) || defined(_WIN64)
	#define unix 0
#endif

#include "V1_data_retrieval.h"
#include "v1_lfsr.h"
#include "dec2bin.h"
#include "GF_ACTIONS.h"


class V1_DECODE : public V1_DATA_RETRIEVAL {
public:
	char alpha[ALPHA_MAX][2];	 										// 	 Encoded characters for alphanumeric mode decoding

	int V1_GLOBAL_CHECK, RS_CHECK;
	int dec_MI;							             			      //     1 --> Numeric || 2 --> Alpha || 4 --> Byte
	int dec_ec_level, dec_cci;									     //      dec_ec_level : 0 -> Medium | 1 -> Low | 2 -> High | 3 -> Q
																    //       dec_cci : 1...41 -> Encoded numbers
	int cws[RX_LEN];											   //        Data Codewords && Error Correction Codewords In Decimal Format Inside Matrix
	int v1_nums[MAX_DC];										  //         This matrix is going to have the encoded numbers if encoding is numeric only
	int v1_errors[MAX_ECC];                                      //          Matrix with ECC Codewords in Decimal Format Based on EC-LEVEL & CCI

	int rx[RX_LEN];
	int S[MSL];												  //             LOW -> S0...S3 || MEDIUM -> S0...S7 || Q -> S0...S12 || HIGH -> S0...S15
		  /* MEMBER FUNCTIONS */
	void choose_v1_decode_mode();
	void find_params();
	void print_nums(int mat[], int N, int param);
	void print_errors(int err[], int N);
	void decode_numeric();
	void print_alpha(char chars[ALPHA_MAX][2], int len);
	void decode_alpha();
	void decode_byte();
	void create_v1_lfsr_errors();
	void check_v1_decode();
	void set_rx();
	void calc_syndromes();
	void rs_eucl();
	void correct();
	void v1_decode(int v1[V1_SIZE][V1_SIZE]);
};

/* ----------------------------------------------------------------------------------------- */
/*       First Find What Mode Is Used, to choose the appropriate decoding procedure         */
/* Find how many numbers are encoded (dec_cci) & the error correction level (dec_ec_level) */
/* -------------------------------------------------------------------------------------- */
void V1_DECODE::choose_v1_decode_mode(void)
{
	dec_MI = bin2dec(v1_MI, 4);
}

void V1_DECODE::find_params(void)
{
	dec_ec_level = bin2dec(v1_ec_level, 2);
	//cout << "Number of encoded characters are :: ";
	switch(dec_MI)
	{
		case 1:
			dec_cci = bin2dec(v1_CCI, 10);		 // CCI :: 10 bits for Numeric Mode
			break;
		case 2:
			dec_cci = bin2dec(v1_CCI, 9);      //   CCI :: 9 bits for Alpha-Numeric Mode
			break;
		case 4:
			dec_cci = bin2dec(v1_CCI, 8);	  //    CCI :: 8 bits for Byte Mode
			break;
		default:
			cout << "\nERROR!\nAborting...\n";
			exit(1);
			break;
	}
	//cout << dec_cci; newline;
}

/* ------------------------------------------------------------------------- */
/* First Scenario, when Numbers are encoded in the QR Code Version 1 Matrix */
/* ----------------------------------------------------------------------- */
void V1_DECODE::print_nums(int mat[], int N, int param)
{
	cout << "\t\t\tNUMBERS\n\n\t\t\t";
	switch(param)
	{
		/* When I have even number of numbers encoded */
		case 0:
			for(int i=0;i<N-1;i++) {
				cout << setfill('0') << setw(3) << mat[i];
			}
			break;
		/* When I have 1 number remaining after the triades of numbers */
		case 1:
			for(int i=0;i<N-1;i++) {
				cout << setfill('0') << setw(3) << mat[i];
			}
			cout << mat[N-1];
			break;
		/* When I have 2 numbers remaining after the triades of numbers */
		case 2:
			for(int i=0;i<N-1;i++) {
				cout << setfill('0') << setw(3) << mat[i];
			}
			if(mat[N-1]<10) {
				cout << setfill('0') << setw(2) << mat[N-1];
			} else {
				cout << mat[N-1];
			}
			break;
		/* This is never reached btw */
		default:
			break;
	}
	newline;
}


void V1_DECODE::print_errors(int err[], int N)
{
	cout << "Error - Correction Codewords (Decimal) are :: "; newline;
	for(int i=0;i<N;i++) {
		cout << "ECC #" << i+1 << " : " << err[i]; newline;
	}
}


void V1_DECODE::decode_numeric(void)
{
	int triades = dec_cci/3;								 // How many sets of 3 (decimal) numbers I have
	int rev     = dec_cci%3;           						//  If I have 1 number OR a set of 2 numbers remaining
	/* First let's put numbers from bit stream to cws[] matrix based on triades && revs */
	int tmp1[10], tmp2[7], tmp3[4], idx=14;
	for(int i=0;i<triades;i++) {
		for(int j=0;j<10;j++) {
			tmp1[j] = v1_bit_stream[idx++];
		}
		cws[i] = bin2dec(tmp1, 10);
	}
	switch(rev)
	{
		case 1:
			for(int k=0;k<4;k++) {
				tmp3[k] = v1_bit_stream[idx++];
			}
			cws[triades] = bin2dec(tmp3, 4);
			break;
		case 2:
			for(int k=0;k<7;k++) {
				tmp2[k] = v1_bit_stream[idx++];
			}
			cws[triades] = bin2dec(tmp2, 7);
			break;
		default:
			break;
	}
	print_nums(cws, triades+1, rev);
}


/* ----------------------------------------------------------------------------- */
/* Second Scenario, when Characters are encoded in the QR Code Version 1 Matrix */
/* --------------------------------------------------------------------------- */
void V1_DECODE::print_alpha(char chars[ALPHA_MAX][2], int len)
{
	cout << "\t\t\tTEXT\n\n\t\t\t";
	for(int i=0;i<len;i++) {
		cout << chars[i];
	}
	newline;
}

void V1_DECODE::decode_alpha(void)
{
	int tmp1[11], tmp2[6], idx=13, aidx=0, num;
	int diades = dec_cci/2;
	int rev    = dec_cci%2;
	for(int i=0;i<diades;i++) {
		for(int j=0;j<11;j++) {
			tmp1[j] = v1_bit_stream[idx++];
		}
		num = bin2dec(tmp1, 11);
		strcpy(alpha[aidx], LUT[num/45]);
		strcpy(alpha[aidx+1], LUT[num%45]);
		aidx+=2;
	}
	switch(rev)
	{
		case 1:
			for(int i=0;i<6;i++) {
				tmp2[i] = v1_bit_stream[idx++];
			}
			strcpy(alpha[aidx], LUT[bin2dec(tmp2, 6)]);
			break;
		default:
			break;
	}
	print_alpha(alpha, dec_cci);
}


/* ----------------------------------------------------------------------- */
/* Third Scenario, when Bytes are encoded in the QR Code Version 1 Matrix */
/* --------------------------------------------------------------------- */
void V1_DECODE::decode_byte(void)
{
	//cout << "Encoded Byte Stream is :: ";
	cout << "\t\t\tTEXT (BYTES)\n\n\t\t\t";
	int tmp[8], idx=12;
	for(int j=0;j<dec_cci;j++) {
		for(int i=0;i<8;i++) {
			tmp[i] = v1_bit_stream[idx++];
		}
		cout << (char) bin2dec(tmp,8);
	}
	newline;
}


/* ---------------------------------------------------------------------------------------------------- */
/* Next Operation for Error - Detection :: Create LFSR Error - Correction Codewords based on ECC Level */
/* -------------------------------------------------------------------------------------------------  */
void V1_DECODE::create_v1_lfsr_errors(void)
{
	int low_input[19], med_input[16], high_input[9], q_input[13], idx=0, tmp[8];
	switch(dec_ec_level)
	{
		/* Medium ECL */
		case 0:
			for(int i=0;i<16;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_nums[i] = bin2dec(tmp,8);
			}
			for(int i=0;i<10;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_errors[i] = bin2dec(tmp, 8);
			}
			break;
		/* Low ECL */
		case 1:
			for(int i=0;i<19;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_nums[i] = bin2dec(tmp,8);
			}
			for(int i=0;i<7;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_errors[i] = bin2dec(tmp, 8);
			}
			break;
		/* High ECL */
		case 2:
			for(int i=0;i<9;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_nums[i] = bin2dec(tmp,8);
			}
			for(int i=0;i<17;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_errors[i] = bin2dec(tmp,8);
			}
			break;
		/* Q ECL */
		case 3:
			for(int i=0;i<13;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_nums[i] = bin2dec(tmp,8);
			}
			for(int i=0;i<13;i++) {
				for(int j=0;j<8;j++) {
					tmp[j] = v1_bit_stream[idx++];
				}
				v1_errors[i] = bin2dec(tmp, 8);
			}
			break;
		default:
			break;
	}
	 /* Now, give DC and LFSR should generate ECC's based on ECL */
	switch(dec_ec_level)
	{
		/* MEDIUM ECL */
		case 0:
			for(int i=0;i<16;i++) {
				med_input[i] = v1_nums[i];
			}
			make_v1_errors(med_input, dec_ec_level);
			break;
		/* LOW ECL */
		case 1:
			for(int i=0;i<19;i++) {
				low_input[i] = v1_nums[i];
			}
			make_v1_errors(low_input, dec_ec_level);
			break;
		/* HIGH ECL */
		case 2:
			for(int i=0;i<9;i++) {
				high_input[i] = v1_nums[i];
			}
			make_v1_errors(high_input, dec_ec_level);
			break;
		/* Q ECL */
		case 3:
			for(int i=0;i<13;i++) {
				q_input[i] = v1_nums[i];
			}
			make_v1_errors(q_input, dec_ec_level);
			break;
		default:
			cout << "if(printed) ? ERROR"; newline;
			break;
	}
}

void V1_DECODE::check_v1_decode(void)
{
	/* Here I set V1_GLOBAL_CHECK variable to 1 || 0, based on ECC's from matrix & LFSR Circuit comparison */
	V1_GLOBAL_CHECK = 0;			// If all ECC's are the same both from Matrix (v1_errors[]) && LFSR Circuit Generated (LFSR_E[])
	switch(dec_ec_level)
	{
		/* MEDIUM EC LEVEL */
		case 0:
			for(int i=0;i<10;i++) {
				if(V1_LFSR_E[i]!=v1_errors[i]) {
					V1_GLOBAL_CHECK = 1;
				}
			}
			break;
		/* LOW EC LEVEL */
		case 1:
			for(int i=0;i<7;i++) {
				if(V1_LFSR_E[i]!=v1_errors[i]) {
					V1_GLOBAL_CHECK = 1;
				}
			}
			break;
		/* HIGH EC LEVEL */
		case 2:
			for(int i=0;i<17;i++) {
				if(V1_LFSR_E[i]!=v1_errors[i]) {
					V1_GLOBAL_CHECK = 1;
				}
			}
			break;
		/* Q EC LEVEL */
		case 3:
			for(int i=0;i<13;i++) {
				if(V1_LFSR_E[i]!=v1_errors[i]) {
					V1_GLOBAL_CHECK = 1;
				}
			}
			break;
		/* Never reached operation under normal circumstances */
		default:
			break;
	}
	//cout << ( (!V1_GLOBAL_CHECK) ? "No errors detected through LFSR comparison." : "Errors detected through LFSR comparison." );
	newline;
}


/* --------------------------------------------------------------------------------------------------- */
/*                               Reed - Solomon - Error - Correction                                  */
/* ------------------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------------------- */
/* STEP #0: Find R(x) polynomial | r0...r25 for version 1 qr codes | Needed for Syndromes Calculation */
/* ------------------------------------------------------------------------------------------------- */
void V1_DECODE::set_rx(void)
{
	int dc_len;		// Data Codewords Length for each ECL(Error-Correction Level)
	int ecc_len,idx=0;
	for(int i=0;i<RX_LEN;i++) {
		rx[i]=0;
	}
	switch(dec_ec_level)
	{
		/* MEDIUM ECL */
		case 0:
			dc_len=16;
			ecc_len=10;
			break;
		/* LOW ECL */
		case 1:
			dc_len=19;
			ecc_len=7;
			break;
		/* HIGH ECL */
		case 2:
			dc_len=9;
			ecc_len=17;
			break;
		/* Q ECL */
		case 3:
			dc_len=13;
			ecc_len=13;
			break;
		default:
			cout << "ERROR occured in R(x) polynomial calculation. Aborting!"; newline;
			exit(1);
			break;
	}


	for(int i=0;i<dc_len;i++) {
		rx[i] = v1_nums[i];
	}
	for(int i=dc_len;i<dc_len+ecc_len;i++) {
		rx[i] = v1_errors[idx++];
	}

	//for(int j=0;j<26;j++) { cout << "RX[" << j << "] = " << rx[j]; newline; }
	//newline;

}


/* -------------------------------------------------------------------------------------------------------------------------------------- */
/*                                STEP #1: From R(x) calculate Syndromes based on ECL(Error Correction Level)                            */
/*        ECL = LOW -> Syndromes = 4 || ECL = MEDIUM -> Syndromes = 8 || ECL = Q -> Syndromes = 12 || ECL = HIGH -> Syndromes = 16      */
/* ----------------------------------------------------------------------------------------------------------------------------------- */
void V1_DECODE::calc_syndromes(void)
{
	/* tt = 2*t , {Si, i=0...2t-1} */
	/*            Correction Capability = (n-k)/2 , i.e. LOW ECL -> up to 2 Errors            */
	/* MEDIUM -> up to 4 errors max || Q -> up to 6 errors max || HIGH -> up to 8 errors max */
	int tt, idx, hold;
	GF256<int>  gf;
	RS_CHECK = 0;
	/* Initialization */
	for(int k=0;k<MSL;k++) {
		S[k]=0;
	}
	switch(dec_ec_level)
	{
		/* MEDIUM ECL */
		case 0:
			tt=8;
			break;
		/* LOW ECL */
		case 1:
			tt=4;
			break;
		/* HIGH ECL */
		case 2:
			tt=16;
			break;
		/* Q ECL */
		case 3:
			tt=12;
			break;
		default:
			cout << "ERROR Occured in Syndromes Calculation. Aborting..";
			exit(1);
	}

	for(int i=0;i<tt;i++) {
		idx=0;
		for(int j=25;j>=0;j--) {
			/* Check for zero component in R(x) | mul(int*int) = alpha^(x+y) | GF exception */
			hold = ( !ret_a_power(rx[idx]) ) ? 0 : ret_integer( gf.add(ret_a_power(rx[idx]),gf.mul(i,j)) );
			S[i] ^= hold;
			idx++;
		}
		RS_CHECK = (S[i]!=0) ? 1 : RS_CHECK;
	}

	if(RS_CHECK) {
		cout << ec_level[dec_ec_level] << " Error Correction Level : Up to " << tt/2 << " errors maximum error correction capability.\n";
	}
	/*
	newline;
	for(int i=0;i<tt;i++) {
		if(!S[i]) {
	    	cout << "S[a^" << i << "] = " << S[i];
	  	} else {
			cout << "S[a^" << i << "] = " << S[i] << " = a^" << ret_a_power(S[i]);
	  	}
		newline;
	}
	*/
}


/* --------------------------------------------------------------------- */
/* STEP #2: From Syndromes calculate Error-Locator-Polynomial Lambda(x) */
/* and Error Magnitude Polynomial Using Extended Euclidean Algorithm   */
/* ------------------------------------------------------------------ */
void V1_DECODE::rs_eucl(void)
{
	const int max_errors=8;
	int roots[max_errors], beta[max_errors], e[max_errors];		// max error correction capability = 8 errors in HIGH ECL
	int idx=0, num=0, denum=0, N;
	//int SS[N], old_r[N], r[N], tmp[N];
	vector<int> SS, old_r, r, tmp;
	GF256<int> gf;
	int q[2], q1[2], q0[2], hold_q[2];
    int gamma=0, k=0, m=0, hold=0, er_sum=0, sum=0, ee=0, divs=0;
    int Omega[8], Lambda[9], B1[2], B2[3], B3[4], B4[5], B5[6], B6[7], B7[8], B8[9];

	for(int i=0;i<2;i++)	q[i]=hold_q[i]=q0[i]=q1[i]=B1[i]=0;
	for(int i=0;i<3;i++)	B2[i]=0;
	for(int i=0;i<4;i++)    B3[i]=0;
	for(int i=0;i<5;i++)    B4[i]=0;
	for(int i=0;i<6;i++)    B5[i]=0;
	for(int i=0;i<7;i++)	B6[i]=0;
	for(int i=0;i<8;i++)	B7[i]=beta[i]=roots[i]=e[i]=Omega[i]=0;
	for(int i=0;i<9;i++)	B8[i]=Lambda[i]=0;

	switch(dec_ec_level)
	{
		/* MEDIUM ECL || Up to 4 errors correction capability || S0...S7   */
		/* Sigma(x) = sigma4*x^4 + sigma3*x^3 + sigma2*x^2 + sigma1*x + 1 */
		case 0:
			N=8;
			divs=4;
			break;
		/* LOW ECL || and sigma1 - sigma2 -> sigma(x) = sigma2*x^2 + sigma1*x + 1 */
		/* Up to 2 errors error correction capability || S0...S3 */
		case 1:
			N=4;
			divs=2;
			break;
		/* HIGH ECL || 16 syndromes S0...S15 */
		/* Up to 8 errors error correction capability || sigm(x) = sigma8*x^8 + ... + sigma0 */
		case 2:
			N=16;
			divs=8;
			break;
		/*  Q ECL | Up to 6 errors correction capability || 13 Syndromes S0...S12  */
		/* Sigma(x) = s6*x^6 + sigma5*x^5 + ... + sigma0 */
		case 3:
			N=12;
			divs=6;
			break;
		default:
			break;
	}
	SS.resize(N);
	old_r.resize(N);
	r.resize(N);
	tmp.resize(N);
	for(int i=0;i<N;i++) {
		SS[i]=S[N-i-1];
		old_r[i]=r[i]=tmp[i]=0;
	}

	/*      Initialization of divisor || LOW : x^2 ... HIGH : x^16      */
	/*       x^2t | t = maximum error correction capability           */
	/* LOW ECL -> x^4 || MEDIUM -> x^8 || Q -> x^12 || HIGH -> x^16 */
	old_r[0]=1;

	while(m<divs)
    {
	    	//cout << "\n\n\t\t\t\tDIVISION " << (m+1) << endl;
        	k=0;
        	while(k<2)
        	{
				q[k] = (!m && !k) ? (255-ret_a_power(SS[k])) : q[k];
				q[k] = (!m && k!=0) ? (ret_a_power(r[k])-ret_a_power(SS[m])) : q[k];
				q[k] = (m!=0 && !k) ? (ret_a_power(old_r[k])-ret_a_power(SS[k])) : q[k];
				q[k] = (m!=0 && k!=0) ? (ret_a_power(r[k])-ret_a_power(SS[0])) : q[k];
				q[k] = (q[k]<0) ? (255+q[k]) : q[k]%255;
				//cout << "q[" << k << "] = a^" << q[k] << endl;
            	for(int i=0;i<N;i++) {
					tmp[i] = (SS[i]==0) ? 0 : ret_integer(gf.add(q[k], ret_a_power(SS[i])));
					//cout << "TMP[" << i << "] = " << tmp[i] << " = a^" << ret_a_power(tmp[i]) << endl;
					r[i] = old_r[i]^tmp[i];
					//cout << "\t\tr[" << i << "] = " << r[i] << " = a^" <<  ret_a_power(r[i]) << endl;
                	er_sum += (!r[i] && k) ? 1 : 0;
            	}

				switch(dec_ec_level)
				{
					/* MEDIUM ECL || up to 4 errors */
					case 0:
						ee = (!m && er_sum==7) ? 1 : ee;
						ee = (m && er_sum==6) ? 2 : ee;
						ee = (m==2 && er_sum==5) ? 3 : ee;
						ee = (m==3 && er_sum==4) ? 4 : ee;
						break;
					/* LOW ECL || up to 2 errors  */
					case 1:
						ee = (!m && er_sum==3) ? 1 : ee;
						ee = (m && er_sum==0) ? 2 : ee;
						break;
					/* HIGH ECL || up to 8 errors */
					case 2:
						ee = (!m && er_sum==15) ? 1 : ee;
						ee = (m==1 && er_sum==14) ? 2 : ee;
						ee = (m==2 && er_sum==13) ? 3 : ee;
						ee = (m==3 && er_sum==12) ? 4 : ee;
						ee = (m==4 && er_sum==11) ? 5 : ee;
						ee = (m==5 && er_sum==10) ? 6 : ee;
						ee = (m==6 && er_sum==9) ? 7 : ee;
						ee = (m==7 && er_sum==8) ? 8 : ee;
						break;
					/* Q ECL || up to 6 errors */
					case 3:
						ee = (!m && er_sum==11) ? 1 : ee;
            			ee = (m && er_sum==10) ? 2 : ee;
            			ee = (m==2 && er_sum==9) ? 3 : ee;
            			ee = (m==3 && er_sum==8) ? 4 : ee;
            			ee = (m==4 && er_sum==7) ? 5 : ee;
            			ee = (m==5 && er_sum==6) ? 6 : ee;
						break;
					default:
						break;
				}
				er_sum=0;

		   		for(int i=0;i<N;i++) {
                	old_r[i] = (i==(N-1)) ? 0 : r[i+1];
                	//cout << "old_r[" << i << "] = " << old_r[i] << " = a^" <<  ret_a_power(old_r[i]) << endl;
            	}
            	k++;
        	}

			for(int i=0;i<2;i++) {
            	hold_q[i] = (m==N-2) ? q[i] : hold_q[i];
        	}

			switch(m)
	        {
	            case 0:
	                for(int j=0;j<2;j++) {
	                    q0[j]=q[j];
	                    /* Gia to B1 */
	                    B1[j]=q0[j];
	                    //cout << "B1[" << j << "] = a^" << B1[j] << "\t";
	                }
	                break;
	            case 1:
	                for(int j=0;j<2;j++) {
	                    q1[j]=q[j];
	                }
	                /* Gia to B2 */
	                for(int j=0;j<3;j++) {
	                    switch(j)
	                    {
	                        case 0:
	                            B2[j] = gf.add(q1[j],q0[j]);
	                            break;
	                        case 1:
	                            B2[j] = ret_integer(gf.add(q1[j-1],q0[j])) ^ ret_integer(gf.add(q1[j],q0[j-1]));
	                            B2[j] = ret_a_power(B2[j]);
	                            break;
	                        case 2:
	                            B2[j] = ret_a_power(1^ret_integer(gf.add(q0[j-1],q1[j-1])));
	                            break;
	                        default:
	                            break;
	                    }
	                    //cout << "B2[" << j << "] = a^" << B2[j] << "\t";
	                }
	                break;
	            case 2:
	                /* Gia to B3 */
	                for(int j=0;j<4;j++) {
	                    switch(j)
	                    {
	                        case 0:
	                            B3[j] = gf.add(q[j],B2[j]);
	                            break;
	                        case 1:
	                            B3[j] = ret_integer(gf.add(q[j-1],B2[j]))^ret_integer(gf.add(q[j],B2[j-1]));
	                            B3[j] = ret_a_power(B3[j]);
	                            break;
	                        case 2:
	                            B3[j] = ret_integer(gf.add(q[j-2],B2[j]))^ret_integer(gf.add(q[j-1],B2[j-1]));
	                            B3[j] ^= ret_integer(B1[j-2]);
	                            B3[j] = ret_a_power(B3[j]);
	                            break;
	                        case 3:
	                            B3[j] = ret_integer(B1[j-2])^ret_integer(gf.add(q[j-2],B2[j-1]));
	                            B3[j] = ret_a_power(B3[j]);
	                            break;
	                        default:
	                            break;
	                    }
	                    //cout << "B3[" << j << "] = a^" << B3[j] << "\t";
	                }
	                break;
	            case 3:
	                /* Gia to B4 */
	                for(int j=0;j<5;j++) {
	                    switch(j)
	                    {
	                        case 0:
	                            B4[j] = gf.add(q[j],B3[j]);
	                            break;
	                        case 1:
	                            B4[j] = ret_a_power( ret_integer(gf.add(q[j],B3[j-1]))^ret_integer(gf.add(q[j-1],B3[j])) );
	                            break;
	                        case 2:
	                            B4[j] = ret_integer(gf.add(q[j-2],B3[j]))^ret_integer(gf.add(q[j-1],B3[j-1]));
	                            B4[j] ^= ret_integer(B2[j-2]);
	                            B4[j] = ret_a_power(B4[j]);
	                            break;
	                        case 3:
	                            B4[j] = ret_integer(gf.add(q[j-3],B3[j]))^ret_integer(gf.add(q[j-2],B3[j-1]));
	                            B4[j] ^= ret_integer(B2[j-2]);
	                            B4[j] = ret_a_power(B4[j]);
	                            break;
	                        case 4:
	                            B4[j] = ret_a_power( ret_integer(gf.add(q[j-3],B3[j-1]))^ret_integer(B2[j-2]) );
	                            break;
	                        default:
	                            break;
	                    }
	                    //cout << "B4[" << j << "] = a^" << B4[j] << "\t";
	                }
	                break;
	            /* Gia to B5 */
	            case 4:
	                for(int j=0;j<6;j++) {
	                    switch(j)
	                    {
	                        case 0:
	                            B5[j] = gf.add(q[j],B4[j]);
	                            break;
	                        case 1:
	                            B5[j] = ret_a_power(ret_integer(gf.add(B4[j-1],q[j]))^ret_integer(gf.add(B4[j],q[j-1])));
	                            break;
	                        case 2:
	                            B5[j] = ret_integer(gf.add(B4[j-1],q[j-1]))^ret_integer(gf.add(B4[j],q[j-2]));
	                            B5[j] ^= ret_integer(B3[j-2]);
	                            B5[j] = ret_a_power(B5[j]);
	                            break;
	                        case 3:
	                            B5[j] = ret_integer(gf.add(B4[j-1],q[j-2]))^ret_integer(gf.add(B4[j],q[j-3]));
	                            B5[j] ^= ret_integer(B3[j-2]);
	                            B5[j] = ret_a_power(B5[j]);
	                            break;
	                        case 4:
	                            B5[j] = ret_integer(gf.add(B4[j-1],q[j-3]))^ret_integer(gf.add(B4[j],q[j-4]));
	                            B5[j] ^= ret_integer(B3[j-2]);
	                            B5[j] = ret_a_power(B5[j]);
	                            break;
	                        case 5:
	                            B5[j] = ret_integer(gf.add(B4[j-1],q[j-4]))^ret_integer(B3[j-2]);
	                            B5[j] = ret_a_power(B5[j]);
	                            break;
	                        default:
	                            break;
	                    }
	                    //cout << "B5[" << j << "] = a^" << B5[j] << "\t";
	                }
	                break;
	            /* Gia to B6 */
	            case 5:
	                for(int j=0;j<7;j++) {
	                    switch(j)
	                    {
	                        case 0:
	                            B6[j] = gf.add(B5[j],q[j]);
	                            break;
	                        case 1:
	                            B6[j] = ret_a_power(ret_integer(gf.add(B5[j-1],q[j]))^ret_integer(gf.add(B5[j],q[j-1])));
	                            break;
	                        case 2:
	                            B6[j] = ret_integer(gf.add(B5[j-1],q[j-1]))^ret_integer(gf.add(B5[j],q[j-2]));
	                            B6[j] ^= ret_integer(B4[j-2]);
	                            B6[j] = ret_a_power(B6[j]);
	                            break;
	                        case 3:
	                            B6[j] = ret_integer(gf.add(B5[j-1],q[j-2]))^ret_integer(gf.add(B5[j],q[j-3]));
	                            B6[j] ^= ret_integer(B4[j-2]);
	                            B6[j] = ret_a_power(B6[j]);
	                            break;
	                        case 4:
	                            B6[j] = ret_integer(gf.add(B5[j-1],q[j-3]))^ret_integer(gf.add(B5[j],q[j-4]));
	                            B6[j] ^= ret_integer(B4[j-2]);
	                            B6[j] = ret_a_power(B6[j]);
	                            break;
	                        case 5:
	                            B6[j] = ret_integer(gf.add(B5[j-1],q[j-4]))^ret_integer(gf.add(B5[j],q[j-5]));
	                            B6[j] ^= ret_integer(B4[j-2]);
	                            B6[j] = ret_a_power(B6[j]);
	                            break;
	                        case 6:
	                            B6[j] = ret_integer(gf.add(B5[j-1],q[j-5]))^ret_integer(B4[j-2]);
	                            B6[j] = ret_a_power(B6[j]);
	                            break;
	                        default:
	                            break;
	                    }
	                    //cout << "B6[" << j << "] = a^" << B6[j] << "\t";
	                }
	                break;
				/* Gia to B7 */
				case 6:
					for(int j=0;j<8;j++) {
						switch(j)
						{
							case 0:
								B7[j] = gf.add(B6[j], q[j]);
								break;
							case 1:
								B7[j] = ret_a_power(ret_integer(gf.add(B6[j-1],q[j]))^ret_integer(gf.add(B6[j],q[j-1])));
								break;
							case 2:
								B7[j] = ret_integer(gf.add(B6[j-1],q[j-1])) ^ ret_integer(gf.add(B6[j],q[j-2]));
								B7[j] ^= ret_integer(B5[j-2]);
								B7[j] = ret_a_power(B7[j]);
								break;
							case 3:
								B7[j] = ret_integer(gf.add(B6[j-1],q[j-2])) ^ ret_integer(gf.add(B6[j],q[j-3]));
								B7[j] ^= ret_integer(B5[j-2]);
								B7[j] = ret_a_power(B7[j]);
								break;
							case 4:
								B7[j] = ret_integer(gf.add(B6[j-1],q[j-3])) ^ ret_integer(gf.add(B6[j],q[j-4]));
								B7[j] ^= ret_integer(B5[j-2]);
								B7[j] = ret_a_power(B7[j]);
								break;
							case 5:
								B7[j] = ret_integer(gf.add(B6[j-1],q[j-4])) ^ ret_integer(gf.add(B6[j],q[j-5]));
								B7[j] ^= ret_integer(B5[j-2]);
								B7[j] = ret_a_power(B7[j]);
								break;
							case 6:
								B7[j] = ret_integer(gf.add(B6[j-1],q[j-5])) ^ ret_integer(gf.add(B6[j],q[j-6]));
								B7[j] ^= ret_integer(B5[j-2]);
								B7[j] = ret_a_power(B7[j]);
								break;
							case 7:
								B7[j] = ret_integer(gf.add(B6[j-1],q[j-6])) ^ ret_integer(B5[j-2]);
								B7[j] = ret_a_power(B7[j]);
								break;
							default:
								break;
							}
							//cout << "B7[" << j << "] = a^" << B7[j] << "\t";
					}
					break;
				/* Gia to B8 */
				case 7:
					for(int j=0;j<9;j++) {
						switch(j)
						{
							case 0:
								B8[j] = gf.add(B7[j],q[j]);
								break;
							case 1:
								B8[j] = ret_a_power(ret_integer(gf.add(B7[j-1],q[j]))^ret_integer(gf.add(B7[j],q[j-1])));
								break;
							case 2:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-1])) ^ ret_integer(gf.add(B7[j],q[j-2]));
								B8[j] ^= ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							case 3:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-2])) ^ ret_integer(gf.add(B7[j],q[j-3]));
								B8[j] ^= ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							case 4:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-3])) ^ ret_integer(gf.add(B7[j],q[j-4]));
								B8[j] ^= ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							case 5:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-4])) ^ ret_integer(gf.add(B7[j],q[j-5]));
								B8[j] ^= ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							case 6:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-5])) ^ ret_integer(gf.add(B7[j],q[j-6]));
								B8[j] ^= ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							case 7:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-6])) ^ ret_integer(gf.add(B7[j],q[j-7]));
								B8[j] ^= ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							case 8:
								B8[j] = ret_integer(gf.add(B7[j-1],q[j-7])) ^ ret_integer(B6[j-2]);
								B8[j] = ret_a_power(B8[j]);
								break;
							default:
								break;
						}
						//cout << "B8[" << j << "] = a^" << B8[j] << "\t";
					}
					break;
	            default:
	                break;
	        }

			/* Change m value to end divisions loop based on errors that have occurred */
			switch(dec_ec_level)
			{
				/* MEDIUM ECL */
				case 0:
					m += (ee) ? 3 : 0;
					m += (ee==2) ? 1 : 0;	// this may is 2 || check later
					m += (ee==3) ? 1 : 0;
					break;
				/* LOW ECL */
				case 1:
					m += (ee==1) ? 1 : 0;
					break;
				/* HIGH ECL */
				case 2:
					// HERE ADD LOGIC FOR HIGH ECL DIVISIONS LOOP EXIT
					m += (ee==1) ? 7 : 0;
					m += (ee==2) ? 6 : 0;
					m += (ee==3) ? 5 : 0;
					m += (ee==4) ? 4 : 0;
					m += (ee==5) ? 3 : 0;
					m += (ee==6) ? 2 : 0;
					m += (ee==7) ? 1 : 0;
					break;
				/* Q ECL */
				case 3:
					m += (ee) ? 5 : 0;
					m += (ee==2) ? 4 : 0;
					m += (ee==3) ? 3 : 0;
					m += (ee==4) ? 2 : 0;
					m += (ee==5) ? 1 : 0;
					break;
				default:
					break;
			}

	        gamma = (ee==1) ? B1[1] : gamma;
	        gamma = (ee==2) ? B2[2] : gamma;
	        gamma = (ee==3) ? B3[3] : gamma;
	        gamma = (ee==4) ? B4[4] : gamma;
	        gamma = (ee==5) ? B5[5] : gamma;
	        gamma = (ee==6) ? B6[6] : gamma;
			gamma = (ee==7) ? B7[7] : gamma;
			gamma = (ee==8) ? B8[8] : gamma;

	    	for(int i=0;i<N;i++) {
				old_r[i] = SS[i];
				SS[i] = (i==(N-1)) ? 0 : r[i+1];
        	}
        	m++;
    	}

		//cout << "\nERRORS = " << ee << "\n";
		//cout << "Gamma = a^" << gamma << "\n";

		/* Now calc Lambda(x) poly based on errors occured | Error Locator Polynomial */
		for(int i=0;i<(ee+1);i++) {
			Lambda[i] = (ee==1) ? (B1[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==2) ? (B2[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==3) ? (B3[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==4) ? (B4[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==5) ? (B5[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==6) ? (B6[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==7) ? (B7[i]-gamma) : Lambda[i];
			Lambda[i] = (ee==8) ? (B8[i]-gamma) : Lambda[i];
			Lambda[i] = (Lambda[i]<0) ? (255+Lambda[i]) : Lambda[i];
			//cout << "Lambda[" << i << "] = a^" << Lambda[i] << endl;
		}
		/* Next solve Lambda(x)=0 to find error locations */
		for(int i=1;i<256;i++) {	// I don't want i=0 cause layter it will cause bug in beta[] matrix, also same as i=255 eventually
				for(int j=0;j<ee+1;j++) {
					sum ^= ret_integer( gf.add(Lambda[j], gf.rs_power(ee+1-j, i)) );
				}
				roots[idx] = (!sum) ? 1 : 0;
				beta[idx] = (!sum) ? (255-i) : 0;
				if(roots[idx]) {
					//cout << "Lambda(a^" << i << ") = 0\n";
					//cout << "Beta[" << idx << "] = " << beta[idx] << endl;
				}
				idx += (roots[idx]) ? 1 : 0;
				sum=0;
		}
		/* Check if I have more errors than accepted by Algorithm (t=max errros accepted) | Only Detection in this situation | No Error Correction is Possible */
		gf.ErrDet(beta, ee, dec_ec_level);
		gf.Exception(beta, ee);
		/* Next calculate Error Magnitude Polynomial Omega(x) */
		for(int i=0;i<ee;i++) {
			Omega[i] = (ee==1) ? (ret_a_power(r[i+N-1])-gamma) : Omega[i];		// HIGH || Q || MEDIUM || LOW ECL
			Omega[i] = (ee==2) ? (ret_a_power(r[i+N-3])-gamma) : Omega[i];		// HIGH || Q || MEDIUM || LOW ECL
			Omega[i] = (ee==3) ? (ret_a_power(r[i+N-5])-gamma) : Omega[i];		// HIGH || Q || MEDIUM ECL
			Omega[i] = (ee==4) ? (ret_a_power(r[i+N-7])-gamma) : Omega[i];		// HIGH || Q || MEDIUM ECL
			Omega[i] = (ee==5) ? (ret_a_power(r[i+N-9])-gamma) : Omega[i];		// HIGH || Q ECL
			Omega[i] = (ee==6) ? (ret_a_power(r[i+N-11])-gamma) : Omega[i];		// HIGH || Q ECL
			Omega[i] = (ee==7) ? (ret_a_power(r[i+N-13])-gamma) : Omega[i];		// HIGH ECL
			Omega[i] = (ee==8) ? (ret_a_power(r[i+N-15])-gamma) : Omega[i];		// HIGH ECL
			Omega[i] = (Omega[i]<0) ? (255+Omega[i]) : Omega[i];
			//cout << "Omega[" << i << "] = a^" << Omega[i] << endl;
		}
		/* Now based on Lambda(x) && Omega(x) correct Rx polynomial based on how many errors (ee) occured */
		switch(ee)
		{
			/* 1 error occured in LOW || MEDIUM || Q || HIGH ECL */
			case 1:
				rx[25-beta[0]] ^= ret_integer(Omega[0]);
				//cout << "RX(" << 25-beta[0] << ") = " << rx[25-beta[0]] << endl;
				break;
			/* 2 errors occured in LOW || MEDIUM || Q || HIGH ECL */
			case 2:
				for(int i=0;i<2;i++) {
					num=0;
					for(int j=0;j<2;j++) {
						hold = (!j) ? (Omega[j]-beta[i]) : Omega[j];
						hold = (hold<0) ? (255+hold) : hold%255;
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					e[i] = (beta[i]+num-Lambda[1]);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				break;
			/* 3 errors occured in MEDIUM || Q || HIGH ECL */
			case 3:
				for(int i=0;i<3;i++) {
					num=0;
					denum=0;
					for(int j=0;j<3;j++) {
						hold = ((255-beta[i])*(2-j))%255;
						hold = (Omega[j]+hold)>255 ? ((Omega[j]+hold)%255) : (Omega[j]+hold);
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					for(int j=0;j<2;j++) {
						hold = ((255-beta[i])*(2-j))%255;
						/* Use Lambda coeffs instead of Lambda' | small poly */
						denum ^= (!j) ? ret_integer((hold+Lambda[j])%255) : ret_integer(Lambda[j+1]);
					}
					denum = ret_a_power(denum);
					e[i] = (beta[i]+num-denum);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				break;
			/* 4 errors occured in MEDIUM || Q || HIGH ECL */
			case 4:
				for(int i=0;i<4;i++) {
					num=0;
					denum=0;
					for(int j=0;j<4;j++) {
						hold = ((255-beta[i])*(3-j));
						hold = (hold>510) ? (hold%510) : hold%255;
						hold = (hold+Omega[j])%255;
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					for(int j=0;j<2;j++) {
						hold = (!j) ? ((255-beta[i])*2)%255 : 0;
						denum ^= (!j) ? ret_integer((hold+Lambda[j+1])%255) : ret_integer((hold+Lambda[j+2])%255);
					}
					denum = ret_a_power(denum);
					e[i] = (beta[i]+num-denum);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				break;
			/* 5 errors occured in Q || HIGH ECL */
			case 5:
				for(int i=0;i<5;i++) {
					num=0;
					denum=0;
					for(int j=0;j<5;j++) {
						hold = gf.rs_power(255-beta[i], 4-j);
						hold = gf.add(hold, Omega[j]);
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					for(int j=0;j<3;j++) {
						hold = (!j) ? gf.add(gf.rs_power(255-beta[i],4), Lambda[0]) : hold;
						hold = (j==1) ? gf.add(gf.rs_power(255-beta[i],2), Lambda[2]) : hold;
						hold = (j==2) ? Lambda[4] : hold;
						denum ^= ret_integer(hold);
					}
					denum = ret_a_power(denum);
					e[i] = (beta[i]+num-denum);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				break;
			/* 6 errors occured in Q || HIGH ECL  */
			case 6:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				for(int i=0;i<6;i++) {
					num=0;
					denum=0;
					for(int j=0;j<6;j++) {
						hold = gf.rs_power(255-beta[i], 5-j);
						hold = gf.add(hold, Omega[j]);
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					for(int j=0;j<3;j++) {
						// logic for calulation of Lambda'(xj^-1)
						hold = (!j) ? gf.add(gf.rs_power(255-beta[i],4), Lambda[1]) : hold;
						hold = (j==1) ? gf.add(gf.rs_power(255-beta[i],2), Lambda[3]) : hold;
						hold = (j==2) ? Lambda[5] : hold;
						denum ^= ret_integer(hold);
					}
					denum = ret_a_power(denum);
					e[i] = (beta[i]+num-denum);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				break;
			/* 7 errors occured in HIGH ECL */
			case 7:
				for(int i=0;i<7;i++) {
					num=0;
					denum=0;
					for(int j=0;j<7;j++) {
						hold = gf.rs_power(255-beta[i], 6-j);
						hold = gf.add(hold, Omega[j]);
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					for(int j=0;j<4;j++) {
						hold = (!j) ? gf.add(gf.rs_power(255-beta[i],6), Lambda[0]) : hold;
						hold = (j==1) ? gf.add(gf.rs_power(255-beta[i],4), Lambda[2]) : hold;
						hold = (j==2) ? gf.add(gf.rs_power(255-beta[i],2), Lambda[4]) : hold;
						hold = (j==3) ? Lambda[6] : hold;
						denum ^= ret_integer(denum);
					}
					e[i] = (beta[i]+num-denum);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				break;
			/* 8 errors occured in HIGH ECL */
			case 8:
				for(int i=0;i<8;i++) {
					num=0;
					denum=0;
					for(int j=0;j<8;j++) {
						hold = gf.rs_power(255-beta[i], 7-j);
						hold = gf.add(hold, Omega[j]);
						num ^= ret_integer(hold);
					}
					num = ret_a_power(num);
					for(int j=0;j<4;j++) {
						hold = (!j) ? gf.add(gf.rs_power(255-beta[i],6), Lambda[1]) : hold;
						hold = (j==1) ? gf.add(gf.rs_power(255-beta[i],4), Lambda[3]) : hold;
						hold = (j==2) ? gf.add(gf.rs_power(255-beta[i],2), Lambda[5]) : hold;
						hold = (j==3) ? Lambda[7] : hold;
						denum ^= ret_integer(hold);
					}
					denum = ret_a_power(denum);
					e[i] = (beta[i]+num-denum);
					e[i] = (e[i]<0) ? (255+e[i]) : e[i]%255;
					rx[25-beta[i]] ^= ret_integer(e[i]);
					//cout << "RX(" << 25-beta[i] << ") = " << rx[25-beta[i]] << endl;
				}
				break;
			default:
				cout << "An error occured. Aborting\n";
				exit(1);
				break;
		}

}


/* -------------------------------------------------------------------------------------------------- */
/* Decode numeric - alphanumeric - byte based on corrected R(x) polynomial and corrected bit stream */
/* ---------------------------------------------------------------------------------------------- */
void V1_DECODE::correct(void)
{
	/* Now from Rx poly correct v1_bit_stream[CML] in order ot have  the correct information */
	int tmp[8], idx=0;
	for(int i=0;i<RX_LEN;i++) {
		dec2bin(rx[i], tmp);
		for(int j=0;j<8;j++) {
			//cout << "\nOriginal v1_bit_stream[" << idx << "] = " << v1_bit_stream[idx];
			v1_bit_stream[idx++] = tmp[j];
			//cout << "/ " << v1_bit_stream[idx] << endl;
		}
	}

	/* Now I have corrected the corrupted bit-stream , so I can decode my bit-stream */
	switch(dec_MI)
	{
		case 1:
			decode_numeric( );
			break;
		case 2:
			decode_alpha( );
			break;
		case 4:
			decode_byte( );
			break;
		default:
			cout << "ERROR in R-S bit stream correction. Aborting...";
			newline;
			exit(1);
	}
}


/* ------------------------------------------------------------------ */
/*             Main Packet for v1_decode.h Header File              */
/* -------------------------------------------------------------- */
void V1_DECODE::v1_decode(int v1[V1_SIZE][V1_SIZE])
{
	if(unix) {
		system("clear");
	} else {
		system("cls");
	}
	cout << setfill('-') << setw(80); newline;
	cout << "\t\t\tQR CODE SYMBOL DECODING \n";
	cout << setfill('-') << setw(80); newline; newline;
	/* Call first all previous header files functions */
	v1_data_retrieval_packet(v1);
	/* Choose Decode Mode to apply using this header file now */
	choose_v1_decode_mode();
	/* Find Mode Indicator and Character Count Idicator in Dec */
	find_params();
	/* Also, create LFSR (for Version 1 QR Codes) ECC's with LFSR Circuit */
	create_v1_lfsr_errors();
	/* Next, check decode algorithm based on ECC's from matrix && ECC's from LFSR Circuit */
	check_v1_decode();
	/* cout << ( (!V1_GLOBAL_CHECK) ? "No Error Correction Needed.\n" : "\n"); */
	/* Reed Solomon Error Correction Algorithm */
	/* Setting R(x) polynomial first */
	set_rx();
	/* Calculating Syndromes */
	calc_syndromes();
	if(RS_CHECK) {
		cout << "\nApplying Reed - Solomon Error Correction Algorithm...";
		newline;
		/* Calculating error-locator-polynomial && error-magnitude polynomial && set error values */
		/*
		cout << "\nBEFORE CORRECTION: \n";
		switch(dec_MI)
		{
			case 1:
				decode_numeric();
				break;
			case 2:
				decode_alpha();
				break;
			case 4:
				decode_byte();
				break;
			default:
				cout << "No Mode Was Choosed. ERROR!\nAborting...";
				newline;
				exit(1);
				break;
		}
		*/
		cout << "\nAFTER CORRECTION: \n";
		rs_eucl();
		/* Decoding based on correct R(x) polynomial and correct bit-stream */
		correct();
	} else {
		//cout << "No Error - Correction Algorithm Needed.\n";
		/* Now based on choose_decode_mode variable decode Numeric || Alpha || Byte Characters */
		switch(dec_MI)
		{
			case 1:
				decode_numeric();
				break;
			case 2:
				decode_alpha();
				break;
			case 4:
				decode_byte();
				break;
			default:
				cout << "No Mode Was Choosed. ERROR!\nAborting...";
				newline;
				exit(1);
				break;
		}
	}
}
#endif /* v1_decode.h */
