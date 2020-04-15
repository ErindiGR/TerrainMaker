#ifndef TEXTURE_H
#define TEXTURE_H

#include "bitmap.h"

template<typename T>
struct Texture
{
    Bitmap<T>* m_bitmap;
    
    uint       m_handle;

    void GenTexture();
    
    void DelTexture();

    void Bind();
};

#endif //TEXTURE_H