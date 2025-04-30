#include "Particle.h"

Particle::Particle(){}

void Particle::setPosition(float x, float y)
{
	xP = x;
	yP = y;
}

void Particle::setRGB(float rr, float gg, float bb)
{
	r = rr;
	g = gg;
	b = bb;
}

void Particle::setAlpha(float aa)
{
	a = aa;
}

void Particle::setMaxLife(int lifeMax)
{
	life = lifeMax;
}

void Particle::setType(int t) 
{
	type = t;
}

void Particle::setSize(float ss)
{
	size = ss;
}

void Particle::enableMove()
{
	isMove = true;
}

void Particle::disableMove()
{
	isMove = false;
}

float * Particle::getPosition()
{
	float vec[2] = { xP,yP };
	float *arrayTemp = vec;
	return arrayTemp;
}

float * Particle::getRGB()
{
	float vec[3] = { r,g,b };
	float *arrayTemp = vec;
	return arrayTemp;
}

float Particle::getAlpha()
{
	return a;
}

int Particle::getLife()
{
	return life;
}

int Particle::getType()
{
	return type;
}

float Particle::getSize()
{
	return size;
}

bool Particle::isMoving()
{
	return isMove;
}
