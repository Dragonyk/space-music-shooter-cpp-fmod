#ifndef UNIVERSE_H
#define UNIVERSE_H

class Universe
{
public:
	Universe();
	void setColors(int,int,int);
	int * getColors();

	void setRed(int);
	void setGreen(int);
	void setBlue(int);
	int getRed();
	int getGreen();
	int getBlue();

private:
	int red, green, blue;
};
#endif // !UNIVERSE_H
