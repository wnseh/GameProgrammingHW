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

	int mapWidth;
	int mapHeight;
	bool done;
	bool win;
	float lastFrameTicks;
	SDL_Event event;
	SDL_Window* displayWindow;
	int Enumber;

	float timeLeftOver;
	int randEnemy;
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
	void EshootBullets(float x, float y, float direction);
	std::vector<Entity*> EnemyBullets;
	std::vector<Entity*> bullet;

	//Sprite Sheet
	GLuint TileSprites;
	GLuint CharSprites;
	GLuint fontSprites;
	GLuint BulletSprites;
	SheetSprite playerImg;
	SheetSprite enemyImg;
	SheetSprite bulletImg;
	SheetSprite bulletImg2;
	float randNum();
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
	Entity* enemy1;
	Entity* enemy2;
	std::vector<Entity*> entities;
	std::vector<Entity*> enemies;


	// Sounds
	Mix_Chunk *jump;
	Mix_Chunk *shoot;
	Mix_Chunk *hit;
	Mix_Music *music1;
	Mix_Music *music23;
};