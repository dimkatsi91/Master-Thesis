/* File   : V1_unmasking.h
 * Author : dimos katsimardos
 * Created on November 9, 2016, 10:08 AM
 *
 * Header file with functions for unmasking procedure for V1 QR Code Matrix
 * that when apply to the matrix unmask it based on mask choosed during encoding procedure
 */
#ifndef V1_UNMASKING_H
#define V1_UNMASKING_H

#include "V1_format_string.h"


class V1_UNMASKING : public V1_FORMAT_STRING {
public:
    int v1_mask_choosed;	// 0-->000 || 1-->001 || 2-->010 || 3-->011 || 4-->100 || 5-->101 || 6-->110 || 7-->111
    int mask000[V1_SIZE][V1_SIZE], mask001[V1_SIZE][V1_SIZE], mask010[V1_SIZE][V1_SIZE], mask011[V1_SIZE][V1_SIZE],
        mask100[V1_SIZE][V1_SIZE], mask101[V1_SIZE][V1_SIZE], mask110[V1_SIZE][V1_SIZE], mask111[V1_SIZE][V1_SIZE];
                    /* MEMBER FUNCTIONS */
    void init_v1(int v1[V1_SIZE][V1_SIZE]);
    void show_v1(int v1[V1_SIZE][V1_SIZE]);
    void copy_v1(int v1[V1_SIZE][V1_SIZE], int tmp[V1_SIZE][V1_SIZE]);
    void find_v1_mask();
    void set_mask000();
    void set_mask001();
    void set_mask010();
    void set_mask011();
    void set_mask100();
    void set_mask101();
    void set_mask110();
    void set_mask111();
    void unmask000(int v1[V1_SIZE][V1_SIZE]);
    void unmask001(int v1[V1_SIZE][V1_SIZE]);
    void unmask010(int v1[V1_SIZE][V1_SIZE]);
    void unmask011(int v1[V1_SIZE][V1_SIZE]);
    void unmask100(int v1[V1_SIZE][V1_SIZE]);
    void unmask101(int v1[V1_SIZE][V1_SIZE]);
    void unmask110(int v1[V1_SIZE][V1_SIZE]);
    void unmask111(int v1[V1_SIZE][V1_SIZE]);
    void v1_unmask(int v1[V1_SIZE][V1_SIZE]);
};

/* Initialization, Show and Copy Functions for Version 1 QR Code Matrix */
void V1_UNMASKING::init_v1(int v1[V1_SIZE][V1_SIZE])
{
    for(int i=0;i<V1_SIZE;i++) {
		for(int j=0;j<V1_SIZE;j++) {
			v1[i][j] = 0;
		}
	}
}

void V1_UNMASKING::show_v1(int v1[V1_SIZE][V1_SIZE])
{
    newline;
    for(int i=0;i<V1_SIZE;i++) {
	    arrange;
		for(int j=0;j<V1_SIZE;j++) {
	    	cout << ( !v1[i][j] ? "O " : "X "  );
		}
		newline;
    }
}

/* Essential Function for Overall Algorithm */
void V1_UNMASKING::copy_v1(int v1[V1_SIZE][V1_SIZE], int tmp[V1_SIZE][V1_SIZE])
{
    for(int i=0;i<V1_SIZE;i++) {
		for(int j=0;j<V1_SIZE;j++) {
	    	tmp[i][j] = v1[i][j];
    	}
	}
}

/* First, based on v1_mask_info[3] matrix, let's set mask_choosed variable to its value */
void V1_UNMASKING::find_v1_mask(void)
{
	v1_mask_choosed = bin2dec(v1_mask_info, 3);								// Returns 0--7 for masks 000--111 respectively
	//cout << "Mask choosed variable :: " << mask_choosed; newline;
}

/* Now, the set_maskxxx functions, where xxx :: 000...111
 * With these functions we fill the maskxxx arrays with 1's
 * where the appropriate condition is true.
 */
