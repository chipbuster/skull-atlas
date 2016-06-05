#include "zerobit.h"
#include "block.h"
#include "waveAnthony.h"
#include "wave.h"
#include "error.h"
#include "zeroblocktable.h"

void cellprocessing(float epsilon,float maxval,float* func,float* lambda,float* recon)
//compression and reconstruction of a cell
{ int dimcell[3];

  wavelet2(CELLSIZE,maxval,epsilon,func,lambda);
  
  dimcell[0]=CELLSIZE; 
  dimcell[1]=CELLSIZE; 
  dimcell[2]=CELLSIZE; 

  thresh(epsilon,dimcell,maxval,lambda);

  rebuildWavelet2(CELLSIZE,recon,lambda);
}/*cellprocessing*/

void blockprocessing(float epsilon,float maxval,float* func,float* lambda,float* recon)
//compression and reconstruction of a block
{ int z,y,x,nvertsnew;
 
  nvertsnew=CELLSIZE*CELLSIZE*CELLSIZE;
  float* inputcell1 = 0;
  inputcell1 = new float[nvertsnew];
  float* processingcell1 = 0;
  processingcell1 = new float[nvertsnew];
  float* outputcell1 = 0;
  outputcell1 = new float[nvertsnew];

  for(z=0; z<BLOCKSIZE; z=z+CELLSIZE) 
  { for(y=0; y<BLOCKSIZE; y=y+CELLSIZE) 
    { for(x=0; x<BLOCKSIZE; x=x+CELLSIZE) 
      {       
        copyblock(BLOCKSIZE,x,y,z,func,inputcell1,2);
        //copy a cell of func to inputcell1     
        
        cellprocessing(epsilon,maxval,inputcell1,processingcell1,outputcell1);              

        reversecopyblock(BLOCKSIZE,x,y,z,lambda,processingcell1,2);
        //copy a block of compression result back from processingblock1

        reversecopyblock(BLOCKSIZE,x,y,z,recon,outputcell1,2);
        //reverse copy outputcell1 to recon  
      }/*for x*/
    }/*for y*/
  }/*for z*/

  delete[] inputcell1;
  delete[] processingcell1;
  delete[] outputcell1;
}/*blockprocessing*/

void volumeprocessing(float epsilon,float maxval,int dim1[3],float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3)
//compression and reconstruction of a volume
//error estimation is done
{ int z,y,x,nvertsnew; 

  nvertsnew=BLOCKSIZE*BLOCKSIZE*BLOCKSIZE;
  float* inputblock1 = 0;
  inputblock1 = new float[nvertsnew];
  float* processingblock1 = 0;
  processingblock1 = new float[nvertsnew];
  float* outputblock1 = 0;
  outputblock1 = new float[nvertsnew];

  for(z=0; z<dim1[2]; z=z+BLOCKSIZE) 
  { fprintf(stderr,"In volumeprocessing,Now is processing %d\n",z);

    for(y=0; y<dim1[1]; y=y+BLOCKSIZE) 
    { for(x=0; x<dim1[0]; x=x+BLOCKSIZE) 
      { 
        copyblockver2(dim1,x,y,z,func,inputblock1,1);
        //copy a block to inputblock1

        blockprocessing(epsilon,maxval,inputblock1,processingblock1,outputblock1);        

        reversecopyblockver2(dim1,x,y,z,recon,outputblock1,1);
        //copy a block back from outputblock1   
        reversecopyblockver2(dim1,x,y,z,lambda,processingblock1,1);
        //copy a block of compression result back from processingblock1
      }/*for x*/
    }/*for y*/
  }/*for z*/

  delete[] inputblock1;
  delete[] processingblock1;
  delete[] outputblock1;

  /*
  dim[0]=newsize1; 
  dim[1]=newsize1; 
  dim[2]=newsize1; 
  nverts=dim[0]*dim[1]*dim[2];
  */

  //errorcalculation(epsilon,dim,maxval,nverts,func,lambda,recon,argv1,argv2,argv3);
}/*volumeprocessing*/

