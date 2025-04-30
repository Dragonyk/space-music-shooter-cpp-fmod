#include "Player.h"


Player::Player()
{
	life = 3;
	power = 10;
	invulnerable = 0;
}

void Player::setLife(int value)
{
	if (value <= maxLife) {
		if(value>0)
			life = value;
		else {
			life = 0;
		}
	}
		
}

void Player::setPower(int value)
{
	if (value <= maxPower) {
		if(value>=0)
			power = value;
	}
}

int Player::getLife()
{
	return life;
}

int Player::getPower()
{
	return life;
}

bool Player::isAlive()
{
	return life>0;
}
