#include <algorithm>
#include <stdint.h>
#include "wave.h"
#include "wavedata.h"
#include "block.h"

float* functest = 0;

template <class T> void fixEndianness(T& x) {
#ifdef _LITTLE_ENDIAN
  char* y = (char*) (void*) &x;
  ::std::reverse(y, y + sizeof(T));
#endif
}

template <class T> T read(FILE* file) {
  T x;
  fread(&x, sizeof(T), 1, file);
  fixEndianness(x);
  return x;
}

void getFloat(float* x, int n, FILE* file) {
  for (int i=0; i<n; ++i) {
    x[i] = read<float>(file);
  }
}

void getShort(unsigned short* x, int n, FILE* file) {
  for (int i=0; i<n; ++i) {
    x[i] = read<uint16_t>(file);
  }
}

void getInt(int* x, int n, FILE* file) {
  for (int i=0; i<n; ++i) {
    x[i] = read<uint32_t>(file);
  }
}

void writeFloat(float* x, int n, FILE* file) {
  for (int i=0; i<n; ++i) {
    float a = *x;
    fixEndianness(a);
    fwrite(&a, sizeof(float), 1, file);
  }
}

void writeUint32(int* x, int n, FILE* file) {
  for (int i=0; i<n; ++i) {
    uint32_t a = *x;
    fixEndianness(a);
    fwrite(&a, sizeof(uint32_t), 1, file);
  }
}

