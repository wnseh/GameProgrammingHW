#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp() :timeLeftOver(0.0), EbulletIndex(0), bulletIndex(0), done(false), lastFrameTicks(0.0f), win(false), mapWidth(0), mapHeight(0) {
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
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 320, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program->programID);
	glViewport(0, 0, 400, 320);
	projectionMatrix.setOrthoProjection(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//	program->setModelMatrix(modelMatrix);
//	program->setViewMatrix(viewMatrix);
//	program->setProjectionMatrix(projectionMatrix);

//	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	player = new Entity(2.1, -1.5, 0.2, 0.2, PLAYER);
	fontSprites = LoadTexture("Images/font1.png");
	TileSprites = LoadTexture("Images/tiles_spritesheet.png");
//	TileSprites = LoadTexture("Images/castle.png");
	CharSprites = LoadTexture("Images/sprites.png");
	playerImg = SheetSprite(CharSprites, 0.0f / 128.0f, 0.0f / 128.0f, 69.0f / 128.0f, 98.0f / 128.0f, 0.4f);
	enemyImg = SheetSprite(CharSprites, 71.0f / 128.0f, 0.0f / 128.0f, 51.0f / 128.0f, 73.0f / 128.0f, 0.4f);
//	blockImg = SheetSprite(TileSprites, 288.0f / 208.0f, 792.0f / 208.0f, 70.0f / 208.0f, 70.0f / 208.0f, 0.4f);
//	startBlock = SheetSprite(Sprites, 101.0f / 256.0f, 86.0f / 256.0f, 13.0f / 256.0f, 54.0f / 256.0f, 0.1f);

	getTxtData();
	Render();
	SDL_GL_SwapWindow(displayWindow);
}



void ClassDemoApp::Render() {
	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	modelMatrix.identity();
	program->setProjectionMatrix(projectionMatrix);
	viewMatrix.identity();
	viewMatrix.Translate(-player->x, -player->y, 0.0);
	program->setViewMatrix(viewMatrix);
	program->setModelMatrix(modelMatrix);
	RenderGameLevel();
}

void ClassDemoApp::placeEntity(string type, float x, float y){
	if (type == "player"){
		// build & draw player
		// set type equal to player
	}
	else if (type == "enemy"){
		// build & draw enemy
		// set type to enemy
	}
}

void ClassDemoApp::getTxtData(){
	ifstream infile("Maps/tile.txt");
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
		else if (line == "[ObjectsLayer]") {
			readEntityData(infile);
		}
	}
}

void ClassDemoApp::RenderGameLevel() {
	//need to draw all the tiles and the entities.
	player->Draw(program, modelMatrix, playerImg);
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

void ClassDemoApp::ProcessInput(float elasped) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				if (player->x >= -1 * MAXXPOS){
					player->velocity_x = -4.0;
				}
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				if (player->y <= MAXXPOS){
					player->velocity_x = 4.0;
					}
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				player->velocity_y = 4.0;
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

void ClassDemoApp::worldToTileCoord(float worldX, float worldY, int *gridX, int *gridY){
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

bool isSolid(int val){
	switch (val){
	case 119:
		return true;
		break;
	case 148:
		return true;
		break;
	case 72:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void ClassDemoApp::Update(float elasped) {
	int gridX;
	int gridY;	
	player->Update(elasped);
	//enemy Update
	if (true){ //it is not static
		player->y += player->velocity_y * elasped;
		worldToTileCoord(player->x, player->y - player->height / 2.0f, &gridX, &gridY);
		if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
			if (isSolid(levelData[gridY][gridX])){
				float gridPosY = -TILE_SIZE * gridY;
				float distance = fabs(gridPosY - (player->y - player->height / 2.0));
				player->y += distance + 0.001;
				player->velocity_y = 0.0f;
			}
		}
		worldToTileCoord(player->x, player->y + player->height / 2.0f, &gridX, &gridY);
		if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
			if (isSolid(levelData[gridY][gridX])){
				float gridPosY = (-TILE_SIZE * gridY) - TILE_SIZE;
				float distance = fabs(gridPosY - (player->y + player->height / 2.0));
				player->y += distance + 0.001;
				player->velocity_y = 0.0f;
			}
		}
		player->x += player->velocity_x * elasped;
		worldToTileCoord(player->x, player->y + player->height / 2.0f, &gridX, &gridY);
		if (gridX > 0 && gridX < LEVEL_WIDTH && gridY > 0 && gridY < LEVEL_HEIGHT){
			if (isSolid(levelData[gridY][gridX])){
				float gridPosX = (TILE_SIZE * gridY) + TILE_SIZE;
				float distance = fabs(gridPosX - (player->y - player->height / 2.0));
				player->y += distance + 0.001;
				player->velocity_y = 0.0f;
			}
		}

	}

}

bool ClassDemoApp::topReached(){
	return player->y == 4.0;
}

bool ClassDemoApp::collided(){
	//check if the player collided with the enemy
	return false;
}

bool ClassDemoApp::gameOver(){
	//if player reaches the top || player collides with enemy
	return topReached() || collided();
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
			mapWidth = 25;//atoi(value.c_str());
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
						levelData[y][x] = val - 1;
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
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}


