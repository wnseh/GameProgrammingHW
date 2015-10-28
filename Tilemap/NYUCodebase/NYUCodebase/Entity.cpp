#include "Entity.h"

Entity::Entity(float Pos_x, float Pos_y, float h, float w, ENTITYTYPE types){
	x = Pos_x;
	y = Pos_y;
	height = h;
	width = w;
	type = types;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	direction_x = 1.0f;
	acceleration_x = 0.0;
	acceleration_y = 0.0;
}
bool Entity::hitwall(float distance) { return (x + distance < -1*4.0) || (x - distance > 4.0f); }

float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)*v0 + t* v1;
}

void Entity::Update(float elapsed){
	float gravity = -5.0f;
	float friction = 6.0f;
	if (type != BLOCK){
		velocity_x = lerp(velocity_x, 0.0, friction*elapsed);
		velocity_x += acceleration_x*elapsed;
		velocity_y += acceleration_y*elapsed;
		velocity_y += gravity * elapsed;
	}
}


bool Entity::collidesWith(Entity* object) {
	return !(((y + (height / 2)) <= (object->y - (object->height / 2))) 
		|| ((y - (height / 2)) >= (object->y + (object->height / 2)))
		|| (((x + (width / 2)) <= (object->x - (object->width / 2))) 
		|| ((x - (width / 2)) >= (object->x + (object->width / 2)))));
}

void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}

ENTITYTYPE Entity::getType(){
	return type;
}
