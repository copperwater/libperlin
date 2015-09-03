#ifndef PERLIN_H
#define PERLIN_H

float perlin_1d(float x,
		float persistence, unsigned char obegin, unsigned char oend);
float perlin_2d(float x, float y,
		float persistence, unsigned char obegin, unsigned char oend);
float perlin_3d(float x, float y, float z,
		float persistence, unsigned char obegin, unsigned char oend);

void set_perlin_seed(unsigned int seed);

void get_perlin_minmax(float* min, float* max, float persistence,
		       unsigned char obegin, unsigned char oend);

#endif
