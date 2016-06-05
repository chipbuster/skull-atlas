#ifndef ZEROBLOCKTABLE_H
#define ZEROBLOCKTABLE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

void zeroblocktable_generation(int newsize1,float *lambda1,int *zeroblocktable1,int type);
//type==1 BLOCK
//type==2 CELL

void zeroblocktable_writing(int newsize1,int indexx1,int indexy1,int indexz1,int whetherzeroblock1,int *zeroblocktable1,int type);
//type==1 BLOCK
//type==2 CELL

int size_zeroblocktable(int newsize1,int type);
//type==1 BLOCK
//type==2 CELL

void empty_zeroblocktable(int zeroblocktable1size,int *zeroblocktable1);
//type==1 BLOCK
//type==2 CELL

void print_zeroblocktable(int zeroblocktable1size,int *zeroblocktable1);
//type==1 BLOCK
//type==2 CELL

float zeroblocktable_zeroblockrate(int *zeroblocktable1,int table_size);

void writebit(int *celltable1,int celltable1_size,int celltable1_index,int cell1_index,int outputbit1);
//write one bit(outputbit1) in the cell1_index position(bit rotating) at the celltable1_index of celltable1

void print_bit_table_partial(int *table1,int begin1,int end1);
//8 bit in one byte, such as 10110101
//print partial table1
//begins at begin1, ends at end1

int countbit(int *table1,int table1_index_begin,int table1_rotate_index_begin,int table1_index_end,int table1_rotate_index_end);
//count the bit from table1_index_begin,table1_rotate_index_begin(rotate)  
//to table1_index_end,table1_rotate_index_end(rotate)

int celltable_generation(float *lambda1,int *celltable1,int celltable1_size);
//generate the celltable for one block
//return the NNNC of one block

void empty_table(int *table1,int begin1,int end1);
//empty the table from begin1 to end1

int table_size_block(int type,int CIA_number);
//return the table size of a block
//type=1 ZSMS table size(CIA_number is used)
//type=2 CBFT table size(CIA_number is not used)
//type=3 ZSMS table span for a single cell(CIA_number is not used)

void DIS_generation_cell_ver1(float *block1,int *DIS1,int previousDISindex,int *currentDISindex);
//DIS generation when ZSMS type is 1

void DIS_generation_cell_ver3(float *block1,int *DIS1,int previousDISindex,int *currentDISindex);
//DIS generation when ZSMS type is 3
//8 vertexes +
//non zero elements that are not among 8 vertexes

void cellinformationarray_generation_cell_ver1(float *block1,int *cell_information_array1,int prev_CIAindex1,int *current_CIAindex1,int ZSMSindex_begin1,int ZSMSindex_next_begin1);
//cell information array generation when ZSMS type is 1
//generate the cellinformationarrary for one cell
//zerobit offset is stored in the cell_information_array1
//ZSMSindex_begin1 & ZSMSindex_next_begin1 are not used in this case

void cellinformationarray_generation_cell_ver3(float *block1,int *cell_information_array1,int prev_CIAindex1,int *current_CIAindex1,int ZSMSindex_begin1,int ZSMSindex_next_begin1);
//cell information array generation when ZSMS type is 3
//generate the cellinformationarrary for one cell
//zerobit offset is stored in the cell_information_array1
//ZSMSindex_begin1 & ZSMSindex_next_begin1 are generated from ZSMStable_generation_cell_ver3

void ZSMStable_generation_cell(float *block1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1);
//generate the ZSMStable for one cell
//block1 is a cell

int vertex(int x,int y,int z,int cellsize1);
//return 1 if [x,y,z] is a vertex
//return 0 if [x,y,z] is not a vertex

int table_generation_block(int index1, float *block1,int *table1,int *cell_information_array1,int prev_CIAindex1,int *current_CIAindex1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1,int *DIStable1,int prev_DISindex1,int *current_DISindex1,int ZSMStype1);
//generate the table for a block
//table1 is the CBFT table
//return the value of NNNC of this block

void table_generation(int dim1[3],float *lambda1,int dimension1,int ZSMStype1);
//generate the table for the whole data
//dimension is unused now

int get_bit_from_bit_table(int *table1,int rowindex1,int rotationindex1);
#endif





