//
// Nome: Carlos Leandro Silva Machado
//

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>

#include <fmod.hpp>
#include <fmod_errors.h>

#include <time.h>
#include <cmath>

#include "Vector2D.h"
#include "Catmull_CHC.h"
#include "tgaload.h"

#include "Universe.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Particle.h"
#include "SFX_Explosion.h"

// Definicoes
#define STATE_PLAYING 0
#define STATE_GAMEOVER 1
#define STATE_GAMEWIN 2

#define MAX_ACC_PLAYER  0.15
#define MAX_LIFE 10

#define RAD2DEG 180.0/M_PI
#define DEG2RAD M_PI/180.0

using namespace math;
using namespace std;
using namespace FMOD;

int option = 0;
int game_state = STATE_PLAYING;

int winWidth = 720;
int winHeight = 720;
GLint contP = 0;

//############# ATRIBUTOS DE  CONTROLE #######################
#define SPECLEN 64
#define WAVELEN 5

#define MAX_ENEMIES 10
#define MAX_ASTEROIDS 10
#define MAX_EXPLOSIONS MAX_ASTEROIDS+MAX_ENEMIES

#define MAX_PROJECTILES 30
#define MAX_VELO_BULLET 0.05

#define MAX_PARTICLES 100

float bullet_speed = 0.05;

float mouseX = 0;
float mouseY = 0;

int up_k    = 0;
int down_k  = 0;
int left_k  = 0;
int right_k = 0;

float playerx = 0;
float playery = 0;

int score = 0;
float gameFrc = 1;
//char * music_name = "musica_doidasadasdas.mp3";
char * music_name = "Highschool_of_the_Dead_Full.mp3";

//Carrega dados
Universe universe = Universe();
Player player;
Enemy * enemies = new Enemy[MAX_ENEMIES];
Asteroid * asteroids = new Asteroid[MAX_ASTEROIDS];
Projectile * bullets = new Projectile[MAX_PROJECTILES];
SFX_Explosion * explosion = new SFX_Explosion[MAX_EXPLOSIONS];

int shoot = 0;

float theta;
int pCirc;
bool isGo = true;

float velocidade = 1.0f;

Vector2D vec1[WAVELEN + 4];
Vector2D vec2[WAVELEN + 4];
Vector2D vec3[WAVELEN + 4];
Vector2D vec4[WAVELEN + 4];

//========================================================
float time_cat = 0.01;
int curPos = 1;
Catmull_CHC ctmull[4];
//========================================================

//Para mostrar o FPS
clock_t start, finish;
double duration, FPSframes, FPS;

using namespace FMOD;
//Variaveis para manipulação do som e espectro
System  *fmodsys;
Sound   *fmodsnd;
Channel *fmodchn;
float *specL, *specR, *spec;

bool paused = false;

#ifndef M_PI
#define M_PI 3.14159265359
#endif

void reshape(int w, int h);
void MoveExplosion();
void DrawExplosion(float cx, float cy);
void Explosion(float cx, float cy);

//Metodo para resetar valores iniciais
void reset() {
	contP = 0;
	time_cat = 0.01;
	curPos = 1;
}

// Qtd máxima de texturas a serem usadas no programa
#define MAX_NO_TEXTURES 8
#define QUAD_TEXTURE 0
#define PRC_TEXTURE 1
#define AST_TEXTURE 2
#define ENY_TEXTURE 3
#define BG_TEXTURE 4
#define PRC2_TEXTURE 5
#define GUI_LIFE_TEXTURE 7

// vetor com os números das texturas
GLuint texture_id[MAX_NO_TEXTURES];
void loadTextures() {
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(MAX_NO_TEXTURES, texture_id);
	texture_id[QUAD_TEXTURE] = 1001;
	texture_id[PRC_TEXTURE] = 1002;
	texture_id[AST_TEXTURE] = 1003;
	texture_id[ENY_TEXTURE] = 1004;
	texture_id[BG_TEXTURE] = 1005;
	texture_id[PRC2_TEXTURE] = 1006;
	texture_id[GUI_LIFE_TEXTURE] = 1008;

	glBindTexture(GL_TEXTURE_2D, texture_id[QUAD_TEXTURE]);
	image_t player_txt;
	tgaLoad("ship1.tga", &player_txt, TGA_FREE | TGA_LOW_QUALITY);

	glBindTexture(GL_TEXTURE_2D, texture_id[PRC_TEXTURE]);
	image_t particle_txt;
	tgaLoad("particle_128.tga", &particle_txt, TGA_FREE | TGA_LOW_QUALITY);

	glBindTexture(GL_TEXTURE_2D, texture_id[AST_TEXTURE]);
	image_t arteroid_txt;
	tgaLoad("asteroid-200.tga", &arteroid_txt, TGA_FREE | TGA_LOW_QUALITY);

	glBindTexture(GL_TEXTURE_2D, texture_id[BG_TEXTURE]);
	image_t bg_txt;
	tgaLoad("universe_bg.tga", &bg_txt, TGA_FREE | TGA_LOW_QUALITY);

	glBindTexture(GL_TEXTURE_2D, texture_id[ENY_TEXTURE]);
	image_t enemy_txt;
	tgaLoad("enemy_ship_1.tga", &enemy_txt, TGA_FREE | TGA_LOW_QUALITY);

	glBindTexture(GL_TEXTURE_2D, texture_id[PRC2_TEXTURE]);
	image_t particle2_txt;
	tgaLoad("flare-256.tga", &particle2_txt, TGA_FREE | TGA_LOW_QUALITY);

	glBindTexture(GL_TEXTURE_2D, texture_id[GUI_LIFE_TEXTURE]);
	image_t guilife_txt;
	tgaLoad("gui_hearth.tga", &guilife_txt, TGA_FREE | TGA_LOW_QUALITY);
}
int ur = 255; 
int ug = 255;
int ub = 255;
float clr = 1; 
float clg = 1;
float clb = 1;

