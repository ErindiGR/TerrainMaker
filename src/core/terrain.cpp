#include "terrain.h"

#include <OpenGL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"


Bitmap         g_height_map((RGBA){0,0,0,0},512,512);
Bitmap         g_color_map((RGBA){0,0,0,0},256,256);

std::vector<vec3>    g_vertices;
std::vector<vec2>    g_uv;

Texture       g_color_texture;

int g_lowf_blur = 0;
int g_highf_blur = 0;

float g_aspect_ratio = 0;
float g_view_distance = 1;
float g_view_angle = 0;
float g_height = 0;


bool g_changed_texture = false;
bool g_changed_height_maps = false;

void
TMC_Draw()
{
    if(g_changed_height_maps)
    {
        TMC_GenHeightMap();
        TMC_GenMesh();
        g_changed_height_maps = false;
    }


    if(g_changed_texture)
    {
        g_color_texture.GenTexture();

        g_changed_texture = false;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW_MATRIX);

    glm::mat4 cam = 
    glm::perspective(glm::radians(90.0f),g_aspect_ratio,0.1f,1000.0f) *
    glm::lookAt(glm::vec3(
        glm::rotate(glm::radians(g_view_angle),glm::vec3(0,1,0))*
        glm::vec4(g_view_distance,g_view_distance,g_view_distance,1)),
        glm::vec3(0,0,0),glm::vec3(0,1,0));

    glLoadMatrixf(glm::value_ptr(cam));


    g_color_texture.Bind();

    glEnable(GL_TEXTURE_2D);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
    for(int i=0;i<g_vertices.size();i++)
    {
        glTexCoord2f(g_vertices[i].x+0.5f,g_vertices[i].z+0.5f);
        glVertex3f(g_vertices[i].x,g_vertices[i].y*g_height,g_vertices[i].z);
    }
    glEnd();
}


glm::vec3
ToVec3(const RGBA& color)
{
        return glm::vec3(color.r,color.g,color.b);   
}

RGBA
ToRGBA(const glm::vec3& vec)
{
    return (RGBA){vec.x,vec.y,vec.z,1};
}


void Blur(Bitmap& bitmap,uint n)
{
    for(uint16 i=0;i<n;i++)
    {
        Bitmap bm = bitmap;
        for(int x=0;x<bitmap.GetWidth();x++)
            for(int y=0;y<bitmap.GetHeight();y++)
            {
                glm::vec3 t = ToVec3(bm.GetPixel(x,y+1));
                glm::vec3 b = ToVec3(bm.GetPixel(x,y-1));
                glm::vec3 l = ToVec3(bm.GetPixel(x-1,y));
                glm::vec3 r = ToVec3(bm.GetPixel(x+1,y));

                glm::vec3 tl = ToVec3(bm.GetPixel(x-1,y+1));
                glm::vec3 bl = ToVec3(bm.GetPixel(x-1,y-1));
                glm::vec3 tr = ToVec3(bm.GetPixel(x+1,y+1));
                glm::vec3 br = ToVec3(bm.GetPixel(x+1,y-1));
                
                glm::vec3 a = ToVec3(bm.GetPixel(x,y));

                uint d = 4;
                uint q = 8;
                uint s = 16;

                a = a * 1.f/4.f
                + t * 1.f/8.f + b * 1.f/8.f
                + r * 1.f/8.f + l * 1.f/8.f
                + tr * 1.f/16.f + bl * 1.f/16.f
                + br * 1.f/16.f + tl * 1.f/16.f;

                bitmap.SetPixel(x,y,ToRGBA(a));
            }
    }
}


Bitmap g_lowf((RGBA){0,0,0,0},128,128);
Bitmap g_highf((RGBA){0,0,0,0},256,256);

vec3
ToVec3(const glm::vec3& a)
{
    return (vec3){a.x,a.y,a.z};
}

void
TMC_GenHeightMap()
{
    Bitmap hf((RGBA){0,0,0,0},256,256);
    hf.Copy(g_highf);

    hf.Mul(0.5f);
    hf.Add(g_lowf);
    hf.Normalize();
    hf.Resize(512,512);

    g_height_map.Copy(hf);


    g_color_texture.m_bitmap = &g_height_map;
    g_changed_texture = true;

    
    g_changed_height_maps = false;
}

void
TMC_GenHFMap()
{
    for(uint x=0;x<g_highf.GetWidth();x++)
        for(uint y=0;y<g_highf.GetHeight();y++)
        {
            float a = rand()%1024/1024.0f;
            g_highf.SetPixel(x,y,(RGBA){a,a,a,1});
        }
    Blur(g_highf,g_highf_blur);

    g_changed_height_maps = true;
}

void
TMC_GenLFMap()
{
    for(uint x=0;x<g_lowf.GetWidth();x++)
        for(uint y=0;y<g_lowf.GetHeight();y++)
        {
            float a = rand()%1024/1024.0f;
            g_lowf.SetPixel(x,y,(RGBA){a,a,a,1});
        }

    Blur(g_lowf,g_lowf_blur);

    g_changed_height_maps = true;
}



void
TMC_GenMesh()
{
    g_vertices.clear();

    for(int x=0;x<g_height_map.GetWidth();x++)
    {
        for(int y=0;y<g_height_map.GetHeight();y++)
        {
            float size = 1.0f/g_height_map.GetWidth();
            glm::vec3 tl(x*size-0.5f,g_height_map.GetPixel(x,y).r,y*size-0.5f);
            glm::vec3 tr((x+1)*size-0.5f,g_height_map.GetPixel(x+1,y).r,y*size-0.5f);
            glm::vec3 bl(x*size-0.5f,g_height_map.GetPixel(x,y+1).r,(y+1)*size-0.5f);
            glm::vec3 br((x+1)*size-0.5f,g_height_map.GetPixel(x+1,y+1).r,(y+1)*size-0.5f);

            g_vertices.push_back(ToVec3(tr));
            g_vertices.push_back(ToVec3(tl));
            g_vertices.push_back(ToVec3(bl));

            g_vertices.push_back(ToVec3(tr));
            g_vertices.push_back(ToVec3(bl));
            g_vertices.push_back(ToVec3(br));
        }
    }
}