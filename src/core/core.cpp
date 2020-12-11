#include "core.h"
#include "texture.h"

#include "thread_vector.h"
#include <OpenGL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>



Bitmap g_HeightMap((RGBA){0,0,0,0});
Bitmap g_ColorMap((RGBA){0,0,0,0});
Bitmap g_ErosionMap((RGBA){0,0,0,0});


#define FREQMAP_COUNT 4

Bitmap g_FreqMap[FREQMAP_COUNT]=
{
    Bitmap((RGBA){0,0,0,0},16,16),
    Bitmap((RGBA){0,0,0,0},32,32),
    Bitmap((RGBA){0,0,0,0},64,64),
    Bitmap((RGBA){0,0,0,0},128,128)
};

int g_FreqMapBlur[FREQMAP_COUNT]=
{
    4,6,12,24
};

float g_FreqMapMul[FREQMAP_COUNT]=
{
    0.5,0.25,0.125,0.0625
};

bool g_FreqNormalize = false;
bool g_FreqView = false;

std::vector<vec3>    g_Vertices;
std::vector<vec3>    g_Normals;
std::vector<vec2>    g_UV;
std::vector<int>     g_Indices;

float g_AspectRatio = 1;
float g_ViewDistance = 50;
float g_ViewAngle = 45.0f;
float g_ViewAngleSpeed = 0.2f;
float g_MeshHeight = 50;
float g_MeshWidth = 50;

int     g_NumErosions = 5000;
int     g_ErosionSteps = 64;
float   g_ErosionAmount = 0.8f;
float   g_ErosionMax = 0.02f;
bool    g_ErosionDeposit=false;

int g_ResultRes = 256;
int g_Tessellation = 256;

bool g_changed_texture = false;

Texture g_ColorTexture;
Texture g_TexHeight;
Texture g_TexFreq[FREQMAP_COUNT];

inline void TMC_SimErsosion(int x, int y);


//0,0 bottom left 1,1 top right
void
TMC_DrawScreenQuad(float xmin,float ymin,float xmax,float ymax)
{    

    xmin = (xmin*2)-1;
    ymin = (ymin*2)-1;
    xmax = (xmax*2)-1;
    ymax = (ymax*2)-1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glNormal3f(0,0,0);
    glVertex2f(xmin,ymin);
    glTexCoord2f(1,0);
    glNormal3f(0,0,0);
    glVertex2f(xmax,ymin);
    glTexCoord2f(1,1);
    glNormal3f(0,0,0);
    glVertex2f(xmax,ymax);
    glTexCoord2f(0,1);
    glNormal3f(0,0,0);
    glVertex2f(xmin,ymax);
    glEnd();

}

void
TMC_DRAW_FREQMAP()
{
    const float a = 1/(float)FREQMAP_COUNT;
    for(int i=0;i<FREQMAP_COUNT;i++)
    {
        g_TexFreq[i].Bind();
        glEnable(GL_TEXTURE_2D);
        TMC_DrawScreenQuad(i*a,1-a*g_AspectRatio,(i+1)*a,1.0f);
    }

    g_TexHeight.Bind();
    glEnable(GL_TEXTURE_2D);
    TMC_DrawScreenQuad(0,1-a*2*g_AspectRatio,a,1-a*g_AspectRatio);
}

void
TMC_Draw()
{

    if(g_ViewAngle<=360.0f)
        g_ViewAngle+=g_ViewAngleSpeed;
    else
        g_ViewAngle=0;



    if(g_FreqView)
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_DEPTH_TEST);

        TMC_DRAW_FREQMAP();
    }



    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    

    glm::mat4 cam = 
    glm::perspective(glm::radians(90.0f),g_AspectRatio,0.1f,1000.0f) *
    glm::lookAt(glm::vec3(glm::rotate(glm::radians(g_ViewAngle),glm::vec3(0,1,0))*
    glm::vec4(g_ViewDistance,g_ViewDistance,g_ViewDistance,1)),
    glm::vec3(0,0,0),glm::vec3(0,1,0));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(cam));
    


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    
    
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float[]){0.4f,0.4f,0.4f,1.0f});
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float[]){0.0f,0.0f,0.0f,0.0f});
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float[]){1.0f,1.0f,1.0f,1.0f});
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,(float[]){0.0f});

    glLightfv(GL_LIGHT0,GL_POSITION,(float[]){1.0f,1.0f,1.0f,0.0f});
    glLightfv(GL_LIGHT0,GL_SPECULAR,(float[]){0.0f,0.0f,0.0f,0.0f});
    glLightfv(GL_LIGHT0,GL_DIFFUSE,(float[]){1.0f,1.0f,1.0f,0.0f});


    g_ColorTexture.Bind();
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_TRIANGLES);
    for(int i=0;i<g_Indices.size();i++)
    {
        const int p = g_Indices[i];

        //glTexCoord2f(g_Vertices[p].x+0.5f,g_Vertices[p].z+0.5f);
        glTexCoord2f(g_UV[p].x,g_UV[p].y);
        glNormal3f(g_Normals[p].x,g_Normals[p].y,g_Normals[p].z);
        glVertex3f(g_Vertices[p].x*g_MeshWidth,g_Vertices[p].y*g_MeshHeight,
                g_Vertices[p].z*g_MeshWidth);
    }
    glEnd();

}


