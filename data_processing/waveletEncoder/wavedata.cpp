#include "wavedata.h"

float orig[3],span[3];
float *p_data;

double maxfuncvalue;
float extension[3];
int levelglobal;

int NNNC_whole[MAX_NUMBER_BLOCK];

int* table_whole = 0;
int* cell_information_array_whole = 0;
int* ZSMStable_whole = 0;
int* DIStable_whole = 0;

int CBFT_end_whole = 0;
int ZSMSindex_end_whole = 0;
int CIAindex_end_whole = 0;
int DISindex_end_whole = 0;

int nonzerocelltotal = 0;
int ZSMS_size_count[MAX_ZSMS_TABLE_SIZE];
