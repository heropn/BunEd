#pragma once
#include <memory>

class Texture2D;
class RenderBuffer;

/*
todo:
- Create screen space outline using framebuffer
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

	// For now creates read/write color and write only depth/stencil
	static std::shared_ptr<FrameBuffer> CreateColorDepthStencilFrameBuffer(int32_t width, int32_t height);

	void Bind() const;
	void Unbind() const;

	const std::shared_ptr<Texture2D>& GetColorBufferTexture() const { return m_ColorBuffer; }
	const std::shared_ptr<Texture2D>& GetDepthStencilBufferTexture() const { return m_DepthStencilBuffer; }

	uint32_t m_StencilView = 0;

private:

	std::shared_ptr<Texture2D> m_ColorBuffer = nullptr;
	std::shared_ptr<Texture2D> m_DepthStencilBuffer = nullptr;

	uint32_t m_ID = 0;

};
