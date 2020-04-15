#include "texture.h"

#include <OpenGL/gl.h>
#include "color.h"

template<typename TColor>
int GetIFormat();

template<typename TColor>
int GetFormat();

template<typename TColor>
int GetFType();


template<typename T>
void Texture<T>::GenTexture()
{
    if(m_handle)
        DelTexture();

    if(!m_bitmap)
        return;

    glGenTextures(1,&m_handle);
    glBindTexture(GL_TEXTURE_2D,m_handle);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D,0,GetIFormat<T>(),m_bitmap->m_width,
    m_bitmap->m_height,0,GetFormat<T>(),GetFType<T>(),
    &m_bitmap->m_pixels[0]);

    glGenerateMipmap(GL_TEXTURE_2D);
}


template<typename T>
void Texture<T>::Bind()
{
    glBindTexture(GL_TEXTURE_2D,m_handle);
}

template<typename T>
void Texture<T>::DelTexture()
{
    glDeleteTextures(1,&m_handle);
}


template<>
int GetIFormat<RGB8>()
{
    return GL_RGB;
}

template<>
int GetIFormat<R16>()
{
    return GL_R;
}

template<>
int GetFormat<RGB8>()
{
    return GL_RGB;
}

template<>
int GetFormat<R16>()
{
    return GL_R;
}

template<>
int GetFType<RGB8>()
{
    return GL_UNSIGNED_BYTE;
}

template<>
int GetFType<R16>()
{
    return GL_UNSIGNED_SHORT;
}

template struct Texture<RGB8>;
template struct Texture<R16>;