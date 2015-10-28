#include "Entity.h"

Entity::Entity(float Pos_x, float Pos_y, float h, float w, float vel, ENTITYTYPE types){
	x = Pos_x;
	y = Pos_y;
	height = h;
	width = w;
	type = types;
	velocity = vel;
	visible = true;
	direction = 1.0f;
}
bool Entity::hitwall(float distance) { return (x + distance < -1*4.0) || (x - distance > 4.0f); }

bool Entity::hitbottom() { return y < -2.9 ; }

bool Entity::collidesWith(Entity* object) {
	return !(((y + (height / 2)) <= (object->Pos_y() - (object->getHeight() / 2))) 
		|| ((y - (height / 2)) >= (object->Pos_y() + (object->getHeight() / 2)))
		|| (((x + (width / 2)) <= (object->Pos_x() - (object->getWidth() / 2))) 
		|| ((x - (width / 2)) >= (object->Pos_x() + (object->getWidth() / 2)))));
}

void Entity::movex(float plusx) {
	x += plusx; 
}
void Entity::movey(float plusy) {
	y += plusy; 
}
void Entity::changeDirection() { 
	direction *= -1; 
}
void Entity::dead() { 
	visible = false; 
}

void Entity::stop() {
	visible = false;
	velocity = 0.0f;
}

void Entity::go(){
	visible = true;
	velocity = 2.0f;
}

void Entity::shoot(){
	visible = true;
	velocity = -2.0f;
}
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}
float Entity::Pos_x(){ 
	return x;
}
float Entity::Pos_y(){ 
	return y;
}
float Entity::getWidth(){ 
	return width; 
}
float Entity::getHeight(){ 
	return height; 
}
float Entity::getVelocity(){ 
	return velocity; 
}
float Entity::getDirection(){
	return direction; 
}
bool Entity::isVisible(){ 
	return visible; 
}
bool Entity::isPlayer(){
	return type == ENTITY_PLAYER;
}
bool Entity::isEnemy(){
	return type == ENTITY_ENEMY;
}
bool Entity::isPBullet(){
	return type == ENTITY_BULLET;
}
bool Entity::isEBullet(){
	return type == ENTITY_ENEMY_BULLET;
}
