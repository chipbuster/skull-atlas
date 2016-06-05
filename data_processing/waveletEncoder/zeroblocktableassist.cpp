#include "zeroblocktable.h"
#include "zeroblocktableassist.h"
#include "block.h"
#include "zerobit.h"

void ZSMSoutputbit(float *block1,int *ZSMStable1,int xpos,int ypos,int zpos,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex)
{ int ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,ZSMStable1_size,outputbit1;

  outputbit1=1-zeroelement(block1,xpos,ypos,zpos); 
 
  //fprintf(stderr, "Begin  ZSMSoutputbit\n");

  ZSMStable1_size=table_size_block(1,1);

  //fprintf(stderr, "outputbit1=%d\n",outputbit1);
  //fprintf(stderr, "ZSMStable1_size=%d\n",ZSMStable1_size);

  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;

  //fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n", ZSMStable1_rowindex_temp);

  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;

  //fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);

  writebit(ZSMStable1,ZSMStable1_size,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,outputbit1);
  //write one bit in the ZSMStable1_rotateindex position(bit rotating) at the ZSMStable1_rowindex of ZSMStable1

  //fprintf(stderr, "End  ZSMSoutputbit\n");

  ZSMStable1_rotateindex_temp++;
  if(ZSMStable1_rotateindex_temp>=8)
  { ZSMStable1_rotateindex_temp=ZSMStable1_rotateindex_temp-8;
    ZSMStable1_rowindex_temp++;
  }/*if*/  

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
}/*ZSMSoutputbit*/

void ZSMSoutputbit16bit(float *block1,int *ZSMStable1,int xpos,int ypos,int zpos,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex,int *counter,int *currentbit)
{
  int ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,ZSMStable1_size,outputbit1;
  int counter_temp,currentbit_temp;

  ZSMStable1_size=table_size_block(1,1);
  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex; 
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;
  counter_temp = *counter;
  currentbit_temp = *currentbit;

  outputbit1=1-zeroelement(block1,xpos,ypos,zpos); 

  //fprintf(stderr, "begin ZSMSoutputbit16bit\n");

  if(outputbit1 == 1)
    currentbit_temp = 1;

  counter_temp++;

  if(counter_temp>=16)
  {
    counter_temp = counter_temp -16;

    writebit(ZSMStable1,ZSMStable1_size,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,currentbit_temp);
    //write one bit in the ZSMStable1_rotateindex position(bit rotating) at the ZSMStable1_rowindex of ZSMStable1

    ZSMStable1_rotateindex_temp++;
    if(ZSMStable1_rotateindex_temp>=8)
    { ZSMStable1_rotateindex_temp=ZSMStable1_rotateindex_temp-8;
      ZSMStable1_rowindex_temp++;
    }/*if*/  

    currentbit_temp = 0;
  }/*if*/

  /*
  fprintf(stderr, "outputbit1=%d\n",outputbit1);
  fprintf(stderr, "currentbit_temp=%d\n",currentbit_temp);
  fprintf(stderr, "counter_temp=%d\n",counter_temp);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "end ZSMSoutputbit16bit\n");
  */

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
  *counter = counter_temp;
  *currentbit = currentbit_temp;
}/*ZSMSoutputbit16bit*/

