#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vec.h"

class Transform
{
public:
	Vec move(Vec v, float a, float b, float c);
	Vec rotate(Vec v, char a, float theta);
	Vec Scale(Vec v, float a);
};

#endif