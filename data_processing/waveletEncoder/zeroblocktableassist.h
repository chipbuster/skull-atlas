#ifndef ZEROBLOCKTABLEASSIST_H
#define ZEROBLOCKTABLEASSIST_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

void ZSMSoutputbit(float *block1,int *ZSMStable1,int xpos,int ypos,int zpos,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex);
//block1 should be a cell

void ZSMSoutputbit16bit(float *block1,int *ZSMStable1,int xpos,int ypos,int zpos,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex,int *counter,int *currentbit);

void ZSMSEdges(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex,int xbegin,int xend,int xstep,int ybegin,int yend,int ystep,int zbegin,int zend,int zstep);

void ZSMSEdgesVer3(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex, int *ZSMStable1_LastLevelrowindex,int *ZSMStable1_LastLevelrotateindex,int *counter,int xbegin,int xend,int xstep,int ybegin,int yend,int ystep,int zbegin,int zend,int zstep);

void ZSMSLastLevelIndex(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex);

void ZSMSLastLevel(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex,int *ZSMStable1_LastLevelrowindex);

void ZSMStable_generation_cell_ver2(float *block1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1);

void ZSMStable_generation_cell_ver3(float *block1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1);

void EnterLevelBit(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex);
//if all elements in currentLevel are 0, put 0
//else put 1

void EnterAllLevelBit(float *block1,int *ZSMStable1,int size1,int maxLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex);
//enter the level bit at all levels

//int GetLevelBit(float *block1,int *ZSMStable1,int size1,int currentLevel,int ZSMStable1_begin_row_levelbit);
//get the bit that indicates whether all elements in currentLevel are all zero
//if 1 is got, not all elements are 0
//if 0 is got, all elements are 0
//bit begins with level 1
//the bit begins at row of ZSMStable1_begin_row_levelbit

int GetNextLevelBit(int *ZSMStable1,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex);
//the same as the one decode.cpp
#endif





