#include "zeroblocktable.h"
#include "zeroblocktableassist.h"
#include "zerobit.h"
#include "block.h"
#include "waveAnthony.h"
#include "wave.h"
#include "wavedata.h"

void zeroblocktable_generation(int newsize1,float *lambda1,int *zeroblocktable1,int type)
{ int z,y,x,whetherzeroblock1;
  int calculatesize,nvertsnew;  

//type==1 BLOCK
//type==2 CELL

  switch(type)
  { case 1:
      calculatesize=BLOCKSIZE;  
      break;
    case 2:
      calculatesize=CELLSIZE;  
      break;
  }/*switch*/

  nvertsnew=calculatesize*calculatesize*calculatesize;
  float* block1 = 0;
  block1 = new float[nvertsnew];

  for(z=0; z<newsize1; z=z+calculatesize) 
  { for(y=0; y<newsize1; y=y+calculatesize) 
    { for(x=0; x<newsize1; x=x+calculatesize) 
      {
        copyblock(newsize1,x,y,z,lambda1,block1,type);
        
        if(zeroblock(block1,type))
          whetherzeroblock1=1;
	//It is a zero block.
        else 
          whetherzeroblock1=0;
        //It is not a zero block.

        zeroblocktable_writing(newsize1,x,y,z,whetherzeroblock1,zeroblocktable1,type);
      }//for x
    }//for y
  }//for z
}//zeroblocktable_generation

void zeroblocktable_writing(int newsize1,int indexx1,int indexy1,int indexz1,int whetherzeroblock1,int *zeroblocktable1,int type)
{ int position1,blockposition1,blockindex1,rotate1;
  int calculatesize;  

//type==1 BLOCK
//type==2 CELL

  switch(type)
  { case 1:
      calculatesize=BLOCKSIZE;  
      break;
    case 2:
      calculatesize=CELLSIZE;  
      break;
  }/*switch*/

  position1=(newsize1/calculatesize)*(newsize1/calculatesize)*(indexz1/calculatesize)+(newsize1/calculatesize)*(indexy1/calculatesize)+(indexx1/calculatesize);
  blockposition1=position1/8;
  blockindex1=position1%8;

  if(whetherzeroblock1==1)
  { rotate1=1;
    for(int i=0;i<blockindex1;i++) 
      rotate1=rotate1<<1;
    zeroblocktable1[blockposition1]=zeroblocktable1[blockposition1]|rotate1;
  }/*if*/
}/*zeroblocktable_writing*/

int size_zeroblocktable(int newsize1,int type)
{ int calculatesize,resultsize;  

//type==1 BLOCK
//type==2 CELL

  switch(type)
  { case 1:
      calculatesize=BLOCKSIZE;  
      break;
    case 2:
      calculatesize=CELLSIZE;  
      break;
  }/*switch*/

  resultsize=(newsize1/calculatesize)*(newsize1/calculatesize)*(newsize1/calculatesize)/8+1;
  return resultsize;
}/*size_zeroblocktable*/

void empty_zeroblocktable(int zeroblocktable1size,int *zeroblocktable1)
{ for(int i=0;i<zeroblocktable1size;i++) 
  {  zeroblocktable1[i]=0;
  }/*for i*/
}/*empty_zeroblocktable*/

void print_zeroblocktable(int zeroblocktable1size,int *zeroblocktable1)
{ for(int i=0; i<zeroblocktable1size;i=i+1) 
  { printf("zeroblocktable1[%d]=%d\n",i,zeroblocktable1[i]);
  }/*for i*/
}/*print_zeroblocktable*/

float zeroblocktable_zeroblockrate(int *zeroblocktable1,int table_size)
{ int tempzeroblocktable;
  float zeroblockstatistics=0.0;

  /*
  printf("In zeroblocktable_zeroblockrate,table_size=%d\n",table_size);
  */

  for(int index=0;index<table_size;index++) 
  { 
    for(int innerindex=0;innerindex<8;innerindex++) 
    {  tempzeroblocktable=zeroblocktable1[index];
       for(int i=0;i<innerindex;i++) 
       {
         tempzeroblocktable=tempzeroblocktable>>1;
       }/*for i*/
       tempzeroblocktable=tempzeroblocktable&1;
       zeroblockstatistics=zeroblockstatistics+tempzeroblocktable;  

       /*
       printf("In zeroblocktable_zeroblockrate's loop,tempzeroblocktable=%d\n",tempzeroblocktable);
       printf("In zeroblocktable_zeroblockrate's loop,zeroblockstatistics=%f\n",zeroblockstatistics);
       */
    }/*for innerindex*/   
  }/*for index*/

  zeroblockstatistics=zeroblockstatistics/(table_size*8);

  return zeroblockstatistics;
}/*zeroblocktable_zeroblockrate*/

