//#pragma once
class Catmull_CHC
{
public:
	#define CAT_POS_DEFAULT 1
	#define CAT_TIME_DEFAULT 0.01f
	Catmull_CHC();
	void setCurPos(int);
	void setAngle(float);
	void setTimeC(float);
	void setPositionIn(float,float);
	void setSizeV(int);

	int getCurPos();
	float getAngle();
	float getTimeC();
	float * getPositionIn();
	int getSizeV();

protected:
	int curPos = CAT_POS_DEFAULT;
	float timeC = CAT_TIME_DEFAULT;
	float angle;
	float x, y;
	int sizeV;
};

