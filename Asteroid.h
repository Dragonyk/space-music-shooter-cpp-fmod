#pragma once
class Asteroid
{
public:
	Asteroid();
	Asteroid(float,float);
	void setSize(float);
	float getSize();

	float x, y, dx, dy, vx, vy;
	int isEnable, isActive;
private:
	float size, vmax, vmax2;
};