void ChoiceColors() {
	int i = rand() % 3 + 1;
	if (i == 1) {
		ur = rand() % 55 + 200; ug = rand() % 50 + 10; ub = rand() % 50 + 10;
	}
	else if (i == 2) {
		ur = rand() % 50 + 10; ug = rand() % 55 + 200; ub = rand() % 50 + 10;
	}
	else if (i == 3) {
		ur = rand() % 50 + 10; ug = rand() % 50 + 10; ub = rand() % 55 + 255;
	}
}

int r = universe.getRed();
int g = universe.getGreen();
int b = universe.getBlue();

void ChangeColors() {
	if (ur > r)		 r += 1;
	else if (ur < r) r -= 1;
	if (ug > g)		 g += 1;
	else if (ur < g) g -= 1;
	if (ub > b)		 b += 1;
	else if (ur < b) b -= 1;
	
	clr = float(r)/255;
	clg = float(g)/255;
	clb = float(b)/255;
	universe.setColors(r,g,b);

}

void DrawImage(int texture, float angle, float cx, float cy, float size, float r, float g, float b, float a) {
	size /= 5;
	glPushMatrix();
	glTranslatef(cx, cy, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glColor4f(r, g, b, a);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, 1.0f);

	glEnd();
	glPopMatrix();
}

float alpha_inv = 1;
void DrawPlayer() {
	float angX = mouseX - player.x;
	float angY = mouseY - player.y;

	float angle = atan2f(angY, angX)*(180 / PI);
	player.angle = angle;

	DrawImage(texture_id[0] , angle, player.x, player.y, 6.0,1,1,1, alpha_inv);
	//DrawTriangle(angle, player.x, player.y, 4.0);
}

void PlayerSpeed(int value = 1) {
	if (value > 0) {
		if (player.speed <= player.vmax)
			player.speed = player.speed * player.acel;
	}
	else
		player.speed = 0.1;
}

float glide = 0.3;
void MovePlayer() {
	if (game_state == STATE_PLAYING) {
		if (up_k == 1 && down_k == 1) {}
		else if (up_k == 1) {
			player.dy = player.dy + MAX_ACC_PLAYER;
			player.dirY = DIR_UP;
		}
		else if (down_k == 1) {
			player.dy = player.dy - MAX_ACC_PLAYER;
			player.dirY = DIR_DOWN;
		}
		else if (up_k == 0 && down_k == 0) {

			if (player.dirY == DIR_UP) {
				if (player.dy > 0) {
					player.dy -= glide;
				}
				if (player.dy < 0) {
					player.dy = 0;
					player.dirY = 0;
				}
			}
			else if (player.dirY == DIR_DOWN) {
				if (player.dy < 0) {
					player.dy += glide;
				}
				if (player.dy > 0) {
					player.dy = 0;
					player.dirY = 0;
				}
			}
		}

		if (left_k == 1 && right_k == 1) {}
		else if (left_k == 1) {
			player.dx = player.dx - MAX_ACC_PLAYER;
			player.dirX = DIR_LEFT;
		}
		else if (right_k == 1) {
			player.dx = player.dx + MAX_ACC_PLAYER;
			player.dirX = DIR_RIGHT;
		}
		else if (left_k == 0 && right_k == 0) {
			if (player.dirX == DIR_RIGHT) {
				if (player.dx > 0) {
					player.dx -= glide;
				}
				else if (player.dx < 0) {
					player.dx = 0.0;
					player.dirX = 0;
				}
			}
			else if (player.dirX == DIR_LEFT) {
				if (player.dx < 0) {
					player.dx += glide;
				}
				else if (player.dx > 0) {
					player.dx = 0.0;
					player.dirX = 0;
				}
			}
		}

		double temp;
		if (down_k == 1 && (temp = (player.dx * player.dx + player.dy * player.dy)) >(player.vmax)) {
			temp = player.vmax / sqrt(temp);
			player.dx *= temp - 0.1;
			player.dy *= temp - 0.1;
		}
		else if ((temp = (player.dx * player.dx + player.dy * player.dy)) > player.vmax2) {
			temp = player.vmax / sqrt(temp);
			player.dx *= temp;
			player.dy *= temp;
		}
		player.y = player.y + player.dy;
		player.x = player.x + player.dx;
	}
}

