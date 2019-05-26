#include "SnakeGame.h"
#include "../lib/resource_manager/resource_manager.h"
#include "../lib/sprite_renderer/sprite_renderer.h"
#include "../lib/text_renderer/text_renderer.h"
#include <sstream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


SpriteRenderer  *Renderer;
SpriteRenderer  *lightRenderer;
TextRenderer	*Text;

SnakeGame::SnakeGame(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

SnakeGame::~SnakeGame()
{
	delete Renderer;
	delete lightRenderer;
}

void SnakeGame::Init()
{
	// rand number
	srand((int)time(0));
	// init snake object
	snakeObj = new SnakeObject();
	// right
	direction = 3;
	snakeObj->setDirection(direction);
	// init bounds
	snakeObj->setBounds(-11.0f, 11.0f, -21.0f, 21.0f);

	boundsBottom = -10.0f;
	boundsTop = 10.0f;
	boundsLeft = -15.0f;
	boundsRight = 15.0f;

	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPos = glm::vec3(5.0f, 30.0f, 5.0f);
	viewPos = glm::vec3(0.0f, 35.0f, 30.0f);

	score = 0;
	currentLength = snakeObj->getLength();
	foodPos = glm::vec2(0.0f);
	hasInput = false;

	// Load shaders
	ResourceManager::LoadShader("Game/shaders/sprite.vs", "Game/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("Game/shaders/lightObject.vs", "Game/shaders/lightObject.frag", nullptr, "lightObject");

	// Configure shaders
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
	//view = glm::rotate(view, -0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("sprite").SetMatrix4("view", view);

	ResourceManager::GetShader("lightObject").Use().SetInteger("image", 0);
	ResourceManager::GetShader("lightObject").SetMatrix4("projection", projection);
	ResourceManager::GetShader("lightObject").SetMatrix4("view", view);
	ResourceManager::GetShader("lightObject").SetVector3f("lightColor", lightColor);
	ResourceManager::GetShader("lightObject").SetVector3f("lightPos", lightPos);
	ResourceManager::GetShader("lightObject").SetVector3f("viewPos", viewPos);

	// Load textures
	ResourceManager::LoadTexture("resources/worry.jpg", GL_TRUE, "head");
	ResourceManager::LoadTexture("resources/brick.png", GL_TRUE, "block");
	ResourceManager::LoadTexture("resources/snake.png", GL_TRUE, "snake");
	ResourceManager::LoadTexture("resources/awesomeface.png", GL_TRUE, "face");

	// Load models
	ResourceManager::LoadModel("resources/LadyBug/LadyBug.obj", "LadyBug");
	ResourceManager::LoadModel("resources/Basketball/Basketball.obj", "Basketball");
	ResourceManager::LoadModel("resources/garden/Garden.obj", "garden");
	ResourceManager::LoadModel("resources/chicken_fix/Rooster.obj", "chicken");
	ResourceManager::LoadModel("resources/floor_basketball/BasketBallCourt.obj", "floor_basketball");
	ResourceManager::LoadModel("resources/Pufferfish/NOVELO_PUFFERFISH.obj", "Pufferfish");

	// Load Text
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("resources/fonts/comicbd.ttf", 24);

	// Set render-specific controls
	Shader spiritShader = ResourceManager::GetShader("sprite");
	Shader lightShader = ResourceManager::GetShader("lightObject");
	Renderer = new SpriteRenderer(spiritShader);
	Renderer->initRenderData();
	lightRenderer = new SpriteRenderer(lightShader);

	createFood();
}

void SnakeGame::Update(GLfloat dt)
{
	if (this->State == GAME_ACTIVE) {
		snakeObj->Move(dt);
	}

	if (snakeObj->getLength() > currentLength) {
		currentLength = snakeObj->getLength();
		updateScore(1);
		createFood();
	}

	if (snakeObj->isDead()) {
		this->State = GAME_DEATH;
	}
}

void SnakeGame::updateScore(int add) {
	score += add;
}


void SnakeGame::ProcessInput(GLfloat dt, int frame_count)
{
	if (frame_count == 0) {
		hasInput = false;
	}

	if (this->State == GAME_ACTIVE && !hasInput) {

		if (this->Keys[GLFW_KEY_W] && !hasInput) {
			if (direction == 1) return;
			direction = 0;
			hasInput = true;
		}
		if (this->Keys[GLFW_KEY_S] && !hasInput) {
			if (direction == 0) return;
			direction = 1;
			hasInput = true;
		}
		if (this->Keys[GLFW_KEY_A] && !hasInput) {
			if (direction == 3) return;
			direction = 2;
			hasInput = true;
		}
		if (this->Keys[GLFW_KEY_D] && !hasInput) {
			if (direction == 2) return;
			direction = 3;
			hasInput = true;
		}

	}
	//snakeObj->Move(direction, dt);
	snakeObj->setDirection(direction);
}

void SnakeGame::createFood() {
	deque<snake_node> nodes = snakeObj->getNextNodes();
	bool goodPos = 0;
	glm::vec2 p = glm::vec2(0.0f);

	while (!goodPos) {
		int x = rand() % 41 - 20;
		int z = rand() % 21 - 10;
		for (int i = 0; i < nodes.size(); i++) {
			if (x != int(nodes.at(i).x) && z != int(nodes.at(i).z)) {
				goodPos = 1;
				break;
			}
		}
		p = glm::vec2(x, z);
	}

	foodPos = p;
	snakeObj->setFoodPos(p);
}

void SnakeGame::Render(int frame_count)
{
	// render text
	std::stringstream ss;
	ss << this->score;
	Text->RenderText("Score:" + ss.str(), 15.0f, 15.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	if (this->State == GAME_DEATH) {
		Text->RenderText("Fucking Dead", this->Width / 2 - 20.0f, 15.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}


	// get textures
	Texture2D texture_head = ResourceManager::GetTexture("head");
	Texture2D texture_face = ResourceManager::GetTexture("face");
	Texture2D texture_block = ResourceManager::GetTexture("block");
	Texture2D texture_snake = ResourceManager::GetTexture("snake");

	// get models
	Model model_LadyBug = ResourceManager::GetModel("LadyBug");
	Model model_Basketball = ResourceManager::GetModel("Basketball");
	Model model_garden = ResourceManager::GetModel("garden");
	Model model_chicken = ResourceManager::GetModel("chicken");
	Model model_floor_basketball = ResourceManager::GetModel("floor_basketball");
	Model model_Pufferfish = ResourceManager::GetModel("Pufferfish");
	

	// render scene
	//lightRenderer->DrawSpriteModel(model_garden, glm::vec3(0.0, -2.0, 0.0), glm::vec3(1.5f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightRenderer->DrawSpriteModel(model_floor_basketball, glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.5f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//lightRenderer->DrawSpriteModel(model_Pufferfish, glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//snakeObj->setBounds(-11.0f, 11.0f, -21.0f, 21.0f);

	// render snake
	//snakeObj->Draw(*Renderer, texture_snake, texture_head);		// ²Î¿¼µã
	snakeObj->DrawModel(*lightRenderer, model_chicken, model_Basketball, frame_count);

	// reder food
	lightRenderer->DrawSpriteModel(model_LadyBug, glm::vec3(foodPos.x, 1.0, foodPos.y), glm::vec3(0.5f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(foodPos.x, 0.0, foodPos.y), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(21.0, 0.0, 11.0), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(-21.0, 0.0, 11.0), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(-21.0, 0.0, -11.0), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Renderer->DrawSprite(texture_face, glm::vec3(21.0, 0.0, -11.0), glm::vec3(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

}