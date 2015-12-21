#include "Entity.h"

Entity::Entity(float Pos_x, float Pos_y, float h, float w, ENTITYTYPE types, ENTITYSTATE states, ANIMATION animations){
	x = Pos_x;
	y = Pos_y;
	height = h;
	width = w;
	type = types;
	direction = 1.0f;
	visible = false;
	onground = false;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0;
	acceleration_y = 0.0;
	animation = animations;
	walking = false;
}
Entity::~Entity(){
	delete this;
}
bool Entity::hitwall(float distance) { return (x + distance < -1*4.0) || (x - distance > 4.0f); }
bool Entity::isPlayer(){
	return type == PLAYER;
}
bool Entity::isIdle(){
	return state == IDLE1 || state == IDLE2;
}
bool Entity::isAngry(){
	return state == ANGRY1 || state == ANGRY2;
}
bool Entity::isDead(){
	return state == DEAD;
}
void Entity::movex(float plusx) {
	x += plusx;
}
void Entity::movey(float plusy) {
	y += plusy;
}

void Entity::shoot(float direction){
	visible = true;
	velocity_x = direction*1.0f;
	velocity_y = float(rand() % 20 - 5)/30 ;
	acceleration_x = direction*10.0f;
}

void Entity::Eshoot(float targetx, float targety){
	visible = true;
	//shooting directly to target(player)
	float magnitude = sqrt(pow(targetx - x, 2) + pow(targety - y, 2));
	velocity_x = (1.3f*(targetx - x))/magnitude;
	velocity_y = (1.3f*(targety - y))/magnitude;
}

void Entity::die(){
	visible = false;
	velocity_x = 0.0f;
}
float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)*v0 + t* v1;
}
bool Entity::isVisible(){
	return visible;
}

void Entity::Update(float elapsed){
	if (visible == true){
		float gravity = -5.0f;
		float friction = 5.0f;
		velocity_x += acceleration_x*elapsed;
		velocity_y += acceleration_y*elapsed;
		if (type == PLAYER || type == ENEMY){
			velocity_x = lerp(velocity_x, 0.0f, friction*elapsed);
			velocity_y += gravity * elapsed;
		}
	}
}
void Entity::EUpdate(float elapsed){
	if (visible == true){
		float gravity = -5.0f;
		velocity_y += gravity * elapsed;
	}
}


float Entity::distance(Entity* object){
	return sqrt(pow(x - object->x,2) + pow(y - object->y,2));
}

void Entity::changeDirection(){
	velocity_x = velocity_x * -1.0f;
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

ENTITYSTATE Entity::getState(){
	return state;
}