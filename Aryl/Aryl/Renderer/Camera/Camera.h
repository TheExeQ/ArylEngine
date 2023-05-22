#pragma once

#include <glm/glm.hpp>

namespace Aryl
{
	class Camera
	{
	public:
		Camera(float degFov, float aspectRatio, float nearZ, float farZ);
		Camera(float left, float right, float bottom, float top, float nearZ, float farZ);

		const glm::vec3& GetPosition() const { return myPosition; }
		void SetPosition(const glm::vec3& position) { myPosition = position; RecalculateViewMatrix(); }

		float GetRotation() const { return myRotation; }
		void SetRotation(float rotation) { myRotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return myProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return myViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return myViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 myProjectionMatrix = glm::mat4(1.f);
		glm::mat4 myViewMatrix = glm::mat4(1.f);
		glm::mat4 myViewProjectionMatrix = glm::mat4(1.f);

		glm::vec3 myPosition = glm::vec3(0.f);
		float myRotation = 0.0f;
	};
}