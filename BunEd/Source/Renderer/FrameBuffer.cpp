#include "pch.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "TexturesManager.h"
#include "Texture2D.h"

std::shared_ptr<FrameBuffer> FrameBuffer::CreateColorDepthStencilFrameBuffer(int32_t width, int32_t height, int32_t multiSampleCount)
{
	std::shared_ptr<FrameBuffer> frameBuffer = std::make_shared<FrameBuffer>();

	glGenFramebuffers(1, &frameBuffer->m_ID);
	frameBuffer->Bind();

	uint32_t textureType = multiSampleCount > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	frameBuffer->m_ColorBuffer = TexturesManager::Get().CreateTexture2D(width, height, GL_RGB, multiSampleCount);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType, frameBuffer->m_ColorBuffer->m_ID, 0);

	//frameBuffer->m_DepthStencilBuffer = std::make_shared<RenderBuffer>(width, height, GL_DEPTH24_STENCIL8);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->m_DepthStencilBuffer->m_ID);

	frameBuffer->m_DepthStencilBuffer = TexturesManager::Get().CreateTexture2D(width, height, GL_DEPTH24_STENCIL8, multiSampleCount);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, textureType, frameBuffer->m_DepthStencilBuffer->m_ID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Invalid framebuffer\n");
		frameBuffer->Unbind();
		assert(false);
		return nullptr;
	}

	glGenTextures(1, &frameBuffer->m_StencilViewID);
	glTextureView(frameBuffer->m_StencilViewID, textureType, frameBuffer->m_DepthStencilBuffer->m_ID, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);
	glBindTexture(textureType, frameBuffer->m_StencilViewID);
	glTexParameteri(textureType, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);

	frameBuffer->Unbind();

	return frameBuffer;
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind(uint32_t target) const
{
	glBindFramebuffer(target, m_ID);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
