#ifndef TERRAIN_H
#define TERRAIN_H

#include "bitmap.h"
#include "color.h"
#include "vector.h"

TMC_API Bitmap<R16>         g_height_map;
TMC_API Bitmap<RGB8>        g_color_map;

TMC_API std::vector<vec3>   g_vertices;
TMC_API std::vector<vec2>   g_uv;


TMC_API void TMC_Draw();

inline void TMC_Foo();

TMC_API void TMC_CalculateMaps();


#endif //TERRAIN_H