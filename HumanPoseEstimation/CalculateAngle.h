#ifndef _CalculateAngle_H
#define _CalculateAngle_H

#include "Data.h"
#include <iostream>

using namespace std;

//能表示空间中一个点 也能表示一个三维向量
class  MyPoint
{
public:
	MyPoint(){};
	MyPoint(float a, float b, float c);
	~MyPoint(){};

	float x;
	float y;
	float z;
};

typedef struct Surface
{
	float a;
	float b;
	float c;
	float d;
};

class CalculateAngle
{
public:

	void getPlane(Data a, Data b, Data c, Surface &sur);
	float getTwoNormalAngle(MyPoint normal1, MyPoint normal2);
	MyPoint getNormal(MyPoint p1, MyPoint p2, MyPoint p3);
	float getCrookedShoulderAngle(MyPoint lShlder, MyPoint rShlder);
	float getTurnRoundAngle(MyPoint lShlder, MyPoint rShlder);
	void printPoint(string pointname, MyPoint point);
	void SphericalCoordinate(MyPoint origin, MyPoint point);

	float radius;
	float theta;
	float phi;

	CalculateAngle(){};
	~CalculateAngle(){};

};

#endif