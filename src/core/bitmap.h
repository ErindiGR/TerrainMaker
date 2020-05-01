#ifndef BITMAP_H
#define BITMAP_H


#include "common.h"

#include <vector>
#include <cmath>

#define RGBA_COUNT 4

union RGBA
{
    struct{
    float r,g,b,a;
    };
    float v[RGBA_COUNT];
};


class Bitmap
{
    uint16              m_width;
    uint16              m_height;

    bool                m_wrap;

    std::vector<RGBA>  m_pixels;

    public:
    
    Bitmap(const RGBA& value,const uint16 width = 1024u,const uint16 height = 1024u,bool wrap = true);

    int GetWidth(){return m_width;}
    int GetHeight(){return m_height;}
    RGBA* GetDataBuffer(){return &m_pixels[0];}

    void CreatePixels(const RGBA& value);

    void Resize(const uint16 width,const uint16 height,const RGBA& value);
    void Resize(const uint16 width,const uint16 height);

    RGBA GetPixel(int x,int y);

    RGBA GetPixel(float x,float y);

    void SetPixel(uint x, uint y,const RGBA& value);

    void Copy(Bitmap& bitmap);

    void Add(Bitmap& bitmap);
    void Mul(Bitmap& bitmap);
    void Mul(const float f);
    void Pow(const float f);
    void Normalize();
};


#endif //BITMAP_H