float inv_cur_time = 0;
float inv_time = 30;
void PlayerIsAttacked() {
	player.setLife(player.getLife()-1);
	player.x = 0;
	player.y = 0;
	player.invulnerable = 1;
	alpha_inv = 0.4;
}

void PlayerInvulnCount() {
	if (player.invulnerable == 1) {
		if (inv_cur_time < inv_time) {
			inv_cur_time += 0.1;
		}
		else
			player.invulnerable = 0;
	}
	else {
		inv_cur_time = 0;
		alpha_inv = 1;
	}
}

int angle_proj = 0;
void DrawBullet(Projectile *p) {
	DrawImage(texture_id[PRC_TEXTURE], angle_proj, p->x, p->y, 3, clr, clg, clb,1);
}

void moveBullet() {
	if (game_state == STATE_PLAYING) {
		int i = 0;

		// Projetil recebe velocidade
		if (shoot == 1) {

			for (i = 0; i < MAX_PROJECTILES; i++) {

				if (bullets[i].active == 0) {
					bullets[i].active = 1;
					bullets[i].x = player.x;
					bullets[i].y = player.y;

					bullets[i].dx = bullet_speed * sin((90 - player.angle)*DEG2RAD);
					bullets[i].dy = bullet_speed * cos((90 - player.angle)*DEG2RAD);

					bullets[i].angle = (90 - player.angle)*DEG2RAD;

					//bullets[i].dx = -MAX_VELO_BULLET * sin(180*DEG2RAD);
					//bullets[i].dy = MAX_VELO_BULLET * cos(180*DEG2RAD);
					cout << player.angle << " angulo do player\n";
					cout << bullets[i].dx << ", " << bullets[i].dy << " dx,dy\n";
					break;
				}
			}

			// Reseta tiro
			shoot = 0;
		}

		//Movimenta projetil
		for (i = 0; i < MAX_PROJECTILES; i++) {

			if (bullets[i].active == 1) {
				bullets[i].x = bullets[i].x + bullets[i].dx;
				bullets[i].y = bullets[i].y + bullets[i].dy;
			}

			if (bullets[i].active == 1) {
				for (int k = 0; k < MAX_ASTEROIDS; k++) {
					if (asteroids[k].isActive)
						if (bullets[i].x >= asteroids[k].x - 1 && bullets[i].x <= asteroids[k].x + 1) {
							if (bullets[i].y >= asteroids[k].y - 1 && bullets[i].y <= asteroids[k].y + 1) {
								Explosion(asteroids[k].x, asteroids[k].y);
								score += 100;
								asteroids[k].isActive = 0;
								bullets[i].active = 0;
								break;
							}
						}
				}

				for (int k = 0; k < MAX_ENEMIES; k++) {
					if (enemies[k].isActive)
						if (bullets[i].x >= enemies[k].x - 1 && bullets[i].x <= enemies[k].x + 1) {
							if (bullets[i].y >= enemies[k].y - 1 && bullets[i].y <= enemies[k].y + 1) {
								Explosion(enemies[k].x, enemies[k].y);
								score += 100;
								enemies[k].isActive = 0;
								bullets[i].active = 0;
								break;
							}
						}
				}
			}

			//Projetil colide com parede
			if (bullets[i].active == 1 && (bullets[i].x > 10 || bullets[i].x < -10 || bullets[i].y > 10 || bullets[i].y < -10)) {
				bullets[i].active = 0;
			}
		}
	}
} // end moveBullet()

Particle createParticle(int pType, GLfloat rgb[3], GLfloat alpha, Vector2D center, int life, GLfloat parSize) {
	Particle ptc;
	ptc.setRGB(rgb[0], rgb[1], rgb[2]);
	ptc.setAlpha(alpha);
	ptc.setPosition(center.x, center.y);
	ptc.setMaxLife(life);
	ptc.setType(pType);
	ptc.setSize(parSize);
	return ptc;
}

void DrawParticle(Particle pp) {
	Vector2D center = Vector2D(pp.getPosition()[0], pp.getPosition()[1]);
	float rgb[3] = { pp.getRGB()[0],pp.getRGB()[1],pp.getRGB()[2] };
	float alpha = pp.getAlpha();
	int pType = pp.getType();
	float parSize = pp.getSize();

	if (pType == PARTICLE_CIRCLE)
		DrawImage(texture_id[1], 0, center.x, center.y, parSize, rgb[0], rgb[1], rgb[2],1);
}

