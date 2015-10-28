#pragma once
#include "ShaderProgram.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite {
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
	void Draw(ShaderProgram* program, Matrix& gameMatrix, const float shiftX, const float shiftY);

	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
};