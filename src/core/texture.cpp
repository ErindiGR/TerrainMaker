#include "texture.h"

#include <OpenGL/gl.h>
#include "color.h"


void
Texture::GenTexture()
{
    if(m_handle)
        DelTexture();

    if(!m_bitmap)
        return;


    glGenTextures(1,&m_handle);

    if(!m_handle)
        return;

    glBindTexture(GL_TEXTURE_2D,m_handle);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_bitmap->GetWidth(),
    m_bitmap->GetHeight(),0,GL_RGBA,GL_FLOAT,
    m_bitmap->GetDataBuffer());

    glGenerateMipmap(GL_TEXTURE_2D);
}


void
Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D,m_handle);
}

void
Texture::DelTexture()
{
    glDeleteTextures(1,&m_handle);
    m_handle = 0;
}
