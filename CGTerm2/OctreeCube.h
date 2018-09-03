#ifndef OCTREECUBE_H
#define OCTREECUBE_H
#include <vector>

#include "Vec.h"
#include "Face.h"
#include "Object.h"
using namespace std;
class OctreeCube
{
public :
	Vec StartPos;
	float length;
	vector<int> objNum;

	bool inCube(Object& obj);
	bool rayIntersect(Vec start,Vec end,float& t);
};
#endif