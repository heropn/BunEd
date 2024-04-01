#include "pch.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "TexturesManager.h"
#include "Texture2D.h"

std::shared_ptr<FrameBuffer> FrameBuffer::CreateColorDepthStencilFrameBuffer(int32_t width, int32_t height)
{
	std::shared_ptr<FrameBuffer> frameBuffer = std::make_shared<FrameBuffer>();

	glGenFramebuffers(1, &frameBuffer->m_ID);
	frameBuffer->Bind();

	frameBuffer->m_ColorBuffer = TexturesManager::Get().CreateTexture2D(width, height, GL_RGB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->m_ColorBuffer->m_ID, 0);

	frameBuffer->m_DepthStencilBuffer = std::make_shared<RenderBuffer>(width, height, GL_DEPTH24_STENCIL8);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->m_DepthStencilBuffer->m_ID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Invalid framebuffer\n");
		frameBuffer->Unbind();
		assert(false);
		return nullptr;
	}

	frameBuffer->Unbind();

	return frameBuffer;
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
