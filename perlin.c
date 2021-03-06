#include <math.h>
#include <stdio.h>
#include "perlin.h"

typedef unsigned char uchar;
typedef unsigned int uint;

unsigned int perlin_seed;

void get_perlin_minmax(float* min, float* max, float persistence,
		       uchar obegin, uchar oend) {
  /* The noise function always returns values from -1 to 1,
     so the maximum and minimum possible value will always be
     1 + 1/2 + 1/4 + ... (as many times as there are octaves).
  */
  float tmp = pow(2, 1-obegin) - pow(2, -oend);
  /*
  uchar i;
  for(i=obegin; i<=oend; i++) {
    tmp += pow(persistence, i);
  }
  */
  *max = tmp;
  *min = -tmp;
}

#ifdef LINEAR_INTERPOLATION
static float interpolate(float v1, float v2, float between) {
  return v1 + (v2-v1)*between;
}
#else
static float interpolate(float v1, float v2, float between) {
  float terp = (1 - cos(between * M_PI)) * .5;
  return v1*(1-terp) + v2*terp;
}
#endif

#define PRIME1 68903
#define PRIME2 1255361
#define PRIME3 1756015823

//If fewer than 3 dimensions are desired, call with 0 for 2nd/3rd parameters.
static float noise(int x, int y, int z) {
  //printf("NOISE %d %d %d\n", x, y, z);
  static union {
    unsigned int i;
    float f;
  } str;
  
  long n = (x*perlin_seed*(perlin_seed+PRIME1))^perlin_seed;
  if(y != 0)
    n += (y*perlin_seed*(perlin_seed+PRIME2))^perlin_seed;
  if(z != 0)
    n += (z*perlin_seed*(perlin_seed+PRIME3))^perlin_seed;
  //printf("%d %ld\n", perlin_seed, n);
    
  n = ((n << 13) ^ n);
  n = n*(n*(n+PRIME1)+PRIME2)+PRIME3;
  str.i = n & 0x7fffff;
  str.i |= 0x40000000;
  str.f -= 3.0;
  //printf("%d %d %d -> %f\n", x, y, z, str.f);
  return str.f;
}

//////////////////////////////////////////////////
// 1D NOISE
//////////////////////////////////////////////////

static float smooth_noise_1d(int x) {
  //Could implement a smoothing function, but for this application it
  //really isn't necessary. Still, in case anyone ever decides to implement
  //it, here is the wrapper function for it.
  return noise(x,0,0);
}

static float interp_noise_1d(float x) {
  int X = floor(x);
  float fracx = x - X;
  return interpolate(smooth_noise_1d(X),smooth_noise_1d(X+1), fracx);
}

float perlin_1d(float x, float persistence, uchar obegin, uchar oend) {
  float total = 0;
  int i;
  uint freq;
  float amp;
  //return noise_1d(x);
  for(i=obegin; i<=oend; i++) {
    freq = 1 << i; //pow(2,i);
    amp = pow(persistence, i);
    total += interp_noise_1d(x*freq) * amp;
  }
  return total;
}

//////////////////////////////////////////////////
// 2D NOISE
//////////////////////////////////////////////////

#define PRIME1_2D 47441
#define PRIME2_2D 1941221
#define PRIME3_2D 1755994567.0

static float smooth_noise_2d(int x, int y) {
  //Could implement a smoothing function, but for this application it
  //really isn't necessary. Still, in case anyone ever decides to implement
  //it, here is the wrapper function for it.
  return noise(x,y,0);
}

static float interp_noise_2d(float x, float y) {
  int X = floor(x);
  int Y = floor(y);   
  float fracx = x - X;
  float fracy = y - Y;
  return
    interpolate(interpolate(smooth_noise_2d(X,Y),
			    smooth_noise_2d(X+1,Y), fracx),
		interpolate(smooth_noise_2d(X,Y+1),
			    smooth_noise_2d(X+1,Y+1), fracx),
		fracy);
}

float perlin_2d(float x, float y, float persistence, uchar obegin, uchar oend) {
  float total = 0;
  int i;
  uint freq;
  float amp;
  //Need to randomize the origin of the noise at each iteration to avoid radial artifacts
  for(i=obegin; i<=oend; i++) {
    freq = 1 << i; //pow(2,i);
    amp = pow(persistence, i);
    total += interp_noise_2d(x*freq + PRIME1_2D*i, y*freq + PRIME1_2D*i) * amp;
    //printf("%f  ",interp_noise_2d(x*freq, y*freq) * amp);
  }
  //printf("\n");
  //printf("%f %f -> %f\n\n", x, y, total);
  return total;
}

////////////////////////////////////////////////////
// 3D NOISE
////////////////////////////////////////////////////

#define PRIME1_3D 15731
#define PRIME2_3D 789221
#define PRIME3_3D 1073741824.0

static float smooth_noise_3d(int x, int y, int z) {
  //Could implement a smoothing function, but for this application it
  //really isn't necessary. Still, in case anyone ever decides to implement
  //it, here is the wrapper function for it.
  return noise(x,y,z);
}

static float interp_noise_3d(float x, float y, float z) {
  int X = floor(x);
  int Y = floor(y);       
  int Z = floor(z);
  float fracx = x - X;
  float fracy = y - Y;
  float fracz = z - Z;
  return
    interpolate(interpolate(interpolate(smooth_noise_3d(X,Y,Z),
					smooth_noise_3d(X+1,Y,Z), fracx),
			    interpolate(smooth_noise_3d(X,Y+1,Z),
					smooth_noise_3d(X+1,Y+1,Z), fracx),
			    fracy),
		interpolate(interpolate(smooth_noise_3d(X,Y,Z+1),
					smooth_noise_3d(X+1,Y,Z+1), fracx),
			    interpolate(smooth_noise_3d(X,Y+1,Z+1),
					smooth_noise_3d(X+1,Y+1,Z+1), fracx),
			    fracy),
		fracz);
}

float perlin_3d(float x, float y, float z, float persistence,
		       uchar obegin, uchar oend) {
  float total = 0;
  int i;
  uint freq;
  float amp;
  for(i=obegin; i<=oend; i++) {
    freq = 1 << i; //pow(2,i);
    amp = pow(persistence, i);
    total += interp_noise_3d(x*freq, y*freq, z*freq) * amp;
  }
  return total;
}

void set_perlin_seed(unsigned int seed) {
  perlin_seed = seed;
}
