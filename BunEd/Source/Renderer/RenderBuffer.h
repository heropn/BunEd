#pragma once

class RenderBuffer
{
public:

	RenderBuffer(int32_t width, int32_t height, uint32_t rbFormat);
	~RenderBuffer();

	RenderBuffer(const RenderBuffer&) = delete;
	RenderBuffer(const RenderBuffer&&) = delete;
	RenderBuffer& operator=(const RenderBuffer&) = delete;

	void Bind() const;
	void Unbind() const;

private:

	friend class FrameBuffer;

	uint32_t m_ID = 0;
};
