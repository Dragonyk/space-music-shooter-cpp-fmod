#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile
{
public:
	Projectile();

	//pos x, pos y
	void setPosition(float,float);

	//float = angle
	void DrawProjectile(float);
	
	int active;
	float x, y, dx, dy, angle;

};
#endif // !PROJECTILE_H
