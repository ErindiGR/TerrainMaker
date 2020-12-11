#ifndef CORE_H
#define CORE_H

#include "bitmap.h"
#include "vector.h"

TMC_CAPI void TMC_SetMeshWidth(float width);
TMC_CAPI void TMC_SetMeshHeight(float height);
TMC_CAPI void TMC_SetViewDistance(float dist);
TMC_CAPI void TMC_SetViewAngleSpeed(float angle);
TMC_CAPI void TMC_SetAspectRatio(float ar);
TMC_CAPI void TMC_SetResultResolution(int res);
TMC_CAPI void TMC_SetTessellation(int tes);
TMC_CAPI void TMC_SetNumErosions(int num);
TMC_CAPI void TMC_SetErosionSteps(int num);
TMC_CAPI void TMC_SetErosionAmount(float amount);
TMC_CAPI void TMC_SetErosionMax(float max);
TMC_CAPI void TMC_SetErosionAndDeposit(bool enable);
TMC_CAPI void TMC_SetFreqMapBlur(int blur,int map);
TMC_CAPI void TMC_SetFreqMapMultiplier(float mul,int map);
TMC_CAPI void TMC_SetFreqNormalize(bool enable);
TMC_CAPI void TMC_SetFreqView(bool enable);


TMC_API Bitmap TMC_GetHeightMap();
TMC_API Bitmap TMC_GetColorMap();
TMC_API Bitmap TMC_GetErosionMap();

TMC_API std::vector<vec3> TMC_GetVertices();
TMC_API std::vector<vec2> TMC_GetUV();
TMC_API std::vector<vec3> TMC_GetNormals();
TMC_API std::vector<int> TMC_GetIndices();


TMC_CAPI void TMC_Draw();

TMC_CAPI void TMC_GenHeightMap();
TMC_CAPI void TMC_GenNoisMap(Bitmap&);
TMC_CAPI void TMC_GenMesh();
TMC_CAPI void TMC_GenErosionMap();
TMC_CAPI void TMC_GenColorMap();


#endif //CORE_H
