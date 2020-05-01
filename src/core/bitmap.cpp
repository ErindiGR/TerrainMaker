#include "bitmap.h"


RGBA
RGBA_Lerp(const RGBA a,const RGBA b,float t)
{
    RGBA ret = {0,0,0,0};

    for(int i=0;i<RGBA_COUNT;i++)
        ret.v[i] = a.v[i] * t + b.v[i] * (1.0f-t);

    return ret;
}



Bitmap::Bitmap(const RGBA& value, const uint16 width, const uint16 height, bool wrap)
{
    m_width = width;
    m_height = height;
    m_wrap = wrap;
    CreatePixels(value);
}

void
Bitmap::CreatePixels(const RGBA& value)
{
    m_pixels.clear();
    uint s = (uint)m_width * (uint)m_height;
    m_pixels.reserve(s);
    for(uint i=0;i<s;i++)
        m_pixels.push_back(value);
}

void
Bitmap::Resize(const uint16 width,const uint16 height)
{
    if(m_width == width && m_height == height)
        return;


    std::vector<RGBA> data(width*height);

    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            float fx = x/(float)width;
            float fy = y/(float)height;

            data[y*width+x] = GetPixel(fx,fy);

        }
    }

    m_width = width;
    m_height = height;
    m_pixels = std::move(data);
}

void
Bitmap::Resize(const uint16 width,const uint16 height,const RGBA& value)
{
    m_width = width;
    m_height = height;
    CreatePixels(value);
}

RGBA
Bitmap::GetPixel(int x,int y)
{
    int xi = x % m_width;
    int yi = y % m_height;

    if(xi<0)
        xi = m_width+xi;
    
    if(y<0)
        yi = m_height+yi;

    int i = yi * m_width + xi;
    
    return m_pixels[i];
}

RGBA
Bitmap::GetPixel(float x,float y)
{
    x = x * m_width;
    y = y * m_height;

    int floor_x = std::floorf(x);
    int ceil_x = std::ceilf(x);

    int floor_y = std::floorf(y);
    int ceil_y = std::ceilf(y);

    RGBA tl = GetPixel(floor_x,ceil_y);
    RGBA tr = GetPixel(ceil_x,ceil_y);
    RGBA bl = GetPixel(floor_x,floor_y);
    RGBA br = GetPixel(ceil_x,floor_y);
    
    return RGBA_Lerp(RGBA_Lerp(tl,tr,x-floor_x),RGBA_Lerp(bl,br,x-floor_x),y-floor_y);
}

void
Bitmap::SetPixel(uint x, uint y,const RGBA& value)
{
    x %= m_width;
    y %= m_height;
    uint i = y * m_width + x;
    if(m_pixels.size()>i)
        m_pixels[i] = value;
}


void
Bitmap::Add(Bitmap& bitmap)
{
    for(int x=0;x<m_width;x++)
    {
        for(int y=0;y<m_height;y++)
        {
            float fx = x/(float)m_width;
            float fy = y/(float)m_height;

            RGBA a = bitmap.GetPixel(fx,fy);
            RGBA b = GetPixel(fx,fy);

            for(int i=0;i<RGBA_COUNT;i++)
                m_pixels[y*m_width+x].v[i] = a.v[i] + b.v[i];

        }
    }
}

void
Bitmap::Mul(Bitmap& bitmap)
{
    for(int x=0;x<m_width;x++)
    {
        for(int y=0;y<m_height;y++)
        {
            float fx = x/(float)m_width;
            float fy = y/(float)m_height;

            RGBA a = bitmap.GetPixel(fx,fy);
            RGBA b = GetPixel(fx,fy);

            for(int i=0;i<RGBA_COUNT;i++)
                m_pixels[y*m_width+x].v[i] = a.v[i] * b.v[i];

        }
    }
}

void
Bitmap::Mul(const float f)
{
    for(int x=0;x<m_width;x++)
    {
        for(int y=0;y<m_height;y++)
        {
            float fx = x/(float)m_width;
            float fy = y/(float)m_height;
            
            RGBA b = GetPixel(fx,fy);
            
            for(int i=0;i<RGBA_COUNT;i++)
                m_pixels[y*m_width+x].v[i] = f * b.v[i];

        }
    }
}

void
Bitmap::Pow(const float f)
{
    for(int x=0;x<m_width;x++)
    {
        for(int y=0;y<m_height;y++)
        {   
            RGBA b = GetPixel(x,y);
            
            for(int i=0;i<RGBA_COUNT;i++)
                m_pixels[y*m_width+x].v[i] = std::powf(b.v[i],f);

        }
    }
}

void 
Bitmap::Normalize()
{
    float max = 0;
    float min = 1000000;
    
    for(int x=0;x<m_width;x++)
        for(int y=0;y<m_height;y++)
        {
            RGBA b = GetPixel(x,y);
            
            for(int i=0;i<RGBA_COUNT;i++)
            {
                if(b.v[i] > max)
                    max = b.v[i];
                
                if(b.v[i] < min)
                    min = b.v[i];
            }
        }

    max = 1.0f/(max-min);
    
    for(int x=0;x<m_width;x++)
    {
        for(int y=0;y<m_height;y++)
        {   
            RGBA b = GetPixel(x,y);
            
            for(int i=0;i<RGBA_COUNT;i++)
                m_pixels[y*m_width+x].v[i] = (b.v[i]-min) * max;

        }
    }
}

void
Bitmap::Copy(Bitmap& bitmap)
{
    int size = bitmap.m_pixels.size();
    if(m_pixels.size() < size)
        m_pixels.resize(size);

    m_width = bitmap.m_width;
    m_height = bitmap.m_height;

    for(int i=0;i<size;i++)
    {
        m_pixels[i] = bitmap.m_pixels[i];
    }

}