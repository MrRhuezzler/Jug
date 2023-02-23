#include <GL/glew.h>

#include "stb/stb_image_write.h"

#include "jug/image.h"

namespace Jug
{
    Image::Image(int width, int height)
        : width(width), height(height)
    {
        allocateMemory();
    }

    Image::~Image()
    {
    }

    void Image::bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Image::unBind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Image::saveData(const char *filename, int w, int h, int comp, const void *data, int stride)
    {
        stbi_flip_vertically_on_write(1);
        stbi_write_png(filename, w, h, comp, data, stride);
    }

    void Image::resize(int nWidth, int nHeight)
    {
        if (width == nWidth && height == nHeight)
            return;

        width = nWidth;
        height = nHeight;

        relaseMemory();
        allocateMemory();
    }

    void Image::setData(const void *data)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    unsigned int Image::getDescriptor() const
    {
        return id;
    }

    int Image::getWidth() const
    {
        return width;
    }

    int Image::getHeight() const
    {
        return height;
    }

    void Image::allocateMemory()
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    void Image::relaseMemory()
    {
        glDeleteTextures(1, &id);
        id = 0;
    }
}