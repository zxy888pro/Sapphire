#pragma once
#include "stdafx.h"
#include <singleton.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Sapphire
{
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;


	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera: public Singleton<Camera>
	{
	public:

		

		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_moveSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
		{
			setPosition(position);
			m_worldUp = up;
			m_yaw = yaw;
			m_pitch = pitch;
			UpdateCamera();
		}
		 
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_moveSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
		{
			setPosition(glm::vec3(posX, posY, posZ));
			m_worldUp = glm::vec3(upX, upY, upZ);;
			m_yaw = yaw;
			m_pitch = pitch;
			UpdateCamera();
		}
		virtual ~Camera();

		glm::vec3 getPosition() const { return m_position; }
		void setPosition(glm::vec3 val) { m_position = val; }

		glm::vec3 getForward() const { return m_forward; }
		void setForward(glm::vec3 val) { m_forward = val; }

		glm::vec3 getUp() const { return m_worldUp; }
		void setUp(glm::vec3 val) { m_worldUp = val; }


		glm::vec3 getRight() const { return m_right; }
		void setRight(glm::vec3 val) { m_right = val; }

		
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

		void ProcessKeyboard(Camera_Movement direction, float deltaTime);
		
		void ProcessMouseScroll(float yoffset);
		


		glm::mat4 GetViewMatrix();

		float getZoom() const { return m_zoom; }
		void setZoom(float val) { m_zoom = val; }
		float getMoveSpeed() const { return m_moveSpeed; }
		void setMoveSpeed(float val) { m_moveSpeed = val; }

		float getMouseSensitivity() const { return m_mouseSensitivity; }
		void setMouseSensitivity(float val) { m_mouseSensitivity = val; }
	private:

		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		float m_moveSpeed;
		float m_mouseSensitivity;
		float m_zoom;



		void UpdateCamera();


	};
}

