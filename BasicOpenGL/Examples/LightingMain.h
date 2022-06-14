#pragma once
#include <SDL/SDL.h>

#include "BasicOpenGL/Camera.h"
#include "BasicOpenGL/InputManager.h"
#include "BasicOpenGL/Shader.h"

class LightingMain {
public:
	LightingMain();

	int start();

private:
	bool run = true;
	bool firstMouse = true;

	const int screen_width = 720;
	const int screen_height = 500;

	Camera camera;

	InputManager inputManager;

	void processInput(SDL_Window* window, float deltaTime);
	unsigned int generateTexture(const std::string& filepath);
};
