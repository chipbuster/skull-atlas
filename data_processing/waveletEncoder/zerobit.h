#ifndef ZEROBIT_H
#define ZEROBIT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

//The size of the cell should be 2^n+1

#define BLOCKSIZE 27
#define CELLSIZE 9

/*
#define BLOCKSIZE 27
#define CELLSIZE 3
*/

/*
#define BLOCKSIZE 27
#define CELLSIZE 9
*/

#define VERYSMALLNUMBER 0.000000001
//#define VERYSMALLNUMBER 1.0e-32

void cellprocessing(float epsilon,float maxval,float* func,float* lambda,float* recon);
//compression and reconstruction of a cell

void blockprocessing(float epsilon,float maxval,float* func,float* lambda,float* recon);
//compression and reconstruction of a block

void volumeprocessing(float epsilon,float maxval,int dim1[3],float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3);
//compression and reconstruction of a volume
//error estimation is done

/*
void blockprocessingerror(float epsilon,float maxval,int newsize1,float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3);

void cellprocessingerror(float epsilon,float maxval,int newsize1,float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3);
*/

void extracalculation(int oldsize1,int newsize1,float epsilon,float* func,float* lambda,float maxval);
//output the compression ratio including the zerobit table 
//1) rawiv file size 
//2) nonzero wavelet coefficients and zerobit encoding file size
//3) compression ratio=1)/2)
#endif













