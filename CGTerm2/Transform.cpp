#include "Transform.h"
#include <iostream>
#include  <math.h>
using namespace std;

Vec Transform::move(Vec v, float a, float b, float c)
{
	Vec returnVec;
	returnVec.setVector(v.getX() + a, v.getY() + b, v.getZ() + c);
	return returnVec;
}

Vec Transform::rotate(Vec v, char a, float theta)
{
	Vec returnVec;

	if (a == 'x')
	{
		returnVec.setVector(v.getX(), v.getY()*cos(theta) - v.getZ()*sin(theta), v.getY()*sin(theta) + v.getZ()*cos(theta));
	}
	if (a == 'y')
	{
		returnVec.setVector(v.getX()*cos(theta) + v.getZ()*sin(theta), v.getY(), -v.getX()*sin(theta) + v.getZ()*cos(theta));
	}
	if (a == 'z')
	{
		returnVec.setVector(v.getX()*cos(theta) - v.getY()*sin(theta), v.getX()*sin(theta) + v.getY()*cos(theta), v.getZ());
	}

	return returnVec;
}

Vec Transform::Scale(Vec v, float a)
{
	Vec returnVec;
	returnVec.setVector(v.getX()*a, v.getY()*a, v.getZ()*a);
	return returnVec;
}