void ZSMSEdges(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex,
int xbegin,int xend,int xstep,int ybegin,int yend,int ystep,int zbegin,int zend,int zstep)
{ int ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp;
  int y, z, x;

  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;

  //fprintf(stderr, "Begin ZSMSEdges\n");

  for (z=zbegin; z<=zend; z+=zstep)
  { for (y=ybegin; y<=yend; y+=ystep) 
    { for (x=xbegin; x<=xend; x+=xstep) 
      {
	ZSMSoutputbit(block1,ZSMStable1,x,y,z,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);

	/*
        fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
        fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
	*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  //fprintf(stderr, "End ZSMSEdges\n");

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
}/*ZSMSEdges*/

void ZSMSEdgesVer3(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex, int *ZSMStable1_LastLevelrowindex,int *ZSMStable1_LastLevelrotateindex,int *counter,int xbegin,int xend,int xstep,int ybegin,int yend,int ystep,int zbegin,int zend,int zstep)
{ int ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,ZSMStable1_LastLevelrowindex_temp,ZSMStable1_LastLevelrotateindex_temp,counter_temp,current_bit_temp;
  int y, z, x;

  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;
  ZSMStable1_LastLevelrowindex_temp = *ZSMStable1_LastLevelrowindex;
  ZSMStable1_LastLevelrotateindex_temp = *ZSMStable1_LastLevelrotateindex;
  counter_temp = *counter;

  current_bit_temp = get_bit_from_bit_table(ZSMStable1,ZSMStable1_LastLevelrowindex_temp,ZSMStable1_LastLevelrotateindex_temp);

  for (z=zbegin; z<=zend; z+=zstep)
  { for (y=ybegin; y<=yend; y+=ystep) 
    { for (x=xbegin; x<=xend; x+=xstep) 
      {
        //fprintf(stderr, "Now we are doing ZSMSoutputbit at %d,%d,%d\n",x,y,z);

        counter_temp++;

        if(current_bit_temp>0)
        {
          ZSMSoutputbit(block1,ZSMStable1,x,y,z,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);
        }/*if(counter_bit_temp>0)*/

        if(counter_temp>=16)
        {
          counter_temp = counter_temp -16;
         
          ZSMStable1_LastLevelrotateindex_temp++;
          if(ZSMStable1_LastLevelrotateindex_temp>=8)
          { ZSMStable1_LastLevelrotateindex_temp=ZSMStable1_LastLevelrotateindex_temp-8;
            ZSMStable1_LastLevelrowindex_temp++;
          }/*if*/  

          current_bit_temp = get_bit_from_bit_table(ZSMStable1,ZSMStable1_LastLevelrowindex_temp,ZSMStable1_LastLevelrotateindex_temp);
        }/*if(counter_temp>=16)*/
      }/*for x*/
    }/*for y*/
  }/*for z*/

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
  *ZSMStable1_LastLevelrowindex = ZSMStable1_LastLevelrowindex_temp ;
  *ZSMStable1_LastLevelrotateindex = ZSMStable1_LastLevelrotateindex_temp;
  *counter = counter_temp;
}/*ZSMSEdgesVer3*/

void ZSMSLastLevelIndex(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex)
{ int ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,ZSMStable1_size;
  int y, z, x, counter, currentbit;
  int numCells = size1-1;
  int fullStep = 1<<currentLevel;
  int halfStep = fullStep>>1;

  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;
  counter = 0;
  currentbit = 0;

  /*
  fprintf(stderr, "//////////////////////////////\n");
  fprintf(stderr, "Beginning  ZSMSLastLevelIndex\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  fprintf(stderr, "currentbit=%d\n",currentbit);
  */

  for (z=0; z<=numCells; z+=fullStep)
  { for (y=0; y<=numCells; y+=fullStep) 
    { for (x=halfStep; x<numCells; x+=fullStep) 
      {
        //fprintf(stderr, "Now we are doing ZSMSoutputbit at %d,%d,%d\n",x,y,z);
       
        ZSMSoutputbit16bit(block1,ZSMStable1,x,y,z,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&counter,&currentbit);
      }/*for x*/
    }/*for y*/
  }/*for z*/

  /*
  fprintf(stderr, "after the for loop\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  fprintf(stderr, "currentbit=%d\n",currentbit);
  */

  for (z=0; z<=numCells; z+=fullStep) 
  { for (y=halfStep; y<numCells; y+=fullStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        //fprintf(stderr, "Now we are doing ZSMSoutputbit at %d,%d,%d\n",x,y,z);

	ZSMSoutputbit16bit(block1,ZSMStable1,x,y,z,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&counter,&currentbit);
      }/*for x*/
    }/*for y*/
  }/*for z*/

  /*
  fprintf(stderr, "after the for loop\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  fprintf(stderr, "currentbit=%d\n",currentbit);
  */

  for (z=halfStep; z<numCells; z+=fullStep) 
  { for (y=0; y<=numCells; y+=halfStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        //fprintf(stderr, "Now we are doing ZSMSoutputbit at %d,%d,%d\n",x,y,z);

        ZSMSoutputbit16bit(block1,ZSMStable1,x,y,z,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&counter,&currentbit);
      }/*for x*/
    }/*for y*/
  }/*for z*/

  /*
  fprintf(stderr, "after the for loop\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  fprintf(stderr, "currentbit=%d\n",currentbit);
  */

  if(counter>0)
  {
    ZSMStable1_size=table_size_block(1,1);
    writebit(ZSMStable1,ZSMStable1_size,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,currentbit);
    //write one bit in ZSMS for the rest of the data

    ZSMStable1_rotateindex_temp++;
    if(ZSMStable1_rotateindex_temp>=8)
    { ZSMStable1_rotateindex_temp=ZSMStable1_rotateindex_temp-8;
      ZSMStable1_rowindex_temp++;
    }/*if*/  
  }/*if*/

  /*
  fprintf(stderr, "after  if(counter>0) \n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  fprintf(stderr, "currentbit=%d\n",currentbit);
  */

  if(ZSMStable1_rotateindex_temp > 0)
  {
    ZSMStable1_rowindex_temp = ZSMStable1_rowindex_temp+1;
    ZSMStable1_rotateindex_temp = 0;
  }/*if*/

  /*
  fprintf(stderr, "after if(ZSMStable1_rotateindex_temp > 0)\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  fprintf(stderr, "currentbit=%d\n",currentbit);
  fprintf(stderr, "//////////////////////////////\n");
  */

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
}/*ZSMSLastLevelIndex*/

void ZSMSLastLevel(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex,int *ZSMStable1_LastLevelrowindex)
{ int ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,ZSMStable1_LastLevelrowindex_temp,ZSMStable1_LastLevelrotateindex_temp,counter;
  int numCells = size1-1;
  int fullStep = 1<<currentLevel;
  int halfStep = fullStep>>1; 

  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;
  ZSMStable1_LastLevelrowindex_temp =*ZSMStable1_LastLevelrowindex;
  ZSMStable1_LastLevelrotateindex_temp =0;
  counter = 0;

  /*
  fprintf(stderr, "//////////////////////////////\n");
  fprintf(stderr, "Beginning  ZSMSLastLevel\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrowindex_temp=%d\n",ZSMStable1_LastLevelrowindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrotateindex_temp=%d\n",ZSMStable1_LastLevelrotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  */

  ZSMSEdgesVer3(block1,ZSMStable1,size1, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&ZSMStable1_LastLevelrowindex_temp,&ZSMStable1_LastLevelrotateindex_temp,&counter,
halfStep,numCells-1,fullStep,
0,numCells,fullStep, 
0,numCells,fullStep);
// X done!
  /*
  fprintf(stderr, "After loop\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrowindex_temp=%d\n",ZSMStable1_LastLevelrowindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrotateindex_temp=%d\n",ZSMStable1_LastLevelrotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  */

  ZSMSEdgesVer3(block1,ZSMStable1,size1, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&ZSMStable1_LastLevelrowindex_temp,&ZSMStable1_LastLevelrotateindex_temp,&counter,
0,numCells,halfStep,
halfStep,numCells-1,fullStep,
0,numCells,fullStep);
// Y done!

  /*
  fprintf(stderr, "After loop\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrowindex_temp=%d\n",ZSMStable1_LastLevelrowindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrotateindex_temp=%d\n",ZSMStable1_LastLevelrotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  */

  ZSMSEdgesVer3(block1,ZSMStable1,size1, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&ZSMStable1_LastLevelrowindex_temp,&ZSMStable1_LastLevelrotateindex_temp,&counter,
0,numCells,halfStep, 
0,numCells,halfStep,
halfStep,numCells-1,fullStep);
// Z done!

  /*
  fprintf(stderr, "After loop\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrowindex_temp=%d\n",ZSMStable1_LastLevelrowindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrotateindex_temp=%d\n",ZSMStable1_LastLevelrotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter);
  */

  if(ZSMStable1_rotateindex_temp > 0)
  {
    ZSMStable1_rowindex_temp = ZSMStable1_rowindex_temp+1;
    ZSMStable1_rotateindex_temp = 0;
  }/*if*/

  /*
  fprintf(stderr, "After  if(ZSMStable1_rotateindex_temp > 0)\n");
  fprintf(stderr, "currentLevel=%d\n",currentLevel);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrowindex_temp=%d\n",ZSMStable1_LastLevelrowindex_temp);
  fprintf(stderr, "ZSMStable1_LastLevelrotateindex_temp=%d\n",ZSMStable1_LastLevelrotateindex_temp);
  fprintf(stderr, "counter=%d\n",counter); 
  fprintf(stderr, "Ending  ZSMSLastLevel\n");
  fprintf(stderr, "//////////////////////////////\n");
  */

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
  *ZSMStable1_LastLevelrowindex = ZSMStable1_LastLevelrowindex_temp ;
}/*ZSMSLastLevel*/

void ZSMStable_generation_cell_ver2(float *block1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1)
{
  int ZSMStable1_rotateindex_temp,ZSMStable1_rowindex_temp,ZSMStable1_LastLevelrowindex_temp;
  int numCells;
  int s,lvl,currentLevel;

  ZSMStable1_rotateindex_temp=0;
  ZSMStable1_rowindex_temp=prev_ZSMSindex1;

  s=CELLSIZE-1;
  lvl=0;
  numCells=CELLSIZE-1;

  while(s>1)
    {
      lvl++; 
      s=(s>>1);
    }

  /*
  for (z=0; z<=numCells; z+=numCells) 
  { for (y=0; y<=numCells; y+=numCells) 
    { for (x=0; x<=numCells; x+=numCells) 
      {
        //fprintf(stderr, "Now we are doing ZSMSoutputbit at %d,%d,%d\n",x,y,z);
        ZSMSoutputbit(block1,ZSMStable1,x,y,z,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);
      }
    }
  }*/

  currentLevel=lvl;

  while(currentLevel>=2) 
  { 
    int fullStep = 1<<currentLevel;
    int halfStep = fullStep>>1; 

    ZSMSEdges(block1,ZSMStable1,CELLSIZE, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,
halfStep,numCells-1,fullStep,
0,numCells,fullStep, 
0,numCells,fullStep);
// X done!

    ZSMSEdges(block1,ZSMStable1,CELLSIZE, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,
0,numCells,halfStep,
halfStep,numCells-1,fullStep,
0,numCells,fullStep);
// Y done!

    ZSMSEdges(block1,ZSMStable1,CELLSIZE, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,
0,numCells,halfStep, 
0,numCells,halfStep,
halfStep,numCells-1,fullStep);
// Z done!

    if(ZSMStable1_rotateindex_temp > 0)
    {
      ZSMStable1_rowindex_temp = ZSMStable1_rowindex_temp+1;
      ZSMStable1_rotateindex_temp = 0;
    }/*if*/

    currentLevel=currentLevel-1;  
  }/*while*/

  /*
  fprintf(stderr, "After the while(currentLevel<lvl)\n");
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  */

  ZSMStable1_LastLevelrowindex_temp = ZSMStable1_rowindex_temp;

  ZSMSLastLevelIndex(block1,ZSMStable1,CELLSIZE,1,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);

  ZSMSLastLevel(block1,ZSMStable1,CELLSIZE,1,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&ZSMStable1_LastLevelrowindex_temp);

  *current_ZSMSindex1=ZSMStable1_rowindex_temp;
  //we should revise this line
}/*ZSMStable_generation_cell_ver2*/

void ZSMStable_generation_cell_ver3(float *block1,int *ZSMStable1,int prev_ZSMSindex1,int *current_ZSMSindex1)
{
  int ZSMStable1_rotateindex_temp,ZSMStable1_rowindex_temp,ZSMStable1_LastLevelrowindex_temp,ZSMStable1_row_levelbit,ZSMStable1_rotate_levelbit;
  int numCells;
  int s,lvl,currentLevel;

  ZSMStable1_rotateindex_temp = 0;
  ZSMStable1_rowindex_temp = prev_ZSMSindex1;
  ZSMStable1_row_levelbit = prev_ZSMSindex1;
  ZSMStable1_rotate_levelbit = 0;

  s=CELLSIZE-1;
  lvl=0;
  numCells=CELLSIZE-1;

  while(s>1)
    {
      lvl++; 
      s=(s>>1);
    }
 
  EnterAllLevelBit(block1,ZSMStable1,CELLSIZE,lvl,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);

  /*
  fprintf(stderr, "////////////////////////////////////\n");
  fprintf(stderr, "Beginning ZSMStable_generation_cell_ver3\n");
  fprintf(stderr, "After EnterAllLevelBit\n");
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
  */

  currentLevel=lvl;

  while(currentLevel>=2) 
  { 
    if(GetNextLevelBit(ZSMStable1,&ZSMStable1_row_levelbit,&ZSMStable1_rotate_levelbit))
    {
      int fullStep = 1<<currentLevel;
      int halfStep = fullStep>>1; 

      ZSMSEdges(block1,ZSMStable1,CELLSIZE, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,
halfStep,numCells-1,fullStep,
0,numCells,fullStep, 
0,numCells,fullStep);
// X done!

      ZSMSEdges(block1,ZSMStable1,CELLSIZE, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,
0,numCells,halfStep,
halfStep,numCells-1,fullStep,
0,numCells,fullStep);
// Y done!

      ZSMSEdges(block1,ZSMStable1,CELLSIZE, currentLevel,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,
0,numCells,halfStep, 
0,numCells,halfStep,
halfStep,numCells-1,fullStep);
// Z done!

      if(ZSMStable1_rotateindex_temp > 0)
      {
        ZSMStable1_rowindex_temp = ZSMStable1_rowindex_temp+1;
        ZSMStable1_rotateindex_temp = 0;
      }/*if*/
    }/*if GetLevelBit*/

    /*
    fprintf(stderr, "After GetLevelBit\n");
    fprintf(stderr, "currentLevel=%d\n",currentLevel);
    fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
    fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
    */

    currentLevel=currentLevel-1;  
  }/*while*/

  if(GetNextLevelBit(ZSMStable1,&ZSMStable1_row_levelbit,&ZSMStable1_rotate_levelbit))
  {
    ZSMStable1_LastLevelrowindex_temp = ZSMStable1_rowindex_temp;

    ZSMSLastLevelIndex(block1,ZSMStable1,CELLSIZE,1,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);

    /*
    fprintf(stderr, "After ZSMSLastLevelIndex\n");
    fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
    fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
    */

    ZSMSLastLevel(block1,ZSMStable1,CELLSIZE,1,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp,&ZSMStable1_LastLevelrowindex_temp);

    /*
    fprintf(stderr, "After ZSMSLastLevel \n");
    fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
    fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);
    */
  }/*if*/

  /*
  fprintf(stderr, "After ZSMSLastLevel\n");
  fprintf(stderr, "currentLevel=%d\n",1);
  fprintf(stderr, "ZSMStable1_rowindex_temp=%d\n",ZSMStable1_rowindex_temp);
  fprintf(stderr, "ZSMStable1_rotateindex_temp=%d\n",ZSMStable1_rotateindex_temp);

  fprintf(stderr, "End ZSMStable_generation_cell_ver3\n");
  fprintf(stderr, "////////////////////////////////////\n");
  */

  *current_ZSMSindex1=ZSMStable1_rowindex_temp;
  //we should revise this line
}/*ZSMStable_generation_cell_ver3*/

void EnterLevelBit(float *block1,int *ZSMStable1,int size1,int currentLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex)
{
  int ZSMStable1_rotateindex_temp,ZSMStable1_rowindex_temp,ZSMStable1_size,currentbit,x,y,z;
  int numCells = size1-1;
  int fullStep = 1<<currentLevel;
  int halfStep = fullStep>>1; 

  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;
  ZSMStable1_size=table_size_block(1,1); 
  currentbit = 0;
  
  for (z=0; z<=numCells; z+=fullStep)
  { for (y=0; y<=numCells; y+=fullStep) 
    { for (x=halfStep; x<numCells; x+=fullStep) 
      {
        if(!zeroelement(block1,x,y,z))
	  currentbit = 1;       
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for (z=0; z<=numCells; z+=fullStep) 
  { for (y=halfStep; y<numCells; y+=fullStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        if(!zeroelement(block1,x,y,z))
	  currentbit = 1;      
      }/*for x*/
    }/*for y*/
  }/*for z*/

  for (z=halfStep; z<numCells; z+=fullStep) 
  { for (y=0; y<=numCells; y+=halfStep) 
    { for (x=0; x<=numCells; x+=halfStep) 
      {
        if(!zeroelement(block1,x,y,z))
	  currentbit = 1;        
      }/*for x*/
    }/*for y*/
  }/*for z*/

  writebit(ZSMStable1,ZSMStable1_size,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp,currentbit);
  //write one bit in the ZSMStable1_rotateindex position(bit rotating) at the ZSMStable1_rowindex of ZSMStable1

  ZSMStable1_rotateindex_temp++;
  if(ZSMStable1_rotateindex_temp>=8)
  { ZSMStable1_rotateindex_temp=ZSMStable1_rotateindex_temp-8;
    ZSMStable1_rowindex_temp++;
  }/*if*/  

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
}/*EnterLevelBit*/


void EnterAllLevelBit(float *block1,int *ZSMStable1,int size1,int maxLevel,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex)
{ int x,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp;
 
  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex;

  for (x=maxLevel; x>=1; x--) 
  {
    EnterLevelBit(block1,ZSMStable1,size1,x,&ZSMStable1_rowindex_temp,&ZSMStable1_rotateindex_temp);        
  }/*for x*/

 if(ZSMStable1_rotateindex_temp > 0)
      {
        ZSMStable1_rowindex_temp = ZSMStable1_rowindex_temp+1;
        ZSMStable1_rotateindex_temp = 0;
      }/*if*/

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;
}/*EnterAllLevelBit*/

/*
int GetLevelBit(float *block1,int *ZSMStable1,int size1,int currentLevel,int ZSMStable1_begin_row_levelbit)
{
  int ZSMStable1_rotateindex_temp,ZSMStable1_rowindex_temp,result1_temp;

  ZSMStable1_rotateindex_temp = currentLevel-1;
  ZSMStable1_rowindex_temp = ZSMStable1_begin_row_levelbit;

  while(ZSMStable1_rotateindex_temp>=8)
  { ZSMStable1_rotateindex_temp=ZSMStable1_rotateindex_temp-8;
    ZSMStable1_rowindex_temp++;
  } 

  result1_temp = get_bit_from_bit_table(ZSMStable1,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp);

  return result1_temp;
}*/

int GetNextLevelBit(int *ZSMStable1,int *ZSMStable1_rowindex,int *ZSMStable1_rotateindex)
//start from maxLevel to 1
{
  int ZSMStable1_rotateindex_temp,ZSMStable1_rowindex_temp,currentbit;
 
  ZSMStable1_rowindex_temp = *ZSMStable1_rowindex;
  ZSMStable1_rotateindex_temp = *ZSMStable1_rotateindex; 
  
  currentbit = get_bit_from_bit_table(ZSMStable1,ZSMStable1_rowindex_temp,ZSMStable1_rotateindex_temp);

  ZSMStable1_rotateindex_temp++;
  if(ZSMStable1_rotateindex_temp>=8)
  { ZSMStable1_rotateindex_temp=ZSMStable1_rotateindex_temp-8;
    ZSMStable1_rowindex_temp++;
  }/*if*/  

  *ZSMStable1_rowindex = ZSMStable1_rowindex_temp;
  *ZSMStable1_rotateindex = ZSMStable1_rotateindex_temp;

  return currentbit;
}/*GetNextLevelBit*/

