float exp_x = 0;
float exp_y = 0;
float exp_speed = 0.05;
int ft_ptc = 10;
#define MAX_EXP_PARTICLES 360/(MAX_PARTICLES/ft_ptc)
void MoveExplosion() {
	if (game_state == STATE_PLAYING) {
		for (int i = 0; i < MAX_EXPLOSIONS; i++) {
			if (explosion[i].isActive) {
				Particle * exp_particle = explosion[i].getParticles();
				int p = 0;
				if (explosion[i].ex_time >= 0) {
					for (int j = 0; j < 360; j += ft_ptc) {
						if (exp_particle[p].isMoving() == true) {
							exp_particle[p].disableMove();
							exp_particle[p].xP = explosion[i].x;
							exp_particle[p].yP = explosion[i].y;

							exp_particle[p].dx = exp_speed * sin(j);
							exp_particle[p].dy = exp_speed * cos(j);
							exp_particle[p].setSize(3);
						}
						if (exp_particle[p].isMoving() == false) {
							explosion[i].ex_time -= 0.01;
							exp_particle[p].setRGB(r, g, b);
							exp_particle[p].setAlpha(explosion[i].ex_time / 100);
							exp_particle[p].xP += exp_particle[p].dx;
							exp_particle[p].yP += exp_particle[p].dy;
						}
						p++;
					}
				}
				else
					explosion[i].isActive = 0;
			}
			else
				continue;
		}
	}
}

void DrawExplosion() {
	for (int i = 0; i < MAX_EXPLOSIONS; i++) {
		if (explosion[i].isActive) {
			Particle * exp_particle = explosion[i].getParticles();
			for (int i = 0; i < MAX_EXP_PARTICLES; i++) {
				DrawImage(texture_id[PRC2_TEXTURE], 0, exp_particle[i].xP, exp_particle[i].yP, 3, clr, clg , clb, exp_particle[i].getAlpha());
				//cout << clr << ", " << clg << "," << clb << "\n";
			}
		}
		else
			continue;
	}
}

void Explosion(float cx, float cy) {
	for (int i = 0; i < MAX_EXPLOSIONS; i++) {
		if (!explosion[i].isActive) {
			explosion[i].x = cx;
			explosion[i].y = cy;
			explosion[i].ex_time = 50;
			Particle * exp_particle = explosion[i].getParticles();
			for (int i = 0; i < MAX_EXP_PARTICLES; i++)
				exp_particle[i].enableMove();
			explosion[i].isActive = 1;
			break;
		}
	}
}

void DrawSpiral(GLfloat startX, GLfloat startY, GLfloat rad, GLfloat a, GLint points) {
	rad /= 10;
	points /= a;
	GLfloat x;
	GLfloat y;

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	float rgb[3] = { 1, 1, 0 };
	float alpha = 1.0f;
	if (pCirc <= (points / 5)) {
		alpha = float(pCirc) / (float(points) / 5);
	}

	for (int i = 1; i <= MAX_PARTICLES; i++) {
		theta = (0.1*(a*(float(i) / 10))) * float(pCirc);

		int green = 255;
		int red = 255;
		int blue = 255;

		rgb[0] -= 0.01;
		rgb[1] = ((float(pCirc) * 255) / float(points)) / 255;
		rgb[2] += 0.01;


		x = (((rad * 2)*theta)* cosf(theta)) + startX;
		y = (((rad * 2)*theta)* sinf(theta)) + startY;
		DrawParticle(createParticle(PARTICLE_CIRCLE, rgb, alpha, Vector2D(x, y), 10, 3.0f));
	}

	glPopMatrix();

	if (pCirc == 1) isGo = true;
	if (pCirc >= points) isGo = false;

	if (isGo == true) pCirc++;
	else pCirc--;
}

int ppos = 0;
void VarPositions() {
	if (ppos == 1) {
		vec1[WAVELEN + 2] = Vector2D(7, -11);
		vec1[WAVELEN + 3] = Vector2D(7, -12);
		vec2[WAVELEN + 2] = Vector2D(-7, 11);
		vec2[WAVELEN + 3] = Vector2D(-7, 12);
		ppos = 0;
	}
	else {
		vec1[WAVELEN + 2] = Vector2D(-7, -11);
		vec1[WAVELEN + 3] = Vector2D(-7, -12);
		vec2[WAVELEN + 2] = Vector2D(7, 11);
		vec2[WAVELEN + 3] = Vector2D(7, 12);
		ppos = 1;
	}
}

void RespawnAsteroids() {
	for (int i = 0; i < MAX_ASTEROIDS; i++) {
		if (asteroids[i].isActive == 0) {
			asteroids[i].x = 0;
			asteroids[i].y = 9;
			asteroids[i].isEnable = 0;
			asteroids[i].isActive = 1;
		}
	}
}

void RespawnEnemies() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].isActive == 0) {
			enemies[i].isActive = 1;
		}
	}
}

