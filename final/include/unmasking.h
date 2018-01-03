/* File   : unmasking.h
 * Author : dimos katsimardos
 * Created November 7, 2016, 4:44 PM
 *
 * Header file with the functions that when they apply in a M1 Micro QR Code
 * will return the un-masked M1 Micro QR Code Matrix
 */
#ifndef UNMASKING_H
#define UNMASKING_H

#include "format_string.h"		// Cause I need the 2 bits for mask, from mask_info[2] matrix
#include "bin2dec_v2.h"

class M1_UNMASKING : public M1_FORMAT_STRING {
public:
	int mask_choosed;		   // 0 --> mask=00  || 1 --> mask=01  || 2 --> mask=10  || 3 --> mask=11
	int mask00[M1_SIZE][M1_SIZE], mask01[M1_SIZE][M1_SIZE], mask10[M1_SIZE][M1_SIZE], mask11[M1_SIZE][M1_SIZE];
	/* MEMBER FUNCTIONS */
	void find_mask();
	void init(int m1[M1_SIZE][M1_SIZE]);
	void show(int mat[M1_SIZE][M1_SIZE]);
	void copy_m1(int m1[M1_SIZE][M1_SIZE], int tmp[M1_SIZE][M1_SIZE]);
	void set_mask00();
	void set_mask01();
	void set_mask10();
	void set_mask11();
	void unmask00(int m1[M1_SIZE][M1_SIZE]);
	void unmask01(int m1[M1_SIZE][M1_SIZE]);
	void unmask10(int m1[M1_SIZE][M1_SIZE]);
	void unmask11(int m1[M1_SIZE][M1_SIZE]);
	void unmask(int m1[M1_SIZE][M1_SIZE]);
};

// First, let's check which mask is used based on my M1 Micro QR Code matrix
// and specifically my mask_info array ::
void M1_UNMASKING::find_mask(void)
{
	mask_choosed = bin2dec(mask_info, 2);
}

/* An __init__ function to initialize my M1 Matrix when I need to inside un-masking functions */
void M1_UNMASKING::init(int m1[M1_SIZE][M1_SIZE])
{
	for(int i=0;i<M1_SIZE;i++)
		for(int j=0;j<M1_SIZE;j++)
			m1[i][j]=0;
}

/* Next, a function that prints a M1 QR Code Matrix */
void M1_UNMASKING::show(int mat[M1_SIZE][M1_SIZE])
{
	newline;
	for(int i=0;i<M1_SIZE;i++) {
		cout << "\t\t\t\t";
		for(int j=0;j<M1_SIZE;j++) {
			cout << ( (mat[i][j]==1) ? "X " : "O " );
		}
		newline;
	}
	newline;
}

/* Another help function to copy my m1 matrix to a temporary tmp array
* Without this copy function my whole algorithm blows down cause of
* too much traffic!!
*/
void M1_UNMASKING::copy_m1(int m1[M1_SIZE][M1_SIZE], int tmp[M1_SIZE][M1_SIZE])
{
	for(int i=0;i<M1_SIZE;i++)
		for(int j=0;j<M1_SIZE;j++)
			tmp[i][j]=m1[i][j];
}

/* First, let's set all masks to the proper arrays */
void M1_UNMASKING::set_mask00(void)
{
	/* Initialize mask 00 and m1 matrix first */
	init(mask00);
	/* Now I apply codition wherever it is necessary */
	/* I cut the data area in #2 areas, vertical and horizontal */
	/* And I put 1(one) where condition = true */
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			mask00[i][j] = !(i%2) ? 1 : 0;
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			mask00[i][j] = !(i%2) ? 1 : 0;
		}
	}
}

void M1_UNMASKING::set_mask01(void)
{
	init(mask01);
	/* Fill my mask01 matrix with 1, where condition is true */
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			mask01[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			mask01[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
		}
	}
}

void M1_UNMASKING::set_mask10(void)
{
	init(mask10);
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			mask10[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			mask10[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
}

void M1_UNMASKING::set_mask11(void)
{
	init(mask11);
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			mask11[i][j] = !((((i+j)%2)+((i*j)%3))%2) ? 1 : 0;
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			mask11[i][j] = !((((i+j)%2)+((i*j)%3))%2) ? 1 : 0;
		}
	}
}

/*    UNMASKING PROCEDURES NEXT     */
/* Unmasking procedure for mask 00 */
void M1_UNMASKING::unmask00(int m1[M1_SIZE][M1_SIZE])
{
	int tmp[M1_SIZE][M1_SIZE];
	copy_m1(m1, tmp);
	set_mask00();
	// Now that I have a mask00 matrix with positions marked for condition imod2==0 with 1,
	// check my m1 matrix and reverse bits where condition is true ::
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			m1[i][j] = (mask00[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask00[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			m1[i][j] = (mask00[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask00[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
}

/* Unmasking procedure for mask 01 */
void M1_UNMASKING::unmask01(int m1[M1_SIZE][M1_SIZE])
{
	int tmp[M1_SIZE][M1_SIZE];
	copy_m1(m1, tmp);
	set_mask01();
	// Now I will reverse me m1 matrix bits, where it is needed ::
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			m1[i][j] = (mask01[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask01[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			m1[i][j] = (mask01[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask01[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
}

/* Unmasking procedure for mask 10 */
void M1_UNMASKING::unmask10(int m1[M1_SIZE][M1_SIZE])
{
	int tmp[M1_SIZE][M1_SIZE];
	copy_m1(m1, tmp);
	set_mask10();
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			m1[i][j] = (mask10[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask10[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			m1[i][j] = (mask10[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask10[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
}

/* Unmasking procedure for mask 11 */
void M1_UNMASKING::unmask11(int m1[M1_SIZE][M1_SIZE])
{
	int tmp[M1_SIZE][M1_SIZE];
	copy_m1(m1, tmp);
	set_mask11();
	for(int i=1;i<M1_SIZE;i++) {
		for(int j=9;j<M1_SIZE;j++) {
			m1[i][j] = (mask11[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask11[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
	for(int i=9;i<M1_SIZE;i++) {
		for(int j=1;j<9;j++) {
			m1[i][j] = (mask11[i][j]==1 && tmp[i][j]==1) ? 0 : m1[i][j];
			m1[i][j] = (mask11[i][j]==1 && tmp[i][j]==0) ? 1 : m1[i][j];
		}
	}
}

/* Now my main function that calls the proper unmasking function based on mask_choosed variable */
void M1_UNMASKING::unmask(int m1[M1_SIZE][M1_SIZE])
{
	/* Step 0 :: Call functions from the format_string.h to fill the format string arrays */
	format_packet(m1);
	/* Step 1 :: Find the mask that is used / calc mask_choosed variable  */
	find_mask();
	//cout << "\nMask CHECK :: " << mask_info[1] << mask_info[0]; newline;
	/* Now apply un-masking procedure based on mask that is used */
	switch(mask_choosed)
	{
		case 0:
			unmask00(m1);
			break;
		case 1:
			unmask01(m1);
			break;
		case 2:
			unmask10(m1);
			break;
		case 3:
			unmask11(m1);
			break;
		default:
			break;
	}
}
#endif /* unmasking.h */
