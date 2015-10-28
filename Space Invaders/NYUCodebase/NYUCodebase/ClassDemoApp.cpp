#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp() :Enumber(ENEMIES), timeLeftOver(0.0), EbulletIndex(0), bulletIndex(0), done(false), lastFrameTicks(0.0f), win(false) {
	Setup();
}

ClassDemoApp::~ClassDemoApp() {
	delete program;
	SDL_Quit();
}
GLuint ClassDemoApp::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
};

void ClassDemoApp::DrawText(int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
};

void ClassDemoApp::Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program->programID);
	glViewport(0, 0, 640, 640);
	projectionMatrix.setOrthoProjection(-4.0f, 4.0f, -4.0f, 4.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);
	player = new Entity(0.0f, -3.0f, 0.3f, 0.2f, 500.0f, ENTITY_PLAYER);

	float enemyposx = -3.5f;
	float enemyposy = 3.0f;
	//Create enemy entities
	for (int i = 0; i < ENEMIES; i++) {
		enemies.push_back(new Entity(enemyposx, enemyposy, 0.2, 0.2, 1.0f, ENTITY_ENEMY));
		enemyposx += 1.0f;
		if (((i + 1) % 7 == 0) && i != 0) {
			enemyposx = -3.5f;
			enemyposy -= 0.6f;
		}
	}
	float bulletposx = 0.0;
	float bulletposy = -4.5;
	for (int i = 0; i < MAX_BULLETS; i++){
		bullet.push_back(new Entity(bulletposx, bulletposy, 0.2, 0.1, 0.0f, ENTITY_BULLET));
	}
	float Ebulletposx = 0.0;
	float Ebulletposy = 4.5;
	for (int i = 0; i < MAX_BULLETS; i++){
		EnemyBullets.push_back(new Entity(bulletposx, bulletposy, 0.2, 0.1, -2.0f, ENTITY_BULLET));
	}
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	spaceInvaderSprites = LoadTexture("sprites.png");
	fontSprites = LoadTexture("font1.png");
	playerImg = SheetSprite(spaceInvaderSprites, 0.0f / 256.0f, 86.0f / 256.0f, 99.0f / 256.0f, 75.0f / 256.0f, 0.3f);
	EnemyImg = SheetSprite(spaceInvaderSprites, 0.0f / 256.0f, 0.0f / 256.0f, 104.0f / 256.0f, 84.0f / 256.0f, 0.4f);
	bulletImg = SheetSprite(spaceInvaderSprites, 101.0f / 256.0f, 86.0f / 256.0f, 13.0f / 256.0f, 54.0f / 256.0f, 0.1f);
	bulletImg2 = SheetSprite(spaceInvaderSprites, 106.0f / 256.0f, 0.0f / 256.0f, 9.0f / 256.0f, 37.0f / 256.0f, 0.1f);
	state = STATE_MAIN_MENU;
	Render();
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::shootBullets(float x, float y){
	bullet[bulletIndex]->isVisible() == true;
	bullet[bulletIndex]->x = x;
	bullet[bulletIndex]->y = y;
	bullet[bulletIndex]->go();
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1){
		bulletIndex = 0;
	}
}

void ClassDemoApp::EshootBullets(float x, float y){
	EnemyBullets[EbulletIndex]->isVisible() == true;
	EnemyBullets[EbulletIndex]->x = x;
	EnemyBullets[EbulletIndex]->y = y;
	EnemyBullets[EbulletIndex]->shoot();
	EbulletIndex++;
	if (EbulletIndex > MAX_BULLETS - 1){
		EbulletIndex = 0;
	}
}

void ClassDemoApp::Render() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (state) {
	case STATE_MAIN_MENU:
	{
		RenderMainMenu();
		break;
	}
	case STATE_GAME_LEVEL: {
		RenderGameLevel();
		break;
	}
	case STATE_GAME_OVER: {
		RenderGameOver();
		break;
	}
	}
}

void ClassDemoApp::RenderMainMenu() {
	modelMatrix.identity();
	modelMatrix.Translate(-2.5f, 2.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "Welcome to", 1.0f, -0.5f);

	modelMatrix.identity();
	modelMatrix.Translate(-3.3f, 0.4f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "SPACE INVADERS!", 1.1f, -0.6f);

	modelMatrix.identity();
	modelMatrix.Translate(-0.2f, -3.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "SPACEBAR to start....", 0.5f, -0.3);
}