//write one bit(outputbit1) in the cell1_index position(bit rotating) at the celltable1_index of celltable1
void writebit(int *celltable1,int celltable1_size,int celltable1_index,int cell1_index,int outputbit1)
{ int rotate1,clearzero1,allone_onezero;

  clearzero1=1;

  for(int i=0;i<cell1_index;i++) 
    clearzero1=clearzero1<<1;

  //printf("clearzero1=%d \n",clearzero1);

  allone_onezero=255;

  allone_onezero=allone_onezero^clearzero1;

  //printf("allone_onezero=%d \n",allone_onezero);

  celltable1[celltable1_index]=celltable1[celltable1_index]&allone_onezero;

  rotate1=outputbit1;

  for(int i=0;i<cell1_index;i++) 
    rotate1=rotate1<<1;

  //printf("rotate1=%d \n",rotate1);

  celltable1[celltable1_index]=celltable1[celltable1_index]|rotate1;
}/*writebit*/

void print_bit_table_partial(int *table1,int begin1,int end1)
{ int output1,rotate1;

  for(int i=begin1; i<(end1+1);i=i+1) 
  { for(int j=7; j>=0;j=j-1) 
    { rotate1=1;
      for(int k=0; k<j;k=k+1) 
      { 
        rotate1=rotate1<<1;
      }/*for k*/

      output1=table1[i]&rotate1;

      for(int k=0; k<j;k=k+1) 
      { 
        output1=output1>>1;
      }/*for k*/

      printf("%d ",output1);
    }/*for j*/
    printf("\n");
  }/*for i*/
}/*print_bit_table_partial*/

int countbit(int *table1,int table1_index_begin,int table1_rotate_index_begin,int table1_index_end,int table1_rotate_index_end)
//count the bit from table1_index_begin,table1_rotate_index_begin(rotate)  
//to table1_index_end,table1_rotate_index_end(rotate)
{ 
  int tempzeroblocktable,statistics;  

  statistics=0;

  if(table1_index_begin<table1_index_end)
  { 
    for(int index=table1_index_begin;index<table1_index_begin+1;index++) 
    { 
      for(int innerindex=table1_rotate_index_begin;innerindex<8;innerindex++) 
      {  tempzeroblocktable=table1[index];
         for(int i=0;i<innerindex;i++) 
         {
           tempzeroblocktable=tempzeroblocktable>>1;
         }/*for i*/
         tempzeroblocktable=tempzeroblocktable&1;
         statistics=statistics+tempzeroblocktable;  
      }/*for innerindex*/   
    }/*for index*/

    for(int index=table1_index_begin+1;index<table1_index_end;index++) 
    { 
      for(int innerindex=0;innerindex<8;innerindex++) 
      {  tempzeroblocktable=table1[index];
         for(int i=0;i<innerindex;i++) 
         {
           tempzeroblocktable=tempzeroblocktable>>1;
         }/*for i*/
         tempzeroblocktable=tempzeroblocktable&1;
         statistics=statistics+tempzeroblocktable;  
      }/*for innerindex*/   
    }/*for index*/

    for(int index=table1_index_end;index<(table1_index_end+1);index++) 
    { 
      for(int innerindex=0;innerindex<(table1_rotate_index_end+1);innerindex++) 
      {  tempzeroblocktable=table1[index];
         for(int i=0;i<innerindex;i++) 
         {
           tempzeroblocktable=tempzeroblocktable>>1;
         }/*for i*/
         tempzeroblocktable=tempzeroblocktable&1;
         statistics=statistics+tempzeroblocktable;  
      }/*for innerindex*/   
    }/*for index*/
  }/*if*/
  else if(table1_index_begin==table1_index_end)
  { 
    for(int index=table1_index_begin;index<(table1_index_begin+1);index++) 
    { 
      for(int innerindex=table1_rotate_index_begin;innerindex<(table1_rotate_index_end+1);innerindex++) 
      {  
         tempzeroblocktable=table1[index];
         for(int i=0;i<innerindex;i++) 
         {
           tempzeroblocktable=tempzeroblocktable>>1;
         }/*for i*/
         tempzeroblocktable=tempzeroblocktable&1;
         statistics=statistics+tempzeroblocktable;  
      }/*for innerindex*/   
    }/*for index*/
  }/*else*/
  else 
  {
    printf("table1_index_begin<=table1_index_end should be true. \n");
  }/*else*/

  return statistics;
}/* countbit*/

