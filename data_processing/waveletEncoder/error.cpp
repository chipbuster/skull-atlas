#include "error.h"
#include "wave.h"
#include "mainassist.h"

double infiniteerror(double epsilon1,int dim1[3],float maxval,float *func,float *recon)
{ 
  int i;

  double maxdifference1;
  double maxdifferencetemp1;
  double percentage1;

  maxdifference1=0.0;

  for(i=0;i<(dim1[0]*dim1[1]*dim1[2]);i++)
  { 
    if(i%(dim1[0]*dim1[1])==0)
      {
	fprintf(stderr, "inifinite error: %d\r", i/(dim1[0]*dim1[1]));
	fflush(stderr);
      }/*if*/

    maxdifferencetemp1=fabs(recon[i]-func[i]);

    if(maxdifferencetemp1>maxdifference1) 
      {
        maxdifference1=maxdifferencetemp1;
      }/*if*/
  }/*for*/

  fprintf(stderr, "\n");

  percentage1=maxdifference1/maxval*100.0;
  //percentage1 = maxdifference1;
  return percentage1;
}//infiniteerror

double squareerror(double epsilon1,int dim1[3],float* func,float* recon)
{ 
  int i;
  double square1,squaredifference1,differencetemp1;
  double percentage1;

  square1=0.0;
  squaredifference1=0.0;

  for(i=0;i<(dim1[0]*dim1[1]*dim1[2]);i++)
  { 
    if(i%(dim1[0]*dim1[1])==0)
      {
	fprintf(stderr, "square error %d\r", i/(dim1[0]*dim1[1]));
	fflush(stderr);
      }
    square1=square1+func[i]*func[i];
    differencetemp1=recon[i]-func[i];
    squaredifference1=squaredifference1+differencetemp1*differencetemp1;
  }/*for i*/
  
  fprintf(stderr, "\n");
  percentage1=sqrt(squaredifference1/square1)*100.0;
  
  return percentage1;
}//squareerror

void errorcalculation(float epsilon1,int* dim1,float MAXVAL1,int nverts1,float* functest1,float* lambdatest1,float* recontest1,char* argv1,char* argv2,char* argv3,char* argv4)
{
  int i;
  char buffer[64];
  double inferr=infiniteerror(10.0,dim1,MAXVAL1,functest1,recontest1);
  double sqerr=squareerror(10.0,dim1,functest1,recontest1);
  int befzero=0;
  int aftzero=0;
  float PSNR=0.00;
  float realcompressionratio;
 
  printf("begin errorcalculation.\n");

  for(i=0; i<nverts1; i++)
    { 
      if(fabs(lambdatest1[i])<=0.0000000001*MAXVAL1)
	aftzero++;
      if(fabs(functest1[i])<=0.0000000001*MAXVAL1)
	befzero++;

      PSNR+=(functest1[i]-recontest1[i])*(functest1[i]-recontest1[i]);
    }/*for i*/

  PSNR=PSNR/(nverts1);
  PSNR=sqrt(PSNR);
  PSNR=20*log10(MAXVAL1/PSNR);

  /*
  sprintf(buffer,"%s%srec.rawiv",argv3,argv2);
  writeRawiv(argv1, buffer, (double*) recontest1,dim1);
  sprintf(buffer,"%s%swav.rawiv",argv3,argv2); 
  */

  sprintf(buffer,"%s%srec.rawiv",argv3,argv2);
  writeRawivnew(argv1, buffer, recontest1,dim1);
  sprintf(buffer,"%s%swav.rawiv",argv3,argv2); 
 
  /*
  printf("We are working in errorcalculation now.\n");
  printf("aftzero is %.d.\n",aftzero);
  printf("nverts1 is %.d.\n",nverts1);
  */

  printf("%5.3f\t%8.4f\t%8.4f\t%8.4f\t%8.4f\t%8.4f\t%8.4f\t%8.4f\t"
	 ,epsilon1
	 ,PSNR
	 ,(float)aftzero/nverts1*100 //percent zero after compression
	 ,(float)befzero/nverts1*100 //percent zero before compression
	 ,1/((nverts1-(float)aftzero)/nverts1)         //compression ratio counting zero
	 ,1/(((nverts1)-(float)aftzero)/((nverts1)-(float)befzero)) //non zero compression ratio
	 ,inferr                                     //infinite error
	 ,sqerr); 

  realcompressionratio = whbfile_compressionratio(epsilon1,argv4);
  Juliooutput(epsilon1,MAXVAL1,PSNR,1/((nverts1-(float)aftzero)/nverts1),1/(((nverts1)-(float)aftzero)/((nverts1)-(float)befzero)),realcompressionratio,argv1);

  printf("end errorcalculation.\n");
}/*errorcalculation*/

