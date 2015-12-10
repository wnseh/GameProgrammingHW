#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp() :timeLeftOver(0.0), done(false), EbulletIndex(0), bulletIndex(0), lastFrameTicks(0.0f), win(false), mapWidth(0), mapHeight(0), Enumber(0) {
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
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program->programID);
	glViewport(0, 0, 600, 600);
	projectionMatrix.setOrthoProjection(-2.0f, 2.0f, -3.0f, 3.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glClear(GL_COLOR_BUFFER_BIT);
	
	//music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music1 = Mix_LoadMUS("Musics/level1.mp3");
	music23 = Mix_LoadMUS("Musics/level23.mp3");
	jump = Mix_LoadWAV("Musics/jump.wav");
	shoot = Mix_LoadWAV("Musics/shoot.wav");
	hit = Mix_LoadWAV("Musics/hit.wav");
	//add bullet noise

	Mix_VolumeMusic(20);
	Mix_VolumeChunk(jump, 1);
	Mix_VolumeChunk(shoot, 3);
	Mix_PlayMusic(music23, -1);
	
	//create player
	player = new Entity(1.0f, -3.0f, 0.4f, 0.2f, PLAYER);
	//set player alive
	player->visible = true;
	//Create enemy entities
	for (int i = 0; i < ENEMIES; i++) {
		enemies.push_back(new Entity(-1.0f, -1.0f, 0.4f, 0.2f, ENEMY));
	}
	float bulletposx = 0.0;
	float bulletposy = -4.5;
	for (int i = 0; i < MAX_BULLETS; i++){
		bullet.push_back(new Entity(bulletposx, bulletposy, 0.2, 0.1, PLAYER_BULLET));
	}
	float Ebulletposx = 0.0;
	float Ebulletposy = -4.5;
	for (int i = 0; i < MAX_BULLETS; i++){
		EnemyBullets.push_back(new Entity(Ebulletposx, Ebulletposy, 0.2, 0.1, ENEMY_BULLET));
	}
	fontSprites = LoadTexture("Images/font1.png");
	TileSprites = LoadTexture("Images/tilesprite.png");
	CharSprites = LoadTexture("Images/sprites.png");
	BulletSprites = LoadTexture("Images/bullet.png");
	playerImg = SheetSprite(CharSprites, 0.0f / 128.0f, 0.0f / 128.0f, 69.0f / 128.0f, 98.0f / 128.0f, 0.4f);
	enemyImg = SheetSprite(CharSprites, 71.0f / 128.0f, 0.0f / 128.0f, 51.0f / 128.0f, 73.0f / 128.0f, 0.4f);
	bulletImg2 = SheetSprite(BulletSprites, 0.0f / 32.0f, 10.0f / 32.0f, 5.0f / 32.0f, 6.0f / 32.0f, 0.1f);
	bulletImg = SheetSprite(BulletSprites, 0.0f / 32.0f, 0.0f / 32.0f, 8.0f / 32.0f, 8.0f / 32.0f, 0.1f);
	state = STATE_MAIN_MENU;
	gameLevel = LEVEL_2;
	getTxtData("Maps/final1.txt");
	Render();
	SDL_GL_SwapWindow(displayWindow);
}
void ClassDemoApp::shootBullets(float x, float y, float direction){
	bullet[bulletIndex]->isVisible() == true;
	bullet[bulletIndex]->x = x;
	bullet[bulletIndex]->y = y;
	bullet[bulletIndex]->shoot(direction);
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1){
		bulletIndex = 0;
	}
}

void ClassDemoApp::EshootBullets(float x, float y, float targetX, float targetY){
	EnemyBullets[EbulletIndex]->isVisible() == true;
	EnemyBullets[EbulletIndex]->x = x;
	EnemyBullets[EbulletIndex]->y = y;
	EnemyBullets[EbulletIndex]->Eshoot(targetX, targetY);
	EbulletIndex++;
	if (EbulletIndex > MAX_BULLETS - 1){
		EbulletIndex = 0;
	}
}



