#include "InputManager.h"

InputManager::InputManager() : mouseCoords(0.0f) {
}

InputManager::~InputManager() {
}

void InputManager::update() {
	for (auto& it : keyMap) {
		prevKeyMap[it.first] = it.second;
	}
}

void InputManager::pressKey(unsigned key_id) {
	keyMap[key_id] = true;
}

void InputManager::releaseKey(unsigned key_id) {
	keyMap[key_id] = false;
}

bool InputManager::isKeyDown(unsigned key_id) {
	auto it = keyMap.find(key_id);
	if (it != keyMap.end()) {
		return it->second;
	}
	return false;
}

bool InputManager::isKeyPressed(unsigned key_id) {
	if (isKeyDown(key_id) && wasKeyDown(key_id)) {
		return true;
	}
	return false;
}

void InputManager::setMouseCoords(float x, float y) {
	mouseCoords.x = x;
	mouseCoords.y = y;
}

bool InputManager::wasKeyDown(unsigned key_id) {
	auto it = prevKeyMap.find(key_id);
	if (it != prevKeyMap.end()) {
		return it->second;
	}
	return false;
}