void V1_UNMASKING::set_mask000(void)
{
    int i,j;
    init_v1(mask000);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask000[i][j] = !((i+j)%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask000[i][j] = !((i+j)%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask000[i][j] = !((i+j)%2) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask000[i][j] = !((i+j)%2) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask000[i][j] = !((i+j)%2) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask001(void) {
    int i,j;
    init_v1(mask001);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask001[i][j] = !(i%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask001[i][j] = !(i%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask001[i][j] = !(i%2) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask001[i][j] = !(i%2) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask001[i][j] = !(i%2) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask010(void)
{
    int i,j;
    init_v1(mask010);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask010[i][j] = !(j%3) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask010[i][j] = !(j%3) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask010[i][j] = !(j%3) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask010[i][j] = !(j%3) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask010[i][j] = !(j%3) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask011(void)
{
    int i,j;
    init_v1(mask011);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask011[i][j] = !((i+j)%3) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask011[i][j] = !((i+j)%3) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask011[i][j] = !((i+j)%3) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask011[i][j] = !((i+j)%3) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask011[i][j] = !((i+j)%3) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask100(void)
{
    int i,j;
    init_v1(mask100);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask100[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask100[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask100[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
    	}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask100[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask100[i][j] = !(((i/2)+(j/3))%2) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask101(void)
{
    int i,j;
    init_v1(mask101);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask101[i][j] = !(((i*j)%2+(i*j)%3)) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask101[i][j] = !(((i*j)%2+(i*j)%3)) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask101[i][j] = !(((i*j)%2+(i*j)%3)) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask101[i][j] = !(((i*j)%2+(i*j)%3)) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask101[i][j] = !(((i*j)%2+(i*j)%3)) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask110(void)
{
    int i,j;
    init_v1(mask110);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask110[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask110[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask110[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask110[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask110[i][j] = !(((i*j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
}

void V1_UNMASKING::set_mask111(void)
{
    int i,j;
    init_v1(mask111);
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
			mask111[i][j] = !(((i+j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	mask111[i][j] = !(((i+j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	mask111[i][j] = !(((i+j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	mask111[i][j] = !(((i+j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	mask111[i][j] = !(((i+j)%2+(i*j)%3)%2) ? 1 : 0;
		}
	}
}

/* UNMASKING PROCEDURES NEXT || 8 unmaskxxx functions */
/* Unmasking procedure for mask 000 */
void V1_UNMASKING::unmask000(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask000();
    /* Now I have set my mask000 matrix based on which I will apply my
     * unmasking procedure for 000 mask, i.e. mask_choosed variable = 0
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask000[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 001 */
void V1_UNMASKING::unmask001(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask001();
    /* Now I have set my mask001 matrix based on which I will apply my
     * unmasking procedure for 001 mask, i.e. mask_choosed variable = 1
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	   		v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask001[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 010 */
void V1_UNMASKING::unmask010(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask010();
    /* Now I have set my mask001 matrix based on which I will apply my
     * unmasking procedure for 001 mask, i.e. mask_choosed variable = 2
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask010[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 011 */
void V1_UNMASKING::unmask011(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask011();
    /* Now I have set my mask011 matrix based on which I will apply my
     * unmasking procedure for 011 mask, i.e. mask_choosed variable = 3
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask011[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 100 */
void V1_UNMASKING::unmask100(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask100();
    /* Now I have set my mask100 matrix based on which I will apply my
     * unmasking procedure for 100 mask, i.e. mask_choosed variable = 4
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask100[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 101 */
void V1_UNMASKING::unmask101(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask101();
    /* Now I have set my mask101 matrix based on which I will apply my
     * unmasking procedure for 101 mask, i.e. mask_choosed variable = 5
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask101[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 110 */
void V1_UNMASKING::unmask110(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask110();
    /* Now I have set my mask110 matrix based on which I will apply my
     * unmasking procedure for 110 mask, i.e. mask_choosed variable = 6
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask110[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Unmasking procedure for mask 111 */
void V1_UNMASKING::unmask111(int v1[V1_SIZE][V1_SIZE])
{
    int i,j,tmp[V1_SIZE][V1_SIZE];
    copy_v1(v1, tmp);
    set_mask111();
    /* Now I have set my mask111 matrix based on which I will apply my
     * unmasking procedure for 111 mask, i.e. mask_choosed variable = 7
     */
    for(i=9;i<V1_SIZE;i++) {
		for(j=9;j<V1_SIZE;j++) {
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=0;j<6;j++) {
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=9;i<13;i++) {
		for(j=7;j<9;j++) {
	   		v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=7;i<9;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
    for(i=0;i<6;i++) {
		for(j=9;j<13;j++) {
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==1) ? 0 : v1[i][j]);
	    	v1[i][j] = ((mask111[i][j]==1 && tmp[i][j]==0) ? 1 : v1[i][j]);
		}
    }
}

/* Now I group my unmask header file using v1_format_string.h header file
 * and mask_choosed variable to apply the correct unmaskxxx function
 */
void V1_UNMASKING::v1_unmask(int v1[V1_SIZE][V1_SIZE])
{

    /* Step 0 :: Call packet from format_string.h file to fill the format string arrays */
    v1_format_string_packet(v1);
    /* Step 1 :: Find mask that is used and set mask_choosed variable */
    find_v1_mask();
    /* Step 2 :: Based on mask_choosed variable apply correct unmaskxxx function */
    //cout << "Used Mask Check :: " << v1_masks[v1_mask_choosed]; newline;
    switch(v1_mask_choosed)
    {
	case 0:
	    unmask000(v1);
	    break;
	case 1:
	    unmask001(v1);
	    break;
	case 2:
	    unmask010(v1);
	    break;
	case 3:
	    unmask011(v1);
	    break;
	case 4:
	    unmask100(v1);
	    break;
	case 5:
	    unmask101(v1);
	    break;
	case 6:
	    unmask110(v1);
	    break;
	case 7:
	    unmask111(v1);
	    break;
	default:
	    cout << "No unmasking procedure is applied. ERROR!"; newline;
	    break;
    }
}
#endif /* v1_unmasking.h */
