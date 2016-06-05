
#include "waveAnthony.h"

void doXEdges(int size, int currentLevel,float* lambda)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=0; z<=numCells; z+=fullStep) {
		for (y=0; y<=numCells; y+=fullStep) {

			for (x=halfStep; x<numCells; x+=fullStep) {
				lambda[size*size*z+size*y+x] = (lambda[size*size*z+size*y+(x-halfStep)]+lambda[size*size*z+size*y+(x+halfStep)])/2.0;
			}

		}
	}
}

void doYEdges(int size, int currentLevel,float* lambda)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=0; z<=numCells; z+=fullStep) {
		for (y=halfStep; y<numCells; y+=fullStep) {
			
			for (x=0; x<=numCells; x+=halfStep) {
				lambda[size*size*z+size*y+x] = (lambda[size*size*z+size*(y-halfStep)+x]+lambda[size*size*z+size*(y+halfStep)+x])/2.0;
			}

		}
	}
}
void threshlevel(int size, int currentLevel,float maxval,float epsilon, float *lambda)
{
  int y, z, x;
  int numCells = size-1;
  int fullStep = 1<<currentLevel;
  int halfStep = fullStep>>1;
  
  for (z=0; z<=numCells; z+=fullStep) 
    {
      for (y=halfStep; y<numCells; y+=fullStep) 
	{
	  
	  for (x=0; x<=numCells; x+=halfStep) 
	    {
       
	      if(fabs(lambda[z*size*size+y*size+x])<=( epsilon/(pow(2,(float)(currentLevel)))*maxval))      		  
		lambda[z*size*size+y*size+x]=0.00;
	      
	    }
	  
	}
    }
}
void doZEdges(int size, int currentLevel,float *lambda)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=halfStep; z<numCells; z+=fullStep) {
		for (y=0; y<=numCells; y+=halfStep) {
			int rowStart = size*size*z+size*y;

			for (x=0; x<=numCells; x+=halfStep) {
				lambda[rowStart+x] = (lambda[size*size*(z-halfStep)+size*y+x]+lambda[size*size*(z+halfStep)+size*y+x])/2.0;
			}

		}
	}
}

void doSubtraction(int size,float *f, float *lam) 
{
  //	double* lam = (double*) lambda;
  //	double* f = (double*) func;

	int c;
	int nverts = (size)*(size)*(size);

	for (c=0; c<nverts; c++) {
		lam[c] = f[c]-lam[c];
	}

	lam[size*size*0 + size*0 + 0]                      = f[size*size*0 + size*0 + 0]         ;
	lam[size*size*0 + size*0 + (size-1)]               = f[size*size*0 + size*0 + (size-1)]      ;
	lam[size*size*0 + size*(size-1) + 0]               = f[size*size*0 + size*(size-1) + 0]      ;
	lam[size*size*0 + size*(size-1) + (size-1)]        = f[size*size*0 + size*(size-1) + (size-1)]   ;
	lam[size*size*(size-1) + size*0 + 0]               = f[size*size*(size-1) + size*0 + 0]      ;
	lam[size*size*(size-1) + size*0 + (size-1)]        = f[size*size*(size-1) + size*0 +(size-1)]   ;
	lam[size*size*(size-1) + size*(size-1) + 0]        = f[size*size*(size-1) + size*(size-1) + 0]   ;
	lam[size*size*(size-1) + size*(size-1) + (size-1)] = f[size*size*(size-1) + size*(size-1) + (size-1)];

}

void wavelet2Helper(int size, int currentLevel,float maxval,float epsilon,float *lambda)
{
	//fprintf(stderr, "Doing X Edges\n");
	doXEdges(size, currentLevel,lambda);
	//fprintf(stderr, "Doing Y Edges\n");
	doYEdges(size, currentLevel,lambda);
	//fprintf(stderr, "Doing Z Edges\n");
	doZEdges(size, currentLevel,lambda);

	//	threshlevel(size,currentLevel,maxval,epsilon,lambda);

}
void wavelet2(int size,float maxval,float epsilon,float *func,float* lambda)
{
        //fprintf(stderr, "Step 1: Copying\n");
	memcpy(lambda, func, sizeof(float)*(size)*(size)*(size));
	//fprintf(stderr, "%f %f\n", func[0], lambda[0]);
	// determine the number of levels by dividing by two
	int numLevels = 0, s = size-1;
	while (s>1) {
		s = (s>>1);
		numLevels++;
	}

	int level;
	//fprintf(stderr, "Step2: Interpolating\n");
	for (level=1; level<=numLevels; level++) {
	        //fprintf(stderr, "Doing level %d\n", level);
		wavelet2Helper(size, level,maxval,epsilon,lambda);
	}
	//fprintf(stderr, "%f %f\n", func[0], lambda[0]);

	//fprintf(stderr, "Step 3: Subtracting\n");
	doSubtraction(size,func,lambda);
}
void rebuildXEdges(int size, int currentLevel,float* recon)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=0; z<=numCells; z+=fullStep) {
		for (y=0; y<=numCells; y+=fullStep) {

			for (x=halfStep; x<numCells; x+=fullStep) {
				recon[size*size*z+size*y+x] = ((recon[size*size*z+size*y+(x-halfStep)]+recon[size*size*z+size*y+(x+halfStep)])/2.0);
			}

		}
	}
}