void RebuildWavelet(int x, int y, int z,int dim, int size,float *lambda, float *recon)
  //accesses func
  //changes lamda
{
  if (size ==1 )
    return;

  //  float v4 = (lambda[x][y][z] + lambda[x][y+size][z])/2;//left

  int dist = size/2;

  static int scounter=0;
  scounter++;
  //if (scounter%50024==0) {
  //  printf("%d\n", scounter);     
 //}
  if(scounter==1)
    {
      recon[x        + dim*y        + dim*dim*z]       =lambda[x        + dim*y        + dim*dim*z];
      recon[(x+size) + dim*y        + dim*dim*z]       =lambda[(x+size) + dim*y        + dim*dim*z];
      recon[x        + dim*(y+size) + dim*dim*z]       =lambda[x        + dim*(y+size) + dim*dim*z];
      recon[(x+size) + dim*(y+size) + dim*dim*z]       =lambda[(x+size) + dim*(y+size) + dim*dim*z];
      recon[x        + dim*y        + dim*dim*(z+size)]=lambda[x        + dim*y        + dim*dim*(z+size)];
      recon[x+size   + dim*y        + dim*dim*(z+size)]=lambda[(x+size) + dim*y        + dim*dim*(z+size)];
      recon[x        + dim*(y+size) + dim*dim*(z+size)]=lambda[x        + dim*(y+size) + dim*dim*(z+size)];
      recon[(x+size) + dim*(y+size) + dim*dim*(z+size)]=lambda[(x+size) + dim*(y+size) + dim*dim*(z+size)];
    }
  float v0 = recon[x        + dim*y        + dim*dim*z];
  float v1 = recon[(x+size) + dim*y        + dim*dim*z];
  float v2 = recon[x        + dim*(y+size) + dim*dim*z];
  float v3 = recon[(x+size) + dim*(y+size) + dim*dim*z];
  float v4 = recon[x        + dim*y        + dim*dim*(z+size)];
  float v5 = recon[(x+size) + dim*y        + dim*dim*(z+size)];
  float v6 = recon[x        + dim*(y+size) + dim*dim*(z+size)];
  float v7 = recon[(x+size) + dim*(y+size) + dim*dim*(z+size)];

  //interpolated values
  float e0  = (v0 + v1)/2;
  float e1  = (v1 + v3)/2;
  float e2  = (v2 + v3)/2;
  float e3  = (v0 + v2)/2;
  float e4  = (v4 + v5)/2;
  float e5  = (v5 + v7)/2;
  float e6  = (v6 + v7)/2;
  float e7  = (v4 + v6)/2;
  float e8  = (v0 + v4)/2;
  float e9  = (v1 + v5)/2;
  float e10 = (v3 + v7)/2;
  float e11 = (v2 + v6)/2;

  float f0 = (e0 + e4)/2;
  float f1 = (e1 + e5)/2;
  float f2 = (e2 + e6)/2;
  float f3 = (e3 + e7)/2;
  float f4 = (e0 + e2)/2;
  float f5 = (e4 + e6)/2;

  float c = (f4 + f5)/2;
  float test = (f0 + f2)/2;
  if (fabs(test-c)>0.001) fprintf(stderr,"ERROR\n");

  recon[(x+dist) + dim*y        + dim*dim*z]= e0 + lambda[(x+dist) + dim*y        + dim*dim*z];
  recon[(x+size) + dim*(y+dist) + dim*dim*z]= e1 + lambda[(x+size) + dim*(y+dist) + dim*dim*z];  
  recon[(x+dist) + dim*(y+size) + dim*dim*z]= e2 + lambda[(x+dist) + dim*(y+size) + dim*dim*z];
  recon[x        + dim*(y+dist) + dim*dim*z]= e3 + lambda[x        + dim*(y+dist) + dim*dim*z];

  recon[(x+dist) + dim*y        + dim*dim*(z+size)]= e4 + lambda[(x+dist) + dim*y        + dim*dim*(z+size)];
  recon[(x+size) + dim*(y+dist) + dim*dim*(z+size)]= e5 + lambda[(x+size) + dim*(y+dist) + dim*dim*(z+size)];  
  recon[(x+dist) + dim*(y+size) + dim*dim*(z+size)]= e6 + lambda[(x+dist) + dim*(y+size) + dim*dim*(z+size)];
  recon[x        + dim*(y+dist) + dim*dim*(z+size)]= e7 + lambda[x        + dim*(y+dist) + dim*dim*(z+size)];

  recon[x        + dim*y        + dim*dim*(z+dist)]= e8  + lambda[x        + dim*y        + dim*dim*(z+dist)];
  recon[(x+size) + dim*y        + dim*dim*(z+dist)]= e9  + lambda[(x+size) + dim*y        + dim*dim*(z+dist)];
  recon[(x+size) + dim*(y+size) + dim*dim*(z+dist)]= e10 + lambda[(x+size) + dim*(y+size) + dim*dim*(z+dist)];
  recon[x        + dim*(y+size) + dim*dim*(z+dist)]= e11 + lambda[x        + dim*(y+size) + dim*dim*(z+dist)];
 
  recon[(x+dist) + dim*y        + dim*dim*(z+dist)]= f0 + lambda[(x+dist) + dim*y        + dim*dim*(z+dist)];
  recon[(x+size) + dim*(y+dist) + dim*dim*(z+dist)]= f1 + lambda[(x+size) + dim*(y+dist) + dim*dim*(z+dist)];  
  recon[(x+dist) + dim*(y+size) + dim*dim*(z+dist)]= f2 + lambda[(x+dist) + dim*(y+size) + dim*dim*(z+dist)];
  recon[x        + dim*(y+dist) + dim*dim*(z+dist)]= f3 + lambda[x        + dim*(y+dist) + dim*dim*(z+dist)];
  recon[(x+dist) + dim*(y+dist) + dim*dim*z]       = f4 + lambda[(x+dist) + dim*(y+dist) + dim*dim*z];
  recon[(x+dist) + dim*(y+dist) + dim*dim*(z+size)]= f5 + lambda[(x+dist) + dim*(y+dist) + dim*dim*(z+size)];

  recon[(x+dist) + dim*(y+dist) + dim*dim*(z+dist)]= c  + lambda[(x+dist) + dim*(y+dist) + dim*dim*(z+dist)];

  //insert hell here

  RebuildWavelet(x,y,z,dim,dist,lambda,recon);
  RebuildWavelet(x+dist,y,z,dim,dist,lambda,recon);
  RebuildWavelet(x,y+dist,z,dim,dist,lambda,recon);
  RebuildWavelet(x+dist,y+dist,z,dim,dist,lambda,recon);
  RebuildWavelet(x,y,z+dist,dim,dist,lambda,recon);
  RebuildWavelet(x+dist,y,z+dist,dim,dist,lambda,recon);
  RebuildWavelet(x,y+dist,z+dist,dim,dist,lambda,recon);
  RebuildWavelet(x+dist,y+dist,z+dist,dim,dist,lambda,recon);

}