float astS;
void DarkHoleCrazy() {
	if (game_state == STATE_PLAYING) {
		float maxL = specL[0];
		for (int i = 1; i < WAVELEN; i++) {
			if (specL[i] > maxL)
				maxL = specL[i];
		}
		maxL = fabs(maxL) * 10;
		gameFrc = maxL;
		if (maxL >= 7) {
			ChoiceColors();
			RespawnAsteroids();
			RespawnEnemies();
		}

		astS = maxL;
		if (astS < 0.1)
			astS = 0.1;
		if (astS > 1.5)
			astS = 1.5;

		float powerrr = fabs(specR[WAVELEN / 2]);
		float proj_fact = 25;

		if (maxL < 0.2 && maxL > 0) {
			maxL = 0.2;
		}
		if (maxL > 0.5) {
			maxL = (maxL + 1.5) - 0.5;
		}
		if (maxL <= 0) {
			proj_fact = 1;
			maxL = 0.01;
		}

		for (int i = 0; i < MAX_PROJECTILES; i++) {
			bullets[i].dx = (maxL / proj_fact) * sin(bullets[i].angle);
			bullets[i].dy = (maxL / proj_fact) * cos(bullets[i].angle);
		}

		DrawSpiral(player.x, player.y, 0.02, maxL, 500);
	}
}

void DrawEnemy(float cx, float cy) {
	float angX = player.x - cx;
	float angY = player.y - cy;

	float angle = atan2f(angY, angX)*(180 / PI);

	DrawImage(texture_id[ENY_TEXTURE], angle, cx, cy, 4.0, 1, 1, 1,1);
}

void loadSystem() {
	player.setLife(MAX_LIFE);

	for (int i = 0; i < MAX_EXPLOSIONS; i++)
		explosion[i] = SFX_Explosion(MAX_EXP_PARTICLES);

	for (int i = 0; i < MAX_ASTEROIDS; i++) {
		//float(rand() % 201 - 100) / 10;
		asteroids[i].x = 0;
		asteroids[i].y = 8;
		asteroids[i].vx = float(rand() % 10 + 1) / 100;
		//asteroids[i].vy = float(rand() % 10 + 1) / 100;
		asteroids[i].vy = float(i + 1) / 100;

		cout << asteroids[i].vx << "<---- vx\n";
		cout << asteroids[i].vy << "<---- vy\n";
	}
}

void MoveAsteroids() {
	if (game_state == STATE_PLAYING) {
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i].isActive) {
				if (asteroids[i].isEnable == 0) {
					asteroids[i].dx = asteroids[i].vx;
					asteroids[i].dy = asteroids[i].vy;
					asteroids[i].isEnable = 1;
				}

				if (asteroids[i].x >= 10)
					asteroids[i].dx = -asteroids[i].vx*astS;
				if (asteroids[i].x <= -10)
					asteroids[i].dx = asteroids[i].vx*astS;
				if (asteroids[i].y >= 10)
					asteroids[i].dy = -asteroids[i].vy*astS;
				if (asteroids[i].y <= -10)
					asteroids[i].dy = asteroids[i].vy*astS;

				if (asteroids[i].x >= player.x - 1 && asteroids[i].x <= player.x + 1) {
					if (asteroids[i].y >= player.y - 1 && asteroids[i].y <= player.y + 1) {
						if (player.invulnerable == 0) {
							PlayerIsAttacked();
						}
					}
				}

				asteroids[i].x += asteroids[i].dx;
				asteroids[i].y += asteroids[i].dy;
			}
		}
		//cout << asteroids[0].x << "<---- Dx\n";
	}
}

void SpawnAsteroids() {
	for (int i = 0; i < MAX_ASTEROIDS; i++) {
		if(asteroids[i].isActive)
			DrawImage(texture_id[AST_TEXTURE], 0 , asteroids[i].x, asteroids[i].y, 5, 1,1,1,1);
	}
}

void MoveEnemies() {
	if (game_state == STATE_PLAYING) {
		int others = MAX_ENEMIES / 2;
		enemies[0].x = ctmull[0].getPositionIn()[0];
		enemies[0].y = ctmull[0].getPositionIn()[1];

		enemies[others].x = ctmull[1].getPositionIn()[0];
		enemies[others].y = ctmull[1].getPositionIn()[1];

		for (int i = 1; i < MAX_ENEMIES / 2; i++) {
			enemies[i].x = enemies[i - 1].x;
			enemies[i].y = enemies[i - 1].y + 2;
			enemies[others + i].x = enemies[(others + i) - 1].x;
			enemies[others + i].y = enemies[(others + i) - 1].y + 2;
		}

		vec1[WAVELEN + 2] = Vector2D(0, player.y);
		vec1[WAVELEN + 3] = Vector2D(player.x, player.y - 1);

		vec2[WAVELEN + 2] = Vector2D(0, player.y);
		vec2[WAVELEN + 3] = Vector2D(player.x, player.y - 1);
	}
}

