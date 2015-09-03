#ifndef PERLIN_H
#define PERLIN_H

extern unsigned int perlin_seed; //Store a call to time(0) here

float perlin_1d(float x,
		float persistence, unsigned char obegin, unsigned char oend);
float perlin_2d(float x, float y,
		float persistence, unsigned char obegin, unsigned char oend);
float perlin_3d(float x, float y, float z,
		float persistence, unsigned char obegin, unsigned char oend);

void get_perlin_minmax(float* min, float* max, float persistence,
		       unsigned char obegin, unsigned char oend);

#endif
