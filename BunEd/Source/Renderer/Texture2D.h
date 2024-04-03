#pragma once
#include <string>

class Texture2D
{
public:

	Texture2D(const std::string& filePath);
	Texture2D(int32_t width, int32_t height, int32_t format);
	~Texture2D();

	void Bind(int32_t index);
	void Unbind();

#ifdef _DEBUG
	const std::string& GetFilePath() const { return m_FilePath; }
#endif

private:

	friend class FrameBuffer;

	void GenerateTexture(const unsigned char* data);

#ifdef _DEBUG
	std::string m_FilePath;
#endif

	int32_t m_Height = 0;
	int32_t m_Width = 0;
	int32_t m_Format = 0;
	int32_t m_BoundIndex = -1;
	uint32_t m_ID = 0;
};
