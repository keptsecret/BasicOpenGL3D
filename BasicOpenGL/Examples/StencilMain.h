#pragma once
#include "BasicOpenGL/Camera.h"
#include "BasicOpenGL/InputManager.h"

#include <string>
#include <iostream>

#include <SDL/SDL.h>

class StencilMain {
public:
	StencilMain();

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
