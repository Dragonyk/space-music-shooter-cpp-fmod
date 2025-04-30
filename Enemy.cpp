#include "Enemy.h"

Enemy::Enemy()
{
	life = 1;
	attack = 1;
	type = 0;
	isActive = 1;
}

void Enemy::setLife(int value)
{
	if (value < 0)
		value = 0;
	life = value;
}

int Enemy::getLife()
{
	return life;
}

void Enemy::setAttack(int value)
{
	if (value < 0)
		value = 0;
	attack = value;
}

int Enemy::getAttack()
{
	return attack;
}

void Enemy::setType(int value)
{
	if (value < 0)
		value = 0;
	type = value;
}

int Enemy::getType()
{
	return type;
}
