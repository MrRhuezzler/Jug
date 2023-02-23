#pragma once

#include <stdint.h>

namespace Jug
{
    class Image
    {
    private:
        unsigned int id;
        int width, height, channels;

    public:
        Image(int width, int height);
        ~Image();

        void bind();
        void unBind();

        void resize(int nWidth, int nHeight);
        void setData(const void *data);

        unsigned int getDescriptor() const;
        int getWidth() const;
        int getHeight() const;

        static void saveData(const char *filename, int w, int h, int comp, const void *data, int stride);

    private:
        void allocateMemory();
        void relaseMemory();
    };

} // namespace Image
