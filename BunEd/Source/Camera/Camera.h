#pragma once
#include "glm/glm.hpp"

namespace CameraMoveFlags
{
	enum Type
	{
		None = 1 << 0,
		Forward = 1 << 1,
		Back = 1 << 2,
		Right = 1 << 3,
		Left = 1 << 4
	};
}

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Init(glm::vec3 cameraPos, glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f));

	void AddMoveFlags(CameraMoveFlags::Type moveFlags);
	void RemoveMoveFlags(CameraMoveFlags::Type moveFlags);

	void HandleKeyEvent(int key, int action, int mods);
	void HandleMouseButtonEvent(int button, int action, int mods);
	void HandleMouseMoved(float lastX, float lastY, float newX, float newY);
	void HandleResizeCallback(int width, int height);
	void Update(float deltaTime);

	void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
	void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }

	inline const glm::mat4x4& GetViewMatrix() const { return m_ViewMatrix; };
	inline const glm::mat4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; };

	inline const glm::vec3& GetForwardVector() const { return m_ForwardVector; }
	inline const glm::vec3 GetRightVector() const { return glm::normalize(glm::cross(m_ForwardVector, m_UpVector)); }

	inline const glm::vec3 GetPosition() const { return m_Position; }

private:

	glm::mat4x4 m_ViewMatrix;
	glm::mat4x4 m_ProjectionMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_ForwardVector;
	glm::vec3 m_UpVector;

	float m_MoveSpeed = 25.0f;
	float m_RotationSpeed = 25.0f;

	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;

	float m_YawDelta = 0.0f;
	float m_PitchDelta = 0.0f;

	bool m_CanRotate = false;

	CameraMoveFlags::Type m_MoveFlags = CameraMoveFlags::Type::None;
};