void ClassDemoApp::Render() {

	switch (state){
	case STATE_MAIN_MENU:{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		modelMatrix.identity();
		viewMatrix.identity();
		program->setProjectionMatrix(projectionMatrix);
		program->setModelMatrix(modelMatrix);
		program->setViewMatrix(viewMatrix);
		RenderMainMenu();
		break;
	}
	case STATE_GAME_LEVEL:
	{
		if (gameLevel == LEVEL_1){
			glClearColor(0.2f, 0.7f, 0.8f, 1.0f);
		}
		else if (gameLevel == LEVEL_2){
			glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		}
		else if (gameLevel == LEVEL_3){
			glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		modelMatrix.identity();
		program->setProjectionMatrix(projectionMatrix);
		viewMatrix.identity();
//		if (player->x > 2 && player->x < 20 && player->y < 5 && player->y > -5){
			viewMatrix.Translate(-player->x, -player->y, 0.0);
//		}
		program->setViewMatrix(viewMatrix);
		program->setModelMatrix(modelMatrix);
		RenderGameLevel();
		break;
	}
	case STATE_GAME_OVER:
	{
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		modelMatrix.identity();
		viewMatrix.identity();
		program->setProjectionMatrix(projectionMatrix);
		program->setModelMatrix(modelMatrix);
		program->setViewMatrix(viewMatrix);
		RenderGameOver();
		break;
	}
	}
}

void ClassDemoApp::placeEntity(string type, float x, float y){
	if (type == "sPLAYER"){
		// build & draw player
		// set type equal to player
		player->x = x;
		player->y = y;
//		player = new Entity(x, y, 0.4f, 0.2f, PLAYER);
	}
	else if (type == "ENEMY"){
		// bring enemies to life
		if (Enumber < ENEMIES-1){
			enemies[Enumber]->visible = true;
			enemies[Enumber]->x = x;
			enemies[Enumber]->y = y;
			enemies[Enumber]->velocity_x = 1.0f;
			Enumber++;
		}
		else{
			Enumber = 0;
		}
//		enemy1 = new Entity(x, y, 0.4f, 0.2f, ENEMY1);
//		enemies.push_back(enemy1);
	}
}

void ClassDemoApp::getTxtData(string file){
	ifstream infile(file);
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
	}
}

void ClassDemoApp::RenderMainMenu() {
	modelMatrix.identity();
	modelMatrix.Translate(-1.6f, 1.5f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "Welcome", 0.8f, -0.4f);
	
	modelMatrix.identity();
	modelMatrix.Translate(-1.75f, 0.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "To Ghost Hunter", 0.6f, -0.35f);

	modelMatrix.identity();
	modelMatrix.Translate(0.0f, -2.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "Space to start.", 0.4f, -0.26f);
	SDL_GL_SwapWindow(displayWindow);

	modelMatrix.identity();
	modelMatrix.Translate(0.0f, -2.5f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "ESC to exit.", 0.4f, -0.26f);
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGameLevel() {
	//need to draw all the tiles and the entities.
	player->Draw(program, modelMatrix, playerImg);
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->isVisible()) {
			enemies[i]->Draw(program, modelMatrix, enemyImg);
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
	vector<float> vertexData;
	vector<float> texCoordData;
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelData[y][x] != 0){
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					TILE_SIZE * x, -TILE_SIZE * y,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), {
					u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u, v,
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
			}
		}
	}

	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	modelMatrix.identity();
	program->setModelMatrix(modelMatrix);
	glBindTexture(GL_TEXTURE_2D, TileSprites);
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size()/2);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGameOver() {
	modelMatrix.identity();
	modelMatrix.Translate(-1.6f, 0.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "Game Over", 1.0f, -0.6f);

	if (win == true) {
		modelMatrix.identity();
		modelMatrix.Translate(-1.5f, 1.5f, 0.0f);
		program->setModelMatrix(modelMatrix);
		DrawText(fontSprites, "WINNER", 0.8f, -0.4f);
	}
	else {
		modelMatrix.identity();
		modelMatrix.Translate(-1.5f, 1.5f, 0.0f);
		program->setModelMatrix(modelMatrix);
		DrawText(fontSprites, "LOSER!!", 0.8f, -0.4f);
	}

	modelMatrix.identity();
	modelMatrix.Translate(0.0f, -2.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontSprites, "ESC to close...", 0.4f, -0.26f);
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::ProcessInput(float elasped) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (state == STATE_MAIN_MENU){
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_GAME_LEVEL;
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				done = true;
			}
		}
		if (state == STATE_GAME_LEVEL){
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					if (player->x >= -1 * MAXXPOS){
						player->velocity_x = -2.0;
						player->direction = -1.0f;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					if (player->y <= MAXXPOS){
						player->velocity_x = 2.0;
						player->direction = 1.0f;
					}
				}
				//if (player->onground == true){
					if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
						Mix_PlayChannel(-1, jump, 0);
						player->velocity_y = 3.0;
					}
				//}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					shootBullets(player->x, player->y, player->direction);
					Mix_PlayChannel(-1, shoot, 0);
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					done = true;
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
	if (gameLevel == LEVEL_2){
		getTxtData("Maps/final2.txt");
//		Mix_PlayMusic(music23, -1);
	}
	if (gameLevel == LEVEL_3){
		getTxtData("Maps/final3.txt");
	}
	Update(elasped);
	ProcessInput(elasped);
	Render();
	
	return done;
}


