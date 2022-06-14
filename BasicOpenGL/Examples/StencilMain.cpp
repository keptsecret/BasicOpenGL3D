#include "StencilMain.h"

#include "BasicOpenGL/Shader.h"
#include "BasicOpenGL/stb_image.h"

#include <glew/glew.h>

StencilMain::StencilMain() :
		camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f)) {
}

int StencilMain::start() {
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
	 * Set scene: 2 cubes and a floor
	 */

	Shader shader("./Shaders/testing_shader.vert", "./Shaders/testing_shader.frag");
	Shader stencilShader("./Shaders/testing_shader.vert", "./Shaders/stencil_shader.frag");

	float cubeVertices[] = {
		// positions          // texture Coords
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
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	// cube vertex data
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// plane vertex data
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int textureMarble = generateTexture("./Textures/marble.jpg");
	unsigned int textureMetal = generateTexture("./Textures/metal.png");

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(screen_width) / screen_height, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		stencilShader.use();
		stencilShader.setMat4("projection", projection);
		stencilShader.setMat4("view", view);

		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// make sure to not draw plane stencil
		glStencilMask(0x00);

		// draw plane
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMetal);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// set initial mask of cubes
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// draw cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMarble);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// set mask of outline
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		// draw outlines by scaling the cubes up
		stencilShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.1f));
		stencilShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f));
		stencilShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		SDL_GL_SwapWindow(sdl_window);
	}

	SDL_Quit();
	return 0;
}

void StencilMain::processInput(SDL_Window* window, float deltaTime) {
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

unsigned int StencilMain::generateTexture(const std::string& filepath) {
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