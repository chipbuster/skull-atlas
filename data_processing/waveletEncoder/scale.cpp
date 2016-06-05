#include "block.h"
#include "zerobit.h"
#include "waveAnthony.h"
#include "wave.h"

void scaledata(int newsize1,float *lambda1,float scalevalue1)
{
  int z,y,x;
  
  for(z=0; z<newsize1; z=z+1) 
  { for(y=0; y<newsize1; y=y+1) 
    { for(x=0; x<newsize1; x=x+1) 
      {
        lambda1[newsize1*newsize1*z+newsize1*y+x]=lambda1[newsize1*newsize1*z+newsize1*y+x]*scalevalue1;
      }/*for x*/
    }/*for y*/
  }/*for z*/
}/*scaledata*/
















