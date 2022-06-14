#include "BasicMain.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BasicOpenGL/stb_image.h"

BasicMain::BasicMain() :
		camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f)) {}

int BasicMain::start() {
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
	std::printf("***    OpenGL version: %s    ***\n", glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(0); // set vsync on (1), off (0)

	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader texShader("./Shaders/tex_shader.vert", "./Shaders/tex_shader.frag");

	// use a texture
	unsigned int texture1;
	generateTexture(texture1, "./Textures/container.jpg", GL_RGB);

	// generate another texture for texture unit
	unsigned int texture2;
	generateTexture(texture2, "./Textures/awesomeface.png", GL_RGBA);

	texShader.use();
	texShader.setInt("texture1", 0);
	texShader.setInt("texture2", 1);
	texShader.setFloat("mixing", 0.3f);

	/*-----------------------------------------------------------------------------------*/
	/*
	 * Draw some cubes
	 */

	float verticesCube[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	unsigned int VAOCube;
	glGenVertexArrays(1, &VAOCube);
	glBindVertexArray(VAOCube);

	unsigned int VBOCube;
	glGenBuffers(1, &VBOCube);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// draw lots of cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// camera and other variables
	const float radius = 10.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// run in loop until close
	while (run) {
		float currFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;
		inputManager.update();
		processInput(sdl_window, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		texShader.use();
		
		glBindVertexArray(VAOCube);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 view = camera.getViewMatrix();
			texShader.setMat4("view", view);

			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 3 == 0) {
				angle = SDL_GetTicks() * 25.0f / 1000.0f;
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			texShader.setMat4("model", model);

			glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(screen_width) / screen_height, 0.1f, 100.0f);
			texShader.setMat4("projection", projection);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		SDL_GL_SwapWindow(sdl_window);
	}

	// unbind and delete stuff
	glDeleteVertexArrays(1, &VAOCube);
	glDeleteBuffers(1, &VBOCube);

	SDL_Quit();
	return 0;
}

void BasicMain::processInput(SDL_Window* window, float deltaTime) {
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

void BasicMain::generateTexture(unsigned int& texture, const std::string& filepath, GLint format) {
	// generate texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "ERROR::Texture: Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
