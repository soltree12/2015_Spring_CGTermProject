#include "PointLight.h"
#include <iostream>
#include <math.h>
using namespace std;

void getRatio(Vec p, Vec a, Vec b, Vec c, float &u, float &v, float &w)
{
	Vec v0 = b - a, v1 = c - a, v2 = p - a;	
	float d00 =v0.dot(v0);
	float d01 = v0.dot(v1);
	float d11 = v1.dot(v1);
	float d20 = v2.dot(v0);
	float d21 = v2.dot(v1);
	float f = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / f;
	w = (d00 * d21 - d01 * d20) / f;
	u = 1.0f - v - w;
}

PointLight::PointLight() 
{
	Vec v;
	position = v;
	intensity = v;
}

Vec PointLight::applyLight(Face f, Vec pos, Object obj, Face hitNormal)
{
	Vec v[3];
	v[0] = f.v1;
	v[1] = f.v2;
	v[2] = f.v3;
	Vec color[3];

	Vec E=obj.Em;
	Vec AL(0,0,0);
	Vec N;
	Vec L;
	Vec V;
	Vec R;
	float VR; 

	Vec Emission;
	Vec Ambient;
	Vec Diffuse;
	Vec Specular;

	for (int i = 0; i < 3;i++)
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
			N =  pos -obj.pos;
		N.normalize();

		if (!obj.isSphere)
		{
			V = Vec(0, 0, 0) - v[i];//원점기준(시점이 원점)
			V.normalize();
			L = position - v[i];
			L.normalize();


			R = Vec::reflect(L, N);//L은 평면에서 시점으로 나가는 벡터
			R.normalize();
			VR = V.dot(R);

			if (V.dot(R) < 0)
				VR = 0;
		}
		else
		{
			V = Vec(0, 0, 0) -	pos;//원점기준(시점이 원점)
			V.normalize();
			L = position - pos;
			L.normalize();
			R = Vec::reflect(L, N);//L은 평면에서 시점으로 나가는 벡터
			R.normalize();
			VR = V.dot(R);
			if (V.dot(R) < 0)
				VR = 0;
		}

		Emission = E;
		Ambient = obj.Kal*AL;
		Diffuse = obj.Kd *(intensity*(N.dot(L)));
		Specular = obj.Ks*(intensity*powf(VR, 10));
		
		if (!obj.isSphere)
		{
			distance = (v[i] - position).length();
		}
		else
			distance = (pos - position).length();

		Diffuse.delNegative();
		Ambient.delNegative();
		Specular.delNegative();
		Emission.delNegative();

		Vec c = Ambient + Emission + Diffuse + Specular;
	
		color[i] = c/(1 + 0.01f*distance);
	}

	Vec resultColor;

	if (!obj.isSphere)
	{
		//cout << "=================" << endl;
		float alpha;
		float beta;
		float gamma;
		getRatio(pos, v[0], v[1], v[2], alpha, beta, gamma);
		//cout << "alpha : " << alpha << " , beta : " << beta << " , gamma : " << gamma << endl;
		resultColor = color[0] * alpha + color[1] * beta + color[2] * gamma;
	}
	else
	{
		resultColor = color[0];
	}

	return resultColor;
}
PointLight::PointLight(Vec p, Vec i) : intensity(i)
{
	position = p;
}
