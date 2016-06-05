#include "zeroblocktable.h"
#include "zerobit.h"
#include "block.h"
#include "wavedata.h"

void output_data(int size1,float *inputcell1,int currentlevel1,FILE *resultfp)
{
  int z,y,x,index1;
  int numCells = size1-1;
  int fullStep = 1<<currentlevel1;
  int halfStep = fullStep>>1; 

  for (z=0; z<=numCells; z+=fullStep)
  { for (y=0; y<=numCells; y+=fullStep) 
    { for (x=halfStep; x<numCells; x+=fullStep) 
      {
        if(!zeroelement(inputcell1,x,y,z))
	{
          index1=size1*size1*z+size1*y+x;
          fwrite(&inputcell1[index1], sizeof(float),1,resultfp);
          //printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);
        }/*if*/      
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for (z=0; z<=numCells; z+=fullStep) 
  { for (y=halfStep; y<numCells; y+=fullStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        if(!zeroelement(inputcell1,x,y,z))
        {
          index1=size1*size1*z+size1*y+x;
          fwrite(&inputcell1[index1], sizeof(float),1,resultfp);
          //printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);
        }/*if*/ 
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for (z=halfStep; z<numCells; z+=fullStep) 
  { for (y=0; y<=numCells; y+=halfStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        if(!zeroelement(inputcell1,x,y,z))
	{
          index1=size1*size1*z+size1*y+x;
          fwrite(&inputcell1[index1], sizeof(float),1,resultfp);
          //printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);
        }/*if*/         
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*output_data*/

void print_output_data(int size1,float *inputcell1,int currentlevel1,FILE *resultfp)
{
  int z,y,x,index1;
  int numCells = size1-1;
  int fullStep = 1<<currentlevel1;
  int halfStep = fullStep>>1; 

  for (z=0; z<=numCells; z+=fullStep)
  { for (y=0; y<=numCells; y+=fullStep) 
    { for (x=halfStep; x<numCells; x+=fullStep) 
      {
        if(!zeroelement(inputcell1,x,y,z))
	{
          index1=size1*size1*z+size1*y+x;
          printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);
        }/*if*/      
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for (z=0; z<=numCells; z+=fullStep) 
  { for (y=halfStep; y<numCells; y+=fullStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        if(!zeroelement(inputcell1,x,y,z))
        {
          index1=size1*size1*z+size1*y+x;
          printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);
        }/*if*/ 
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for (z=halfStep; z<numCells; z+=fullStep) 
  { for (y=0; y<=numCells; y+=halfStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        if(!zeroelement(inputcell1,x,y,z))
	{
          index1=size1*size1*z+size1*y+x;
          printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);
        }/*if*/         
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*print_output_data*/

void encode_whole_data(const char* fname,int dim1[3],float *func1)
{ int tempzero,nvertsnew,z,y,x,innerz,innery,innerx,innerinnerz,innerinnery,innerinnerx,index1;
  int blockint,cellint,CBFT_table_size,s,lvl,numCells;
  unsigned char temp_unsignedchar;

  blockint=BLOCKSIZE;
  cellint=CELLSIZE;

  FILE *resultfp = fopen(fname,"wb");
  if (resultfp == NULL) 
  {
    fprintf(stderr, "cannot open %s\n", fname);
    exit(-1);
  }

  //printf("These results are located in encode.cpp.\n");

  fwrite(&dim1[0], sizeof(int), 1, resultfp);
  fwrite(&dim1[1], sizeof(int), 1, resultfp);
  fwrite(&dim1[2], sizeof(int), 1, resultfp);

  //fwrite(&newsize1, sizeof(int), 1, resultfp);
  //printf("newsize1=%d\n",newsize1);

  fwrite(&blockint, sizeof(int), 1, resultfp);
  //printf("blockint=%d\n",blockint);

  fwrite(&cellint, sizeof(int), 1, resultfp);
  //printf("cellint=%d\n",cellint);

  CBFT_table_size = table_size_block(2,-1);
  fwrite(&CBFT_table_size, sizeof(int), 1, resultfp);
  //printf("CBFT_table_size=%d\n",CBFT_table_size);
 
  fwrite(&ZSMSindex_end_whole, sizeof(int), 1, resultfp);
  //printf("ZSMSindex_end_whole=%d\n",ZSMSindex_end_whole);

  nvertsnew=BLOCKSIZE*BLOCKSIZE*BLOCKSIZE;
  float* inputblock1 = 0;
  inputblock1 = new float[nvertsnew];

  nvertsnew=CELLSIZE*CELLSIZE*CELLSIZE;
  float* inputcell1 = 0;
  inputcell1 = new float[nvertsnew];

  index1=0;  
  for(z=0; z<dim1[2]; z=z+BLOCKSIZE) 
  { for(y=0; y<dim1[1]; y=y+BLOCKSIZE) 
    { for(x=0; x<dim1[0]; x=x+BLOCKSIZE) 
      { 
        copyblockver2(dim1,x,y,z,func1,inputblock1,1);
        //copy a block to inputblock1

        if(!zeroblock(inputblock1,1))
	{     
          fwrite(&NNNC_whole[index1], sizeof(int), 1, resultfp); 
          //printf("NNNC_whole[%d]=%d\n",index1,NNNC_whole[index1]);

          index1++;
        }/*if(!zeroblock(inputblock1,1))*/
        else
	{
          tempzero=0;
          fwrite(&tempzero, sizeof(int), 1, resultfp);
          //printf("NNNC_whole output=%d\n",0);
        }/*else*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for(int j=0;j<CBFT_end_whole;j++)
  {
    temp_unsignedchar=(unsigned char)table_whole[j];
    fwrite(&temp_unsignedchar,sizeof(unsigned char),1,resultfp);
  }/*for j*/

  //print_bit_table_partial(table_whole,0,CBFT_end_whole-1);

  fwrite(cell_information_array_whole, sizeof(int),CIAindex_end_whole,resultfp); 

  /*
  for(int j=0;j<CIAindex_end_whole;j++)
  {
    printf("cell_information_array_whole[%d]=%d\n",j,cell_information_array_whole[j]); 
  }
  */

  fwrite(DIStable_whole, sizeof(int),DISindex_end_whole,resultfp); 

  /*
  for(int j=0;j<DISindex_end_whole;j++)
  {
    printf("DIStable_whole[%d]=%d\n",j,DIStable_whole[j]); 
  }
  
  printf("The results of ZSMS table\n");
  */

  for(int j=0;j<ZSMSindex_end_whole;j++)
  {
     temp_unsignedchar=(unsigned char)ZSMStable_whole[j];
     fwrite(&temp_unsignedchar,sizeof(unsigned char),1,resultfp);
  }/*for j*/

  //print_bit_table_partial(ZSMStable_whole,0,ZSMSindex_end_whole-1);
 
  nvertsnew=CELLSIZE*CELLSIZE*CELLSIZE;

  s=CELLSIZE-1;
  lvl=0;
  numCells=CELLSIZE-1;

  while(s>1)
  {
    lvl++; 
    s=(s>>1);
  }/*while*/
 
  for(z=0; z<dim1[2]; z=z+BLOCKSIZE) 
  { for(y=0; y<dim1[1]; y=y+BLOCKSIZE) 
    { for(x=0; x<dim1[0]; x=x+BLOCKSIZE) 
      { 
        copyblockver2(dim1,x,y,z,func1,inputblock1,1);
        //copy a block to inputblock1

        if(!zeroblock(inputblock1,1))
	{     
          for(innerz=0; innerz<BLOCKSIZE; innerz=innerz+CELLSIZE) 
          { for(innery=0; innery<BLOCKSIZE; innery=innery+CELLSIZE) 
            { for(innerx=0; innerx<BLOCKSIZE; innerx=innerx+CELLSIZE) 
              {       
                copyblock(BLOCKSIZE,innerx,innery,innerz,inputblock1,inputcell1,2);
                //copy a cell of inputblock1 to inputcell1     

                if(!zeroblock(inputcell1,2))
	        {
                  for (innerinnerz=0; innerinnerz<=numCells; innerinnerz+=numCells)
                  { for (innerinnery=0; innerinnery<=numCells; innerinnery+=numCells) 
                    { for (innerinnerx=0; innerinnerx<=numCells; innerinnerx+=numCells) 
                      {
                        index1=CELLSIZE*CELLSIZE*innerinnerz+CELLSIZE*innerinnery+innerinnerx;
                        fwrite(&inputcell1[index1], sizeof(float),1,resultfp);    
                        //printf("inputcell1[%d]=%.20f\n",index1,inputcell1[index1]);               
                      }/*for innerinnerx*/
                    }/*for innerinnery*/
                  }/*for innerinnerz*/
          
                  for(int currentlevel=lvl; currentlevel>=1; currentlevel--) 
                    output_data(CELLSIZE,inputcell1,currentlevel,resultfp);

		  /*
                  for(int currentlevel=lvl; currentlevel>=1; currentlevel--) 
                    print_output_data(CELLSIZE,inputcell1,currentlevel,resultfp);
		  */
                }/*if(!zeroblock(inputcell1,2))*/
              }/*for innerx*/
            }/*for innery*/
          }/*for innerz*/
        }/*if(!zeroblock(inputblock1,1))*/       
      }/*for x*/
    }/*for y*/
  }/*for z*/

  delete[] inputcell1;
  delete[] inputblock1;

  fclose(resultfp);
}/*encode_whole_data*/

void encode_program(const char* fname,int dim1[3],float *func1)
{
  encode_whole_data(fname,dim1,func1);
}/*encode_program*/







