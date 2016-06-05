#include "pdb.h"

// Sample usage:
//   ./pdb menge81_float.rawiv 0.5 outfile
//
// "outfile0.5rec.rawiv" and "outfile0.5type3.whb" will be generated.
int main(int argc, char *argv[])
{
  float epsilon=atof(argv[2]);
  float MAXVAL=0.00;
  int dim[3],dimnew[3],ZSMStype1;
  int nvertsnew,newsize1,nverts;
  char buffer[64];
  float scalevalue1=1.0;

  readRawiv(argv[1],dim,MAXVAL);  //also grabs the max value

  fprintf(stderr, "dimensions\n");
  fprintf(stderr, "%d %d %d\n",dim[0],dim[1],dim[2]);
  fprintf(stderr, "maxval\n");
  fprintf(stderr, "%f\n",MAXVAL);

  int lvl=0;
  int tmp=dim[0]-1;

  while(tmp>1)
    {
      lvl++;
      tmp=(tmp>>1);
    }

  lvl-=1;

  newsize1=paddingsizever2(dim);

  nvertsnew=newsize1*newsize1*newsize1;
  float* functestnew = 0;
  functestnew = new float[nvertsnew];
  lambdatest = new float[nvertsnew];
  recontest = new float[nvertsnew];

  table_whole = new int[nvertsnew];
  cell_information_array_whole = new int[nvertsnew];
  ZSMStable_whole = new int[nvertsnew];
  DIStable_whole = new int[nvertsnew];

  dimnew[0]=paddingsize(dim[0]);
  dimnew[1]=paddingsize(dim[1]);
  dimnew[2]=paddingsize(dim[2]);
  paddingver3(dim,dimnew,functest,functestnew);

  fprintf(stderr, "newsize1=%d\n",newsize1);
  fprintf(stderr, "dimnew[0]=%d\n",dimnew[0]);
  fprintf(stderr, "dimnew[1]=%d\n",dimnew[1]);
  fprintf(stderr, "dimnew[2]=%d\n",dimnew[2]);

  /*
  scaledata(newsize1,functestnew,scalevalue1);
  MAXVAL=MAXVAL*scalevalue1;
  */

  //mainzeroblocktable(epsilon,MAXVAL,newsize1,functestnew,lambdatest,recontest,argv[1],argv[2],argv[3]);
  volumeprocessing(epsilon,MAXVAL,dimnew,functestnew,lambdatest,recontest,argv[1],argv[2],argv[3]);

  /*
  extracalculation(oldsize1,newsize1,epsilon,functestnew,lambdatest,MAXVAL);
  */

  /*
  output_rawiv("original.raw",newsize1,functestnew);
  output_rawiv("lambda.raw",newsize1,lambdatest);
  output_rawiv("reconstruct.raw",newsize1,recontest);
  */

  ZSMStype1=3;
  table_generation(dimnew,lambdatest,-1,ZSMStype1);

  sprintf(buffer,"%s%s%s%d.whb",argv[3],argv[2],"type",ZSMStype1);
  //sprintf(buffer,"%s%s.whb",argv[3],argv[2]);
  encode_program(buffer,dimnew,lambdatest);

  nverts=dimnew[0]*dimnew[1]*dimnew[2];
  errorcalculation(epsilon,dimnew,MAXVAL,nverts,functestnew,lambdatest,recontest,argv[1],argv[2],argv[3],argv[4]);

  delete[] functestnew;
  delete[] recontest;
  delete[] lambdatest;
}/*main*/

