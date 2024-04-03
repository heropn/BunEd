#pragma once
#include <string>

class Texture3D
{
public:

	Texture3D(const std::vector<std::string>& filePaths);
	~Texture3D();

	void Bind(int32_t index);
	void Unbind();
	
private:

	uint32_t m_ID = 0;
	int32_t m_BoundIndex = -1;
};