inline void adjust(int x, int y, int z, int dim, float eps, float maxval, float* func, float* lambda)
{
  if (fabs(lambda[z*dim*dim+y*dim+x])<(eps*maxval)) {
    //printf("WHA\n");
    func[z*dim*dim+y*dim+x] -= lambda[z*dim*dim+y*dim+x];
    lambda[z*dim*dim+y*dim+x]  = 0.0;
  }
}

void Wavelet(int x, int y, int z,int dim, int size,int lvl,float eps,float maxval, float *func, float *lambda)
  //accesses func
  //changes lamda
{
  if (size ==1 )
    return;

  //  float v4 = (lambda[x][y][z] + lambda[x][y+size][z])/2;//left

  int dist = size/2;
  //int lvl=0;
  //int tmp=dist;

  static int scounter=0;
  scounter++;

  //  if (scounter%50024==0) {
  // printf("%d\n", scounter);     
  //}
  /*
  while(tmp>1)
    {
      lvl++;
      tmp=(tmp>>1);
    }
  */
  if(scounter==1)
    {
      lambda[x        + dim*y        + dim*dim*z]       =func[x        + dim*y        + dim*dim*z];
      lambda[(x+size) + dim*y        + dim*dim*z]       =func[(x+size) + dim*y        + dim*dim*z];
      lambda[x        + dim*(y+size) + dim*dim*z]       =func[x        + dim*(y+size) + dim*dim*z];
      lambda[(x+size) + dim*(y+size) + dim*dim*z]       =func[(x+size) + dim*(y+size) + dim*dim*z];
      lambda[x        + dim*y        + dim*dim*(z+size)]=func[x        + dim*y        + dim*dim*(z+size)];
      lambda[x+size   + dim*y        + dim*dim*(z+size)]=func[(x+size) + dim*y        + dim*dim*(z+size)];
      lambda[x        + dim*(y+size) + dim*dim*(z+size)]=func[x        + dim*(y+size) + dim*dim*(z+size)];
      lambda[(x+size) + dim*(y+size) + dim*dim*(z+size)]=func[(x+size) + dim*(y+size) + dim*dim*(z+size)];
    }

  float v0 = func[x        + dim*y        + dim*dim*z];
  float v1 = func[(x+size) + dim*y        + dim*dim*z];
  float v2 = func[x        + dim*(y+size) + dim*dim*z];
  float v3 = func[(x+size) + dim*(y+size) + dim*dim*z];
  float v4 = func[x        + dim*y        + dim*dim*(z+size)];
  float v5 = func[(x+size) + dim*y        + dim*dim*(z+size)];
  float v6 = func[x        + dim*(y+size) + dim*dim*(z+size)];
  float v7 = func[(x+size) + dim*(y+size) + dim*dim*(z+size)];


  //interpolated values
  float e0  = (v0 + v1)/2;
  float e1  = (v1 + v3)/2;
  float e2  = (v2 + v3)/2;
  float e3  = (v0 + v2)/2;
  float e4  = (v4 + v5)/2;
  float e5  = (v5 + v7)/2;
  float e6  = (v6 + v7)/2;
  float e7  = (v4 + v6)/2;
  float e8  = (v0 + v4)/2;
  float e9  = (v1 + v5)/2;
  float e10 = (v3 + v7)/2;
  float e11 = (v2 + v6)/2;

  float f0 = (e0 + e4)/2;
  float f1 = (e1 + e5)/2;
  float f2 = (e2 + e6)/2;
  float f3 = (e3 + e7)/2;
  float f4 = (e0 + e2)/2;
  float f5 = (e4 + e6)/2;

  float c = (f4 + f5)/2;
  float test = (f0 + f2)/2;
  if (fabs(test-c)>0.001) fprintf(stderr,"ERROR\n");

  lambda[(x+dist) + dim*y        + dim*dim*z]= -e0 + func[(x+dist) + dim*y        + dim*dim*z];
  lambda[(x+size) + dim*(y+dist) + dim*dim*z]= -e1 + func[(x+size) + dim*(y+dist) + dim*dim*z];  
  lambda[(x+dist) + dim*(y+size) + dim*dim*z]= -e2 + func[(x+dist) + dim*(y+size) + dim*dim*z];
  lambda[x        + dim*(y+dist) + dim*dim*z]= -e3 + func[x        + dim*(y+dist) + dim*dim*z];

  lambda[(x+dist) + dim*y        + dim*dim*(z+size)]= -e4 + func[(x+dist) + dim*y        + dim*dim*(z+size)];
  lambda[(x+size) + dim*(y+dist) + dim*dim*(z+size)]= -e5 + func[(x+size) + dim*(y+dist) + dim*dim*(z+size)];  
  lambda[(x+dist) + dim*(y+size) + dim*dim*(z+size)]= -e6 + func[(x+dist) + dim*(y+size) + dim*dim*(z+size)];
  lambda[x        + dim*(y+dist) + dim*dim*(z+size)]= -e7 + func[x        + dim*(y+dist) + dim*dim*(z+size)];

  lambda[x        + dim*y        + dim*dim*(z+dist)]= -e8  + func[x        + dim*y        + dim*dim*(z+dist)];
  lambda[(x+size) + dim*y        + dim*dim*(z+dist)]= -e9  + func[(x+size) + dim*y        + dim*dim*(z+dist)];
  lambda[(x+size) + dim*(y+size) + dim*dim*(z+dist)]= -e10 + func[(x+size) + dim*(y+size) + dim*dim*(z+dist)];
  lambda[x        + dim*(y+size) + dim*dim*(z+dist)]= -e11 + func[x        + dim*(y+size) + dim*dim*(z+dist)];
 
  lambda[(x+dist) + dim*y        + dim*dim*(z+dist)]= -f0 + func[(x+dist) + dim*y        + dim*dim*(z+dist)];
  lambda[(x+size) + dim*(y+dist) + dim*dim*(z+dist)]= -f1 + func[(x+size) + dim*(y+dist) + dim*dim*(z+dist)];  
  lambda[(x+dist) + dim*(y+size) + dim*dim*(z+dist)]= -f2 + func[(x+dist) + dim*(y+size) + dim*dim*(z+dist)];
  lambda[x        + dim*(y+dist) + dim*dim*(z+dist)]= -f3 + func[x        + dim*(y+dist) + dim*dim*(z+dist)];
  lambda[(x+dist) + dim*(y+dist) + dim*dim*z]       = -f4 + func[(x+dist) + dim*(y+dist) + dim*dim*z];
  lambda[(x+dist) + dim*(y+dist) + dim*dim*(z+size)]= -f5 + func[(x+dist) + dim*(y+dist) + dim*dim*(z+size)];

  lambda[(x+dist) + dim*(y+dist) + dim*dim*(z+dist)]= -c  + func[(x+dist) + dim*(y+dist) + dim*dim*(z+dist)];

  //insert hell here

  float eps2=eps/(pow(2,(float) lvl));//this is what changes it from old thresholding method to new 
  

  adjust(x+dist , y      , z      , dim, eps2, maxval, func, lambda);
  adjust(x+size , y+dist , z      , dim, eps2, maxval, func, lambda);
  adjust(x+dist , y+size , z      , dim, eps2, maxval, func, lambda);
  adjust(x      , y+dist , z      , dim, eps2, maxval, func, lambda);

  adjust(x+dist , y      , z+size , dim, eps2, maxval, func, lambda);
  adjust(x+size , y+dist , z+size , dim, eps2, maxval, func, lambda);
  adjust(x+dist , y+size , z+size , dim, eps2, maxval, func, lambda);
  adjust(x      , y+dist , z+size , dim, eps2, maxval, func, lambda);

  adjust(x      , y      , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x+size , y      , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x+size , y+size , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x      , y+size , z+dist , dim, eps2, maxval, func, lambda);

  adjust(x+dist , y      , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x+size , y+dist , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x+dist , y+size , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x      , y+dist , z+dist , dim, eps2, maxval, func, lambda);
  adjust(x+dist , y+dist , z      , dim, eps2, maxval, func, lambda);
  adjust(x+dist , y+dist , z+size , dim, eps2, maxval, func, lambda);

  adjust(x+dist , y+dist , z+dist , dim, eps2, maxval, func, lambda);

  Wavelet(x,y,z,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x+dist,y,z,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x,y+dist,z,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x+dist,y+dist,z,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x,y,z+dist,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x+dist,y,z+dist,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x,y+dist,z+dist,dim,dist,lvl-1,eps,maxval,func,lambda);
  Wavelet(x+dist,y+dist,z+dist,dim,dist,lvl-1,eps,maxval,func,lambda);


}

