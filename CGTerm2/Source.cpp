#define STB_IMAGE_IMPLEMENTATION	
#define RAD(X) ((X)*3.1415/180.0)

#include "Camera.h"
#include "Face.h"
#include "Object.h"
#include "Screen.h"
#include "Vec.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Transform.h"
#include "Pattern.h"
#include "stb_image.h"
#include "OctreeCube.h"

#include <math.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

static int index = 0;
static int final = 1000;
int maxDepth;

vector<Object> objg;
Object o;
Object o2;
Object o3;
Object o4;

vector<OctreeCube> octree;
vector<OctreeCube> octreeFinal;

Transform tr;

Pattern pat1;
vector<string> pArray;
vector<Pattern> patterns;
vector<Object> buttons;

vector<Vec> bColor;

DirectionalLight dLight;
PointLight pLight;
SpotLight spLight;

int theta = 1;
Screen scr;
int X = 800, Y = 800;

void makeOctree()
{
	octree.clear();
	octreeFinal.clear();
	OctreeCube firstOctree;
	firstOctree.StartPos = Vec(-25, -25, -25);
	firstOctree.length = 50;
	octree.push_back(firstOctree);
	for (int m = 0; m < 55; m++)
	{
		OctreeCube tempCube;
		tempCube = octree[0];
		int numInTree = 0;

		for (int i = 0; i < 67; i++)
		{
			if (objg[i].isSphere == false)
			{
				if (tempCube.inCube(objg[i]))
				{
					numInTree++;

				}
			}
			if (objg[i].isSphere == true)
			{
				float a = Vec::getDistance(objg[i].pos, tempCube.StartPos + (tempCube.length / 2));
				float b = objg[i].radius + (tempCube.length*sqrt(3)) / 2;

				if (a < b)
				{
					numInTree++;
				}
			}
		}

		if (numInTree >= 2)
		{
			OctreeCube oct[8];
			float x = tempCube.StartPos.getX();
			float y = tempCube.StartPos.getY();
			float z = tempCube.StartPos.getZ();
			float l = tempCube.length;

			oct[0].StartPos = Vec(x, y, z);
			oct[1].StartPos = Vec(x + (l / 2), y, z);
			oct[2].StartPos = Vec(x, y + (l / 2), z);
			oct[3].StartPos = Vec(x + (l / 2), y + (l / 2), z);
			oct[4].StartPos = Vec(x, y, z + (l / 2));
			oct[5].StartPos = Vec(x + (l / 2), y, z + (l / 2));
			oct[6].StartPos = Vec(x, y + (l / 2), z + (l / 2));
			oct[7].StartPos = Vec(x + (l / 2), y + (l / 2), z + (l / 2));

			for (int k = 0; k < 8; k++)
			{
				oct[k].length = tempCube.length / 2;
				for (int i = 0; i < objg.size(); i++)
				{
					if (objg[i].isSphere == false)
					{
						if (oct[k].inCube(objg[i]))
						{
							oct[k].objNum.push_back(i);

						}
					}
					if (objg[i].isSphere == true)
					{
						float a = Vec::getDistance(objg[i].pos, oct[k].StartPos + (oct[k].length / 2));
						float b = objg[i].radius + (oct[k].length*sqrt(3)) / 2;

						if (a < b)
						{
							oct[k].objNum.push_back(i);
						}
					}
				}
			}


			for (int i = 0; i < 8; i++)
				octree.push_back(oct[i]);
			octree.erase(octree.begin(), octree.begin() + 1);
		}
		else
		{
			octreeFinal.push_back(tempCube);
			octree.erase(octree.begin(), octree.begin() + 1);
		}
	}

	for (int i = 0; i < octree.size(); i++)
	{
		octreeFinal.push_back(octree[i]);
	}

	cout << "Number of octree : " << octreeFinal.size() << endl;
}

