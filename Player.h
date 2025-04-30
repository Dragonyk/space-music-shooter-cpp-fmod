#pragma once

#ifndef PLAYER_H
#define PLAYER_H
#define DIR_UP 1
#define DIR_DOWN 2 
#define DIR_RIGHT 1 
#define DIR_LEFT 2

class Player
{
public:
	Player();
	void setLife(int);
	void setPower(int);
	int getLife();
	int getPower();
	bool isAlive();

	float x, y, angle, dx, dy, vmax, vmax2, radius;
	float acel, speed, velX, velY;

	int dirY, dirX, invulnerable;
private:
	const int maxLife = 200;
	const int maxPower = 100;
	int life;
	int power;
};
#endif // !PLAYER_H
