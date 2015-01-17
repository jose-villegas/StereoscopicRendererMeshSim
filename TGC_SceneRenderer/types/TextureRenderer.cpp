#include "TextureRenderer.h"
using namespace types;

TextureRenderer::TextureRenderer(void)
{
    this->depthRenderBufferId = 0;
    this->frameBufferId = 0;
    this->enableDepthBuffer = false;
    this->depthTexture = nullptr;
    this->width = this->height = 0;
}


TextureRenderer::~TextureRenderer(void)
{
}

bool types::TextureRenderer::createRenderTarget(const unsigned int width, const unsigned int height)
{
    // store framebuffer render size
    this->width = width;
    this->height = height;
    // create framebuffer render target
    glGenFramebuffers(1, &this->frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferId);
    // check frame buffer status
    bool status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
    // unbind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // return creation status
    return status;
}

void types::TextureRenderer::addColorAttachment(
    const Texture::TextureFilteringMode min /*= Nearest*/,
    const Texture::TextureFilteringMode mag /*= Nearest*/,
    const Texture::TextureWrappingMode sWrap /*= ClampToEdge*/,
    const Texture::TextureWrappingMode tWrap /*= ClampToEdge*/,
    const bool generateMipmaps /*= false*/,
    const unsigned int readType /*= GL_UNSIGNED_BYTE*/,
    const unsigned internalFormat /*= GL_RGB*/,
    const unsigned format /*= GL_RGB */)
{
    if (this->frameBufferId == 0) { return; }

    Texture *newTex = new Texture();
    newTex->createTexture(this->width, this->height, min, mag, sWrap, tWrap, generateMipmaps, readType, internalFormat, format, nullptr);
    // bind frame buffer to store texture attachment
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferId);
    // configure texture to framebuffer texture
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->drawBuffers.size(), newTex->getOGLTexId(), 0);
    // store color attachment to drawbuffers
    this->drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + this->drawBuffers.size());
    glDrawBuffers(this->drawBuffers.size(), &this->drawBuffers[0]);
    // store texture pointer
    this->colorAttachments.push_back(newTex);
    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void types::TextureRenderer::attachDepthTexture()
{
    // no framebuffer set or depth component already attached to framebuffer
    if (this->enableDepthBuffer || this->frameBufferId == 0) { return; }

    this->enableDepthBuffer = true;
    depthTexture = new Texture();
    depthTexture->createTexture(this->width, this->height,
                                Texture::TextureFilteringMode::Nearest,
                                Texture::TextureFilteringMode::Nearest,
                                Texture::TextureWrappingMode::ClampToEdge,
                                Texture::TextureWrappingMode::ClampToEdge, false, GL_FLOAT,
                                GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT,
                                nullptr);
    // bind frame buffer to set depth texture
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferId);
    // associate depth texture with framebuffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getOGLTexId(), 0);
    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void types::TextureRenderer::attachDepthRenderBuffer()
{
    if (this->frameBufferId == 0 || this->enableDepthBuffer) { return; }

    this->enableDepthBuffer = true;
    // bind frame buffer to set renderbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferId);
    // create depth render buffer
    glGenRenderbuffers(1, &this->depthRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depthRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthRenderBufferId);
    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void types::TextureRenderer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferId);
}

void types::TextureRenderer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

