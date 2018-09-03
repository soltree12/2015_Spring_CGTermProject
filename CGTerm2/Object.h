#ifndef OBJECT_H
#define OBJECT_H

#include "Vec.h"
#include "Face.h"
#include <vector>
#include <string>
using namespace std;

struct Object
{
	vector<Face> face;
	vector<Face> normal;
	string name;
	Vec Kal;
	Vec Kd;
	Vec Ks;
	Vec Em;
	bool rayIn=false;
	float nr;
	bool isReflect;
	bool isReFract;

	bool isSphere;
	Vec pos;
	float radius;
};
#endif