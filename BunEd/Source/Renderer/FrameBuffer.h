#pragma once
#include <memory>

class Texture2D;
class RenderBuffer;

/*
todo:
- Add support for adding multiple post process effects
*/

class FrameBuffer
{
public:

	FrameBuffer() = default;
	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer(const FrameBuffer&&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	~FrameBuffer();

	static std::shared_ptr<FrameBuffer> CreateColorDepthStencilFrameBuffer(int32_t width, int32_t height, int32_t multiSampleCount = 1);

	void Bind(uint32_t target = GL_FRAMEBUFFER) const;
	void Unbind() const;

	const std::shared_ptr<Texture2D>& GetColorBufferTexture() const { return m_ColorBuffer; }
	uint32_t GetStencilBufferTextureViewId() const { return m_StencilViewID; }

private:

	std::shared_ptr<Texture2D> m_ColorBuffer = nullptr;
	std::shared_ptr<Texture2D> m_DepthStencilBuffer = nullptr;

	uint32_t m_StencilViewID = 0;
	uint32_t m_ID = 0;

};
