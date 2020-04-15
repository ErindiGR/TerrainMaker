#include "terrain.h"

#include <OpenGL/gl.h>
#include "texture.h"


Bitmap<R16>          g_height_map((R16){0});
Bitmap<RGB8>         g_color_map((RGB8){0,0,255});

std::vector<vec3>    g_vertices;
std::vector<vec2>    g_uv;

Texture<RGB8>        g_color_texture;


void TMC_Draw()
{
    glEnable(GL_TEXTURE_2D);
    g_color_texture.Bind();
    glBegin(GL_TRIANGLES);
    for(int i=0;i<g_vertices.size();i++)
    {
        glTexCoord2f(g_vertices[i].x,g_vertices[i].y);
        glVertex3f(g_vertices[i].x,g_vertices[i].y,g_vertices[i].z);
    }
    glEnd();
}


bool done = false;
void TMC_CalculateMaps()
{
    if(done)
        return;

    g_vertices.push_back((vec3){0,1,0});
    g_vertices.push_back((vec3){1,0,0});
    g_vertices.push_back((vec3){0,0,0});


    for(int x=0;x<g_color_map.m_width;x++)
    for(int y=0;y<g_color_map.m_height;y++)
    {
        uint8 a = rand()%255;
        g_color_map.SetPixel(x,y,(RGB8){a,a,a});
    }

    g_color_texture.m_bitmap = &g_color_map;

    g_color_texture.GenTexture();
}