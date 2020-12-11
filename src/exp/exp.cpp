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
TMEXP_MeshOBJ()
{
    std::vector<vec3> v = TMC_GetVertices();
    std::vector<vec3> vn = TMC_GetNormals();
    std::vector<vec2> vt = TMC_GetUV();
    std::vector<int> f = TMC_GetIndices();


    FILE* file = fopen(TMC_GetPath().c_str(),"w");

    if(!file)
    {
        printf("error: failed to open file %s.\n",TMC_GetPath().c_str());
        return 1;
    }

    char* a = new char[256];

    for(int i=0;i<v.size();i++)
    {
        sprintf(a,"v %f %f %f \n",v[i].x,v[i].y,v[i].z);
        fwrite(a,strlen(a),sizeof(char),file);
    }

    for(int i=0;i<vt.size();i++)
    {
        sprintf(a,"vt %f %f \n",vt[i].x,vt[i].y);
        fwrite(a,strlen(a),sizeof(char),file);
    }

    for(int i=0;i<vn.size();i++)
    {
        sprintf(a,"vn %f %f %f \n",vn[i].x,vn[i].y,vn[i].z);
        fwrite(a,strlen(a),sizeof(char),file);
    }


    for(int i=0;i<f.size();i+=3)
    {
        const int f1 = f[i]+1;
        const int f2 = f[i+1]+1;
        const int f3 = f[i+2]+1;

        sprintf(a,"f %d/%d/%d %d/%d/%d %d/%d/%d \n",f1,f1,f1,f2,f2,f2,f3,f3,f3);
        fwrite(a,strlen(a),sizeof(char),file);
    }

    fclose(file);

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
            &TMEXP_MeshOBJ,
            "OBJ (*.obj)|*.obj",
            "Mesh",
            "Mesh(OBJ)",
            NULL
        }
    );
}