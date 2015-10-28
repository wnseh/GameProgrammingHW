#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"
#include "SheetSprite.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define FIXED_TIMESTEP 0.016666f
#define MAX_TIMESTEPS 6
#define MAXXPOS 4.0
#define MAXYPOS 3.5
#define MAX_BULLETS 40
#define ENEMIES 35
enum STATE { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

class ClassDemoApp {
public:
	ClassDemoApp();
	~ClassDemoApp();
	void Setup();
	void ProcessInput(float elapsed);
	void shootBullets(float x, float y);
	void EshootBullets(float x, float y);
	bool CheckWall();
	bool hitBottom();
	bool UpdateAndRender();
	void Render();
	int bulletIndex;
	int EbulletIndex;
	bool done;
	bool win;
	float lastFrameTicks;
	SDL_Event event;
	SDL_Window* displayWindow;
	int Enumber;
	float EnemyBullet;
	float PlayerBullet;
	float timeLeftOver;
	int randEnemy;
	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;
	GLuint spaceInvaderSprites;
	GLuint fontSprites;
	SheetSprite playerImg;
	SheetSprite EnemyImg;
	SheetSprite bulletImg;
	SheetSprite bulletImg2;
	float randNum();
	void Update(float elapsed);
	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver();
	bool gameOver();
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTexture(const char *image_path);
	STATE state;
	Entity* player;
	std::vector<Entity*> enemies;
	std::vector<Entity*> bullet;
	std::vector<Entity*> EnemyBullets;
};