void SpawnEnemies() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].isActive) {
			DrawEnemy(enemies[i].x, enemies[i].y);

			if (enemies[i].x >= player.x - 1 && enemies[i].x <= player.x + 1) {
				if (enemies[i].y >= player.y - 1 && enemies[i].y <= player.y + 1) {
					if (player.invulnerable == 0) {
						PlayerIsAttacked();
					}
				}
			}

		}
	}
}

void DrawBG() {
	glColor4f(float(universe.getRed())/255, float(universe.getGreen())/255, float(universe.getBlue())/255, 1);
	glBindTexture(GL_TEXTURE_2D, texture_id[BG_TEXTURE]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10, 10, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10, 10, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10, -10, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10, -10, 1.0f);
	glEnd();
}

void CheckBorder() {
	if (player.x > 10) {
		player.x = 10;
	}
	else if (player.x < -10) {
		player.x = -10;
	}

	if(player.y > 10){
		player.y = 10;
	}
	else if(player.y < -10){
		player.y = -10;
	}
}

void ChangePointCurves() {
	float fact = 1;
	float sg = 1;

	float newSpeed = 0;
	for (int i = 2; i < (WAVELEN + 2); i++) {
		if (i % 2 == 0)sg = 1;
		else sg = -1;
		vec1[i].x = -7 + sg*(specL[i - 2] * fact);
		vec2[i].x = 7 + sg*(specR[i - 2] * fact);
	}

	newSpeed = fabs(specR[0] * 10);
	if (newSpeed>1)
		newSpeed = float(rand() % 21 + 10) / 10;
	else if (newSpeed < 0.5)
		newSpeed = float(rand() % 11 + 2) / 20;
	//cout << newSpeed << "\n";
	velocidade = newSpeed;
}

Vector2D Catmull(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, GLfloat t) {

	float t_d = 0.5;
	float t_c = (t / 2);
	float t_b = (t*t) / 2;
	float t_a = (t*t*t) / 2;

	GLfloat h1 = (-t_a + 2 * t_b - t_c);
	GLfloat h2 = (3 * t_a - 5 * t_b + 2 * t_d);
	GLfloat h3 = (-3 * t_a + 4 * t_b + t_c);
	GLfloat h4 = (t_a - t_b);

	GLfloat x = h1*p0.x + h2*p1.x + h3*p2.x + h4*p3.x;
	GLfloat y = h1*p0.y + h2*p1.y + h3*p2.y + h4*p3.y;
	Vector2D p = Vector2D(x, y);

	return p;
}

int testPosCatmull(int pos, int sizeVec) {
	if (pos < 0) {
		return sizeVec - 1;
	}
	if (pos>sizeVec) {
		return 1;
	}
	if (pos>sizeVec - 1) {
		return 0;
	}
	return pos;
}

//============== CURVA CATMULL ==================
void DrawCatmull(Vector2D vec[], int sizeVec, float a, int numMull, int showLine = 0) {
	a = 0.01*a;
	Vector2D p0;
	Vector2D p1;
	Vector2D p2;
	Vector2D p3;

	float r = 1;
	float g = 0;
	float b = 0;
	Vector2D p, pt2;
	float time_temp = 0;
	for (int pt = 1; pt <= ctmull[numMull].getCurPos(); pt++) {
		if ((pt == 0 || pt == sizeVec - 2 || pt == sizeVec - 1)) {
			continue;
		}
		if (pt <= ctmull[numMull].getCurPos() - 1)
			time_temp = 1;
		else
			time_temp = ctmull[numMull].getTimeC();
		p0 = vec[testPosCatmull(pt - 1, sizeVec)];
		p1 = vec[testPosCatmull(pt, sizeVec)];
		p2 = vec[testPosCatmull(pt + 1, sizeVec)];
		p3 = vec[testPosCatmull(pt + 2, sizeVec)];

		for (float t = 0; t <= time_temp; t += a) {
			p = Catmull(p0, p1, p2, p3, t);
			pt2 = Catmull(p0, p1, p2, p3, t + 0.01);
		}
	}
	if (showLine == 1) {
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int pt = 1; pt < sizeVec - 2; pt++) {
			if ((pt == 0 || pt == sizeVec - 2 || pt == sizeVec - 1)) {
				continue;
			}
			p0 = vec[testPosCatmull(pt - 1, sizeVec)];
			p1 = vec[testPosCatmull(pt, sizeVec)];
			p2 = vec[testPosCatmull(pt + 1, sizeVec)];
			p3 = vec[testPosCatmull(pt + 2, sizeVec)];
			for (float t = 0; t < 1; t += 0.1) {
				Vector2D vp = Catmull(p0, p1, p2, p3, t);
				glVertex2f(vp.x, vp.y);
			}
		}
		glEnd();
	}
	Vector2D att;
	if (ctmull[numMull].getCurPos()<sizeVec - 1) {
		att = Vector2D(pt2.x, pt2.y) - Vector2D(p.x, p.y);
	}
	else {
		att = Vector2D(p.x, p.y);
	}
	float angle = atan2f(att.y, att.x)*(180 / PI);

	ctmull[numMull].setAngle(angle);
	ctmull[numMull].setPositionIn(p.x, p.y);
	if (ctmull[numMull].getCurPos() < sizeVec - 2 && ctmull[numMull].getTimeC() >= 1) {
		ctmull[numMull].setCurPos(ctmull[numMull].getCurPos() + 1);
		ctmull[numMull].setTimeC(CAT_TIME_DEFAULT);
	}
	if (ctmull[numMull].getCurPos() >= sizeVec - 2) {
		ctmull[numMull].setCurPos(CAT_POS_DEFAULT);
		ctmull[numMull].setTimeC(CAT_TIME_DEFAULT);

		if(numMull == 0)
			std::reverse(std::begin(vec1), std::end(vec1));
		else
			std::reverse(std::begin(vec2), std::end(vec2));
	}
	if (ctmull[numMull].getTimeC() < 1) {
		ctmull[numMull].setTimeC(ctmull[numMull].getTimeC() + a);
	}
}

