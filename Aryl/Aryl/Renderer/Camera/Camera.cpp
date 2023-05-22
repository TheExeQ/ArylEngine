#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Aryl
{
	Camera::Camera(float degFov, float aspectRatio, float nearZ, float farZ)
		: myProjectionMatrix(glm::perspective(glm::radians(degFov), aspectRatio, nearZ, farZ))
	{
		myViewProjectionMatrix = myProjectionMatrix * myViewMatrix;
	}

	Camera::Camera(float left, float right, float bottom, float top, float nearZ, float farZ)
		: myProjectionMatrix(glm::ortho(left, right, bottom, top, nearZ, farZ))
	{
		myViewProjectionMatrix = myProjectionMatrix * myViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), myPosition) * 
			glm::rotate(glm::mat4(1.f), glm::radians(myRotation), glm::vec3(0.f, 0.f, 1.f));

		myViewMatrix = glm::inverse(transform);
		myViewProjectionMatrix = myProjectionMatrix * myViewMatrix;
	}
}