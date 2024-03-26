#include "pch.h"
#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture2D::Texture2D(const std::string& filePath)
#ifdef _DEBUG
	: m_FilePath(filePath)
#endif
{
	stbi_set_flip_vertically_on_load(true);
	// Currently 4-components, because some textures were invalid
	unsigned char* imageData = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_CompNum, 4);

	if (imageData)
	{
		GenerateTexture(imageData);
		stbi_image_free(imageData);
	}
	else
	{
		printf("Unable to load image data from file %s", filePath.c_str());
	}
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_ID);
}

void Texture2D::Bind(int index)
{
	m_BoundIndex = index;
	glBindTextureUnit(m_BoundIndex, m_ID);
}

void Texture2D::Unbind()
{
	glBindTextureUnit(m_BoundIndex, 0);
	m_BoundIndex = -1;
}

void Texture2D::GenerateTexture(const unsigned char* data)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format = GL_RGB;

	switch (m_CompNum)
	{
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		printf("Unknown number of components in texture");
		__debugbreak();
		break;
	}

	// Currently hardcoded, because some textures were displayed inproperly
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
