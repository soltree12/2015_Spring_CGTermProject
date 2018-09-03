#include "OctreeCube.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>

using namespace std;

bool OctreeCube::rayIntersect(Vec start,Vec end,float& t)
{
	Vec v = end - start;
	v.normalize();
	float x = 1.0f / v.getX();
	float y = 1.0f / v.getY();
	float z = 1.0f / v.getZ();
	float t1 = (StartPos.getX() - start.getX())*x;
	float t2 = (StartPos.getX() + length - start.getX())*x;
	float t3 = (StartPos.getY() - start.getY())*y;
	float t4 = (StartPos.getY()+length- start.getY())*y;
	float t5 = (StartPos.getZ() - start.getZ())*z;;
	float t6 = (StartPos.getZ() + length - start.getZ())*z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	return true;
}

bool OctreeCube::inCube(Object& obj)
{
	float bMinx, bMiny, bMinz, bMaxx, bMaxy, bMaxz, aMinx, aMiny, aMinz, aMaxx, aMaxy, aMaxz;
	float l = length;
	bMinx = StartPos.getX();
	bMiny = StartPos.getY();
	bMinz = StartPos.getZ();
	bMaxx = StartPos.getX()+l;
	bMaxy = StartPos.getY()+l;
	bMaxz = StartPos.getZ()+l;
	
	aMinx = obj.face[0].v1.getX();
	aMiny = obj.face[0].v1.getY();
	aMinz = obj.face[0].v1.getZ();
	aMaxx = obj.face[0].v1.getX();
	aMaxy = obj.face[0].v1.getY();
	aMaxz = obj.face[0].v1.getZ();
	for (unsigned int i = 0; i < obj.face.size(); i++)
	{
		Vec tri[3];
		tri[0] = obj.face[i].v1;
		tri[1] = obj.face[i].v2;
		tri[2] = obj.face[i].v3;
		for (int j = 0; j < 3; j++)
		{
			if (tri[j].getX() < aMinx)
				aMinx = tri[j].getX();
			if (tri[j].getY() < aMiny)
				aMiny = tri[j].getY();
			if (tri[j].getZ() < aMinz)
				aMinz = tri[j].getZ();
			if (tri[j].getX() > aMaxx)
				aMaxx = tri[j].getX();
			if (tri[j].getY() > aMaxy)
				aMaxy = tri[j].getY();
			if (tri[j].getZ() > aMaxz)
				aMaxz = tri[j].getZ();
		}
	}

	if (aMaxx < bMinx || aMinx > bMaxx) return false;
	if (aMaxy < bMiny || aMiny > bMaxy) return false;
	if (aMaxz < bMinz || aMinz > bMaxz) return false;
	return true;
}