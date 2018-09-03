#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "Object.h"
#include "Face.h"
#include "Vec.h"

class DirectionalLight 
{
public :
	DirectionalLight();
	DirectionalLight(Vec v, Vec i);
	Vec getDirection()const;
	Vec getIntensity()const;
	void setDirection(float a, float b, float c);
	void setDirection(Vec v);
	void setIntensity(Vec i);
	Vec applyLight(Face f, Vec pos, Object obj, Face hitNormal);
	Vec direction;//Normalize된 상태로 저장
	Vec intensity;
};

#endif