void writeRawiv(const char* fname,char* fresultname, float* buffer,int dim[3])
  //void writeRawiv(const char* fname,char* fresultname, double* buffer,int dim[3])
{
	int nverts,ncells;
	float minext[3], maxext[3];
        int dtype;
	// float orig[3], span[3];

	nverts=dim[0]*dim[1]*dim[2];

	// determine file data type
	struct stat filestat;
	if (stat(fname, &filestat) < 0) {
		fprintf(stderr, "cannot find data file %s\n", fname);
		exit(-1);
	}
	int sz = filestat.st_size;  

	FILE *fp = fopen(fname, "rb");
	if (fp == NULL) {
		fprintf(stderr, "cannot open %s\n", fname);
		exit(-1);
	}

	FILE *resultfp = fopen(fresultname,"wb");
	if (resultfp == NULL) {
		fprintf(stderr, "cannot open %s\n", fresultname);
		exit(-1);
	}

	getFloat(minext, 3, fp);
	getFloat(maxext, 3, fp);
	getInt(&nverts, 1, fp);
	getInt(&ncells, 1, fp);
	getInt(dim, 3, fp);
	getFloat(orig, 3, fp);
	getFloat(span, 3, fp);


	writeFloat(minext, 3, resultfp);
	writeFloat(maxext, 3, resultfp);
	writeUint32(&nverts, 1, resultfp);
	writeUint32(&ncells, 1, resultfp);
	writeUint32(dim, 3, resultfp);
	writeFloat(orig, 3, resultfp);
	writeFloat(span, 3, resultfp);

	nverts = dim[0]*dim[1]*dim[2]; 
	dtype =(int)(sz / (2 * nverts));
        //p_data = new float[nverts];

        printf("We are working at writeRawiv now.\n");
        printf(" data type: %d\n", dtype);
        printf("minext: %f %f %f\n", minext[0], minext[1], minext[2]);
        printf("maxext: %f %f %f\n", maxext[0], maxext[1], maxext[2]);	
        printf("nverts: %d \n", nverts); 
        printf("ncells: %d \n", ncells);  
        printf("dim: %d %d %d", dim[0], dim[1], dim[2]); 
        printf("orig: %f %f %f\n", orig[0], orig[1], orig[2]);
        printf("span: %f %f %f\n", span[0], span[1], span[2]);	 

  if(dtype==2) 
  { 
    writeFloat(buffer, nverts, resultfp);
  }
  else
    fprintf(stderr, "We can only handle float type currently.\n");

  fclose(fp);
  fclose(resultfp);
}//writeRawiv

