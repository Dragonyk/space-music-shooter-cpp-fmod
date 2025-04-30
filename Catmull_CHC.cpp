#include "Catmull_CHC.h"
#include "Vector2D.h"
using namespace math;


Catmull_CHC::Catmull_CHC()
{
}

void Catmull_CHC::setCurPos(int pos)
{
	curPos = pos;
}

void Catmull_CHC::setAngle(float ang)
{
	angle = ang;
}

void Catmull_CHC::setTimeC(float t)
{
	timeC = t;
}

void Catmull_CHC::setPositionIn(float xx, float yy)
{
	x = xx;
	y = yy;
}

void Catmull_CHC::setSizeV(int s)
{
	sizeV = s;
}

int Catmull_CHC::getCurPos()
{
	return curPos;
}

float Catmull_CHC::getAngle()
{
	return angle;
}

float Catmull_CHC::getTimeC()
{
	return timeC;
}

float * Catmull_CHC::getPositionIn()
{
	float posIn[2] = {x,y};
	float *temp = posIn;

	return temp;
}

int Catmull_CHC::getSizeV()
{
	return sizeV;
}
