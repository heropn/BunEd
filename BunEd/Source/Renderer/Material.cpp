#include "pch.h"
#include "Material.h"
#include "Shader.h"

void Material::BindShader()
{
	if (m_Shader)
	{
		m_Shader->Bind();
	}
}