void writeRawivnew(const char* fname,char* fresultname, float* buffer,int dim[3])
  //void writeRawiv(const char* fname,char* fresultname, double* buffer,int dim[3])
{
	int nverts,ncells;
	float minext[3], maxext[3];
        int dtype;
        int dimtemp[3];
	// float orig[3], span[3];

	// determine file data type
	struct stat filestat;
	if (stat(fname, &filestat) < 0) {
		fprintf(stderr, "cannot find data file %s\n", fname);
		exit(-1);
	}
	int sz = filestat.st_size;  

	FILE *fp = fopen(fname, "rb");
	if (fp == NULL) {
		fprintf(stderr, "cannot open %s\n", fname);
		exit(-1);
	}

	FILE *resultfp = fopen(fresultname,"wb");
	if (resultfp == NULL) {
		fprintf(stderr, "cannot open %s\n", fresultname);
		exit(-1);
	}

	getFloat(minext, 3, fp);
	getFloat(maxext, 3, fp);
	getInt(&nverts, 1, fp);
	getInt(&ncells, 1, fp);
	getInt(dimtemp, 3, fp);
	getFloat(orig, 3, fp);
	getFloat(span, 3, fp);

        nverts=dim[0]*dim[1]*dim[2];
        ncells=(dim[0]-1)*(dim[1]-1)*(dim[2]-1);

        for (int i=0; i<3; i++) 
        { 
          span[i]=(maxext[i]-minext[i])/(dim[i]-1);
        }/*for z*/

        writeFloat(minext, 3, resultfp);
        writeFloat(maxext, 3, resultfp);
        writeUint32(&nverts, 1, resultfp);
        writeUint32(&ncells, 1, resultfp);
        writeUint32(dim, 3, resultfp);
        writeFloat(orig, 3, resultfp);
        writeFloat(span, 3, resultfp);

	dtype =(int)(sz / (2 * nverts));
        dtype=2;
        //p_data = new float[nverts];

        printf("We are working at writeRawiv now.\n");
        printf(" data type: %d\n", dtype);
        printf("minext: %f %f %f\n", minext[0], minext[1], minext[2]);
        printf("maxext: %f %f %f\n", maxext[0], maxext[1], maxext[2]);	
        printf("nverts: %d \n", nverts); 
        printf("ncells: %d \n", ncells);  
        printf("dim: %d %d %d", dim[0], dim[1], dim[2]); 
        printf("orig: %f %f %f\n", orig[0], orig[1], orig[2]);
        printf("span: %f %f %f\n", span[0], span[1], span[2]);	 

  if(dtype==2) 
  {
       for (int z=0; z<dim[2]; z++) 
       { for (int y=0; y<dim[1]; y++) 
         { for (int x=0; x<dim[0]; x++) 
           {
             writeFloat(&buffer[dim[0]*dim[1]*z+dim[0]*y+x], 1, resultfp);
           }/*for x*/
         }/*for y*/
       }/*for z*/
  }//if    
  else
    fprintf(stderr, "We can only handle float type currently.\n");

  fclose(fp);
  fclose(resultfp);
}//writeRawivnew


