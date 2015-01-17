#pragma once
#include "Texture.h"
#include <vector>
namespace types {

    class TextureRenderer {

        private:
            // render data
            unsigned int frameBufferId;
            unsigned int width;
            unsigned int height;
            // color attachments
            std::vector<unsigned int> drawBuffers;
            std::vector<Texture *> colorAttachments;
            // depth component
            bool enableDepthBuffer;
            Texture *depthTexture;
            unsigned int depthRenderBufferId;

        public:

            TextureRenderer(void);
            ~TextureRenderer(void);

            bool createRenderTarget(const unsigned int width, const unsigned int height);
            void addColorAttachment(const Texture::TextureFilteringMode min = Texture::TextureFilteringMode::Nearest,
                                    const Texture::TextureFilteringMode mag = Texture::TextureFilteringMode::Nearest,
                                    const Texture::TextureWrappingMode sWrap = Texture::TextureWrappingMode::ClampToEdge,
                                    const Texture::TextureWrappingMode tWrap = Texture::TextureWrappingMode::ClampToEdge,
                                    const bool generateMipmaps = false,
                                    const unsigned int readType = GL_UNSIGNED_BYTE,
                                    const unsigned internalFormat = GL_RGB,
                                    const unsigned format = GL_RGB
                                   );
            // attach depth component to a texture, can't use this if rendering depth to a render buffer
            void attachDepthTexture();
            // attach depth component to a render buffer, can't use this if rendering depth to texture
            void attachDepthRenderBuffer();
            // binds the associated framebuffer object
            void bind();
            // unbinds this frame buffer object
            void unbind();
    };
}

