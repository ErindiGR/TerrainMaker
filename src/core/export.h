#ifndef EXPORT_IMPORT_H
#define EXPORT_IMPORT_H

#include "common.h"

#include <vector>
#include <string>


typedef int (*func) (void);


struct EFunc
{
    func        m_export_func;
    const char* m_extension;
    const char* m_filename;
    const char* m_menu_name;
    const char* m_description;
};


TMC_API void TMC_AddExportImportFunc(EFunc eifunc);

TMC_API std::vector<EFunc> TMC_GetAllEFunc();

TMC_API void TMC_SetPath(const std::string& path);
TMC_API std::string TMC_GetPath();


#endif //EXPORT_IMPORT_H