void Juliooutput(float epsilon1,float MAXVAL1,float PSNR,float zero_compression_ratio,float nonzero_compression_ratio,float realcompressionratio,char* argv1)
{ int outputlen1=0;
  char buffer1[64];

  double2string(PSNR,buffer1,&outputlen1,4);
  textfileoutput("PSNRresult.txt",buffer1);
  textfileoutput("PSNRresult.txt","  ");
  printf("nverts1 is %.20f\n",PSNR);

  double2string((double)zero_compression_ratio,buffer1,&outputlen1,4);
  textfileoutput("zero_compression_ratioresult.txt",buffer1);
  textfileoutput("zero_compression_ratioresult.txt","  ");
  printf("nverts1 is %.20f\n",zero_compression_ratio);

  double2string((double)nonzero_compression_ratio,buffer1,&outputlen1,4);
  textfileoutput("nonzero_compression_ratioresult.txt",buffer1);
  textfileoutput("nonzero_compression_ratioresult.txt","  ");
  printf("nverts1 is %.20f\n",nonzero_compression_ratio);

  double2string((double)realcompressionratio,buffer1,&outputlen1,4);
  textfileoutput("realcompressionratioresult.txt",buffer1);
  textfileoutput("realcompressionratioresult.txt","  ");
  printf("nverts1 is %.20f\n",realcompressionratio);

  if(fabs(epsilon1)<=0.0000000001*MAXVAL1)
  {
    textfileoutput("PSNRresult.txt","\n");
    textfileoutput("zero_compression_ratioresult.txt","\n");  
    textfileoutput("nonzero_compression_ratioresult.txt","\n");
    textfileoutput("realcompressionratioresult.txt","\n");
    textfileoutput("rawivnamelist.txt",argv1);
    textfileoutput("rawivnamelist.txt","\n");
  }/*if*/
}/*Juliooutput*/

void textfileoutput(char *fresultname,char *buffer1)
{
  FILE *resultfp = fopen(fresultname,"a");
	
  if(resultfp == NULL) 
  {
    fprintf(stderr, "cannot open %s\n", fresultname);
    exit(-1);
  }/*if*/

  fwrite(buffer1,strlen(buffer1),1,resultfp);

  fclose(resultfp);
}/*textfileoutput*/

float whbfile_compressionratio(float epsilon1,char *fresultname)
{
  char bufferend[1024],fname[1024],rawivfname[1024];
  int ZSMStype1;
  float compressionratio;

  printf("begin whbfile_compressionratio\n");

  ZSMStype1=3; 
  sprintf(bufferend,"type%d.whb",ZSMStype1);

  sprintf(rawivfname,fresultname);

  printf("rawivfname=%s\n",rawivfname);

  struct stat filestat;
  if (stat(rawivfname, &filestat) < 0) {
    fprintf(stderr, "cannot find data file %s\n",rawivfname);
    exit(-1);
  }/*if*/
  int rawivsz = filestat.st_size;  

  printf("rawivsz=%d\n",rawivsz);
  sprintf(fname,"%s%.3f%s",fresultname,epsilon1,bufferend);
  printf("fname=%s\n",fname);

  if (stat(fname, &filestat) < 0) {
    fprintf(stderr, "cannot find data file %s\n", fname);
    exit(-1);
  }
  int sz = filestat.st_size;  

  printf("sz=%d\n",sz);

  compressionratio= (float)rawivsz / (float)sz;

  printf("compressionratio at threshold %f=%f\n",epsilon1,compressionratio);
  printf("end whbfile_compressionratio\n");

  return  compressionratio;
}/*whbfile_compressionratio*/
