#include "export.h"


std::vector<EFunc> g_ExportFuncs;
std::string g_ExportPath;

void TMC_AddExportImportFunc(EFunc eifunc)
{
    g_ExportFuncs.push_back(eifunc);
}


std::vector<EFunc>
TMC_GetAllEFunc()
{
    return g_ExportFuncs;
}

void
TMC_SetPath(const std::string& path)
{
    g_ExportPath = path;
}

std::string
TMC_GetPath()
{
    return g_ExportPath;
}