/*
void blockprocessingerror(float epsilon,float maxval,int newsize1,float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3)
{ int z,y,x,nvertsnew;
  float maxvalcell; 
  int dim[3],nverts;

  nvertsnew=CELLSIZE*CELLSIZE*CELLSIZE;
  float* inputcell1 = 0;
  inputcell1 = new float[nvertsnew];
  float* processingcell1 = 0;
  processingcell1 = new float[nvertsnew];
  float* outputcell1 = 0;
  outputcell1 = new float[nvertsnew];

  for(z=0; z<BLOCKSIZE; z=z+CELLSIZE) 
  { for(y=0; y<BLOCKSIZE; y=y+CELLSIZE) 
    { for(x=0; x<BLOCKSIZE; x=x+CELLSIZE) 
      {
        copyblock(BLOCKSIZE,x,y,z,func,inputcell1,2);
       
        if(!zeroblock(inputcell1,2))
	{
          maxvalcell=fabs_maximum_block(inputcell1,CELLSIZE);
          cellprocessing(maxvalcell,epsilon,inputcell1,processingcell1,outputcell1);

          reversecopyblock(BLOCKSIZE,x,y,z,recon,outputcell1,2);

          reversecopyblock(BLOCKSIZE,x,y,z,lambda,processingcell1,2);
        }
        else
        {
          reversecopyblock(BLOCKSIZE,x,y,z,recon,inputcell1,2);

          reversecopyblock(BLOCKSIZE,x,y,z,lambda,inputcell1,2);
        }

        dim[0]=CELLSIZE; 
        dim[1]=CELLSIZE; 
        dim[2]=CELLSIZE; 
        nverts=dim[0]*dim[1]*dim[2];

        errorcalculation(epsilon,dim,maxval,nverts,func,lambda,recon,argv1,argv2,argv3);  
      }
    }
  }

  delete[] inputcell1;
  delete[] processingcell1;
  delete[] outputcell1;
}
*/

/*
void cellprocessingerror(float epsilon,float maxval,int newsize1,float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3)
{
  int dimcell[3];
  int dim[3],nverts;

  wavelet2(CELLSIZE,maxval,epsilon,func,lambda);
  
  dimcell[0]=CELLSIZE; 
  dimcell[1]=CELLSIZE; 
  dimcell[2]=CELLSIZE; 

  thresh(epsilon,dimcell,maxval,lambda);

  rebuildWavelet2(CELLSIZE,recon,lambda);

  dim[0]=CELLSIZE; 
  dim[1]=CELLSIZE; 
  dim[2]=CELLSIZE; 
  nverts=dim[0]*dim[1]*dim[2];

  errorcalculation(epsilon,dim,maxval,nverts,func,lambda,recon,argv1,argv2,argv3);  
}*/

void extracalculation(int oldsize1,int newsize1,float epsilon,float* func,float* lambda,float maxval)
//output the compression ratio including the zerobit table 
//1) rawiv file size 
//2) nonzero wavelet coefficients and zerobit encoding file size
//3) compression ratio=1)/2)
{
  int oldnerts,newnerts;
  int befzero=0;
  int aftzero=0;
  int zeroblocktable1size,zerocelltable1size,compressioncost;
  float compressionratio;

  oldnerts=oldsize1*oldsize1*oldsize1;
  newnerts=newsize1*newsize1*newsize1;

  for(int i=0; i<newnerts; i++)
    { 
      if(fabs(func[i])<=0.0000000001*maxval)
	befzero++;
      if(fabs(lambda[i])<=0.0000000001*maxval)
	aftzero++;
    }/*for i*/

  befzero=befzero-(newnerts-oldnerts);
  aftzero=aftzero-(newnerts-oldnerts);

  zeroblocktable1size=size_zeroblocktable(newsize1,1);
  zerocelltable1size=size_zeroblocktable(newsize1,2);

  compressioncost=(oldnerts-aftzero)*1.5+zeroblocktable1size+zerocelltable1size;
  compressionratio=(float)oldnerts/((float)compressioncost);
  //  compressionratio=((float)oldnerts-(float)befzero)/((float)compressioncost);

  printf("%d\t%d\t%8.4f\n"
	 ,oldnerts
         ,compressioncost
	 ,compressionratio);
}/*extracalculation*/




