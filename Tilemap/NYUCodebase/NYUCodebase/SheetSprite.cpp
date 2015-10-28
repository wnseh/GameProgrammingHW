#include "SheetSprite.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

SheetSprite::SheetSprite() {
	size = textureID = u = v = width = height = 0;
}
SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) :textureID(textureID), u(u), v(v),
width(width), height(height), size(size){}

void SheetSprite::Draw(ShaderProgram* program, Matrix& gameMatrix, const float shiftX, const float shiftY){
	GLfloat texCoords[] = {
		u, v + height,
		u + width, v,
		u, v,
		u + width, v,
		u, v + height,
		u + width, v + height
	};
	float aspect = width / height;
	float vertices[] = {
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, -0.5f * size };

	gameMatrix.identity();
	gameMatrix.Translate(shiftX, shiftY, 0.0);
	program->setModelMatrix(gameMatrix);

	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);


}