//float minmaxRawiv(float* buffer,int dim[3],int minmax1,int index1);
//calculate the min/max in a volume data
//buffer dimension is dim[0]*dim[1]*dim[2]
//minmax1==1, caculate the min
//minmax1==2, caculate the max
//index1==1, caculate the x coordinate
//index1==2, caculate the y coordinate
//index1==3, caculate the z coordinate

void thresh(float epsilon, int dim[3],float maxval,float* lambda)
{ float tempstore[8];

  int size=dim[0];
  int nverts=dim[0]*dim[1]*dim[2];

  int x,xlvl,y,ylvl,z,zlvl;
  int tmp=0;

  int plvl; //the level at that point

  int s=(size-1);
  int lvl=0;

  while(s>1)
    {
      lvl++; 
      s=(s>>1);
    }
  //printf("level of pdb: %d\n",lvl);

  //keep the original values
  tempstore[0] = lambda[size*size*0 + size*0 + 0]         ;
  tempstore[1] = lambda[size*size*0 + size*0 + (size-1)]      ;
  tempstore[2] = lambda[size*size*0 + size*(size-1) + 0]      ;
  tempstore[3] = lambda[size*size*0 + size*(size-1) + (size-1)]   ;
  tempstore[4] = lambda[size*size*(size-1) + size*0 + 0]      ;
  tempstore[5] = lambda[size*size*(size-1) + size*0 +(size-1)]   ;
  tempstore[6] = lambda[size*size*(size-1) + size*(size-1) + 0]   ;
  tempstore[7] = lambda[size*size*(size-1) + size*(size-1) + (size-1)];

  for(int lcv=0; lcv<=nverts; lcv++)
    {
      /*
      if(lcv%(dim[0]*dim[1])==0)
	{
	  fprintf(stderr,"thresholding: %d\r",lcv/(dim[0]*dim[1]));
	  fflush(stderr);
	}
      */

      tmp=lcv;
      z=tmp/(dim[0]*dim[1]);
      tmp=tmp%(dim[0]*dim[1]);
      y=tmp/dim[0];
      x=tmp%dim[0];

      xlvl=findlevel(x,lvl);
      ylvl=findlevel(y,lvl);
      zlvl=findlevel(z,lvl);

      if(xlvl>ylvl)//x wins y
	{if(xlvl>zlvl)
	  plvl=xlvl;//x wins z
	else
	  plvl=zlvl;// z wins x and y
	}
      else//y wins x
	{if(ylvl>zlvl)
	  plvl=ylvl;//y wins x
	else
	  plvl=zlvl;// z wins y and x
	}

      //used to test the thresholding
      //lambda[lcv]=plvl;
      //	  	   
      if(plvl!=0)
      {
	//  if(fabs(lambda[lcv])<=( epsilon*maxval))

        //  if(fabs(lambda[lcv])<=( epsilon/(pow(2,(float)(lvl-plvl)*1)))*maxval)      	
	//Here is the new version

	      if(fabs(lambda[lcv])<=( epsilon/(pow(2,(float)(lvl-plvl)*1.5)))*maxval)      		  
	      lambda[lcv]=0.00;
	}
    }

  //fprintf(stderr, "\n");

  //restore the original values
  lambda[size*size*0 + size*0 + 0] = tempstore[0];
  lambda[size*size*0 + size*0 + (size-1)] = tempstore[1];
  lambda[size*size*0 + size*(size-1) + 0] = tempstore[2];
  lambda[size*size*0 + size*(size-1) + (size-1)] = tempstore[3];
  lambda[size*size*(size-1) + size*0 + 0] = tempstore[4];
  lambda[size*size*(size-1) + size*0 +(size-1)] = tempstore[5];
  lambda[size*size*(size-1) + size*(size-1) + 0] = tempstore[6];
  lambda[size*size*(size-1) + size*(size-1) + (size-1)] = tempstore[7];  
}/*thresh*/

