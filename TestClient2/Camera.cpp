#include "stdafx.h"
#include "Camera.h"


namespace Sapphire
{

	 

	Camera::~Camera()
	{

	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch /*= true*/)
	{
		xoffset *= getMouseSensitivity();
		yoffset *= getMouseSensitivity();

		m_yaw += xoffset;
		m_pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		UpdateCamera();
	}

	void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		{
			float velocity = getMoveSpeed() * deltaTime;
			if (direction == FORWARD)
				m_position += m_forward * velocity;
			if (direction == BACKWARD)
				m_position -= m_forward * velocity;
			if (direction == LEFT)
				m_position -= m_right * velocity;
			if (direction == RIGHT)
				m_position += m_right * velocity;
		}
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		{
			if (getZoom() >= 1.0f && getZoom() <= 45.0f)
				setZoom(getZoom() - yoffset);
			if (getZoom() <= 1.0f)
				setZoom(1.0f);
			if (getZoom() >= 45.0f)
				setZoom(45.0f);
		}
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	void Camera::UpdateCamera()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_forward = glm::normalize(front);
		// Also re-calculate the Right and Up vector
	    m_right = glm::normalize(glm::cross(m_forward, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_forward));
	}

}