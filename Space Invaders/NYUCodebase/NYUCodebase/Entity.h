#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>
enum ENTITYTYPE { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_BULLET, ENTITY_ENEMY_BULLET };

class Entity {
public:
	Entity(float x, float y, float h, float w, float vel, ENTITYTYPE type);
	float x;
	float y;
	ENTITYTYPE type;
	bool visible;
	float height;
	float width;
	float velocity;
	float direction;

	float Pos_x();
	float Pos_y();
	float getWidth();
	float getHeight();
	float getVelocity();
	float getDirection();
	bool isPlayer();
	bool isEnemy();
	bool isPBullet();
	bool isEBullet();
	bool hitwall(float distance);
	bool hitbottom();
	bool collidesWith(Entity* object);
	bool isVisible();
	void movex(float x);
	void movey(float y);
	void changeDirection();
	void dead();
	void stop();
	void go();
	void shoot();
	void Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite);

};