unsigned int findlevel(unsigned int val,unsigned int maxlvl)
{
  int rtrn=maxlvl;

  while( ( ( val & 0x0001 )!=1) && rtrn!=0)
    {
      val=(val>>1);
      rtrn--;
    }

  if(rtrn>maxlvl)
    {
      fprintf(stderr,"error level exceeded: %d",rtrn);
      exit(-1);
    }

  return(rtrn);
}

void testlevel(int x, int y, int z,int lvl, int size, float *lambda,int dim)
{
  if (size ==1 )
    return;


  static int counter=0;
  counter++;
  if (counter%50024==0) {
    //printf("%d\n", counter);     
  }

  //  float v4 = (func[x][y][z] + func[x][y+size][z])/2;//left

  int dist = size/2;
  
  if(counter==1)
    {
      lambda[x        + dim*y        + dim*dim*z]        =0;
      lambda[(x+size) + dim*y        + dim*dim*z]        =0;
      lambda[x        + dim*(y+size) + dim*dim*z]        =0;
      lambda[(x+size) + dim*(y+size) + dim*dim*z]        =0;
      lambda[x        + dim*y        + dim*dim*(z+size)] =0;
      lambda[(x+size) + dim*y        + dim*dim*(z+size)] =0;
      lambda[x        + dim*(y+size) + dim*dim*(z+size)] =0;
      lambda[(x+size) + dim*(y+size) + dim*dim*(z+size)] =0;
    }

  //interpolated values

  lambda[(x+dist) + dim*y        + dim*dim*z]        = lvl;
  lambda[(x+size) + dim*(y+dist) + dim*dim*z]        = lvl;
  lambda[(x+dist) + dim*(y+size) + dim*dim*z]        = lvl;
  lambda[x        + dim*(y+dist) + dim*dim*z]        = lvl;

  lambda[(x+dist) + dim*y        + dim*dim*(z+size)] = lvl;
  lambda[(x+size) + dim*(y+dist) + dim*dim*(z+size)] = lvl;
  lambda[(x+dist) + dim*(y+size) + dim*dim*(z+size)] = lvl;
  lambda[x        + dim*(y+dist) + dim*dim*(z+size)] = lvl;
  
  lambda[x        + dim*y        + dim*dim*(z+dist)] = lvl;
  lambda[(x+size) + dim*y        + dim*dim*(z+dist)] = lvl;
  lambda[(x+size) + dim*(y+size) + dim*dim*(z+dist)] = lvl;
  lambda[x        + dim*(y+size) + dim*dim*(z+dist)] = lvl;

  lambda[(x+dist) + dim*y        + dim*dim*(z+dist)] = lvl;
  lambda[(x+size) + dim*(y+dist) + dim*dim*(z+dist)]     = lvl;
  lambda[(x+dist) + dim*(y+size) + dim*dim*(z+dist)] = lvl;
  lambda[x        + dim*(y+dist) + dim*dim*(z+dist)] = lvl;
  lambda[(x+dist) + dim*(y+dist) + dim*dim*z]        = lvl;
  lambda[(x+dist) + dim*(y+dist) + dim*dim*(z+size)] = lvl;

  lambda[(x+dist) + dim*(y+dist) + dim*dim*(z+dist)] = lvl;


  testlevel(x,y,z,lvl+1,dist,lambda,dim);
  testlevel(x+dist,y,z,lvl+1,dist,lambda,dim);
  testlevel(x,y+dist,z,lvl+1,dist,lambda,dim);
  testlevel(x+dist,y+dist,z,lvl+1,dist,lambda,dim);
  testlevel(x,y,z+dist,lvl+1,dist,lambda,dim);
  testlevel(x+dist,y,z+dist,lvl+1,dist,lambda,dim);
  testlevel(x,y+dist,z+dist,lvl+1,dist,lambda,dim);
  testlevel(x+dist,y+dist,z+dist,lvl+1,dist,lambda,dim);
}

