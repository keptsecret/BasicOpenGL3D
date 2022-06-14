#pragma once
#include <SDL/SDL.h>

#include "BasicOpenGL/Camera.h"
#include "BasicOpenGL/InputManager.h"
#include "BasicOpenGL/Shader.h"

class ModelMain {
	ModelMain();

	int start();

private:
	bool run = true;
	bool firstMouse = true;

	const int screen_width = 800;
	const int screen_height = 600;

	InputManager inputManager;
	Camera camera;

	void processInput(SDL_Window* window, float deltaTime);
	unsigned int generateTexture(const std::string& filepath);
};