bool isSolid(int val){
	switch (val){
	case 5:
		return true;
		break;
	case 33:
		return true;
		break;
	case 34:
		return true;
		break;
	case 35:
		return true;
		break;
	case 36:
		return true;
		break;
	case 37:
		return true;
		break;
	case 101: // dead
		return true;
		break;
	case 102: // dead
		return true;
		break;
	case 103: // dead
		return true;
		break;
	case 190:
		return true;
		break;
	case 114:
		return true;
		break;
	case 153:
		return true;
		break;
	case 157:
		return true;
		break;
	case 139:
		return true;
		break;
	case 159:
		return true;
		break;
	case 141:
		return true;
		break;
	case 116: // dead
		return true;
		break;
	case 117: // dead
		return true;
		break;
	case 118: // dead
		return true;
		break;
	case 205:
		return true;
		break;
	case 222:
		return true;
		break;
	case 217:
		return true;
		break;
	case 201:
		return true;
		break;
	case 254:
		return true;
		break;
	case 238:
		return true;
		break;
	case 113:
		return true;
		break;
	case 255:
		return true;
		break;
	case 204:
		return true;
		break;
	case 202:
		return true;
		break;
	case 234:
		return true;
		break;
	case 223:
		return true;
		break;
	case 220:
		return true;
		break;
	case 249:
		return true;
		break;
		
	default:
		return false;
		break;
	}
}

