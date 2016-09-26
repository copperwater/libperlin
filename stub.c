#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "perlin.h"

typedef unsigned char uchar;

int main(int argc, char* argv[]) {
  uchar obegin = 0;
  uchar oend = 7;
  float persistence = 0.5;
  float x,y;
  set_perlin_seed(time(0));

  //perlin_2d(0.4,0.3,persistence,obegin,oend);
  for(x=-2;x<=2;x+=0.05) {
    /*
      for(y=-2;y<=2;y+=0.05) {
      printf("%f ",perlin_2d(x,y,persistence,obegin,oend));
    }
    printf("\n");
    */
    printf("%f\n", perlin_1d(x,persistence,obegin,oend));
  }
  /**/
  return 0;
}

