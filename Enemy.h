#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
public:
	Enemy();
	void setLife(int);
	int getLife();

	void setAttack(int);
	int getAttack();

	void setType(int);
	int getType();

	float x, y, dx, dy, vmax, vmax2;
	int isActive;
private:
	int life, attack, type;
};
#endif // !ENEMY_H