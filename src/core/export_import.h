#ifndef EXPORT_IMPORT_H
#define EXPORT_IMPORT_H

#include "common.h"


typedef int (*func) (void);


struct EIFunc
{
    func        export_func;
    func        import_func;
    const char* extension;
};


TMC_API void AddExportImportFunc(const char* name,EIFunc eifunc);


#endif //EXPORT_IMPORT_H
