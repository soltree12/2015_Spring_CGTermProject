#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Screen.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Pixels.h"
#include "stb_image_write.h"
#include "OctreeCube.h"
#include <glut.h>
#include <iostream>

using namespace std;

void calEquation(float a, float b, float c, float d, float i, float j, float &x, float &y);
Vec getFaceNormal(Face& f);
bool inTriangle(Vec& v0, Vec& v1, Vec& v2);
int toColorInt(float a);

Screen::Screen() :cam(Camera({ Vec(5, 5, 0), Vec(0, 0, -1), Vec(0, 1, 0), 24 })), left(-24), right(24), down(-24), up(24){}

Screen::Screen(Camera camera, float l, float r, float d, float u) : cam(camera), left(l), right(r), down(d), up(u){}

void Screen::raytracing(float pixelX, float pixelY, vector<Object>& objg, PointLight pLight, DirectionalLight dLight, SpotLight spLight,vector<OctreeCube>& octree,int maxDepth,int fileNum)
{
	fileNum;
	const int px = static_cast<int>(pixelX);
	
	const int py = static_cast<int>(pixelY);
	Vec from = cam.pos;
	Vec to;
	Vec ray;
	Vec pixelColor;
	vector<int> rArray;
	vector<int> gArray;
	vector<int> bArray;

	unsigned char *rgb = new unsigned char[px*py*3];


	float width = right - left;
	float height = up - down;

	float dx = width / pixelX;
	float dy = height / pixelY;
	cout << "Lines : ";
	for (int i = 0; i < (int)pixelY; i++)
	{
		if ((i+1)%100==0&&i!=0)
			cout << i+1 << " ";
		for (int j = 0; j < (int)pixelX; j++)
		{
			to = from+Vec(left + (j + 0.5)*dx, up - (i + 0.5)*dy, -cam.nearr);
			pixelColor = shade(from, to, objg, pLight, 0, dLight, spLight, 1, octree, maxDepth);
			int r = toColorInt(pixelColor.getX());
			int g = toColorInt(pixelColor.getY());
			int b = toColorInt(pixelColor.getZ());
			rArray.push_back(r);
			gArray.push_back(g);
			bArray.push_back(b);
			
			glColor3f(pixelColor.getX(), pixelColor.getY(), pixelColor.getZ());
	
			glRectf(left + dx*j, up-  dx*i, left + dx*(j + 1), up - dx*(i + 1));
		}
	}

	for (unsigned int i = 0; i < rArray.size(); i++)
	{
		rgb[3*i] = rArray[i];
		rgb[3*i+1]= gArray[i];
		rgb[3*i+2] = bArray[i];
	}

	string fileName = "picture_" + to_string(fileNum)+".png";
	stbi_write_png(fileName.c_str(), px, py, 3, rgb,3*px);

	delete[] rgb;
}

int toColorInt(float a)
{
	if (a < 0)
		a = 0;
	else if (a >= 1)
		a = 1;
	return a * 255;
}

Vec Screen::shade(Vec from, Vec to, vector<Object>& objg, PointLight pLight, int depth, DirectionalLight dLight, SpotLight spLight, float prevNr, vector<OctreeCube>& octree,int maxDepth)
{
	Vec reflect;
	Vec refract;
	Vec hitPos;
	Vec c(0.1, 0.1, 0.1);//color(초기값 : background color)
	Face hitFace;
	Face hitNormal;
	Object currentObj;
	bool isIntersect = false;
	isIntersect = shootray(from, to, objg, hitFace, hitPos, hitNormal, currentObj,octree);

	if (isIntersect)
	{
		bool isShadow1 = shootray(hitPos + (pLight.position - hitPos)*0.01, pLight.position, objg,octree);//pointLight 그림자검사
		bool isShadow2 = shootray(hitPos + (spLight.position - hitPos)*0.01, spLight.position, objg,octree);//spotLight 그림자검사
		if (!isShadow1)
		{
			c += pLight.applyLight(hitFace, hitPos, currentObj,hitNormal);
		}
		if (!isShadow2)
		{
			c += spLight.applyLight(hitFace, hitPos, currentObj, hitNormal);
		}
		c += dLight.applyLight(hitFace, hitPos, currentObj, hitNormal);
		if (depth < maxDepth)
		{
			Vec v = from - hitPos; 
			Vec n;
			if (!currentObj.isSphere)
				n = getFaceNormal(hitFace).getNormalized();
			else
				n = (hitPos - currentObj.pos).getNormalized();
			if (v.dot(n) > 0 && currentObj.isReflect)
			{
				//reflection
				reflect = Vec::reflect(v, n);
				c += shade(hitPos + reflect*0.001, hitPos + reflect, objg, pLight, depth + 1, dLight, spLight,currentObj.nr,octree,maxDepth)*(float)0.3;
			}
			if (currentObj.isReFract)
			{
				//refraction
				refract = Vec::refract(v.getNormalized(), n, prevNr,currentObj.nr);
				c += shade(hitPos + refract*0.001, hitPos + refract, objg, pLight, depth + 1, dLight, spLight,currentObj.nr,octree,maxDepth)*(float)0.4;
			}
		}
	}
	return c;
}

