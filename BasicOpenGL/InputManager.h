#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

class InputManager {
public:
	InputManager();
	~InputManager();

	void update();

	void pressKey(unsigned int key_id);
	void releaseKey(unsigned int key_id);
	// returns true if key is held down
	bool isKeyDown(unsigned int key_id);
	// returns true if key was just pressed
	bool isKeyPressed(unsigned int key_id);

	void setMouseCoords(float x, float y);
	glm::vec2 getMouseCoords() const { return mouseCoords; }

private:
	std::unordered_map<unsigned int, bool> keyMap;
	std::unordered_map<unsigned int, bool> prevKeyMap;
	glm::vec2 mouseCoords;

	bool wasKeyDown(unsigned int key_id);
};
