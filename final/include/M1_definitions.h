/* File   : M1_definitions.h
 * Author : dimos katsimardos
 * Created no April 5, 2017, 4:19 PM
 *
 * This header file contains all definitions
 * about matrices and sizes that are used for
 * M1 QR Codes decoding procedure
 */
#ifndef M1_DEFINITIONS_H
#define M1_DEFINITIONS_H

/* ---------------------------------------------- */
/*              M1_format_string.h              */
/* ------------------------------------------- */
#include <iostream>
using std::cout;

#define M1_SIZE 11
#define newline { cout << "\n"; }


/* ---------------------------------------------- */
/*                M1_Unmasking.h                */
/* ------------------------------------------- */
;

/* ---------------------------------------------- */
/*              M1_data_retrieval.h              */
/* ------------------------------------------- */
#define tipose(X) { for(int i=0;i<8;i++) { cout << X[i]; } }
#define tipose2(Y) { for(int i=0;i<4;i++) { cout << Y[i]; } }
#define maximum 36										    // When I have 5 numbers encoded in M1 Micro QR Code | D1-D2-E1-E2(8bits) & D3(4bits)

#include <string.h>												  //    strcpy  function
#include <iomanip>												 //     setfill function
using std::setfill;
using std::setw;

#endif /* M1_definitions.h */