//Desenha texto
void DrawTxt(char *text, int txt_size, int r, int g, int b, Vector2D pos) {
	glColor3ub(r, g, b);
	glRasterPos2f(pos.x, pos.y);
	for (int ii = 0; ii < txt_size; ii++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *(text++));
	}
}

//Para pegar a onda (amplitudes ao longo do tempo)
void GetWave() {
	bool playing;
	fmodchn->getPaused(&playing);

	if (!playing)
	{
		//Pega a onda de som (som stereo,  para o "ouvido" direito e para o esquerdo)
		fmodchn->getWaveData(specL, WAVELEN, 0);
		fmodchn->getWaveData(specR, WAVELEN, 1);
	}
	else {
	};
}

void LoadVectors() {
	float absh = 20 / float(WAVELEN);

	vec1[0] = Vector2D(-7, 12);
	vec1[1] = Vector2D(-7, 11);
	vec1[WAVELEN + 2] = Vector2D(-7, -11);
	vec1[WAVELEN + 3] = Vector2D(-7, -12);

	vec2[0] = Vector2D(7, -12);
	vec2[1] = Vector2D(7, -11);
	vec2[WAVELEN + 2] = Vector2D(7, 11);
	vec2[WAVELEN + 3] = Vector2D(7, 12);

	float vec1PosY = vec1[1].y - absh;
	float vec2PosY = vec2[1].y + absh;
	for (int i = 2; i < (WAVELEN + 2); i++) {
		vec1[i] = Vector2D(-7, vec1PosY);
		vec1PosY -= absh;
		vec2[i] = Vector2D(7, vec2PosY);
		vec2PosY += absh;
	}
}

//Desenha o Inimigo na posição de cada curva
void MoveCurves() {
	DrawCatmull(vec1, WAVELEN + 4, velocidade, 0);
	DrawCatmull(vec2, WAVELEN + 4, velocidade, 1);
}
void initFMOD(char * path);
void ResetGame() {
	duration = 0;
	start = 0;
	finish = 0;
	score = 0;
	player.setLife(MAX_LIFE);
	RespawnAsteroids();
	RespawnEnemies();
	game_state = STATE_PLAYING;

	initFMOD(music_name);
	fmodchn->setPaused(false);
	start = clock();
	glutMainLoop();
	fmodsnd->release();
	fmodsys->close();
	fmodsys->release();
	free(specR);
	free(specL);
}

void GameUpdate() {
		unsigned int time_endd;
		fmodsnd->getLength(&time_endd, FMOD_TIMEUNIT_MS);

		finish = clock();
		duration += (double)(finish - start) / CLOCKS_PER_SEC;
		start = clock();

	if (player.getLife() <= 0)
		game_state = STATE_GAMEOVER;
	else if (duration >= (time_endd / 1000)+1)
		game_state = STATE_GAMEWIN;
}

void DrawGUI() {
	float pos_x = -9;
	for (int i = 0; i < player.getLife(); i++) {
		DrawImage(texture_id[GUI_LIFE_TEXTURE], 180, pos_x, 9, 3, 0,1,0,1);
		pos_x += 1.5;
	}
	char score_txt[200];
	sprintf_s(score_txt, 200, "Score: %i", score);
	char music_txt[200];
	sprintf_s(music_txt, 200, "%s", music_name);

	glDisable(GL_TEXTURE_2D);
	DrawTxt(score_txt, 15, 255, 255, 255, Vector2D(6, 9));
	DrawTxt(music_txt, 200, 0, 255, 0, Vector2D(-9.5, 7.5));

	if (game_state != STATE_PLAYING) {
		DrawTxt("Aperte 1 para recomecar", 24, 255, 0, 0, Vector2D(-3, 0));
	}
	glEnable(GL_TEXTURE_2D);
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	GetWave();
	DrawBG();
	
	DarkHoleCrazy();
	DrawPlayer();
	SpawnAsteroids();
	SpawnEnemies();
	DrawExplosion();

	// Desenha os projeteis na tela
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (bullets[i].active == 1) {
			DrawBullet(&bullets[i]);
		}
	}
	DrawGUI();
	glutSwapBuffers();
}

