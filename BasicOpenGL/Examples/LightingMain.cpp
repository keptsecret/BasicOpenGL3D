#include "LightingMain.h"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BasicOpenGL/stb_image.h"

LightingMain::LightingMain() :
		camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f)) {}

int LightingMain::start() {
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

	/*-----------------------------------------------------------------------------------*/
	/*
	 * Boxes with lights
	 */

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	Shader lightingShader("./Shaders/lighting_shader.vert", "./Shaders/multi_lighting_shader.frag");
	Shader lightsShader("./Shaders/baby_shader.vert", "./Shaders/light_shader.frag");

	unsigned int boxTexture = generateTexture("./Textures/container2.png");
	unsigned int boxSpecular = generateTexture("./Textures/container2_specular.png");

	// set material properties
	lightingShader.use();
	lightingShader.setFloat("material.shininess", 64.0f);
	lightingShader.setInt("material.diffuseMap", 0);
	lightingShader.setInt("material.specularMap", 1);

	// make box vao and vbo
	unsigned int VAOCube, VBO;
	glGenVertexArrays(1, &VAOCube);
	glBindVertexArray(VAOCube);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// separate vao for lights but using same geometry
	unsigned int VAOLight;
	glGenVertexArrays(1, &VAOLight);
	glBindVertexArray(VAOLight);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearDepth(1.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw cube object
		// set light settings that affect cube color
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.getPosition());
		lightingShader.setInt("light.type", 2);

		// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.2f, 0.3f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09f);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.6f, 0.9f, 0.1f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09f);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.1f, 0.6f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09f);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
		// spot light
		lightingShader.setVec3("spotLight.position", camera.getPosition());
		lightingShader.setVec3("spotLight.direction", camera.getDirection());
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.07f);
		lightingShader.setFloat("spotLight.quadratic", 0.017f);
		lightingShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));

		// set texture maps
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, boxSpecular);

		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(screen_width) / screen_height, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glBindVertexArray(VAOCube);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// draw light objects (also a cube but emits light)
		lightsShader.use();
		lightsShader.setMat4("projection", projection);
		lightsShader.setMat4("view", view);

		glBindVertexArray(VAOLight);
		for (glm::vec3& pointLightPosition : pointLightPositions) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPosition);
			model = glm::scale(model, glm::vec3(0.2f));
			lightsShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		SDL_GL_SwapWindow(sdl_window);
	}

	// unbind and delete stuff
	glDeleteVertexArrays(1, &VAOCube);
	glDeleteVertexArrays(1, &VAOLight);
	glDeleteBuffers(1, &VBO);

	SDL_Quit();
	return 0;
}

void LightingMain::processInput(SDL_Window* window, float deltaTime) {
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

unsigned int LightingMain::generateTexture(const std::string& filepath) {
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