void passObject(Vec from, Vec to, vector<int>& objNumber,vector<int>& objFinal,vector<OctreeCube>& octree)//옥트리 큐브를 ray가 지나가는지 체크
{
	float boxT = 1000000;
	for (unsigned int i = 0; i < octree.size(); i++)
	{
		if (octree[i].rayIntersect(from, to, boxT))
			for (unsigned int j = 0; j < octree[i].objNum.size(); j++)
			{
				objNumber.push_back(octree[i].objNum[j]);
			}
	}


	for (unsigned int i = 0; i < objNumber.size(); i++)
	{
		if (i == 0)
		{
			objFinal.push_back(objNumber[0]);
		}
		for (unsigned int j = 0; j < i; j++)
		{
			if (objNumber[i] == objNumber[j])
				break;
			if (i - 1 == j)
			{
				objFinal.push_back(objNumber[i]);
			}
		}
	}
}

bool Screen::shootray(Vec from, Vec to, vector<Object>& objg,vector<OctreeCube>& octree)//shadow검사용 ray
{
	Vec n;
	Vec v = to - from;
	Vec vInFace;
	Vec faceVec1;
	Vec faceVec2;
	v.normalize();
	float d;
	float t;
	bool isShadow = false;
	bool isIn = false;
	float minT = 1000000;//t값이 백만 이상이면 오류남
	float boxT = 1000000;

	vector<int> objNumber;
	vector<int> objFinal;
	passObject(from, to, objNumber, objFinal, octree);

	for (unsigned int k = 0; k<objFinal.size(); k++)
	{
		int j = objFinal[k];
		if (objg[j].isSphere == false)
		{
			for (unsigned int i = 0; i < objg[j].face.size(); i++)
			{
				faceVec1 = objg[j].face[i].v2 - objg[j].face[i].v1;
				faceVec2 = objg[j].face[i].v3 - objg[j].face[i].v1;
				n = faceVec1.cross(faceVec2);
				d = -n.dot(objg[j].face[i].v1);

				t = -(from.dot(n) + d) / (v.dot(n));

				vInFace = from + v*t;//평면기준으로 계산 전

				vInFace = vInFace - objg[j].face[i].v1;//평면기준으로 계산 후

				isIn = inTriangle(faceVec1, faceVec2, vInFace);
				if (isIn&&t > 0)
				{
					isShadow = true;
				}
			}
		}

		else//구일때
		{
			float a, b, c;
			float xd, yd, zd;
			float x0, y0, z0;
			float xc, yc, zc;
			float r;
			float dis;
			float t0, t1;
			xd = v.getX();
			yd = v.getY();
			zd = v.getZ();
			x0 = from.getX();
			y0 = from.getY();
			z0 = from.getZ();
			xc = objg[j].pos.getX();
			yc = objg[j].pos.getY();
			zc = objg[j].pos.getZ();
			r = objg[j].radius;

			a = xd*xd + yd*yd + zd*zd;
			b = 2 * (xd*(x0 - xc) + yd*(y0 - yc) + zd*(z0 - zc));
			c = (x0 - xc)*(x0 - xc) + (y0 - yc)*(y0 - yc) + (z0 - zc)*(z0 - zc) - r*r;

			dis = b*b - 4 * a*c;

			if (dis == 0)
			{
				t = -b / (2 * a);
				if (t > 0)
				{
					isShadow = true;
				}
				
			}
			else if (dis > 0)
			{
				t0 = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
				t1 = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
				if (t0 > t1)
					t = t0;
				if (t1 > t0)
					t = t1;

				if (t > 0 )
				{
					isShadow = true;
				}
			}

		}

	}

	if (isShadow)
		return true;
	else
		return false;
}

