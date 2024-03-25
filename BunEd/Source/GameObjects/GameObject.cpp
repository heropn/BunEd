#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
	: m_Transform(glm::identity<glm::mat4x4>())
{
}
