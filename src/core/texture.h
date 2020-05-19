#ifndef TEXTURE_H
#define TEXTURE_H

#include "bitmap.h"

class Texture
{
    Bitmap* m_bitmap;
    uint    m_handle;
    bool    m_dirty;

    public:

    Texture()
    {
        m_bitmap=0;
        m_handle=0;
        m_dirty=false;
    }

    void SetBitmap(Bitmap* bitmap);

    void GenTexture();
    
    void DelTexture();

    void Bind();
};

#endif //TEXTURE_H