#pragma once
#include <string>

class Texture2D
{
public:

	Texture2D(const std::string& filePath);
	~Texture2D();

	void Bind(int index);
	void Unbind();

	const std::string& GetFilePath() const { return m_FilePath; }

private:

	void GenerateTexture(const unsigned char* data);

#ifdef _DEBUG
	std::string m_FilePath;
#endif

	int m_Height = 0;
	int m_Width = 0;
	int m_CompNum = 0;
	int m_BoundIndex = -1;
	unsigned int m_ID = 0;
};