void rebuildYEdges(int size, int currentLevel,float *recon)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=0; z<=numCells; z+=fullStep) {
		for (y=halfStep; y<numCells; y+=fullStep) {
			
			for (x=0; x<=numCells; x+=halfStep) {
				recon[size*size*z+size*y+x] = ((recon[size*size*z+size*(y-halfStep)+x]+recon[size*size*z+size*(y+halfStep)+x])/2.0);
			}

		}
	}
}

void rebuildZEdges(int size, int currentLevel,float *recon)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=halfStep; z<numCells; z+=fullStep) {
		for (y=0; y<=numCells; y+=halfStep) {
			
			for (x=0; x<=numCells; x+=halfStep) {
				recon[size*size*z+size*y+x] = ((recon[size*size*(z-halfStep)+size*y+x]+recon[size*size*(z+halfStep)+size*y+x])/2.0);
			}

		}
	}
}

void addXEdges(int size, int currentLevel,float *recon,float* lambda)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=0; z<=numCells; z+=fullStep) {
		for (y=0; y<=numCells; y+=fullStep) {

			for (x=halfStep; x<numCells; x+=fullStep) {
				recon[size*size*z+size*y+x] += lambda[size*size*z+size*y+x];
			}

		}
	}
}

void addYEdges(int size, int currentLevel,float *recon,float* lambda)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=0; z<=numCells; z+=fullStep) {
		for (y=halfStep; y<numCells; y+=fullStep) {
			
			for (x=0; x<=numCells; x+=halfStep) {
				recon[size*size*z+size*y+x] += lambda[size*size*z+size*y+x];
			}
		}
	}
}

void addZEdges(int size, int currentLevel,float* recon,float* lambda)
{
	int y, z, x;
	int numCells = size-1;
	int fullStep = 1<<currentLevel;
	int halfStep = fullStep>>1;

	for (z=halfStep; z<numCells; z+=fullStep) {
		for (y=0; y<=numCells; y+=halfStep) {
			
			for (x=0; x<=numCells; x+=halfStep) {
				recon[size*size*z+size*y+x] += lambda[size*size*z+size*y+x];
			}

		}
	}
}

void rebuildWavelet2Helper(int size, int currentLevel,float* recon)
{
	//fprintf(stderr, "Doing X Edges\n");
	rebuildXEdges(size, currentLevel,recon);
	//fprintf(stderr, "Doing Y Edges\n");
	rebuildYEdges(size, currentLevel,recon);
	//fprintf(stderr, "Doing Z Edges\n");
	rebuildZEdges(size, currentLevel,recon);
}

void doAddition(int size, int currentLevel,float* recon, float* lambda) 
{
	/*
	int fullStep = 1<<currentLevel;

	int x,y,z;
	
	// make the corners 0
	recon[0][0][0] = 0.0;
	recon[0][0][size] = 0.0;
	recon[0][size][0] = 0.0;
	recon[0][size][size] = 0.0;
	recon[size][0][0] = 0.0;
	recon[size][0][size] = 0.0;
	recon[size][size][0] = 0.0;
	recon[size][size][size] = 0.0;

	for (z=0; z<=size; z+=fullStep) {
		for (y=0; y<=size; y+=fullStep) {
			for (x=0; x<=size; x+=fullStep) {
				recon[z][y][x]+=lambda[z][y][x];
			}
		}
	}
	*/
	//fprintf(stderr, "Adding X Edges\n");
	addXEdges(size, currentLevel,recon,lambda);
	//fprintf(stderr, "Adding Y Edges\n");
	addYEdges(size, currentLevel,recon,lambda);
	//fprintf(stderr, "Adding Z Edges\n");
	addZEdges(size, currentLevel,recon,lambda);

}

void rebuildWavelet2(int size,float *recon, float* lambda)
{
	// copy the corners

	recon[size*size*0 + size*0 + 0]                      = lambda[size*size*0 + size*0 + 0]         ;
	recon[size*size*0 + size*0 + (size-1)]               = lambda[size*size*0 + size*0 + (size-1)]      ;
	recon[size*size*0 + size*(size-1) + 0]               = lambda[size*size*0 + size*(size-1) + 0]      ;
	recon[size*size*0 + size*(size-1) + (size-1)]        = lambda[size*size*0 + size*(size-1) + (size-1)]   ;
	recon[size*size*(size-1) + size*0 + 0]               = lambda[size*size*(size-1) + size*0 + 0]      ;
	recon[size*size*(size-1) + size*0 + (size-1)]        = lambda[size*size*(size-1) + size*0 +(size-1)]   ;
	recon[size*size*(size-1) + size*(size-1) + 0]        = lambda[size*size*(size-1) + size*(size-1) + 0]   ;
	recon[size*size*(size-1) + size*(size-1) + (size-1)] = lambda[size*size*(size-1) + size*(size-1) + (size-1)];

	//fprintf(stderr, "Step 1: Copying\n");
	//memcpy(recon, lambda, sizeof(double)*(size+1)*(size+1)*(size+1));
	// determine the number of levels by dividing by two
	int numLevels = 0, s = size-1;
	while (s>1) {
		s = (s>>1);
		numLevels++;
	}

	int level;
	//fprintf(stderr, "Interpolating\n");
	for (level=numLevels; level>=1; level--) {
	        //fprintf(stderr, "Doing level %d\n", level);
		rebuildWavelet2Helper(size, level,recon);
		doAddition(size, level,recon,lambda);
	}
}