int celltable_generation(float *lambda1,int *celltable1,int celltable1_size)
{
  int x,y,z,nvertsnew,outputbit1;
  int celltable1_index=0,cell1_index=0;
  int cell_NNNC=0;

  nvertsnew=CELLSIZE*CELLSIZE*CELLSIZE;
  float* inputcell1 = 0;
  inputcell1 = new float[nvertsnew];

  for(z=0; z<BLOCKSIZE; z=z+CELLSIZE) 
  { for(y=0; y<BLOCKSIZE; y=y+CELLSIZE) 
    { for(x=0; x<BLOCKSIZE; x=x+CELLSIZE) 
      {       
        copyblock(BLOCKSIZE,x,y,z,lambda1,inputcell1,2);
        //copy a cell of lambda1 to inputcell1     

        outputbit1=1-zeroblock(inputcell1,2); 
        cell_NNNC=cell_NNNC+outputbit1;

        writebit(celltable1,celltable1_size,celltable1_index,cell1_index,outputbit1);    

        cell1_index++;
        if(cell1_index>=8)
        { cell1_index=cell1_index-8;
          celltable1_index++;
        }/*if*/    
      }/*for x*/
    }/*for y*/
  }/*for z*/

  return cell_NNNC;
}/*celltable_generation*/

int table_size_block(int type,int CIA_number)
{ int size1;

  switch(type)
  {
    case 1:
      size1=((CELLSIZE*CELLSIZE*CELLSIZE)/8+1)*CIA_number;
      break;
    case 2:
      size1=((BLOCKSIZE/CELLSIZE)*(BLOCKSIZE/CELLSIZE)*(BLOCKSIZE/CELLSIZE))/8+1;
      break;
    case 3:
      size1=(CELLSIZE*CELLSIZE*CELLSIZE)/8+1;
      break;
  }/*switch*/
  
  return size1;
}/*table_size_block*/

void empty_table(int *table1,int begin1,int end1)
{ int z;

  for(z=begin1; z<(end1+1); z=z+1) 
  { 
    table1[z]=0;
    //printf("table1[%d]=%d\n",z,table1[z]);
  }/*for z*/
}/*empty_table*/

void cellinformationarray_generation_cell_ver1(float *block1,int *cell_information_array1,int prev_CIAindex1,int *current_CIAindex1,int ZSMSindex_begin1,int ZSMSindex_next_begin1)
{
  int step;

  step=(CELLSIZE*CELLSIZE*CELLSIZE/8)+1;

  if(prev_CIAindex1==0)
  {  
    cell_information_array1[0]=0;
  }/*if*/
  else
    cell_information_array1[prev_CIAindex1]=cell_information_array1[prev_CIAindex1-1]+step;

  *current_CIAindex1=prev_CIAindex1+1;
}/*cellinformationarray_generation_cell_ver1*/

void cellinformationarray_generation_cell_ver3(float *block1,int *cell_information_array1,int prev_CIAindex1,int *current_CIAindex1,int ZSMSindex_begin1,int ZSMSindex_next_begin1)
{
  if(prev_CIAindex1==0)
  {  
    cell_information_array1[0]=0;
  }/*if*/

  cell_information_array1[prev_CIAindex1+1]=cell_information_array1[prev_CIAindex1]+ZSMSindex_next_begin1-ZSMSindex_begin1;

  *current_CIAindex1=prev_CIAindex1+1;
}/*cellinformationarray_generation_cell_ver3*/

void ZSMStable_generation_cell(float *block1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1)
{
  int step,index,ZSMStable1_rotateindex,ZSMStable1_rowindex,ZSMStable1_size;
  int x_coor,y_coor,z_coor,outputbit1;

  step=CELLSIZE*CELLSIZE*CELLSIZE;
  index=0;
  ZSMStable1_rotateindex=0;
  ZSMStable1_rowindex=prev_ZSMSindex1;
  ZSMStable1_size=table_size_block(1,1);

  while(index<step) 
  { cell_coordinate(index,&x_coor,&y_coor,&z_coor);

    outputbit1=1-zeroelement(block1,x_coor,y_coor,z_coor); 

    writebit(ZSMStable1,ZSMStable1_size,ZSMStable1_rowindex,ZSMStable1_rotateindex,outputbit1);
    //write one bit in the ZSMStable1_rotateindex position(bit rotating) at the ZSMStable1_rowindex of ZSMStable1

    index=index+1;

    ZSMStable1_rotateindex++;
    if(ZSMStable1_rotateindex>=8)
    { ZSMStable1_rotateindex=ZSMStable1_rotateindex-8;
      ZSMStable1_rowindex++;
    }/*if*/  
  }/*while*/

  *current_ZSMSindex1=prev_ZSMSindex1+ZSMStable1_size;
}/*ZSMStable_generation_cell*/

