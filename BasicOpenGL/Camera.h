#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	Camera(glm::vec3 pos, glm::vec3 u, float y, float p) :
			position(pos), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(u), yaw(y), pitch(p), speed(2.5f), sensitivity(0.1f), zoomLevel(45.0f) {
		update();
	}

	float getZoom() const { return zoomLevel; }
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getDirection() const { return front; }

	glm::mat4 getViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}

	void update() {
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(direction);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	void move(CameraMovement mvmt, float deltaTime) {
		float velocity = speed * deltaTime;
		switch (mvmt) {
			case FORWARD:
				position += front * velocity;
				break;
			case BACKWARD:
				position -= front * velocity;
				break;
			case LEFT:
				position -= right * velocity;
				break;
			case RIGHT:
				position += right * velocity;
				break;
		}
	}

	void rotate(float xoffset, float yoffset, bool constrainPitch = true) {
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (constrainPitch) {
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		update();
	}

	void zoom(float yoffset) {
		zoomLevel -= yoffset;
		if (zoomLevel < 1.0f)
			zoomLevel = 1.0f;
		if (zoomLevel > 45.0f)
			zoomLevel = 45.0f; 
	}

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float speed;
	float sensitivity;
	float zoomLevel;
};