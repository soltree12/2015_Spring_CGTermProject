#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "Object.h"
#include "Face.h"
#include "Vec.h"
class SpotLight
{
public :
	SpotLight();
	SpotLight(Vec p, Vec d, Vec i, float a, float f);
	void setValues(Vec p, Vec d, Vec i, float a, float f);
	void setPosition(Vec p);
	void setDirection(Vec d);
	void setIntensity(Vec i);
	Vec getPosition() const;
	Vec getDirection() const;
	Vec getIntensity() const;
	float getFactor() const;
	Vec SpotLight::applyLight(Face f, Vec pos, Object obj, Face hitNormal);

	Vec position;
	Vec direction;
	Vec intensity;
	float cutoff;
	float factor;
};

#endif