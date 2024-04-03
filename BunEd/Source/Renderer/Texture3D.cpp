#include "pch.h"
#include "Texture3D.h"
#include "stb/stb_image.h"

Texture3D::Texture3D(const std::vector<std::string>& filePaths)
{
	assert(filePaths.size() == 6);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

	int32_t width, height, compNum;

	for (int32_t i = 0; i < filePaths.size(); i++)
	{
		unsigned char* data = stbi_load(filePaths[i].c_str(), &width, &height, &compNum, 3);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("Unable to open file: %s\n", filePaths[i].c_str());
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture3D::~Texture3D()
{
	glDeleteTextures(1, &m_ID);
}

void Texture3D::Bind(int32_t index)
{
	m_BoundIndex = index;
	glBindTextureUnit(m_BoundIndex, m_ID);
}

void Texture3D::Unbind()
{
	glBindTextureUnit(m_BoundIndex, 0);
	m_BoundIndex = -1;
}
