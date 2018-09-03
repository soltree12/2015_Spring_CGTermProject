#include "Vec.h"
#include <iostream>

using std::cout;
using std::endl;

Vec::Vec(){}

Vec::Vec(float a, float b, float c) :x(a), y(b), z(c){}

void Vec::output()
{
	cout << " x : " << x << "/ y : " << y << " / z : " << z << endl;
}

void Vec::setVector(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

float Vec::getX()const
{
	return x;
}

float Vec::getY()const
{
	return y;
}

float Vec::getZ()const
{
	return z;
}

void Vec::setX(float X)
{
	x = X;
}
void Vec::setY(float Y)
{
	y = Y;
}

void Vec::setZ(float Z)
{
	z = Z;
}

bool Vec::isNormalized() const
{
	if (x >= -1 && x <= 1 && y>=-1 && y <= 1 && z>=-1 && z <= 1)
		return true;
	else
		return false;
}

void Vec::zero()
{
	x = 0;
	y = 0;
	z = 0;
}

float Vec::length()
{
	return sqrt((x*x) + (y*y) + (z*z));
}

void Vec::normalize()
{
	float l = length();
	if (l != 0)
	{
		x /= l;
		y /= l;
		z /= l;
	}
}

void Vec::delNegative()
{
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (z < 0)
		z = 0;
}

Vec Vec::getNormalized()
{
	float l = length();
	return Vec(x / l, y / l, z / l);
}

float Vec::dot(const Vec &vec1, const Vec& vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}
float Vec::dot(const Vec &vec)const
{
	return x*vec.x + y*vec.y + z*vec.z;
}
Vec Vec::cross(const Vec &vec1, const Vec& vec2)
{
	return Vec(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}
Vec Vec::cross(const Vec &vec)const
{
	return Vec(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}
void Vec::addX(float val)
{
	x += val;
}
void Vec::addY(float val)
{
	y += val;
}
void Vec::addZ(float val)
{
	z += val;
}

float Vec::getDistance(const Vec &v1, const Vec &v2)
{
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float dz = v2.z - v1.z;

	return sqrt(dx*dx + dy*dy + dz*dz);
}

Vec Vec::reflect(Vec &v, const Vec &n)//평면에서 시점으로 나가는 벡터 기준
{
		return n*(n.dot(v) * 2) - v;
}

Vec Vec::refract(Vec &v, const Vec &n, float nr1, float nr2)
{
	float r = nr1 / nr2;
	Vec l = v*-1;
	float c = n.dot(l)*-1;
	return l*r + n*(c*r - sqrtf(1 - r*r*(1 - c*c)));
}
void Vec::operator +=(const Vec &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

void Vec::operator -=(const Vec &vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

Vec Vec::operator +(const Vec &vec)
{
	return Vec(x + vec.x, y + vec.y, z + vec.z);
}
Vec Vec::operator +(float val)const
{
	return Vec(x + val, y + val, z + val);
}
Vec Vec::operator - (const Vec &vec)
{
	return Vec(x - vec.x, y - vec.y, z - vec.z);
}
Vec Vec::operator - (float val)const
{
	return Vec(x - val, y - val, z - val);
}
Vec Vec::operator *(const Vec &vec)const
{
	return Vec(x * vec.x, y * vec.y, z * vec.z);
}
Vec Vec::operator *(float val)const
{
	return Vec(x * val, y * val, z * val);
}
Vec Vec::operator /(float val)const
{
	return Vec(x / val, y / val, z / val);
}
void Vec::operator *=(const float val)
{
	x *= val;
	y *= val;
	z *= val;
}
void Vec::operator /=(const float val)
{
	x /= val;
	y /= val;
	z /= val;
}


