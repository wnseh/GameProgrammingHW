#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
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
#define ENEMIES 15
#define MAXXPOS 2.0
#define MAXYPOS 4.0
#define MAX_BULLETS 40
#define LEVEL_HEIGHT 20
#define LEVEL_WIDTH 60
#define TILE_SIZE 0.3f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 16
enum STATE { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER, LEVEL_1, LEVEL_2, LEVEL_3 };

class ClassDemoApp {
public:
	ClassDemoApp();
	~ClassDemoApp();
	void Setup();
	void ProcessInput(float elapsed);

//	bool CheckWall();
//	bool hitBottom();
	bool UpdateAndRender();
	void Render();

	int AniNum;
	int playerlife;
	int jumpcounter;
	float animationtime;
	float playerAniTime;
	int mapWidth;
	int mapHeight;
	bool done;
	bool win;
	float lastFrameTicks;
	SDL_Event event;
	SDL_Window* displayWindow;

	float timeLeftOver;

	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;

	//Bullet Stuff
	float EnemyBullet;
	float PlayerBullet;
	int bulletIndex;
	int EbulletIndex;
	void shootBullets(float x, float y, float direction);
	void EshootBullets(float x, float y, float targetX, float targetY);
	std::vector<Entity*> EnemyBullets;
	std::vector<Entity*> bullet;

	//Enemy Stuff
	int randEnemy;
	int Enumber;
	std::vector<Entity*> enemies;

	//Sprite Sheet
	GLuint TileSprites;
	GLuint CharSprites;
	GLuint EnemySprites;
	GLuint fontSprites;
	GLuint BulletSprites;
	GLuint PlayerSprites;
	GLuint GunSprite;
	SheetSprite playerImg;
	SheetSprite player1;
	SheetSprite player2;
	SheetSprite player3;
	SheetSprite player4;
	SheetSprite player5;
	SheetSprite player6;
	SheetSprite player7;
	SheetSprite player8;
	SheetSprite playera;
	SheetSprite playerb;
	SheetSprite playerc;
	SheetSprite playerd;
	SheetSprite playere;
	SheetSprite playerf;
	SheetSprite playerg;
	SheetSprite playerh;
	SheetSprite enemyIdle1;
	SheetSprite enemyIdle2;
	SheetSprite enemyAngry1;
	SheetSprite enemyAngry2;
	SheetSprite enemyDead;
	SheetSprite bulletImg;
	SheetSprite bulletImg2;
	SheetSprite gunImg1;
	SheetSprite gunImg2;
	SheetSprite gunImg12;
	SheetSprite gunImg22;

	float randNum();
	void loadStuff();
	void Update(float elapsed);
	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver();
	bool gameOver();
	bool readHeader(ifstream &stream);
	bool readLayerData(ifstream &stream);
	bool readEntityData(ifstream &stream);
	bool topReached();
	bool collided();
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	void getTxtData(string file);
	void placeEntity(string type, float x, float y);
	void worldToTileCoord(float worldX, float worldY, int *gridX, int *gridY);
	GLuint LoadTexture(const char *image_path);
	unsigned char ** levelData;
	STATE state;
	STATE gameLevel;
	Entity* player;
	Entity* gun;
	std::vector<Entity*> entities;

	// Sounds
	Mix_Chunk *jump;
	Mix_Chunk *shoot;
	Mix_Chunk *hit;
	Mix_Music *music1;
	Mix_Music *music23;
};