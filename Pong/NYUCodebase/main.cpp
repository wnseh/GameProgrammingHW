#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
//#include "ClassDemoApp.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char * image_path){
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

class Ball{
public:
	Ball();
	float xVal, Yval = 0.0035f;
	void move(float BallX, float BallY, float elapsed){

	}
};


int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	//	SDL_Joystick *joystickc = SDL_JoystickOpen(0);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);


#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Matrix projectionMatrix;
	Matrix matrix;
	Matrix paddle1;
	Matrix paddle2;
	Matrix ball;
	Matrix winsign;
	Matrix winsign2;
	Matrix viewMatrix;
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	//	GLuint emojiTexture = LoadTexture(RESOURCE_FOLDER"emoji.png");
	SDL_Event event;
	bool done = false;
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;
	float paddle1Y = 0.0f;
	float paddle2Y = 0.0f;
	float ballX = 0.0f;
	float ballY = 0.0f;
	float ballXval = 0.0005f;
	float ballYval = 0.0005f;
	float paddle1Right = -3.3f;
	float paddle1Top = 0.4f;
	float paddle1Bot = -0.4f;
	float paddle2Left = 3.3f;
	float paddle2Top = 0.4f;
	float paddle2Bot = -0.4f;
	GLuint win1 = LoadTexture("hud_p3.png");
	GLuint alienWalk = LoadTexture("flatLight09.png");
	GLuint win3 = LoadTexture("hud_p1.png");
	GLuint paddle = LoadTexture("shadedLight12.png");

	//float xDir;

	while (!done) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.scancode == SDL_SCANCODE_UP){
					paddle2Y += 0.1f;
					paddle2Top += 0.1f;
					paddle2Bot += 0.1f;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
					paddle2Y -= 0.1f;
					paddle2Top -= 0.1f;
					paddle2Bot -= 0.1f;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_W){
					paddle1Y += 0.1f;
					paddle1Top += 0.1f;
					paddle1Bot += 0.1f;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_S){
					paddle1Y -= 0.1f;
					paddle1Top -= 0.1f;
					paddle1Bot -= 0.1f;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
					ballX = 0.0f;
					ballY = 0.0f;
				}
			}
		}
		//Matrix
		paddle1.identity();
		paddle1.Translate(0.0, paddle1Y, 0.0);
		paddle2.identity();
		paddle2.Translate(0.0, paddle2Y, 0.0);
		//		ball.Rotate(angle); // Radians = Degrees * (PI / 180)

		//collision
		ball.identity();
		ballX += ballXval;
		ballY += ballYval;
		if (ballY + 0.1f >= 1.9f){
			ballYval = -ballYval;
		}
		if (ballY - 0.1f <= -1.9f){
			ballYval = -ballYval;
		}
		if (ballX + 0.1f >= 3.4f && ballY < paddle2Top && ballY > paddle2Bot){
			ballXval = -ballXval;
		}
		if (ballX - 0.1f <= -3.4f && ballY < paddle1Top && ballY > paddle1Bot){
			ballXval = -ballXval;
		}
		if (ballX >= 3.55f){
			ballX = 0.0f;
			ballY = 0.0f;
			winsign2.Translate(1.0f, 0.0f, 0.0f);
		}
		if (ballX - 0.1f <= -3.55f){
			ballX = 0.0f;
			ballY = 0.0f;
			winsign.Translate(-1.0f, 0.0f, 0.0f);
		}


		ball.Translate(ballX, ballY, 0);




		program.setModelMatrix(paddle1);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		glUseProgram(program.programID);

		//Win sign
		program.setModelMatrix(winsign);
		float win[] = { 3.8f, 1.8f, 3.6f, 1.8f, 3.6f, 1.6f,
			3.6f, 1.6f, 3.8f, 1.6f, 3.8f, 1.8f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, win);
		glEnableVertexAttribArray(program.positionAttribute);
		float winCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, winCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, win1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);

		program.setModelMatrix(winsign2);
		float win2[] = { -3.6f, 1.8f, -3.8f, 1.8f, -3.8f, 1.6f,
			-3.8f, 1.6f, -3.6f, 1.6f, -3.6f, 1.8f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, win2);
		glEnableVertexAttribArray(program.positionAttribute);
		float winCoords2[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, winCoords2);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, win3);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);


		program.setModelMatrix(paddle1);
		//Paddle1
		float verticies2[] = { -3.3f, 0.4f, -3.4f, 0.4f, -3.4f, -0.4f,
			-3.4f, -0.4f, -3.3f, -0.4f, -3.3f, 0.4f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticies2);
		glEnableVertexAttribArray(program.positionAttribute);
		float texCoords2[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, paddle);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);

		//top wall
		program.setModelMatrix(matrix);
		float verticiesTop[] = { 3.55f, 2.0f, -3.55f, 2.0f, -3.55f, 1.9f, -3.55f, 1.9f, 3.55f, 1.9f, 3.55f, 2.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticiesTop);
		glEnableVertexAttribArray(program.positionAttribute);
		float texCoords22[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords22);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, paddle);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);

		//bottom wall
		program.setModelMatrix(matrix);
		float verticiesBottom[] = { 3.55f, -2.0f, -3.55f, -2.0f, -3.55f, -1.9f, -3.55f, -1.9f, 3.55f, -1.9f, 3.55f, -2.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticiesBottom);
		glEnableVertexAttribArray(program.positionAttribute);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords22);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, paddle);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);

		//middle line
		program.setModelMatrix(matrix);
		float verticiesMid[] = { 0.02f, 2.0, -0.02f, 2.0f, -0.02f, -2.0, -0.02f, -2.0, 0.02f, -2.0f, 0.02f, 2.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticiesMid);
		glEnableVertexAttribArray(program.positionAttribute);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords22);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, paddle);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);

		//Image 3 : paddle2
		program.setModelMatrix(paddle2);
		float verticies3[] = { 3.4f, 0.4f, 3.3f, 0.4f, 3.3f, -0.4f,
			3.3f, -0.4f, 3.4f, -0.4f, 3.4f, 0.4f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticies3);
		glEnableVertexAttribArray(program.positionAttribute);
		//Texture
		float texCoords3[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, paddle);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);

		//Image 1 : ball
		program.setModelMatrix(ball);
		float verticies1[] = { -0.1f, -0.1f, -0.1f, 0.1f, 0.1f, 0.1f,
			0.1f, 0.1f, 0.1f, -0.1f, -0.1f, -0.1f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticies1);
		glEnableVertexAttribArray(program.positionAttribute);
		//Texture
		float texCoords1[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, alienWalk);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.texCoordAttribute);
		glDisableVertexAttribArray(program.positionAttribute);



		SDL_GL_SwapWindow(displayWindow);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	SDL_Quit();
	return 0;
}