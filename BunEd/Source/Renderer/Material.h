#pragma once
#include <memory>

class Shader;

struct Material
{
	std::shared_ptr<Shader> m_Shader = nullptr;
	
	bool m_IsTranslucent = false;
};
