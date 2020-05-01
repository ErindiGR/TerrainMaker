#ifndef TEXTURE_H
#define TEXTURE_H

#include "bitmap.h"

struct Texture
{
    Bitmap* m_bitmap;
    
    uint       m_handle;

    Texture()
    {
        m_bitmap=0;
        m_handle=0;
    }

    void GenTexture();
    
    void DelTexture();

    void Bind();
};

#endif //TEXTURE_H