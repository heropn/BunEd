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
	int compNum = 0;
	unsigned char* imageData = stbi_load(filePath.c_str(), &m_Width, &m_Height, &compNum, 4);

	if (imageData)
	{
		switch (compNum)
		{
		case 1:
			m_Format = GL_RED;
			break;
		case 3:
			m_Format = GL_RGB;
			break;
		case 4:
			m_Format = GL_RGBA;
			break;
		default:
			printf("Unknown number of components in texture");
			__debugbreak();
			break;
		}

		GenerateTexture(imageData);
		stbi_image_free(imageData);
	}
	else
	{
		printf("Unable to load image data from file %s", filePath.c_str());
	}
}

Texture2D::Texture2D(int32_t width, int32_t height, int32_t format)
{
	m_Width = width;
	m_Height = height;
	m_Format = format;

	GenerateTexture(nullptr);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (m_Format == GL_DEPTH24_STENCIL8)
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, m_Format, m_Width, m_Height);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, data);

	}

	//glGenerateMipmap(GL_TEXTURE_2D);
}
