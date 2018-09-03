#ifndef VEC_H
#define VEC_H

class Vec
{
public:
	Vec();
	Vec(float a, float b, float c);
	void setVector(float a, float b, float c);
	void output();
	float getX()const;
	float getY()const;
	float getZ()const;
	void setX(float X);
	void setY(float Y);
	void setZ(float Z);
	bool isNormalized()const;
	void zero();
	void normalize();
	Vec getNormalized();
	static float dot(const Vec &vec1, const Vec& vec2);
	float dot(const Vec &vec)const;
	static Vec cross(const Vec &vec1, const Vec& vec2);
	Vec cross(const Vec &vec)const;	
	static Vec reflect(Vec &v,const Vec &n);
	static Vec refract(Vec &v, const Vec &n, float nr1,float nr2);
	float length();
	void delNegative();
	void addX(float val);
	void addY(float val);
	void addZ(float val);;

	static float getDistance(const Vec &v1, const Vec &v2);

	void operator +=(const Vec &vec);
	void operator -=(const Vec &vec);
	Vec operator +(const Vec &vec);
	Vec operator +(float val)const;
	Vec operator -(const Vec &vec);
	Vec operator -(float val)const;
	Vec operator *(const Vec &vec)const;
	Vec operator *(float val)const;
	Vec operator /(float val)const;
	void operator *=(const float val);
	void operator /=(const float val);

private:
	float x, y, z;
};

#endif