void readObj(Object& o, string file)
{
	ifstream fin;
	string str;
	vector<Vec> vnArray;
	vector<Vec> vArray;
	float a = 0, b = 0, c = 0;
	int x = 0, y = 0, z = 0;

	fin.open(file);
	if (fin.fail())
	{
		cout << "Input file opening failed. \n";
		exit(1);
	}

	int count = 0;
	while (getline(fin, str))
	{
		if (str[0] == 'v'&&str[1] == ' ')
		{
			sscanf_s(str.c_str(), "%*s %f %f %f", &a, &b, &c);
			vArray.push_back(Vec(a, b, c));
		}

		if (str[0] == 'v'&&str[1] == 'n'&&str[2] == ' ')
		{
			sscanf_s(str.c_str(), "%*s %f %f %f", &a, &b, &c);
			vnArray.push_back(Vec(a, b, c));
		}
		if (str[0] == 'f'&&str[1] == ' ')
		{
			sscanf_s(str.c_str(), "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &x, &y, &z);
			o.face.push_back(Face({ vArray[x - 1], vArray[y - 1], vArray[z - 1] }));
			o.normal.push_back(Face({ vnArray[x - 1], vnArray[y - 1], vnArray[z - 1] }));
		}
		count++;
	}

	fin.close();
}

void readPattern()
{
	ifstream fin;
	string str;
	fin.open("pattern.txt");
	if (fin.fail())
	{
		cout << "pattern input failed." << endl;
		exit(0);
	}
	while (getline(fin, str))
	{
		pArray.push_back(str);
	}
}

void objectInit()
{
	for (int i = 0; i < 64; i++)
	{
		bColor.push_back(Vec(0.1, 0.1, 0.1));
	}

	for (int i = 0; i < 64; i++) // 버튼 객체 변수 초기화
	{
		Object temp;
		temp.nr = 1.1;
		temp.name = "버튼" + to_string(i + 1);
		temp.Em = Vec(0, 0, 0);
		temp.Kal = Vec(0, 0, 0);
		temp.Kd = bColor[i];
		temp.Ks = Vec(0.5, 0.5, 0.5);
		temp.isReflect = true;
		temp.isReFract = true;
		temp.isSphere = false;
		string fileName;
		fileName = to_string(i) + ".obj";

		readObj(temp, fileName);
		objg.push_back(temp);
	}

	Object floor;
	floor.nr = 1.5;
	floor.name = "바닥";
	floor.Kal = Vec(0, 0, 0);
	floor.Kd = Vec(0.9, 0.9, 0.9);
	floor.Ks = Vec(0.5, 0.5, 0.5);
	floor.Em = Vec(0, 0, 0);
	floor.isReflect = true;
	floor.isReFract = true;
	floor.isSphere = false;
	readObj(floor, "Floor_0.obj");
	objg.push_back(floor);

	o3.nr = 1.5;
	o3.name = "구";
	o3.Kal = Vec(0, 0, 0);
	o3.Kd = Vec(1, 1, 0.9);
	o3.Ks = Vec(0.5, 0.5, 0.5);
	o3.Em = Vec(0, 0, 0);
	o3.isReflect = true;
	o3.isReFract = true;
	o3.isSphere = true;
	o3.pos = Vec(6, 3, -10);
	o3.radius = 3;
	objg.push_back(o3);

	o4.nr = 1.5;
	o4.name = "구2";
	o4.Kal = Vec(0, 0, 0);
	o4.Kd = Vec(1, 1, 0.9);
	o4.Ks = Vec(0.5, 0.5, 0.5);
	o4.Em = Vec(0, 0, 0);
	o4.isReflect = true;
	o4.isReFract = true;
	o4.isSphere = true;
	o4.pos = Vec(-6, 3, -10);
	o4.radius = 3;
	objg.push_back(o4);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	scr.raytracing(X, Y, objg, pLight, dLight, spLight, octreeFinal, maxDepth, index);

	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(scr.left, scr.right, scr.down, scr.up);
}

void lightInit()
{
	pLight.position = Vec(10, 0, 2);
	pLight.intensity = Vec(0.9, 0.9, 0.9);

	dLight.direction = Vec(0, -1, 0);
	dLight.intensity = Vec(1, 0, 0);

	spLight.direction = Vec(0, -1, 0);
	spLight.position = Vec(0, 100, -40);
	spLight.factor = 2;
	spLight.intensity = Vec(1, 1, 1);
	spLight.cutoff = cos(RAD(15));
}

void idle()
{
	Vec red(1, 0, 0);
	Vec green(1, 1, 0);
	Vec blue(0, 0, 1);
	Vec black(0, 0, 0);

	index++;
	for (int i = 0; i < 64; i++)
	{
		objg[i].Kd = Vec(0, 0, 0);
	}
	if (index < pArray.size())
	{
		for (int i = 0; i < 64; i++)
		{
			switch (static_cast<int>(pArray[index][i] - '0'))
			{
				case 1: //1번 패턴(점)
				{
					Pattern temp;
					temp.current = 0;
					temp.length = 1;
					temp.number = 1;
					temp.startPos = i;
					patterns.push_back(temp);
					break;
				}

				case 2://2번 패턴(십자 모양)
				{
					Pattern temp;
					temp.current = 0;
					temp.length = 8;
					temp.number = 2;
					temp.startPos = i;
					patterns.push_back(temp);
					break;
				}

				case 3://3번 패턴(주변 둘러싸는 사각형)			
				{
					Pattern temp;
					temp.current = 0;
					temp.length = 1;
					temp.number = 3;
					temp.startPos = i;
					patterns.push_back(temp);
					break;
				}
			}
		}
	}

	for (int i = 0; i < patterns.size(); i++)
	{
		if (patterns[i].current < patterns[i].length)
		{
			if (patterns[i].number == 1)
			{
				objg[patterns[i].startPos].Kd = green;
				patterns[i].current++;
			}

			if (patterns[i].number == 2)//2번 패턴
			{
				int pos = patterns[i].startPos;
				int current = patterns[i].current;
				int left = -1 * current + pos;
				int up = -8 * current + pos;
				int down = 8 * current + pos;
				int right = 1 * current + pos;
				if (left / 8 == pos / 8)
				{
					objg[left].Kd = green;
					if (patterns[i].current == 1)
						objg[left].Kd = red;
				}
				if (right / 8 == pos / 8)
				{
					objg[right].Kd = green;
					if (patterns[i].current == 1)
						objg[right].Kd = red;
				}
				if (up >= 0)
				{
					objg[up].Kd = green;
					if (patterns[i].current == 1)
						objg[up].Kd = red;
				}
				if (down < 64)
				{
					objg[down].Kd = green;
					if (patterns[i].current == 1)
						objg[down].Kd = red;
				}
				patterns[i].current++;
			}

			if (patterns[i].number == 3)//3번 패턴
			{
				int pos = patterns[i].startPos;
				int left = -1 + pos;
				int up = -8 + pos;
				int down = 8 + pos;
				int right = 1 + pos;
				int lu = pos - 9;
				int ru = pos - 7;
				int ld = pos + 7;
				int rd = pos + 9;
				if (left / 8 == pos / 8)
					objg[left].Kd = green;
				if (right / 8 == pos / 8)
					objg[right].Kd = green;
				if (up >= 0)
					objg[up].Kd = green;
				if (down < 64)
					objg[down].Kd = green;
				if (((lu + 8) / 8 == pos / 8) && (lu >= 0))
					objg[lu].Kd = green;
				if (((ru + 8) / 8 == pos / 8) && (ru >= 0))
					objg[ru].Kd = green;
				if (((ld - 8) / 8 == pos / 8) && (ld < 64))
					objg[ld].Kd = green;
				if (((rd - 8) / 8 == pos / 8) && (rd < 64))
					objg[rd].Kd = green;
				patterns[i].current++;
			}
		}
	}

	objg[65].pos = tr.rotate(objg[65].pos, 'z', RAD(theta));
	objg[66].pos = tr.rotate(objg[66].pos, 'z', RAD(theta));

	if (index == final)
		exit(1);
	cout << endl << "==========================================================";
	cout <<endl<< "picture_" << index <<" Creating..."<< endl;

	makeOctree();
	glutPostRedisplay();
}

void definePattern()
{
	pat1.length = 7;
	pat1.number = 1;
	pat1.current = 0;
}

void writePattern()
{
	string fileName;
	ofstream fout;
	fout.open("pattern.txt");
	for (int k = 0; k < 1191; k++)
	{
		if (k < 10)
			fileName = "000" + to_string(k) + ".png";
		if (k >= 10 && k < 100)
			fileName = "00" + to_string(k) + ".png";
		if (k >= 100 && k < 1000)
			fileName = "0" + to_string(k) + ".png";
		if (k >= 1000 && k < 10000)
			fileName = to_string(k) + ".png";

		char pat[64];
		for (int i = 0; i < 64; i++)
		{
			pat[i] = '0';
		}
		int x, y, n;
		int r, g, b;
		int px = 378, py = 125;
		unsigned char *data = stbi_load(fileName.c_str(), &x, &y, &n, 3);
		for (int j = 0; j < 8; j++)
			for (int i = 0; i < 8; i++)
			{
				r = static_cast<int>(data[(640 * (py + j * 40) + (px + i * 30)) * 3]);
				g = static_cast<int>(data[(640 * (py + j * 40) + (px + i * 30)) * 3 + 1]);
				b = static_cast<int>(data[(640 * (py + j * 40) + (px + i * 30)) * 3 + 2]);
				if (r > 200 || g > 200 || b >> 200)
				{
					pat[j * 8 + i] = '1';
				}
			}
		for (int i = 0; i < 64; i++)
		{
			fout.put(pat[i]);
		}
		fout.put('\n');
		stbi_image_free(data);
	}
	fout.close();
}

void interactionInit()
{
	ifstream fin;
	fin.open("setting.txt");
	if (fin.fail())
	{
		cout << "Setting file input failed. " << endl;
		exit(1);
	}
	string str;
	for (int i = 0; i < 5; i++)
	{
		int a, b;
		float f1, f2, f3;
		getline(fin, str);
		if (i == 0)
		{
			sscanf_s(str.c_str(), "%f,%f,%f %*s", &f1, &f2, &f3);
			scr.cam.pos = Vec(f1, f2, f3);
		}

		if (i == 1)
		{
			sscanf_s(str.c_str(), "%d,%d %*s", &a, &b);
			X = a;
			Y = b;
		}

		if (i == 2)
		{
			sscanf_s(str.c_str(), "%d %*s", &a);
			maxDepth = a;
		}

		if (i == 3)
		{
			sscanf_s(str.c_str(), "%d %*s", &a);
			index = a;
		}

		if (i == 4)
		{
			sscanf_s(str.c_str(), "%d %*s", &a);
			final = a;
		}
	}

	fin.close();
}

int main(int argc, char **argv)
{
	interactionInit();
	index--;
	scr.left = -X / (2);
	scr.right = X / (2);
	scr.up = Y / (2);
	scr.down = -Y / (2);
	scr.cam.nearr = (X + Y) / 4;

	definePattern();
	readPattern();
	objectInit();
	lightInit();

	objg[65].pos = tr.rotate(objg[65].pos, 'z', RAD(index));
	objg[66].pos = tr.rotate(objg[66].pos, 'z', RAD(index));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(X, Y);
	glutCreateWindow("example");

	glutIdleFunc(idle);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
}

