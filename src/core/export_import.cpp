#include "export_import.h"

#include <unordered_map>

std::unordered_map<const char*,EIFunc> g_export_inport_funcs(20);

void AddExportImportFunc(const char* name,EIFunc eifunc)
{
    g_export_inport_funcs[name] = eifunc;
}