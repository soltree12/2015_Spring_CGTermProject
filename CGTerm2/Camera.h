#ifndef CAMERA_H
#define CAMERA_H

#include "Vec.h"

struct Camera
{
	Vec pos; 
	Vec dir; 
	Vec up; 
	float nearr;
};

#endif