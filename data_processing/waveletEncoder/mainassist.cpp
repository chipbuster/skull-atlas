#include "mainassist.h"
#include "zerobit.h"
#include "block.h"
#include "zeroblocktable.h"
#include "error.h"

int maximum(int dim1[3])
{ int maximumtemp=0;

  maximumtemp=dim1[0];

  maximumtemp = (dim1[1]>maximumtemp ? dim1[1] : maximumtemp );
  maximumtemp = (dim1[2]>maximumtemp ? dim1[2] : maximumtemp );

  return maximumtemp;
}/*maximum*/

void mainzeroblocktable(float epsilon,float maxval,int newsize1,float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3)
{
  int zeroblocktable1size,zerocelltable1size;
  float zeroblockrate_result1,zerocellrate_result1;

  zeroblocktable1size=size_zeroblocktable(newsize1,1);
  zerocelltable1size=size_zeroblocktable(newsize1,2);
  /*
  printf("zeroblocktable1size=%d\n",zeroblocktable1size);
  printf("zerocelltable1size=%d\n",zerocelltable1size);
  */

  int* zeroblocktable1 = 0;
  zeroblocktable1 = new int[zeroblocktable1size];
  int* zerocelltable1 = 0;
  zerocelltable1 = new int[zerocelltable1size];

  empty_zeroblocktable(zeroblocktable1size,zeroblocktable1);
  zeroblocktable_generation(newsize1,func,zeroblocktable1,1);
  /*
  print_zeroblocktable(zeroblocktable1size,zeroblocktable1);
  */

  empty_zeroblocktable(zerocelltable1size,zerocelltable1);
  zeroblocktable_generation(newsize1,func,zerocelltable1,2);
  /*
  print_zeroblocktable(zerocelltable1size,zerocelltable1);
  */

  zeroblockrate_result1=zeroblocktable_zeroblockrate(zeroblocktable1,zeroblocktable1size);
  printf("zeroblockrate=%f\n",zeroblockrate_result1);
  zerocellrate_result1=zeroblocktable_zeroblockrate(zerocelltable1,zerocelltable1size);
  printf("zerocellrate=%f\n",zerocellrate_result1);
}/*mainzeroblocktable*/

void integer2string(int input1,char* output1,int* outputlen1)
{ int number,digit,index;

  *outputlen1=0;
  number=input1;
  while(number>0)
  { 
    number=number/10;
    *outputlen1=*outputlen1+1;
  }/*while(number>0)*/

  if(*outputlen1>0)
  {
    output1[*outputlen1]='\0';
    index=*outputlen1-1;
    number=input1;
    while(number>0)
    { digit=number%10;
      number=number/10;

      switch(digit)
      {
        case 0:
          output1[index]='0';
          break;
        case 1:
          output1[index]='1';
          break;
        case 2:
          output1[index]='2';
          break;
        case 3:
          output1[index]='3';
          break;
        case 4:
          output1[index]='4';
          break;
        case 5:
          output1[index]='5';
          break;
        case 6:
          output1[index]='6';
          break;
        case 7:
          output1[index]='7';
          break;
        case 8:
          output1[index]='8';
          break;
        case 9:
          output1[index]='9';
          break;
      }/*switch*/
   
      index=index-1;
    }/*while(number>0)*/
  }/* if(*outputlen1>0)*/
  else
  {
    output1[0]='0';
    output1[1]='\0';
  }/*else*/
}/*integer2string*/

void double2string(double input1,char* output1,int* outputlen1,int precision)
{ int integerpart,decimalpart;
  double decimal;
  char dot[4],integerstring[20],decimalstring[10];
 
  //precision=2;
  integerpart=(int)input1;
  integer2string(integerpart,integerstring,outputlen1);
  strcpy(output1,integerstring);
  strcpy(dot,".");
  strcat(output1,dot); 

  decimal=input1-(int)input1; 
  for(int i=0;i<precision;i++)
  {
    decimal=decimal*10;
    decimalpart=(int)(decimal);
    integer2string(decimalpart,decimalstring,outputlen1);
    strcat(output1,decimalstring);  
    decimal=decimal-decimalpart;
  }/*for i*/
}/*double2string*/





