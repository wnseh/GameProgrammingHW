#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>
enum ENTITYTYPE { PLAYER, ENEMY, BLOCK, PLAYER_BULLET, ENEMY_BULLET, GUN, HEART };
enum ENTITYSTATE{IDLE1, IDLE2, ANGRY1, ANGRY2, DEAD};
enum ANIMATION {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT};
class Entity {
public:
	Entity(float x, float y, float h, float w, ENTITYTYPE type, ENTITYSTATE state=IDLE1, ANIMATION animation = ONE);
	~Entity();
	float x;
	float y;
	ENTITYTYPE type;
	ENTITYSTATE state;
	ANIMATION animation;
	float height;
	float width;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float direction;
	bool visible;
	bool onground;
	bool walking;

	//fucntion
	void changeDirection();
	void movey(float x);
	void movex(float y);
	float distance(Entity* object);
	void die();
	float lerp(float v0, float v1, float t);
	bool isPlayer();
	void shoot(float direction);
	void Eshoot(float targetx, float targety);
	bool isVisible();
	bool isIdle();
	bool isAngry();
	bool isDead();
	void Update(float elapsed);
	void EUpdate(float elapsed);
	ENTITYTYPE getType();
	ENTITYSTATE getState();
	bool hitwall(float distance);
	bool collidesWith(Entity* object);
	void Draw(ShaderProgram* program, Matrix& modelMatrix, SheetSprite& sprite);
};