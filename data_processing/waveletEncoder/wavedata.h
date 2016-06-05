#ifndef WAVEDATA_H
#define WAVEDATA_H

#define MAX_TABLE_SIZE 125
#define MAX_NUMBER_BLOCK 1000000
#define MAX_NUMBER_CELL_IN_BLOCK 10000
#define MAX_VOLUME_DIVIDE_8 18609625
#define MAX_ZSMS_TABLE_SIZE 100

extern float orig[3],span[3];
extern float *p_data;

extern double maxfuncvalue;
extern float extension[3];
extern int levelglobal;

extern int NNNC_whole[MAX_NUMBER_BLOCK];

extern int* table_whole;
extern int* cell_information_array_whole;
extern int* ZSMStable_whole;
extern int* DIStable_whole;

extern int CBFT_end_whole;
extern int ZSMSindex_end_whole;
extern int CIAindex_end_whole;
extern int DISindex_end_whole;

extern int nonzerocelltotal;
extern int ZSMS_size_count[MAX_ZSMS_TABLE_SIZE];

#endif
