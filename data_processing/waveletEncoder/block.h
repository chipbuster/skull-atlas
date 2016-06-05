#ifndef BLOCK_H
#define BLOCK_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

#define LARGENEGATIVE -1.0e-32

int paddingsize(int size1);
//returns the size of the new data

int paddingsizever2(int dim1[3]);

void padding(int oldsize1,int newsize1,float *lambda1,float* lambdanew1);

void paddingver2(int dim1[3],int newsize1,float *lambda1,float* lambdanew1);

void paddingver3(int dim[3],int dimnew[3],float *lambda1,float* lambdanew1);

int zeroblock(float *block1,int type);
//type==1 BLOCK
//type==2 CELL
//return 1 for zero block
//return 0 for non-zero block

void copyblock(int newsize1,int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type);
//type==1 BLOCK
//type==2 CELL
//copy the block from [indexx1,indexy1,indexz1] in lambdanew1 to block1
//lambdanew1 has the size of newsize1

void reversecopyblock(int newsize1,int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type);
//type==1 BLOCK
//type==2 CELL
//copy block1 to the block from [indexx1,indexy1,indexz1] in lambdanew1
//lambdanew1 has the size of newsize1

void copyblockver2(int dim1[3],int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type);

void reversecopyblockver2(int dim1[3],int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type);

void print_block(float *block1,int type);
//type==1 BLOCK
//type==2 CELL

void print_block_from_whole(int newsize1,float *block1,int type,int startx1,int starty1,int startz1);
//type==1 BLOCK
//type==2 CELL

float fabs_maximum_block(float *block1,int newsize1);
//print the maximum in the block

void print_volume(float *block1,int newsize1);
//print the whole volume with size newsize1^3

void copy_volume(float *newvolume1,float *oldvolume1,int newsize1);
//copy the block from newvolume1 to oldvolume1
//newvolume1 and oldvolume1 have the size of newsize1

void cell_coordinate(int index,int *x_coor,int *y_coor,int *z_coor);
//given the index, 
//calculate the x_coor,y_coor,z_coor in a cell

void cell_coordinate(int index,int *x_coor,int *y_coor,int *z_coor);
//given the index, 
//calculate the x_coor,y_coor,z_coor in a cell

int zeroelement(float *block1,int x_coor,int y_coor,int z_coor); 
//tell if a cell is a zero element at coordinate (x,y,z)
//return 1 if zeroelement

void copyzeroblock(int newsize1,float *output1);
//copy zero to output1 of size newsize1 

int equalblock(int newsize1,float *block1,float *block2);
//if block1 and block2 are equal, output 1
//otherwise, output 0

void output_rawiv(const char* fname,int newsize1,float *func1);
//output the data in func1 of size newsize1*newsize1*newsize1 into fname
#endif





