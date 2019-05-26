#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object/SnakeObject.h"

using namespace std;


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_DEATH
};

class SnakeGame
{
private:
	SnakeObject* snakeObj;
	int direction;
	float boundsTop;
	float boundsBottom;
	float boundsLeft;
	float boundsRight;
	int score;
	int currentLength;
	bool hasInput;
	glm::vec2 foodPos;

	glm::vec3 lightPos;
	glm::vec3 lightColor;
	glm::vec3 viewPos;

public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	// Constructor/Destructor
	SnakeGame(GLuint width, GLuint height);
	~SnakeGame();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt, int frame_count);
	void Update(GLfloat dt);
	void Render(int frame_count);
	void updateScore(int add);
	void createFood();
};

#endif