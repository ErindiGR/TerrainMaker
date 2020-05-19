#include "exp.h"

#include <algorithm>
#include <string>

#include <export.h>
#include <core.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int
TMEXP_HeightmapPNG()
{
    Bitmap heightmap = TMC_GetHeightMap();
    std::vector<RGBA> h = heightmap.GetPixels();

    const int size = h.size();

    std::vector<uint16> hv(size);

    for(int i=0;i<size;i++)
    {
        const float a = std::clamp(h[i].r,0.0f,1.0f);
        hv[i] = (uint16)(a * 0xffff);
    }

    stbi_write_png(TMC_GetPath().c_str(),heightmap.GetWidth(),heightmap.GetHeight(),2,&hv[0],0);

    return 0;
}

int
TMEXP_HeightmapBMP()
{
    Bitmap heightmap = TMC_GetHeightMap();
    std::vector<RGBA> h = heightmap.GetPixels();

    const int size = h.size();

    std::vector<uint16> hv(size);

    for(int i=0;i<size;i++)
    {
        const float a = std::clamp(h[i].r,0.0f,1.0f);
        hv[i] = (uint16)(a * 0xffff);
    }

    stbi_write_bmp(TMC_GetPath().c_str(),heightmap.GetWidth(),heightmap.GetHeight(),2,&hv[0]);

    return 0;
}


void
Register()
{
    
    TMC_AddExportImportFunc(
        (EFunc)
        {
            &TMEXP_HeightmapPNG,
            "PNG (*.png)|*.png",
            "Heightmap",
            "Heightmap(PNG)",
            NULL 
        }
    );

    TMC_AddExportImportFunc(
        (EFunc)
        {
            &TMEXP_HeightmapBMP,
            "BMP (*.bmp)|*.bmp",
            "Heightmap",
            "Heightmap(BMP)",
            NULL
        }
    );
}