inline glm::vec4
ToVec4(const RGBA& color)
{
    return glm::vec4(color.r,color.g,color.b,color.a);   
}

inline RGBA
ToRGBA(const glm::vec4& vec)
{
    return (RGBA){vec.x,vec.y,vec.z,vec.w};
}


void Blur(Bitmap& bitmap,uint n)
{
    for(uint16 i=0;i<n;i++)
    {
        Bitmap bm = bitmap;
        for(int x=0;x<bitmap.GetWidth();x++)
            for(int y=0;y<bitmap.GetHeight();y++)
            {
                glm::vec4 t = ToVec4(bm.GetPixel(x,y+1));
                glm::vec4 b = ToVec4(bm.GetPixel(x,y-1));
                glm::vec4 l = ToVec4(bm.GetPixel(x-1,y));
                glm::vec4 r = ToVec4(bm.GetPixel(x+1,y));

                glm::vec4 tl = ToVec4(bm.GetPixel(x-1,y+1));
                glm::vec4 bl = ToVec4(bm.GetPixel(x-1,y-1));
                glm::vec4 tr = ToVec4(bm.GetPixel(x+1,y+1));
                glm::vec4 br = ToVec4(bm.GetPixel(x+1,y-1));
                
                glm::vec4 a = ToVec4(bm.GetPixel(x,y));

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




inline vec3
ToVec3(const glm::vec3& a)
{
    return (vec3){a.x,a.y,a.z};
}

void
TMC_GenHeightMap()
{
    {
        ThreadVector threads;
        for(int i=0;i<FREQMAP_COUNT;i++)
        {
            threads.Add(std::thread(TMC_GenNoisMap,std::ref(g_FreqMap[i])));
        }

        threads.Join();

        for(int i=0;i<FREQMAP_COUNT;i++)
        {
            threads.Add(std::thread(Blur,std::ref(g_FreqMap[i]),g_FreqMapBlur[i]));
        }

        threads.Join();

        if(g_FreqNormalize)
        {
            for(int i=0;i<FREQMAP_COUNT;i++)
            {
                threads.Add(std::thread(&Bitmap::Normalize,std::ref(g_FreqMap[i])));
            }

            threads.Join();
        }
    }
    


    Bitmap res((RGBA){0,0,0,0},g_ResultRes,g_ResultRes);
    
    for(int i=0;i<FREQMAP_COUNT;i++)
    {
        Bitmap a = g_FreqMap[i];

        for(int i=1;a.GetWidth() < g_ResultRes;i++)
        {
            a.Resize(a.GetWidth()*glm::pow(2,i),a.GetHeight()*glm::pow(2,i));
            Blur(a,2);
        }
        a.Mul(g_FreqMapMul[i]);
        res.Add(a);
    }

    g_HeightMap = res;


    g_TexFreq[0].SetBitmap(&g_FreqMap[0]);
    g_TexFreq[1].SetBitmap(&g_FreqMap[1]);
    g_TexFreq[2].SetBitmap(&g_FreqMap[2]);
    g_TexFreq[3].SetBitmap(&g_FreqMap[3]);
    g_TexHeight.SetBitmap(&g_HeightMap);

}

void
TMC_GenNoisMap(Bitmap& map)
{
    for(uint x=0;x<map.GetWidth();x++)
        for(uint y=0;y<map.GetHeight();y++)
        {
            float a = rand()%1000/1000.0f;
            map.SetPixel(x,y,(RGBA){a,a,a,a});
        }
}



void
TMC_GenMesh()
{
    g_Vertices.clear();
    g_Normals.clear();
    g_Indices.clear();

    const int tes = g_Tessellation;

    for(int x=0;x<tes;x++)
    {
        for(int y=0;y<tes;y++)
        {
            const float tex = 1.0f/tes;

            const float fx = x*tex;
            const float fy = y*tex;

            glm::vec3 p(fx-0.5f,g_HeightMap.GetPixel(fx,fy).r-0.5f,fy-0.5f);
            g_Vertices.push_back(ToVec3(p));
        }
    }


    
    for(int i=0;i<g_Vertices.size()-tes-1;i++)
    {

        //lazy solution
        if(i%tes == tes-1)
            continue;

        g_Indices.push_back(i);
        g_Indices.push_back(i+1);
        g_Indices.push_back(i+tes);


        g_Indices.push_back(i+1);
        g_Indices.push_back(i+1+tes);
        g_Indices.push_back(i+tes);
    }


    g_Normals.resize(g_Vertices.size());
    for(int i=0;i<g_Indices.size();i+=3)
    {
        const int p = g_Indices[i];
        const int p2 = g_Indices[i+1];
        const int p3 = g_Indices[i+2];

        vec3 av = g_Vertices[p];
        vec3 bv = g_Vertices[p2];
        vec3 cv = g_Vertices[p3];

        glm::vec3 a(av.x,av.y,av.z);
        glm::vec3 b(bv.x,bv.y,bv.z);
        glm::vec3 c(cv.x,cv.y,cv.z);

        c -=b;
        b -=a;

        a=glm::normalize(glm::cross(b,c));

        g_Normals[p] = ToVec3(a);
        g_Normals[p2] = ToVec3(a);
        g_Normals[p3] = ToVec3(a);
    }
    
    
    g_UV.resize(g_Vertices.size());
    for(int i=0;i<g_Indices.size();i++)
    {
        g_UV[g_Indices[i]] = (vec2){g_Vertices[g_Indices[i]].x+0.5f,g_Vertices[g_Indices[i]].z+0.5f};
    }
}


void
TMC_GenErosionMap()
{
    g_ErosionMap = Bitmap((RGBA){0,0,0,0},g_ResultRes,g_ResultRes);

    for(int i=0;i<g_NumErosions;i++)
    {
        int x = rand()%g_ErosionMap.GetWidth();
        int y = rand()%g_ErosionMap.GetHeight();

        TMC_SimErsosion(x,y);
    }

    Blur(std::ref(g_ErosionMap),4);

    g_HeightMap.Add(g_ErosionMap);
}

inline void
TMC_SimErsosion(int x, int y)
{

    const float tex = 1.0f/(float)g_ErosionMap.GetWidth();


    const float dir[8][2] =
    {
        {0,1},
        {0,-1},
        {1,0},
        {-1,0},
        {1,1},
        {1,-1},
        {-1,1},
        {-1,-1}
    };

    for(int s=0;s<g_ErosionSteps;s++)
    {
        const float fx = x/(float)g_ErosionMap.GetWidth();
        const float fy = y/(float)g_ErosionMap.GetHeight();

        const float current = g_HeightMap.GetPixel(fx,fy).r;

        const float np[8] ={
        g_HeightMap.GetPixel(fx,fy+tex).r
        //+ g_deposit_map.GetPixel(fx,fy+tex).r 
        ,
        g_HeightMap.GetPixel(fx,fy-tex).r
        //+ g_deposit_map.GetPixel(fx,fy-tex).r
        ,
        g_HeightMap.GetPixel(fx+tex,fy).r
        //+ g_deposit_map.GetPixel(fx+tex,fy).r
        ,
        g_HeightMap.GetPixel(fx-tex,fy).r
        //+ g_deposit_map.GetPixel(fx-tex,fy).r
        ,
        g_HeightMap.GetPixel(fx+tex,fy+tex).r
        //+ g_deposit_map.GetPixel(fx+tex,fy+tex).r
        ,
        g_HeightMap.GetPixel(fx+tex,fy-tex).r
        //+ g_deposit_map.GetPixel(fx+tex,fy-tex).r
        ,
        g_HeightMap.GetPixel(fx-tex,fy+tex).r
        //+ g_deposit_map.GetPixel(fx-tex,fy+tex).r
        ,
        g_HeightMap.GetPixel(fx-tex,fy-tex).r
        //+ g_deposit_map.GetPixel(fx-tex,fy-tex).r
        };

        int low=0;
        for(int d=0;d<8;d++)
            if(np[low]>np[d])
                low=d;

        float dif = current - np[low];

        if(dif <= 0.0001f)
            break;

        dif = glm::abs(dif);

        float amount = glm::min(dif * g_ErosionAmount,0.003f);

        float o = g_ErosionMap.GetPixel(x,y).r;
        
        if(g_ErosionDeposit)
        {
            //erosion and deposit
            //GLITCHY
            o +=amount*(s/(float)g_ErosionSteps-0.5f)*2.0f;
            const float fs = s/(float)g_ErosionSteps;
            const float pi = 3.14f;
            o +=amount*(glm::max(glm::sin(fs*pi-1.6f),glm::cos(fs*1.49f*pi+1.6f)*0.5f+0.5f)-0.5f)*2.0f;
        }
        else
            o -=amount;

        o = glm::max(o,-1 * g_ErosionMax);
        o = glm::min(o,g_ErosionMax);
        g_ErosionMap.SetPixel(x,y,(RGBA){o,o,o,o});

        x += dir[low][0];
        y += dir[low][1];
    }
}



void
TMC_GenColorMap()
{
    g_ColorMap = Bitmap((RGBA){0,0,0,0},g_ResultRes,g_ResultRes);
    const double tex = 1.0f/(double)g_ColorMap.GetWidth();

    for(int x=0;x<g_ColorMap.GetWidth();x++)
    {
        for(int y=0;y<g_ColorMap.GetHeight();y++)
        {
            const double fx = x*tex;
            const double fy = y*tex;

            const float c = g_HeightMap.GetPixel(fx,fy).r;
            float r = g_HeightMap.GetPixel(fx+tex,fy).r;
            float t = g_HeightMap.GetPixel(fx,fy+tex).r;

            r -=c;
            t -=c;


            const glm::vec3 right(0,r,tex);
            const glm::vec3 top(tex,t,0);
            glm::vec3 dir = glm::normalize(glm::cross(right,top));
            
            glm::vec3 col(0.47,0.38,0.3);

            const glm::vec3 water(0.1,0.1,0.6);
            const glm::vec3 grass(0.1,0.5,0.1);
            const glm::vec3 rock(0.47,0.38,0.3);
            const glm::vec3 snow(1,1,1);

            const float waterh = 0.2f;
            const float grassh = 0.36f;
            const float rockh = 0.95f;

            if(c<waterh)
                col = glm::mix(grass,water,(waterh-c)/waterh);
            else if(c<grassh)
                col = glm::mix(rock,grass,(grassh-c)/(grassh-waterh));
            else if(c<rockh)
                col = glm::mix(snow,rock,(rockh-c)/(rockh-grassh));
            else
                col = glm::vec3(1,1,1);
            
            //debug
            //col=dir;

            g_ColorMap.SetPixel(x,y,(RGBA){col.x,col.y,col.z,1});
            
        }
    }

    g_ColorTexture.SetBitmap(&g_ColorMap);
}




void
TMC_SetMeshWidth(float width)
{g_MeshWidth = width;}
void
TMC_SetMeshHeight(float height)
{g_MeshHeight = height;}
void
TMC_SetViewDistance(float dist)
{g_ViewDistance = dist;}
void
TMC_SetViewAngleSpeed(float angle)
{g_ViewAngleSpeed = angle;}
void
TMC_SetAspectRatio(float ar)
{g_AspectRatio = ar;}
void
TMC_SetResultResolution(int res)
{g_ResultRes = res;}
void
TMC_SetNumErosions(int num)
{g_NumErosions =num;}
void
TMC_SetFreqMapBlur(int blur,int map)
{
    if(map<FREQMAP_COUNT)
        g_FreqMapBlur[map] = blur;
}
void
TMC_SetErosionSteps(int num)
{g_ErosionSteps = num;}
void
TMC_SetTessellation(int tes)
{g_Tessellation = tes;}
void
TMC_SetFreqMapMultiplier(float mul,int map)
{
    if(map<FREQMAP_COUNT)
        g_FreqMapMul[map] = mul;
}
void
TMC_SetErosionAmount(float amount)
{g_ErosionAmount=amount;}
void
TMC_SetErosionMax(float max)
{g_ErosionMax = max;}
void
TMC_SetErosionAndDeposit(bool enable)
{g_ErosionDeposit = enable;}
void
TMC_SetFreqNormalize(bool enable)
{g_FreqNormalize = enable;}
void
TMC_SetFreqView(bool enable)
{g_FreqView = enable;}




Bitmap
TMC_GetHeightMap()
{
    return g_HeightMap;
}

Bitmap
TMC_GetErosionMap()
{
    return g_ErosionMap;
}

Bitmap
TMC_GetColorMap()
{
    return g_ColorMap;
}


std::vector<vec3>
TMC_GetVertices()
{
    return g_Vertices;
}

std::vector<vec2>
TMC_GetUV()
{
    return g_UV;
}

std::vector<vec3>
TMC_GetNormals()
{
    return g_Normals;
}

std::vector<int>
TMC_GetIndices()
{
    return g_Indices;
}