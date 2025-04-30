#ifndef SFX_EXPLOSION_H
#define SFX_EXPLOSION_H

#include "Particle.h"

class SFX_Explosion
{
public:
	SFX_Explosion(int);
	SFX_Explosion();
	void setParticle(Particle, int);
	Particle * getParticles();

	float x, y, ex_time;
	int isActive, qtdParticles;
private:
	Particle * particles;
};
#endif // !SFX_EXPLOSION_H