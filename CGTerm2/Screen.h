#ifndef SCREEN_H
#define SCREEN_H

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "OctreeCube.h"

class Screen
{
public :
	Screen(Camera camera,float l,float r,float d, float u);
	Screen();
	void raytracing(float pX, float pY, vector<Object>& objg, PointLight pLight, DirectionalLight dLight, SpotLight spLight, vector<OctreeCube>& octree, int maxDepth,int fileNum);//ÇÈ¼¿ x, ÇÈ¼¿ y
	bool shootray(Vec from, Vec to, vector<Object>& objg, Face& hitFace, Vec& hitpos, Face& hitNormal, Object& currentObj, vector<OctreeCube>& octree);
	bool shootray(Vec from, Vec to, vector<Object>& objg, vector<OctreeCube>& octree);
	Vec shade(Vec from, Vec to, vector<Object>& objg, PointLight pLight, int depth, DirectionalLight dLight, SpotLight spLight, float prevNr, vector<OctreeCube>& octree, int maxDepth);
	Camera cam;
	float left;
	float right;
	float down;
	float up;
};

#endif