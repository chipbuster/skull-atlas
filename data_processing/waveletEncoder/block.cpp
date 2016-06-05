#include "block.h"
#include "zerobit.h"
#include "waveAnthony.h"
#include "wave.h"

int paddingsize(int size1)
{ int tempsize1;

  if((size1%BLOCKSIZE)==0)
    return size1;
  else
  {
    tempsize1=((size1/BLOCKSIZE)+1)*BLOCKSIZE;
    return tempsize1;
  }
}/*paddingsize*/

int paddingsizever2(int dim1[3])
{ int tempsize1,size1;

  size1=-1;

  for(int i=0;i<3;i++)
  {
    if(dim1[i]>size1) 
      size1=dim1[i];
  }/*for i*/

  if((size1%BLOCKSIZE)==0)
    return size1;
  else
  {
    tempsize1=((size1/BLOCKSIZE)+1)*BLOCKSIZE;
    return tempsize1;
  }
}/*paddingsize*/


void padding(int oldsize1,int newsize1,float *lambda1,float* lambdanew1)
{ int z,y,x;
  
  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      {
        if((z>=oldsize1)||(y>=oldsize1)||(x>=oldsize1))
	  lambdanew1[newsize1*newsize1*z+newsize1*y+x]=0.0;
        else
          lambdanew1[newsize1*newsize1*z+newsize1*y+x]=lambda1[oldsize1*oldsize1*z+oldsize1*y+x];
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*padding*/

void paddingver2(int dim[3],int newsize1,float *lambda1,float* lambdanew1)
{ int z,y,x;
  
  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      {
        if((z>=dim[2])||(y>=dim[1])||(x>=dim[0]))
	  lambdanew1[newsize1*newsize1*z+newsize1*y+x]=0.0;
        else
          lambdanew1[newsize1*newsize1*z+newsize1*y+x]=lambda1[dim[1]*dim[0]*z+dim[0]*y+x];
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*paddingver2*/

void paddingver3(int dim[3],int dimnew[3],float *lambda1,float* lambdanew1)
{ int z,y,x;
  
  for(z=0; z<dimnew[2]; z=z+1) 
  { for(y=0; y<dimnew[1]; y=y+1) 
    { for(x=0; x<dimnew[0]; x=x+1) 
      {
        if((z>=dim[2])||(y>=dim[1])||(x>=dim[0]))
	  lambdanew1[dimnew[1]*dimnew[0]*z+dimnew[0]*y+x]=0.0;
        else
          lambdanew1[dimnew[1]*dimnew[0]*z+dimnew[0]*y+x]=lambda1[dim[1]*dim[0]*z+dim[0]*y+x];
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*paddingver3*/

int zeroblock(float *block1,int type)
{ int whether_zeroblock=1;
  int z,y,x;
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

  for(z=0; z<calculatesize; z=z+1) 
  { for(y=0; y<calculatesize; y=y+1) 
    { for(x=0; x<calculatesize; x=x+1) 
      {
	if(fabs(block1[calculatesize*calculatesize*z+calculatesize*y+x])>=VERYSMALLNUMBER)
          whether_zeroblock=0;
      }/*for x*/
    }/*for y*/
  }/*for z*/
  
  return whether_zeroblock;
}/*zeroblock*/

void copyblock(int newsize1,int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type)
{ int z,y,x;
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

  for(z=indexz1; z<(indexz1+calculatesize); z=z+1) 
  { for(y=indexy1; y<(indexy1+calculatesize); y=y+1) 
    { for(x=indexx1; x<(indexx1+calculatesize); x=x+1) 
      {
	block1[calculatesize*calculatesize*(z-indexz1)+calculatesize*(y-indexy1)+(x-indexx1)]=lambdanew1[newsize1*newsize1*z+newsize1*y+x];
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*copyblock*/

void reversecopyblock(int newsize1,int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type)
{ int z,y,x;
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

  for(z=indexz1; z<(indexz1+calculatesize); z=z+1) 
  { for(y=indexy1; y<(indexy1+calculatesize); y=y+1) 
    { for(x=indexx1; x<(indexx1+calculatesize); x=x+1) 
      { lambdanew1[newsize1*newsize1*z+newsize1*y+x]=	
        block1[calculatesize*calculatesize*(z-indexz1)+calculatesize*(y-indexy1)+(x-indexx1)];           
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*reversecopyblock*/

void copyblockver2(int dim1[3],int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type)
{ int z,y,x;
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

  for(z=indexz1; z<(indexz1+calculatesize); z=z+1) 
  { for(y=indexy1; y<(indexy1+calculatesize); y=y+1) 
    { for(x=indexx1; x<(indexx1+calculatesize); x=x+1) 
      {
	block1[calculatesize*calculatesize*(z-indexz1)+calculatesize*(y-indexy1)+(x-indexx1)]=lambdanew1[dim1[1]*dim1[0]*z+dim1[0]*y+x];
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*copyblockver2*/

void reversecopyblockver2(int dim1[3],int indexx1,int indexy1,int indexz1,float *lambdanew1,float *block1,int type)
{ int z,y,x;
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

  for(z=indexz1; z<(indexz1+calculatesize); z=z+1) 
  { for(y=indexy1; y<(indexy1+calculatesize); y=y+1) 
    { for(x=indexx1; x<(indexx1+calculatesize); x=x+1) 
      { lambdanew1[dim1[1]*dim1[0]*z+dim1[0]*y+x]=	
        block1[calculatesize*calculatesize*(z-indexz1)+calculatesize*(y-indexy1)+(x-indexx1)];           
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*reversecopyblock*/

void print_block(float *block1,int type)
//type==1 BLOCK
//type==2 CELL
{ int x,y,z;
  int calculatesize;

  switch(type)
  { case 1:
      calculatesize=BLOCKSIZE;
      printf("Now a block is printing.\n");  
      break;
    case 2:
      calculatesize=CELLSIZE; 
      printf("Now a cell is printing.\n");   
      break;
  }/*switch*/
  
  for(z=0; z<calculatesize; z=z+1) 
  { for(y=0; y<calculatesize; y=y+1) 
    { for(x=0; x<calculatesize; x=x+1) 
      { printf("block1[%d,%d,%d]=%.50f\n",z,y,x,block1[calculatesize*calculatesize*z+calculatesize*y+x]);
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*print_block*/

void print_block_from_whole(int newsize1,float *block1,int type,int startx1,int starty1,int startz1)
//type==1 BLOCK
//type==2 CELL
{ int x,y,z;
  int calculatesize;

  switch(type)
  { case 1:
      calculatesize=BLOCKSIZE;
      printf("Now a block is printing.\n");  
      break;
    case 2:
      calculatesize=CELLSIZE; 
      printf("Now a cell is printing.\n");   
      break;
  }/*switch*/
  
  for(z=startz1; z<(startz1+calculatesize); z=z+1) 
  { for(y=starty1; y<(starty1+calculatesize); y=y+1) 
    { for(x=startx1; x<(startx1+calculatesize); x=x+1) 
      { printf("block1[%d,%d,%d]=%.50f\n",z,y,x,block1[newsize1*newsize1*z+newsize1*y+x]);
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*print_block_from_whole*/

float fabs_maximum_block(float *block1,int newsize1)
{ int x,y,z;
  float maximumtemp;

  maximumtemp=LARGENEGATIVE;

  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      { 
        maximumtemp=(fabs(block1[newsize1*newsize1*z+newsize1*y+x])>maximumtemp) ? fabs(block1[newsize1*newsize1*z+newsize1*y+x]) : maximumtemp;
      }/*for x*/
    }/*for y*/
  }/*for z*/

  return maximumtemp;
}/*fabs_maximum_block*/

void print_volume(float *block1,int newsize1)
{ int x,y,z;
  
  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      { printf("block1[%d,%d,%d]=%f\n",z,y,x,block1[newsize1*newsize1*z+newsize1*y+x]);
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*print_volume*/

void copy_volume(float *newvolume1,float *oldvolume1,int newsize1)
{ int x,y,z;
  
  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      { newvolume1[newsize1*newsize1*z+newsize1*y+x]=oldvolume1[newsize1*newsize1*z+newsize1*y+x];
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*copy_volume*/

void cell_coordinate(int index,int *x_coor,int *y_coor,int *z_coor)
//given the index, 
//calculate the x_coor,y_coor,z_coor in a cell
{
  *y_coor=index%(CELLSIZE*CELLSIZE);
  *z_coor=index/(CELLSIZE*CELLSIZE);
  *x_coor= *y_coor%CELLSIZE;
  *y_coor= *y_coor/CELLSIZE;
}/*cell_coordinate*/

int zeroelement(float *block1,int x_coor,int y_coor,int z_coor)
//tell if a cell is a zero element at coordinate (x,y,z)
//return 1 if zeroelement
{ int whether_zeroelement;

  if(fabs(block1[CELLSIZE*CELLSIZE*z_coor+CELLSIZE*y_coor+x_coor])>=VERYSMALLNUMBER)
    whether_zeroelement=0;
  else
    whether_zeroelement=1;

  return whether_zeroelement; 
}/*zeroelement*/

void copyzeroblock(int newsize1,float *output1)
{
  int x,y,z;
  
  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      { output1[newsize1*newsize1*z+newsize1*y+x]=0;
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*copyzeroblock*/

int equalblock(int newsize1,float *block1,float *block2)
{
  int x,y,z,output1;

  output1=1;  

  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      { if(fabs(block1[newsize1*newsize1*z+newsize1*y+x]-block2[newsize1*newsize1*z+newsize1*y+x])>VERYSMALLNUMBER)
          output1=0;
      }/*for x*/
    }/*for y*/
  }/*for z*/

  return output1;
}/*copyzeroblock*/

void output_rawiv(const char* fname,int newsize1,float *func1)
{
  int file_size_temp;

  FILE *resultfp = fopen(fname,"wb");

  if (resultfp == NULL) 
  {
    fprintf(stderr, "cannot open %s\n", fname);
    exit(-1);
  }

  file_size_temp=newsize1*newsize1*newsize1;
  fwrite(func1, sizeof(float),file_size_temp,resultfp);

  fclose(resultfp);
}/*output_rawiv*/




