void DIS_generation_cell_ver1(float *block1,int *DIS1,int previousDISindex,int *currentDISindex)
{
  int z,y,x,tempDISindex;

  if(previousDISindex==0)
  {  
    tempDISindex=0;
    DIS1[0]=0;
  }
  else
    tempDISindex=DIS1[previousDISindex];

  for(z=0; z<CELLSIZE; z++) 
  { for(y=0; y<CELLSIZE; y++) 
    { for(x=0; x<CELLSIZE; x++) 
      { 
        if(fabs(block1[CELLSIZE*CELLSIZE*z+CELLSIZE*y+x])>VERYSMALLNUMBER)
	{      
          tempDISindex++;
        }/*if(fabs*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  DIS1[previousDISindex+1]=tempDISindex;
  *currentDISindex=previousDISindex+1;
}/*DIS_generation_cell_ver1*/

int vertex(int x,int y,int z,int cellsize1)
{
  int result1=0;

  for(int innerz=0; innerz<cellsize1; innerz=innerz+cellsize1-1) 
  { for(int innery=0; innery<cellsize1; innery=innery+cellsize1-1) 
    { for(int innerx=0; innerx<cellsize1; innerx=innerx+cellsize1-1) 
      { 
        if((innerz==x)&&(innery==y)&&(innerx==z))
	{
           result1=1;
        }/*if*/ 
      }/*for x*/
    }/*for y*/
  }/*for z*/

  return result1;
}/*vertex*/

void DIS_generation_cell_ver3(float *block1,int *DIS1,int previousDISindex,int *currentDISindex)
{
  int z,y,x,tempDISindex;

  if(previousDISindex==0)
  {  
    tempDISindex=0;
    DIS1[0]=0;
  }/*if*/
  else
    tempDISindex=DIS1[previousDISindex];

  for(z=0; z<CELLSIZE; z++) 
  { for(y=0; y<CELLSIZE; y++) 
    { for(x=0; x<CELLSIZE; x++) 
      { 
        if(!vertex(x,y,z,CELLSIZE))
	{
          if(fabs(block1[CELLSIZE*CELLSIZE*z+CELLSIZE*y+x])>VERYSMALLNUMBER)
  	  {      
            tempDISindex++;
          }/*if*/
        }/*if*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  tempDISindex = tempDISindex+8;

  DIS1[previousDISindex+1]=tempDISindex;
  *currentDISindex=previousDISindex+1;
}/*DIS_generation_cell_ver3*/

int table_generation_block(int index1, float *lambda1,int *table1,int *cell_information_array1,int prev_CIAindex1,int *current_CIAindex1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1,int *DIStable1,int prev_DISindex1,int *current_DISindex1,int ZSMStype1)
{
  int z,y,x,nvertsnew,table1_size,NNNC_cell;
  int ZSMSindex_begin1,CIAindex_begin1,DISindex_begin1,ZSMSindex_next_begin1,CIAindex_next_begin1,DISindex_next_begin1,celltable_generation_index1;

  nvertsnew=CELLSIZE*CELLSIZE*CELLSIZE;
  float* inputcell1 = 0;
  inputcell1 = new float[nvertsnew];

  table1_size=table_size_block(2, -1);
  celltable_generation_index1= table1_size*index1;
  NNNC_cell=celltable_generation(lambda1,&table1[celltable_generation_index1],table1_size);

  ZSMSindex_begin1=prev_ZSMSindex1;
  CIAindex_begin1=prev_CIAindex1;
  DISindex_begin1=prev_DISindex1;
  
  for(z=0; z<BLOCKSIZE; z=z+CELLSIZE) 
  { for(y=0; y<BLOCKSIZE; y=y+CELLSIZE) 
    { for(x=0; x<BLOCKSIZE; x=x+CELLSIZE) 
      { 
        copyblock(BLOCKSIZE,x,y,z,lambda1,inputcell1,2);
        //copy a cell to inputcell1

        if(!zeroblock(inputcell1,2))
	{   
          switch(ZSMStype1)
	  {
	    case 1:
              ZSMStable_generation_cell(inputcell1,ZSMStable1,ZSMSindex_begin1,&ZSMSindex_next_begin1);
              cellinformationarray_generation_cell_ver1(inputcell1,cell_information_array1,CIAindex_begin1,&CIAindex_next_begin1,ZSMSindex_begin1,ZSMSindex_next_begin1);
              DIS_generation_cell_ver1(inputcell1,DIStable1,DISindex_begin1,&DISindex_next_begin1);
              break;
            case 2:
              //ZSMStable_generation_cell_ver2(inputcell1,ZSMStable1,ZSMSindex_begin1,&ZSMSindex_next_begin1);
              break;
            case 3:
              ZSMStable_generation_cell_ver3(inputcell1,ZSMStable1,ZSMSindex_begin1,&ZSMSindex_next_begin1);
              cellinformationarray_generation_cell_ver3(inputcell1,cell_information_array1,CIAindex_begin1,&CIAindex_next_begin1,ZSMSindex_begin1,ZSMSindex_next_begin1);
	      /*
              printf("The result of ZSMStable_generation_cell_ver3 \n");
              printf("ZSMSindex_begin1=%d\n",ZSMSindex_begin1);
              printf("ZSMSindex_next_begin1=%d\n",ZSMSindex_next_begin1);
	      */
              DIS_generation_cell_ver3(inputcell1,DIStable1,DISindex_begin1,&DISindex_next_begin1);
              break;
          }/*switch*/

          nonzerocelltotal++;
          ZSMS_size_count[ZSMSindex_next_begin1-ZSMSindex_begin1]++;

          ZSMSindex_begin1=ZSMSindex_next_begin1;
          CIAindex_begin1=CIAindex_next_begin1;
          DISindex_begin1=DISindex_next_begin1;
        }/*if(!zeroblock(inputcell1,1))*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  delete[] inputcell1;

  *current_ZSMSindex1=ZSMSindex_begin1;
  *current_CIAindex1=CIAindex_begin1;
  *current_DISindex1=DISindex_begin1;

  return NNNC_cell;
}/*table_generation_block*/

void table_generation(int dim1[3],float *lambda1,int dimension1,int ZSMStype1)
{
  int z,y,x,nvertsnew;
  int index1,ZSMSindex_begin1,CIAindex_begin1,DISindex_begin1,ZSMSindex_next_begin1,CIAindex_next_begin1,DISindex_next_begin1;

  nvertsnew=BLOCKSIZE*BLOCKSIZE*BLOCKSIZE;
  float* inputblock1 = 0;
  inputblock1 = new float[nvertsnew];

  index1=0;
  ZSMSindex_begin1=0;
  CIAindex_begin1=0;
  DISindex_begin1=0;
  nonzerocelltotal=0;
  
  for(z=0; z<MAX_ZSMS_TABLE_SIZE; z++) 
    ZSMS_size_count[z]=0;

  for(z=0; z<dim1[2]; z=z+BLOCKSIZE) 
  { for(y=0; y<dim1[1]; y=y+BLOCKSIZE) 
    { for(x=0; x<dim1[0]; x=x+BLOCKSIZE) 
      { 
        copyblockver2(dim1,x,y,z,lambda1,inputblock1,1);
        //copy a block to inputblock1

        if(!zeroblock(inputblock1,1))
	{      
          NNNC_whole[index1]=table_generation_block(index1,inputblock1,table_whole,cell_information_array_whole,CIAindex_begin1,&CIAindex_next_begin1,ZSMStable_whole,ZSMSindex_begin1,&ZSMSindex_next_begin1,DIStable_whole,DISindex_begin1,&DISindex_next_begin1,ZSMStype1);
          //printf("NNNC_whole[%d]=%d\n",index1,NNNC_whole[index1]);

          ZSMSindex_begin1=ZSMSindex_next_begin1;
          CIAindex_begin1=CIAindex_next_begin1;
          DISindex_begin1=DISindex_next_begin1;
          index1++;
        }/*if(!zeroblock(inputblock1,1))*/
        else
	{
          //printf("This is a zero block\n");
        }/*else*/

	/*
        printf("This is the table of ZSMS(0-39).\n");
        print_bit_table_partial(ZSMStable_whole,0,39);
	*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  CBFT_end_whole = table_size_block(2, -1)*index1;
  ZSMSindex_end_whole = ZSMSindex_next_begin1;
  CIAindex_end_whole = CIAindex_next_begin1;
  DISindex_end_whole = DISindex_next_begin1;

  delete[] inputblock1;
}/*table_generation*/

int get_bit_from_bit_table(int *table1,int rowindex1,int rotationindex1)
{ int output1,rotate1;

   rotate1=1;
   for(int k=0; k<rotationindex1;k=k+1) 
   { 
     rotate1=rotate1<<1;
   }/*for k*/

   output1=table1[rowindex1]&rotate1;

   for(int k=0; k<rotationindex1;k=k+1) 
   { 
     output1=output1>>1;
   }/*for k*/

   return output1;
   //printf("%d ",output1);
}/*get_bit_from_bit_table*/
