#ifndef TERRAIN_H
#define TERRAIN_H

#include "bitmap.h"
#include "color.h"
#include "vector.h"

TMC_API Bitmap        g_height_map;
TMC_API Bitmap        g_color_map;

TMC_API std::vector<vec3>   g_vertices;
TMC_API std::vector<vec2>   g_uv;

TMC_API int g_lowf_blur;
TMC_API int g_highf_blur;

TMC_API float g_aspect_ratio;

TMC_API float g_view_distance;
TMC_API float g_view_angle;
TMC_API float g_height;

TMC_API void TMC_Draw();

void TMC_GenHeightMap();
void TMC_GenHFMap();
void TMC_GenLFMap();
void TMC_GenMesh();


#endif //TERRAIN_H