#ifndef ENCODE_H
#define ENCODE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

void output_data(int size1,float *inputcell1,int currentlevel1,FILE *resultfp);
//output data at one level

void print_output_data(int size1,float *inputcell1,int currentlevel1,FILE *resultfp);
//print output data at one level

void encode_whole_data(const char* fname,int dim1[3],float *func1);
//write all data into a file

void encode_program(const char* fname,int dim1[3],float *func1);
#endif





