#pragma once
#include <memory>
#include <string>

/*
todo: Delete invalid textures, do not compare string, but some kind of id
*/

class Texture2D;

class TexturesManager
{
public:

	TexturesManager(const TexturesManager&) = delete;
	TexturesManager& operator=(const TexturesManager&) = delete;

	std::shared_ptr<Texture2D> CreateTexture2D(int32_t width, int32_t height, int32_t format, int32_t multisampleCount = 1);
	std::shared_ptr<Texture2D> FindOrCreateTexture2D(const std::string& filePath);
	std::shared_ptr<Texture2D> FindTexture2D(const std::string& filePath);

	static TexturesManager& Get() { return s_Instance; }

private:

	TexturesManager() = default;

	static TexturesManager s_Instance;

	std::vector<std::weak_ptr<Texture2D>> m_Textures2D;
};
