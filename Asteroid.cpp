#include "Asteroid.h"



Asteroid::Asteroid()
{
	size  = 1;
	vmax  = 1;
	vmax2 = 1; 
	isEnable = 0;
	isActive = 1;
}

Asteroid::Asteroid(float s, float mx)
{
	if (s < 0.5)
		s = 0.5;
	if (mx <= 0)
		mx = 0.1;

	size  = s;
	vmax  = mx;
	vmax2 = mx*mx;
	isEnable = 0;
	isActive = 1;
}

void Asteroid::setSize(float value)
{
	if (value < 0.5)
		value = 0.5;
	size = value;
}

float Asteroid::getSize()
{
	return size;
}
