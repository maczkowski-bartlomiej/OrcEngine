#pragma once

#include "Math/Vector2.hpp"

#include <glm/glm.hpp>

namespace orc {

class Camera
{
public:
	Camera(float left, float right, float bottom, float top);

	void setRotation(float rotation);

	void setPosition(float x, float y);
	void setPosition(const Vector2f& position);

	float getRotation() const;
	Vector2f getPosition() const;
	const glm::mat4& getViewProjectionMatrix() const;

private:
	void recalculateViewMatrix();

	float m_rotation;

	glm::vec3 m_position; 
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewProjectionMatrix;
};

}
