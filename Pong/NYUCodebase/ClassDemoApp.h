#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
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
#define LEVEL_HEIGHT 40
#define LEVEL_WIDTH 25
#define TILE_SIZE 0.2f
#define SPRITE_COUNT_X 13
#define SPRITE_COUNT_Y 13
#define MAPWIDTH 1
#define MAPHEIGHT 1
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
	int mapWidth;
	int mapHeight;
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
	GLuint TileSprites;
	GLuint CharSprites;
	GLuint fontSprites;
	SheetSprite playerImg;
	SheetSprite enemyImg;
	SheetSprite blockImg;
	SheetSprite startBlock;
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
	void getTxtData();
	void placeEntity(string type, float x, float y);
	void worldToTileCoord(float worldX, float worldY, int *gridX, int *gridY);
	GLuint LoadTexture(const char *image_path);
	unsigned char ** levelData;// [MAPWIDTH][MAPHEIGHT];
	STATE state;
	Entity* player;
	std::vector<Entity*> entities;
//	std::vector<Entity*> blocks;
//	std::vector<Entity*> EnemyBullets;
};