int fscreen = 1;
void Fullscreen() {
	if (fscreen == 1) {
		glutFullScreen();
		fscreen = 0;
	}
	else {
		glutReshapeFunc(reshape);
		glutPositionWindow(0,0);
		fscreen = 1;
	}
}

/* init FMOD with gived mp3 file */
void initFMOD(char * path)
{
	System_Create(&fmodsys);
	fmodsys->init(1, FMOD_INIT_NORMAL, 0);
	cout << "\nfile: " << path << endl;
	fmodsys->createSound(path,
		FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM,
		0, &fmodsnd);
	fmodsys->playSound(FMOD_CHANNEL_FREE, fmodsnd, true, &fmodchn);
	specL = (float*)malloc(SPECLEN*sizeof(float));
	specR = (float*)malloc(SPECLEN*sizeof(float));
	unsigned int len;
	fmodchn->setVolume(1);
	fmodsnd->getLength(&len, FMOD_TIMEUNIT_MS);
	fmodchn->setPosition((int)(len*0.0), FMOD_TIMEUNIT_MS);
};

//Eventos do Teclado
void keyPress(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		up_k    = 1;
		break;
	case 'a':
		left_k  = 1;
		break;
	case 's':
		down_k  = 1;
		break;
	case 'd':
		right_k = 1;
		break;
	case '1':
		if(game_state != STATE_PLAYING)
			ResetGame();
		break;
	case 27:
		if (fscreen == 0)
			Fullscreen();
		else
			exit(0);
		break;
	}
}

//Eventos do Teclado
void keyRelease(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		up_k = 0;
		break;
	case 'a':
		left_k = 0;
		break;
	case 's':
		down_k = 0;
		break;
	case 'd':
		right_k = 0;
		break;
	case 'p':
		Fullscreen();
		break;
	}
}

void MouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		shoot = 1;
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		shoot = 0;
}

void MotionCallback(int x, int y) {
	float xx = float(x) / float(winWidth);
	xx = -10 + (xx * 20);
	float yy = ((float(winHeight) - float(y)) / float(winHeight));
	yy = -10 + (yy * 20);

	mouseX = xx;
	mouseY = yy;
}

float fact = 0.1;
float tcount = 0;
//Timer para animação
void Timer(int iUnused) {
	angle_proj+=5;
	if (angle_proj >= 360) {
		angle_proj = 0;
	}
	
	PlayerInvulnCount();
	MoveCurves();
	MovePlayer();
	moveBullet();
	MoveAsteroids();
	MoveEnemies();
	ChangeColors();
	CheckBorder();
	MoveExplosion();
	GameUpdate();

	if (tcount >= 1) {
		ChangePointCurves();
		tcount = 0;
	}

	tcount += 0.8;
	glutPostRedisplay();
	glutTimerFunc(30, Timer, 0);
}

void reshape(int w, int h) {
	if (h == 0)
		h = 1;
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Initialize() {
	player.dx = 0;
	player.dy = 0;
	player.vmax = 0.5;
	player.vmax2 = 0.25;
	player.dirY = 0;
	player.dirX = 0;

	LoadVectors();
	loadSystem();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int iArgc, char** cppArgv) {
	srand(time(NULL));
	glutInit(&iArgc, cppArgv);
	GLint center_x = (glutGet(GLUT_SCREEN_WIDTH) - winWidth) / 2;
	GLint center_y = (glutGet(GLUT_SCREEN_HEIGHT) - winHeight) / 2;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(center_x, center_y);
	glutCreateWindow("Music War");
	loadTextures();
	Initialize();
	glutReshapeFunc(reshape);
	glutDisplayFunc(Draw);
	glutMouseFunc(MouseCallback);
	glutMotionFunc(MotionCallback);
	glutPassiveMotionFunc(MotionCallback);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	Timer(0);

	cout << "Initialize sounds :";
	//initFMOD("ola.mp3");
	//initFMOD("Highschool_of_the_Dead_Full.mp3");
	//initFMOD("pirate.mp3");
	initFMOD(music_name);
	//initFMOD( "09_Dense_of_the_sugar.mp3" );
	fmodchn->setPaused(false);

	start = clock();
	glutMainLoop();

	fmodsnd->release();
	fmodsys->close();
	fmodsys->release();
	free(specR);
	free(specL);
	return 0;
}