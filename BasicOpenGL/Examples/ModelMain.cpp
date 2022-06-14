#include "ModelMain.h"

#include <string>
#include <iostream>

#include "BasicOpenGL/Model.h"
#include "BasicOpenGL/stb_image.h"

ModelMain::ModelMain() :
		camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f)) {
}

int ModelMain::start() {
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_ShowCursor(SDL_DISABLE);

	// create a window
	const std::string window_name = "SDL Window";
	Uint32 flags = SDL_WINDOW_OPENGL;

	SDL_Window* sdl_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, flags);

	if (sdl_window == nullptr) {
		printf("SDL Window could not be created!");
		return -1;
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);
	if (gl_context == nullptr) {
		printf("SDL Context could not be created!");
		return -1;
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("Could not initialize glew!");
		return -1;
	}

	// check opengl version
	printf("***    OpenGL version: %s    ***\n", glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(0); // set vsync on (1), off (0)

	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);

	/*-----------------------------------------------------------------------------------*/
	/*
	 * Initialize test model
	 */

	Shader modelShader("./Shaders/lighting_shader.vert", "./Shaders/lighting_shader.frag");
	Model backpackModel("./Assets/backpack/backpack.obj");

	// camera and other variables
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// run in loop until close
	while (run) {
		float currFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;
		inputManager.update();
		processInput(sdl_window, deltaTime);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClearDepth(1.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.use();
		modelShader.setInt("light.type", 1);
		modelShader.setVec3("light.position", 0.2f, 0.1f, 1.0f);
		modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		modelShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
		modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("light.constant", 1.0f);
		modelShader.setFloat("light.linear", 0.07f);
		modelShader.setFloat("light.quadratic", 0.017f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(screen_width) / screen_height, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		modelShader.setMat4("model", model);
		backpackModel.draw(modelShader);

		SDL_GL_SwapWindow(sdl_window);
	}

	SDL_Quit();
	return 0;
}

void ModelMain::processInput(SDL_Window* window, float deltaTime) {
	SDL_Event event;

	float xoff, yoff;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				run = false;
				break;
			case SDL_KEYDOWN:
				inputManager.pressKey(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				//inputManager.setMouseCoords(event.motion.xrel, event.motion.yrel);
				xoff = event.motion.x - screen_width / 2;
				yoff = screen_height / 2 - event.motion.y;

				camera.rotate(xoff, yoff);
				SDL_WarpMouseInWindow(window, screen_width / 2, screen_height / 2);
				break;
			case SDL_MOUSEWHEEL:
				// zoom in on scroll
				camera.zoom(static_cast<float>(event.wheel.y));
				break;
		}
	}

	if (inputManager.isKeyPressed(SDLK_w)) {
		camera.move(FORWARD, deltaTime);
	}
	if (inputManager.isKeyPressed(SDLK_s)) {
		camera.move(BACKWARD, deltaTime);
	}
	if (inputManager.isKeyPressed(SDLK_a)) {
		camera.move(LEFT, deltaTime);
	}
	if (inputManager.isKeyPressed(SDLK_d)) {
		camera.move(RIGHT, deltaTime);
	}
}

unsigned int ModelMain::generateTexture(const std::string& filepath) {
	// generate texture
	unsigned int texture;
	glGenTextures(1, &texture);

	// load texture data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		GLint format;
		if (nrChannels == 1) {
			format = GL_RED;
		} else if (nrChannels == 3) {
			format = GL_RGB;
		} else if (nrChannels == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		std::cout << "ERROR::Texture: Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}