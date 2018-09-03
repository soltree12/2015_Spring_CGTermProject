#include "SpotLight.h"
#include <math.h>
#include <iostream>

using namespace::std;

void getRatio3(Vec p, Vec a, Vec b, Vec c, float &u, float &v, float &w)
{
	Vec v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = v0.dot(v0);
	float d01 = v0.dot(v1);
	float d11 = v1.dot(v1);
	float d20 = v2.dot(v0);
	float d21 = v2.dot(v1);
	float f = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / f;
	w = (d00 * d21 - d01 * d20) / f;
	u = 1.0f - v - w;
}

SpotLight::SpotLight() : cutoff(0), factor(0)
{
	Vec v;
	v.setVector(0, 0, 0);
	position = v;
	direction = v;
	intensity = v;
}

Vec SpotLight::applyLight(Face f, Vec pos, Object obj, Face hitNormal)
{
	Vec v[3];
	v[0] = f.v1;
	v[1] = f.v2;
	v[2] = f.v3;
	Vec color[3];

	Vec E=obj.Em;
	Vec AL(0, 0, 0);
	Vec N;
	Vec L;
	Vec V;
	Vec R;
	float VR;

	Vec Emission;
	Vec Ambient;
	Vec Diffuse;
	Vec Specular;

	float spotFactor;

	for (int i = 0; i < 3; i++)
	{
		Emission = E;

		float distance = 0;
		Vec normalSum;

		if (!obj.isSphere)
		{
			switch (i)
			{
			case 0: N = hitNormal.v1; break;
			case 1: N = hitNormal.v2; break;
			case 2: N = hitNormal.v3; break;
			}
		}
		else
			N = pos - obj.pos;
		N.normalize();


		if (!obj.isSphere)
		{
			V = Vec(0, 0, 0) - v[i];//��������
			V.normalize();
			L = position - v[i];
			L.normalize();


			R = Vec::reflect(L, N);//L�� ��鿡�� �������� ������ ����
			R.normalize();
			VR = V.dot(R);

			if (V.dot(R) < 0)
				VR = 0;
		}
		else
		{
			V = Vec(0, 0, 0) - pos;//��������
			V.normalize();
			L = position - pos;
			L.normalize();


			R = Vec::reflect(L, N);//L�� ��鿡�� �������� ������ ����
			R.normalize();
			VR = V.dot(R);

			if (V.dot(R) < 0)
				VR = 0;
		}
		spotFactor = L.dot(direction.getNormalized() *-1);
		if (spotFactor > cutoff)
		{
			Emission = E;
			Ambient = obj.Kal*AL;
			Diffuse = obj.Kd *(intensity*(N.dot(L)));
			Specular = obj.Ks*(intensity*powf(VR, 3));
			if (!obj.isSphere)
				distance = (v[i] - position).length();
			else
				distance = (pos - position).length();
			Diffuse.delNegative();
			Ambient.delNegative();
			Specular.delNegative();
			Emission.delNegative();

			Vec c = Ambient + Emission + Diffuse + Specular;

			c.setX(c.getX()*(pow(direction.dot(L*-1), factor)));
			c.setY(c.getY()*(pow(direction.dot(L*-1), factor)));
			c.setZ(c.getZ()*(pow(direction.dot(L*-1), factor)));
			color[i] = c / (1 + 0.01*distance);
		}

		else
			color[i] = Vec(0, 0, 0);
	}

	Vec resultColor;

	if (!obj.isSphere)
	{
		float alpha;
		float beta;
		float gamma;
		getRatio3(pos, v[0], v[1], v[2], alpha, beta, gamma);
		resultColor = color[0] * alpha + color[1] * beta + color[2] * gamma;
	}
	else
	{
		resultColor = color[0];
	}
	
	return resultColor;
}

void SpotLight::setPosition(Vec p)
{
	position = p;
}
void SpotLight::setDirection(Vec d)
{
	direction = d;
}
void SpotLight::setIntensity(Vec i)
{
	intensity = i;
}

Vec SpotLight::getPosition() const { return position; }
Vec SpotLight::getDirection() const { return direction; }
Vec SpotLight::getIntensity() const{ return intensity; }
float SpotLight::getFactor() const{ return factor; }