#ifndef CORE_H
#define CORE_H

#include "bitmap.h"
#include "vector.h"

TMC_API void TMC_SetMeshWidth(float width);
TMC_API void TMC_SetMeshHeight(float height);
TMC_API void TMC_SetViewDistance(float dist);
TMC_API void TMC_SetViewAngleSpeed(float angle);
TMC_API void TMC_SetAspectRatio(float ar);
TMC_API void TMC_SetResultResolution(int res);
TMC_API void TMC_SetTessellation(int tes);
TMC_API void TMC_SetNumErosions(int num);
TMC_API void TMC_SetErosionSteps(int num);
TMC_API void TMC_SetErosionAmount(float amount);
TMC_API void TMC_SetErosionMax(float max);
TMC_API void TMC_SetErosionAndDeposit(bool enable);
TMC_API void TMC_SetFreqMapBlur(int blur,int map);
TMC_API void TMC_SetFreqMapMultiplier(float mul,int map);
TMC_API void TMC_SetFreqNormalize(bool enable);
TMC_API void TMC_SetFreqView(bool enable);


TMC_API Bitmap TMC_GetHeightMap();
TMC_API Bitmap TMC_GetColorMap();
TMC_API Bitmap TMC_GetErosionMap();

TMC_API std::vector<vec3> TMC_GetVertices();
TMC_API std::vector<vec2> TMC_GetUV();


TMC_API void TMC_Draw();

TMC_API void TMC_GenHeightMap();
TMC_API void TMC_GenNoisMap(Bitmap&);
TMC_API void TMC_GenMesh();
TMC_API void TMC_GenErosionMap();
TMC_API void TMC_GenColorMap();


#endif //CORE_H