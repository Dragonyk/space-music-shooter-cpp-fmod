#include "Universe.h"



Universe::Universe()
{
	red = 255;
	green = 255;
	blue = 255;
}

int checkColor(int value) {
	if (value < 0)
		value = 0;
	if (value > 255)
		value = 255;
	return value;
}

void Universe::setColors(int r, int g, int b)
{
	red = checkColor(r);
	green = checkColor(g);
	blue = checkColor(b);
}

int * Universe::getColors()
{
	int * temp = new int[3];
	temp[0] = red;
	temp[1] = green;
	temp[2] = blue;
	return temp;
}

void Universe::setRed(int value)
{
	red = checkColor(value);
}

void Universe::setGreen(int value)
{
	green = checkColor(value);
}

void Universe::setBlue(int value)
{
	blue = checkColor(value);
}

int Universe::getRed()
{
	return red;
}

int Universe::getGreen()
{
	return green;
}

int Universe::getBlue()
{
	return blue;
}