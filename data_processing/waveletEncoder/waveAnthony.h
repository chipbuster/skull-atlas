#ifndef WAVEANTHONY_H
#define WAVEANTHONY_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

#define acess(pointername, size, k, j, i) pointername[((size)*(size)*(k)+(size)*(j)+(i))]

void doXEdges(int size, int currentLevel,float* lambda);
void doYEdges(int size, int currentLevel,float* lambda);
void doZEdges(int size, int currentLevel,float *lambda);
void threshlevel(int size, int currentLevel,float maxval,float epsilon, float *lambda);
void doSubtraction(int size,float *f, float *lam); 
void wavelet2Helper(int size, int currentLevel,float maxval,float epsilon,float *lambda);
void wavelet2(int size,float maxval,float epsilon,float *func,float* lambda);
void rebuildXEdges(int size, int currentLevel,float* recon);
void rebuildYEdges(int size, int currentLevel,float *recon);
void rebuildZEdges(int size, int currentLevel,float *recon);
void addXEdges(int size, int currentLevel,float *recon,float* lambda);
void addYEdges(int size, int currentLevel,float *recon,float* lambda);
void addZEdges(int size, int currentLevel,float* recon,float* lambda);
void rebuildWavelet2Helper(int size, int currentLevel,float* recon);
void doAddition(int size, int currentLevel,float* recon, float* lambda); 
void rebuildWavelet2(int size,float *recon, float* lambda);

#endif