bool Screen::shootray(Vec from, Vec to, vector<Object>& objg, Face& hitFace, Vec& hitPos, Face& hitNormal, Object& currentObj, vector<OctreeCube>& octree)//맞은 
{
	//intersect 검사용 ray
	vector<int> objNumber;
	vector<int> objFinal;
	Vec n;
	Vec v = to - from;
	Vec vInFace;
	Vec faceVec1;
	Vec faceVec2;
	v.normalize();
	float d;
	float t;
	bool isIn = false;
	float minT = 1000000;//t값이 백만 이상이면 오류남

	passObject(from, to, objNumber, objFinal,octree);

	for (unsigned int k = 0; k<objFinal.size(); k++)
	{
		int j = objFinal[k];
		if (objg[j].isSphere == false)
		{
			for (unsigned int i = 0; i < objg[j].face.size(); i++)
			{
				isIn = false;
				faceVec1 = objg[j].face[i].v2 - objg[j].face[i].v1;
				faceVec2 = objg[j].face[i].v3 - objg[j].face[i].v1;
				n = faceVec1.cross(faceVec2);
				d = -n.dot(objg[j].face[i].v1);

				t = -(from.dot(n) + d) / (v.dot(n));
				vInFace = from + v*t;//평면기준으로 계산 전
				vInFace = vInFace - objg[j].face[i].v1;//평면기준으로 계산 후


				isIn = inTriangle(faceVec1, faceVec2, vInFace);
				if (isIn)
				{
					if (t > 0 && minT > t)//반직선 고려, 제일작은 t를 구함
					{
						minT = t;
						hitFace = objg[j].face[i];
						hitNormal = objg[j].normal[i];
						currentObj = objg[j];
					}
				}
			}
		}

		else//구일때
		{
			float a, b, c;
			float xd, yd, zd;
			float x0, y0, z0;
			float xc, yc, zc;
			float r;
			float dis;
			float t0, t1;
			xd = v.getX();
			yd = v.getY();
			zd = v.getZ();
			x0 = from.getX();
			y0 = from.getY();
			z0 = from.getZ();
			xc = objg[j].pos.getX();
			yc = objg[j].pos.getY();
			zc = objg[j].pos.getZ();
			r = objg[j].radius;

			a = xd*xd + yd*yd + zd*zd;
			b = 2 * (xd*(x0 - xc) + yd*(y0 - yc) + zd*(z0 - zc));
			c = (x0 - xc)*(x0 - xc) + (y0 - yc)*(y0 - yc) + (z0 - zc)*(z0 - zc) - r*r;

			dis = b*b - 4 * a*c;

			if (dis == 0)
			{
				t = -b / (2 * a);
				if (t > 0 && t < minT)
				{
					minT = t;

					currentObj = objg[j];
				}
			}
			else if (dis > 0)
			{
				t0 = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
				t1 = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
				if (t0 < t1)
					t = t0;
				if (t1 < t0)
					t = t1;

				if (t > 0 && t < minT)
				{
					currentObj = objg[j];
					minT = t;
				}
			}
		}
	}

	if (minT != 1000000)
	{
		hitPos = v*minT + from;
		return true;
	}
	else
	{
		hitFace = Face({ Vec(0, 0, 0), Vec(0, 0, 0), Vec(0, 0, 0) });

		hitPos = Vec(0, 0, 0);
		return false;
	}
}

void calEquation(float a, float b, float c, float d, float i, float j, float &x, float &y)//연립방정식 계산
{
	float detA = a*d - b*c;
	if (detA == 0)
	{
		cout << "Error : detA is zero!";
		exit(1);
	}

	x = (d*i - b*j) / detA;
	y = (a*j - c*i) / detA;
}

Vec getFaceNormal(Face& f)
{
	return (f.v2 - f.v1).cross(f.v3 - f.v1);
}

bool inTriangle(Vec& v0, Vec& v1, Vec& v2)
{
	float dot00 = v0.dot(v0);
	float dot01 = v0.dot(v1);
	float dot02 = v0.dot(v2);
	float dot11 = v1.dot(v1);
	float dot12 = v1.dot(v2);

	float	invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float	u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float	v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}