#pragma once

class Particle
{
public:
	Particle();
#define PARTICLE_LINE 1
#define PARTICLE_CIRCLE 2

	void setPosition(float,float);
	void setRGB(float,float,float);
	void setAlpha(float);
	void setMaxLife(int);
	void setType(int);
	void setSize(float);
	void enableMove();
	void disableMove();

	float * getPosition();
	float * getRGB();
	float getAlpha();
	int getLife();
	int getType();
	float getSize();
	bool isMoving();

	float dx, dy;
	float xP, yP;
protected:
	float r, g, b, a;
	int life;
	int type;
	float size;
	bool isMove = true;
};

