#ifndef BITMAP_H
#define BITMAP_H


#include <vector>
#include <assert.h>
#include "common.h"



template<typename T>
struct Bitmap
{
    //dont edit
    uint16              m_width;
    //dont edit
    uint16              m_height;

    bool                m_wrap;

    std::vector<T>  m_pixels;

    
    Bitmap(const T& value,const uint16 width = 1024u,const uint16 height = 1024u,bool wrap = true)
    {
        m_width = width;
        m_height = height;
        m_wrap = wrap;

        CreatePixels(value);
    }

    void CreatePixels(const T& value)
    {
        m_pixels.clear();

        uint s = (uint)m_width * (uint)m_height;

        m_pixels.reserve(s);

        for(uint i=0;i<s;i++)
            m_pixels.push_back(value);
    }

    void Resize(const uint16 width,const uint16 height,const T& value)
    {
        m_width = width;
        m_height = height;

        CreatePixels(value);
    }

    T* GetPixel(uint16 x, uint16 y)
    {
        x /= m_width;
        y /= m_height;

        uint i = y * m_width + x;

        return &m_pixels[i];
    }

    void SetPixel(uint16 x, uint16 y,const T& value)
    {
        x %= m_width;
        y %= m_height;

        uint i = (uint)y * (uint)m_width + x;

        if(m_pixels.size()>i)
            m_pixels[i] = value;

    }
};


#endif //BITMAP_H