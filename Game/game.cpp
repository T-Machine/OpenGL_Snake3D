#include "game.h"
#include "../lib/resource_manager/resource_manager.h"
#include "../lib/sprite_renderer/sprite_renderer.h"


SpriteRenderer  *Renderer;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("Game/shaders/sprite.vs", "Game/shaders/sprite.frag", nullptr, "sprite");

	// Configure shaders
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("sprite").SetMatrix4("view", view);

	// Load textures
	ResourceManager::LoadTexture("resources/worry.jpg", GL_TRUE, "face");

	// Set render-specific controls
	Shader spiritShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(spiritShader);
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
	Texture2D texture = ResourceManager::GetTexture("face");
	Renderer->DrawSprite(texture, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(texture, glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(texture, glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	for (float i = -15.0f; i <= 15.0f; i += 1.0f) {
		Renderer->DrawSprite(texture, glm::vec3(i, 10.0, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer->DrawSprite(texture, glm::vec3(i, -10.0, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	for (float i = -10.0f; i <= 10.0f; i += 1.0f) {
		Renderer->DrawSprite(texture, glm::vec3(15.0, i, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer->DrawSprite(texture, glm::vec3(-15.0, i, 0.0), glm::vec3(0.15f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}