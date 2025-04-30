#include "SFX_Explosion.h"



SFX_Explosion::SFX_Explosion(int value)
{
	qtdParticles = value;
	particles = new Particle[qtdParticles];
}

SFX_Explosion::SFX_Explosion() {
	qtdParticles = 36;
	particles = new Particle[qtdParticles];
}

void SFX_Explosion::setParticle(Particle p, int pos)
{
	particles[pos] = p;
}

Particle * SFX_Explosion::getParticles()
{
	return particles;
}
