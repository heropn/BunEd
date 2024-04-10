#include "pch.h"
#include "TexturesManager.h"
#include "Texture2D.h"

TexturesManager TexturesManager::s_Instance;

std::shared_ptr<Texture2D> TexturesManager::CreateTexture2D(int32_t width, int32_t height, int32_t format, int32_t multisampleCount)
{
	return std::make_shared<Texture2D>(width, height, format, multisampleCount);
}

std::shared_ptr<Texture2D> TexturesManager::FindOrCreateTexture2D(const std::string& filePath)
{
	std::shared_ptr<Texture2D> texture = FindTexture2D(filePath);

	if (texture)
	{
		return texture;
	}

	texture = std::make_shared<Texture2D>(filePath);
	m_Textures2D.emplace_back(texture);
	return texture;
}

std::shared_ptr<Texture2D> TexturesManager::FindTexture2D(const std::string& filePath)
{
	for (const auto& it : m_Textures2D)
	{
		if (!it.expired())
		{
			std::shared_ptr<Texture2D> candidate = it.lock();
			if (candidate->GetFilePath().compare(filePath.c_str()) == 0)
			{
				return candidate;
			}
		}
	}

	return nullptr;
}