void readRawiv(const char* fname, int dim[3],float &maxval)
{
  
  int nverts,ncells;
  float minext[3], maxext[3];
  
  maxval=0.00;
  //        int leveltemp[3];
  // float orig[3], span[3];
  
  // determine file data type
  struct stat filestat;
  if (stat(fname, &filestat) < 0) {
    fprintf(stderr, "cannot find data file %s\n", fname);
    return;
    exit(-1);
  }
  int sz = filestat.st_size;  
  
  FILE *fp = fopen(fname, "rb");
  if (fp == NULL) {
    fprintf(stderr, "cannot open %s\n", fname);
    exit(-1);
  }

  printf("We are working at readRawiv now.\n");

  getFloat(minext, 3, fp);
  getFloat(maxext, 3, fp);
  getInt(&nverts, 1, fp);
  getInt(&ncells, 1, fp);
  getInt(dim, 3, fp);
  getFloat(orig, 3, fp);
  getFloat(span, 3, fp);

  nverts = dim[0]*dim[1]*dim[2]; 
  int dtype = (int)(sz / (2 * nverts));
  /*
  printf("dim: %d %d %d", dim[0], dim[1], dim[2]);
  printf(" data type: %d\n", dtype);
  printf("orig: %f %f %f\n", orig[0], orig[1], orig[2]);
  printf("span: %f %f %f\n", span[0], span[1], span[2]);	
  printf("size: %d\n",sz);
  */
  //builds the arrays to store the data
  delete [] functest;
  functest = new float[nverts];
  p_data = new float[nverts];
  
  switch (dtype) {
  case 0:
    {	// unsigned char 
      unsigned char* ucdata = new unsigned char[nverts];
      fread(ucdata, sizeof(unsigned char), nverts, fp);
      for(int i = 0; i < nverts; i++) {
	p_data[i] = ucdata[i];
      }
      delete[] ucdata;
      break;
    }
  case 1:
    {	// unsigned short
      unsigned short* usdata = new unsigned short[nverts];
      getShort(usdata, nverts, fp);
      for(int i = 0; i < nverts; i++) 
	{
	  p_data[i] = usdata[i];
	}
      delete[] usdata;
      break;
    }
  case 2:		// float
    getFloat(functest, nverts, fp);
    break;
  }
  
  if(dtype==2) 
    {
      maxval = functest[0];
      for(int lcv=0;lcv<nverts; lcv++)
	maxval = (fabs(functest[lcv])>maxval ? fabs(functest[lcv]) : maxval);
    }
  else
    fprintf(stderr,"We can only handle float type currently.\n");
  
   printf("We are working at readRawiv now.\n");
        printf(" data type: %d\n", dtype);
        printf("minext: %f %f %f\n", minext[0], minext[1], minext[2]);
        printf("maxext: %f %f %f\n", maxext[0], maxext[1], maxext[2]);	
        printf("nverts: %d \n", nverts); 
        printf("ncells: %d \n", ncells);  
        printf("dim: %d %d %d", dim[0], dim[1], dim[2]); 
        printf("orig: %f %f %f\n", orig[0], orig[1], orig[2]);
        printf("span: %f %f %f\n", span[0], span[1], span[2]);



  fclose(fp);
  
}//readRawiv






























