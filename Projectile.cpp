#include <GL/glut.h>
#include "Projectile.h"

Projectile::Projectile()
{
	active = 0;
}

void Projectile::setPosition(float px, float py)
{
	x = px;
	y = py;
}
