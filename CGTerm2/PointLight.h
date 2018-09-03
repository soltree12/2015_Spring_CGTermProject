#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "Object.h"
#include "Vec.h"
#include "Face.h"

class PointLight 
{
public:
	PointLight();
	PointLight(Vec p, Vec i);
	Vec getPosition() const;
	void setPosition(float a, float b, float c);
	void setPosition(Vec v);
	Vec getIntensity() const;
	void setIntensity(Vec a);
	Vec applyLight(Face f, Vec pos, Object obj, Face hitNormal);
	Vec position;
	Vec intensity;
};
#endif