void ClassDemoApp::worldToTileCoord(float worldX, float worldY, int *gridX, int *gridY){
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

void ClassDemoApp::Update(float elasped) {
	if (state == STATE_GAME_LEVEL){
		if (gameOver() == true){
			state = STATE_GAME_OVER;
		}
		int gridX;
		int gridY;
		player->Update(elasped);
		for (int i = 0; i < ENEMIES; i++){
			if (enemies[i]->isVisible()){
				enemies[i]->EUpdate(elasped);
				enemies[i]->x += enemies[i]->velocity_x * elasped;
				enemies[i]->y += enemies[i]->velocity_y * elasped;
				//enemy Collision
				worldToTileCoord(enemies[i]->x, enemies[i]->y - enemies[i]->height / 2.0f, &gridX, &gridY);
				//bottom
				if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
					if (isSolid(levelData[gridY][gridX])){
						float gridPosY = -TILE_SIZE * gridY;
						float distance = fabs(gridPosY - (enemies[i]->y - enemies[i]->height / 2.0));
						enemies[i]->y += distance + 0.01;
						enemies[i]->velocity_y = 0.0f;
					}
				}
				//top
				worldToTileCoord(enemies[i]->x, enemies[i]->y + enemies[i]->height / 2.0f, &gridX, &gridY);
				if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
					if (isSolid(levelData[gridY][gridX])){
						player->velocity_y -= 0.2f;
					}
				}
				//left
				worldToTileCoord(enemies[i]->x - enemies[i]->width / 2.0f, enemies[i]->y, &gridX, &gridY);
				if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
					if (isSolid(levelData[gridY][gridX])){
						enemies[i]->velocity_x *= -1.0f;
					}
				}
				//right
				worldToTileCoord(enemies[i]->x + enemies[i]->width / 2.0f, enemies[i]->y, &gridX, &gridY);
				if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
					if (isSolid(levelData[gridY][gridX])){
						enemies[i]->velocity_x *= -1.0f;
					}
				}
				//collision with player
				if (enemies[i]->collidesWith(player)) {
					player->visible = false;
					break;
				}
				EnemyBullet += elasped;
				//Idle state
				if (enemies[i]->distance(player)>1){
					//do what you are currently doing
						//just moving around and colliding
				}
				//Following state
				if (enemies[i]->distance(player) < 1.0f){
					//if player is at the right move right
					if (enemies[i]->x - player->x < 0){
						enemies[i]->velocity_x = fabs(enemies[i]->velocity_x);
					}
					//if player is at the left move left
					else if (enemies[i]->x - player->x > 0.0f){
						enemies[i]->velocity_x = fabs(enemies[i]->velocity_x)* (-1);
					}
				}
				//shooting state
				if (enemies[i]->distance(player) <1.0f && enemies[i]->distance(player)>0.8f){
					if (EnemyBullet > 5.0f){
						EshootBullets(enemies[i]->x, enemies[i]->y, player->x, player->y);
						EnemyBullet = 0.0f;
					}
				}
			}
		}
		player->onground = false;
		if (true){ //it is not static
			float lev2x = 1.0f;
			float lev2y = -0.5f;
			float lev3x = 1.0f;
			float lev3y = 1.0f;
			//Collision
			//bottom
			player->y += player->velocity_y * elasped;
			worldToTileCoord(player->x, player->y - player->height / 2.0f, &gridX, &gridY);
			if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
				if (isSolid(levelData[gridY][gridX])){
					//if player collides with traps he dies
					if (levelData[gridY][gridX] == 101 || levelData[gridY][gridX] == 102 || levelData[gridY][gridX] == 103 ||
						levelData[gridY][gridX] == 116 || levelData[gridY][gridX] == 117 || levelData[gridY][gridX] == 118){
						player->visible = false;
					}
					float gridPosY = -TILE_SIZE * gridY;
					float distance = fabs(gridPosY - (player->y - player->height / 2.0));
					player->onground = true;
					player->y += distance + 0.001;
					player->velocity_y = 0.0f;
					
					if (levelData[gridY][gridX] == 190 || levelData[gridY][gridX] == 114 || levelData[gridY][gridX] == 113) {
						if (gameLevel == LEVEL_1){
							gameLevel = LEVEL_2;
							player->x = lev2x;
							player->y = lev2y;
						}
						else if (gameLevel == LEVEL_2){
							gameLevel = LEVEL_3;
							player->x = lev3x;
							player->y = lev3y;
						}
						else if (gameLevel == LEVEL_3){
							win = true;
							state = STATE_GAME_OVER;
						}
					}
				}
			}
			//top
			worldToTileCoord(player->x, player->y + player->height / 2.0f, &gridX, &gridY);
			if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
				if (isSolid(levelData[gridY][gridX])){
					if (levelData[gridY][gridX] == 101 || levelData[gridY][gridX] == 102 || levelData[gridY][gridX] == 103 ||
						levelData[gridY][gridX] == 116 || levelData[gridY][gridX] == 117 || levelData[gridY][gridX] == 118){
						player->visible = false;
					}

					float gridPosY = (-TILE_SIZE * gridY) - TILE_SIZE;
					float distance = fabs(gridPosY - (player->y + player->height / 2.0));
					player->y -= distance - 0.001;
					player->velocity_y -= 0.2f;

					if (levelData[gridY][gridX] == 190 || levelData[gridY][gridX] == 114 || levelData[gridY][gridX] == 113) {
						if (gameLevel == LEVEL_1){
							gameLevel = LEVEL_2;
							player->x = lev2x;
							player->y = lev2y;
						}
						else if (gameLevel == LEVEL_2){
							gameLevel = LEVEL_3;
							player->x = lev3x;
							player->y = lev3y;
						}
						else if (gameLevel == LEVEL_3){
							win = true;
							state = STATE_GAME_OVER;
						}
					}
				}
			}
			//left
			player->x += player->velocity_x * elasped;
			worldToTileCoord(player->x - player->width / 2.0f, player->y, &gridX, &gridY);
			if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
				if (isSolid(levelData[gridY][gridX])){
					if (levelData[gridY][gridX] == 101 || levelData[gridY][gridX] == 102 || levelData[gridY][gridX] == 103 ||
						levelData[gridY][gridX] == 116 || levelData[gridY][gridX] == 117 || levelData[gridY][gridX] == 118){
						player->visible = false;
					}
					float gridPosX = (TILE_SIZE * gridX) + TILE_SIZE;
					float distance = fabs(gridPosX - (player->x - player->width / 2.0));
					player->x += distance + 0.001;
					player->velocity_x = 0.0f;
					if (levelData[gridY][gridX] == 190 || levelData[gridY][gridX] == 114 || levelData[gridY][gridX] == 113) {
						if (gameLevel == LEVEL_1){
							gameLevel = LEVEL_2;
							player->x = lev2x;
							player->y = lev2y;
						}
						else if (gameLevel == LEVEL_2){
							gameLevel = LEVEL_3;
							player->x = lev3x;
							player->y = lev3y;
						}
						else if (gameLevel == LEVEL_3){
							win = true;
							state = STATE_GAME_OVER;
						}
					}
				}
			}
			//right
			worldToTileCoord(player->x + player->width / 2.0f, player->y, &gridX, &gridY);
			if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
				if (isSolid(levelData[gridY][gridX])){
					if (levelData[gridY][gridX] == 101 || levelData[gridY][gridX] == 102 || levelData[gridY][gridX] == 103 ||
						levelData[gridY][gridX] == 116 || levelData[gridY][gridX] == 117 || levelData[gridY][gridX] == 118){
						player->visible = false;
					}
					float gridPosX = (TILE_SIZE * gridX);// -TILE_SIZE;
					float distance = fabs(gridPosX - (player->x + player->width / 2.0));
					player->x -= distance - 0.001;
					player->velocity_x = 0.0f;
					if (levelData[gridY][gridX] == 190 || levelData[gridY][gridX] == 114 || levelData[gridY][gridX] == 113) {
						if (gameLevel == LEVEL_1){
							gameLevel = LEVEL_2;
							player->x = lev2x;
							player->y = lev2y;
						}
						else if (gameLevel == LEVEL_2){
							gameLevel = LEVEL_3;
							player->x = lev3x;
							player->y = lev3y;
						}
						else if (gameLevel == LEVEL_3){
							win = true;
							state = STATE_GAME_OVER;
						}
					}
				}
			}
			//Enemy bullet Update
			for (int i = 0; i < EnemyBullets.size(); i++) {
				if (EnemyBullets[i]->isVisible()) {
					EnemyBullets[i]->movex(EnemyBullets[i]->velocity_x * elasped);
					EnemyBullets[i]->movey(EnemyBullets[i]->velocity_y * elasped);
					//bottom collision with wall
					worldToTileCoord(EnemyBullets[i]->x, EnemyBullets[i]->y - EnemyBullets[i]->height / 2.0f, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							EnemyBullets[i]->die();
						}
					}
					//top
					worldToTileCoord(EnemyBullets[i]->x, EnemyBullets[i]->y + EnemyBullets[i]->height / 2.0f, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							EnemyBullets[i]->die();
						}
					}
					//left
					worldToTileCoord(EnemyBullets[i]->x - EnemyBullets[i]->width / 2.0f, EnemyBullets[i]->y, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							EnemyBullets[i]->die();
						}
					}
					//right
					worldToTileCoord(EnemyBullets[i]->x + EnemyBullets[i]->width / 2.0f, EnemyBullets[i]->y, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							EnemyBullets[i]->die();
						}
					}
					if (EnemyBullets[i]->collidesWith(player)) {
						player->visible = false;
						EnemyBullets[i]->die();
						break;
					}
				}
			}
			//bullet Update
			for (int i = 0; i < bullet.size(); i++) {
				if (bullet[i]->isVisible()) {
					bullet[i]->movex(bullet[i]->velocity_x * elasped);
					bullet[i]->movey(bullet[i]->velocity_y * elasped);
					//bottom collision with wall
					worldToTileCoord(bullet[i]->x, bullet[i]->y - bullet[i]->height / 2.0f, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							bullet[i]->die();
						}
					}
					//top
					worldToTileCoord(bullet[i]->x, bullet[i]->y + bullet[i]->height / 2.0f, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							bullet[i]->die();
						}
					}
					//left
					worldToTileCoord(bullet[i]->x - bullet[i]->width / 2.0f, bullet[i]->y, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							bullet[i]->die();
						}
					}
					//right
					worldToTileCoord(bullet[i]->x + bullet[i]->width / 2.0f, bullet[i]->y, &gridX, &gridY);
					if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
						if (isSolid(levelData[gridY][gridX])){
							bullet[i]->die();
						}
					}
					for (int j = 0; j < enemies.size(); j++) {
						if (enemies[j]->isVisible() && bullet[i]->collidesWith(enemies[j])) {
							enemies[j]->die();
							bullet[i]->die();
							break;
						}
					}
				}
			}
		}
	}
}

bool ClassDemoApp::readHeader(ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool ClassDemoApp::readLayerData(ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val -1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

bool ClassDemoApp::readEntityData(ifstream &stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

bool ClassDemoApp::gameOver(){
	return !player->isVisible();
}

