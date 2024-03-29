#include "pch.h"
#include "Camera.h"

Camera::Camera() :
	m_ViewMatrix(glm::identity<glm::mat4x4>()),
	m_ProjectionMatrix(glm::identity<glm::mat4x4>()),
	m_Position(glm::zero<glm::vec3>()),
	m_ForwardVector(glm::zero<glm::vec3>()),
	m_UpVector(glm::vec3(0.0f, 1.0f, 0.0f))
{
}

Camera::~Camera()
{

}

void Camera::Init(glm::vec3 cameraPos, glm::vec3 cameraTarget)
{
	m_Position = cameraPos;
	m_ForwardVector = glm::normalize(cameraTarget - m_Position);

	m_Pitch = asinf(m_ForwardVector.x);
	m_Yaw = asinf(m_ForwardVector.z / cosf(m_Pitch)) / (glm::pi<float>() / 180.0f);

	float width = static_cast<float>(Window::Get().GetWidth());
	float height = static_cast<float>(Window::Get().GetHeight());

	m_ProjectionMatrix = glm::perspectiveFovRH(m_FovAngle, width, height, m_NearPlane, m_FarPlane);

	Window::Get().m_KeyStateChangedDispatcher.Bind(CALLBACK_3(&Camera::HandleKeyEvent, this));
	Window::Get().m_CursorMovedDispatcher.Bind(CALLBACK_4(&Camera::HandleMouseMoved, this));
	Window::Get().m_MouseButtonStateChangedDispatcher.Bind(CALLBACK_3(&Camera::HandleMouseButtonEvent, this));
	Window::Get().m_WindowSizeChangedDispatcher.Bind(CALLBACK_2(&Camera::HandleResizeCallback, this));
}

void Camera::AddMoveFlags(CameraMoveFlags::Type moveFlags)
{
	m_MoveFlags = static_cast<CameraMoveFlags::Type>(m_MoveFlags | moveFlags);
}

void Camera::RemoveMoveFlags(CameraMoveFlags::Type moveFlags)
{
	m_MoveFlags = static_cast<CameraMoveFlags::Type>(m_MoveFlags & ~moveFlags);
}

void Camera::HandleKeyEvent(int key, int action, int mods)
{
	CameraMoveFlags::Type moveFlag = CameraMoveFlags::Type::None;

	switch (key)
	{
	case GLFW_KEY_W:
		moveFlag = CameraMoveFlags::Forward;
		break;
	case GLFW_KEY_S:
		moveFlag = CameraMoveFlags::Back;
		break;
	case GLFW_KEY_D:
		moveFlag = CameraMoveFlags::Right;
		break;
	case GLFW_KEY_A:
		moveFlag = CameraMoveFlags::Left;
		break;
	default:
		return;
	}

	if (action == GLFW_PRESS)
	{
		AddMoveFlags(moveFlag);
	}
	else if (action == GLFW_RELEASE)
	{
		RemoveMoveFlags(moveFlag);
	}
}

void Camera::HandleMouseButtonEvent(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		m_CanRotate = action != GLFW_RELEASE;
	}
}

void Camera::HandleMouseMoved(float lastX, float lastY, float newX, float newY)
{
	if (m_CanRotate)
	{
		m_YawDelta += newX - lastX;
		m_PitchDelta += newY - lastY;
	}
}

void Camera::HandleResizeCallback(int width, int height)
{
	m_ProjectionMatrix = glm::perspectiveFovRH(m_FovAngle, static_cast<float>(width), static_cast<float>(height), m_NearPlane, m_FarPlane);
}

void Camera::Update(float deltaTime)
{
	if (m_CanRotate && (m_YawDelta != 0.0f || m_PitchDelta != 0.0f))
	{
		m_Yaw += m_YawDelta * m_RotationSpeed * deltaTime;
		m_Pitch -= m_PitchDelta * m_RotationSpeed * deltaTime;

		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

		float radianPitch = m_Pitch * (glm::pi<float>() / 180.0f);
		float radianYaw = m_Yaw * (glm::pi<float>() / 180.0f);

		m_ForwardVector = glm::vec3(cosf(radianYaw) * cosf(radianPitch), sinf(radianPitch), sinf(radianYaw) * cosf(radianPitch));

		m_YawDelta = 0.0f;
		m_PitchDelta = 0.0f;
	}

	glm::vec3 moveDirection = glm::vec3(0.0f);

	if (m_MoveFlags & CameraMoveFlags::Forward)	moveDirection += GetForwardVector();
	if (m_MoveFlags & CameraMoveFlags::Back)	moveDirection -= GetForwardVector();
	if (m_MoveFlags & CameraMoveFlags::Right)	moveDirection += GetRightVector();
	if (m_MoveFlags & CameraMoveFlags::Left)	moveDirection -= GetRightVector();

	if (glm::dot(moveDirection, moveDirection) > 0.0f)
	{
		m_Position += moveDirection * m_MoveSpeed * deltaTime;
	}

	m_ViewMatrix = glm::lookAtRH(m_Position, m_Position + m_ForwardVector, m_UpVector);
}