void ClassDemoApp::RenderGameLevel() {
	player->Draw(program, modelMatrix, playerImg);
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->isVisible() == true) {
			enemies[i]->Draw(program, modelMatrix, EnemyImg);
		}
	}
	for (int i = 0; i < bullet.size(); i++){
		if (bullet[i]->isVisible()){
			bullet[i]->Draw(program, modelMatrix, bulletImg2);
		}
	}
	for (int i = 0; i < EnemyBullets.size(); i++){
		if (EnemyBullets[i]->isVisible()){
			EnemyBullets[i]->Draw(program, modelMatrix, bulletImg);
		}
	}
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGameOver() {
	modelMatrix.identity();
	modelMatrix.Translate(-2.5f, 1.5f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "Game Over", 1.1f, -0.6f);

	if (win == true) {
		modelMatrix.identity();
		modelMatrix.Translate(-3.65f, 0.35f, 0.0f);
		program->setModelMatrix(modelMatrix);
		DrawText(fontSprites, "This will be the only time you win", 0.58f, -0.38f);
	}
	else {
		modelMatrix.identity();
		modelMatrix.Translate(-2.0f, 0.0f, 0.0f);
		program->setModelMatrix(modelMatrix);
		DrawText(fontSprites, "LOSER!!", 1.3f, -0.4f);
	}

	modelMatrix.identity();
	modelMatrix.Translate(-0.2f, -3.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "ESC to close...", 0.5f, -0.3);
	SDL_GL_SwapWindow(displayWindow);
}



void ClassDemoApp::ProcessInput(float elasped) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (state == STATE_MAIN_MENU) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_GAME_LEVEL;

			}
		}
		if (state == STATE_GAME_LEVEL){
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					if (player->Pos_x() >= -1*MAXXPOS){
						player->movex(-0.4);
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					if (player->Pos_y() <= MAXXPOS){
						player->movex(0.4);
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					shootBullets(player->x, player->y);
				}
			}
		}
		if (state == STATE_GAME_OVER) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				done = true;
			}
		}
	}
}

bool ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elasped = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	float fixedElasped = elasped + timeLeftOver;
//	if (fixedElasped > FIXED_TIMESTEP*MAX_TIMESTEPS){
//		fixedElasped = FIXED_TIMESTEP*MAX_TIMESTEPS;
//	}
//	while (fixedElasped >= FIXED_TIMESTEP){
//		fixedElasped -= FIXED_TIMESTEP;
//	}
	Update(elasped);
	ProcessInput(elasped);
	Render();
	
	return done;
}

bool ClassDemoApp::CheckWall(){
	for (int j = 0; j < enemies.size(); j++){
		if (enemies[j]->hitwall(-0.2)){
			if (enemies[j]->isVisible() == true){
				return true;
			}
		}
	}
}

float ClassDemoApp::randNum(){
	return floor(rand() % ENEMIES);
}

void ClassDemoApp::Update(float elasped) {
	if (state == STATE_GAME_LEVEL) {
		if (gameOver() == true) {
			state = STATE_GAME_OVER;
		}
		EnemyBullet += elasped;
		if (EnemyBullet > 0.2f) {
			int index = randNum();
			EnemyBullet = 0.0f;
			if (enemies[index]->isVisible()){
				EshootBullets(enemies[index]->x, enemies[index]->y);
			}
			else{
				for (int i = 0; i < enemies.size(); i++){
					if (enemies[i]->isVisible()){
						EshootBullets(enemies[index]->x, enemies[index]->y);;
						//break;
					}
				}
			}
		}
		for (int i = 0; i < EnemyBullets.size(); i++) {
			if (EnemyBullets[i]->isVisible()) {
				EnemyBullets[i]->movey(EnemyBullets[i]->getVelocity() * elasped);
				if (EnemyBullets[i]->Pos_y() < -1 * MAXYPOS) {
					EnemyBullets[i]->stop();
				}
				else if (EnemyBullets[i]->collidesWith(player)) {
					EnemyBullets[i]->stop();
					player->dead();
					state = STATE_GAME_OVER;
					break;
				}
			}
		}
		for (int i = 0; i < bullet.size(); i++) {
			if (bullet[i]->isVisible()) {
				bullet[i]->movey(bullet[i]->getVelocity() * elasped);
				if (bullet[i]->Pos_y() > MAXYPOS) {
					bullet[i]->stop();
				}
				for (int j = 0; j < enemies.size(); j++) {
					if (enemies[j]->isVisible() && bullet[i]->collidesWith(enemies[j])) {
						enemies[j]->dead();
						bullet[i]->stop();
						--Enumber;
						if (Enumber == 0)
							win = true;
						break;
					}
				}
			}
		}
		if (CheckWall() == true){
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->changeDirection();
				enemies[i]->movex(enemies[i]->getDirection() * enemies[i]->getVelocity() * elasped + (enemies[i]->getDirection() * 0.1));
				enemies[i]->movey(-0.3f);
				if (player->collidesWith(enemies[i])) {
					state = STATE_GAME_OVER;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->movex(enemies[i]->getDirection() * enemies[i]->getVelocity() * elasped);
				if (player->collidesWith(enemies[i])) {
					state = STATE_GAME_OVER;
					break;
				}
			}
		}
	}
}

bool ClassDemoApp::gameOver(){
	return Enumber == 0 || !player->isVisible();
}


