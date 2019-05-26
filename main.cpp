#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

//#include "Game/game.h"
#include "Game/SnakeGame.h"
#include "lib/resource_manager/resource_manager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define MAX_POINT 42000
#define SCREEN_W 800
#define SCREEN_H 600

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//void processInput(GLFWwindow *window);

const char* glsl_version = "#version 330";

//���ڼ�����֡ʱ���
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//���ڼ�������ƶ�
float last_mouseX = 400.0f, last_mouseY = 300.0f, yaw = -90.0f, pitch = 0.0f;
bool firstMove = true;


SnakeGame Snake(SCREEN_W, SCREEN_H);

int main()
{
	//ʵ����GLFW����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Snake", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//����������������Ϊ��ǰ�߳���������
	glfwMakeContextCurrent(window);
	//ע�ᴰ�����Żص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//ע������ƶ��ص�����
	glfwSetCursorPosCallback(window, mouse_callback);
	//ע����̻ص�����
	glfwSetKeyCallback(window, key_callback);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// ���޳�
	//glEnable(GL_CULL_FACE);
	// ��ȼ��
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// init game
	Snake.Init();
	Snake.State = GAME_ACTIVE;

	int frame_count = -1;

	while (!glfwWindowShouldClose(window))
	{
		//����ʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		//��������
		//processInput(window);

		Snake.ProcessInput(deltaTime, frame_count);
		frame_count++;
		if (frame_count >= 5) {
			Snake.Update(deltaTime);
			frame_count = 0;
		}
		
		glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Snake.Render(frame_count);

		glfwSwapBuffers(window);
		//glfwPollEvents();
	}

	//������Դ
	ResourceManager::Clear();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Snake.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Snake.Keys[key] = GL_FALSE;
	}

	/*if (action == GLFW_PRESS) {
		Snake.ProcessInput(deltaTime);
	}*/
	
}

/*
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float speed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

	}

}
*/

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMove) {
		last_mouseX = xpos;
		last_mouseY = ypos;
		firstMove = false;
	}

	yaw += (xpos - last_mouseX) * 0.05;
	pitch += (last_mouseY - ypos) * 0.05;
	pitch = pitch > 89.0f ? 89.0f : pitch;
	pitch = pitch < -89.0f ? -89.0f : pitch;

	last_mouseX = xpos;
	last_mouseY = ypos;


}
