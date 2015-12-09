#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>
enum ENTITYTYPE { PLAYER, ENEMY1, ENEMY2, BLOCK, ENTITY_BULLET };

class Entity {
public:
	Entity(float x, float y, float h, float w, ENTITYTYPE type);
	float x;
	float y;
	ENTITYTYPE type;
	float height;
	float width;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float direction_x;
	float direction;
	bool visible;
	bool onground;

	//fucntion
	void movey(float x);
	void movex(float y);
	void die();
	float lerp(float v0, float v1, float t);
	bool isPlayer();
	void shoot(float direction);
	bool isVisible();
	void Update(float elapsed);
	ENTITYTYPE getType();
	bool hitwall(float distance);
	bool collidesWith(Entity* object);
	void Draw(ShaderProgram* program, Matrix& modelMatrix, SheetSprite& sprite);
};