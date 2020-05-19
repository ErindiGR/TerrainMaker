#ifndef PLUGINS_H
#define PLUGINS_H

#include <wx/dir.h>

#if defined(__WIN32)

typedef HMODULE DLH;
typedef FARPROC DLSYM;

#elif defined(__linux__)  || defined(__APPLE__) 

#include <dlfcn.h>

typedef void* DLH;
typedef void* DLSYM;

#endif

DLH
LoadLib(const char* libname)
{

#if defined __WIN32
    return LoadLibraryA(libname);
#elif defined __linux__  || defined __APPLE__ 
    return dlopen(libname,RTLD_LAZY);
#endif

}

DLSYM
GetDLSym(DLH dlh,const char* sym_name)
{

#if defined __WIN32
    return GetProcAddress(dlh,sym_name);
#elif defined __linux__  || defined __APPLE__ 
    return dlsym(dlh,sym_name);
#endif

}


std::vector<std::string>
GetAllPlugins()
{
    std::vector<std::string> ret;

    wxDir dir("plugins/");

    if(!dir.IsOpened())
    {
        printf("error: plugins dir not found.");
        return ret;
    }

    const char* dlext =
#if defined(__linux__)
    ".so"
#elif defined(__WIN32)
    ".dll"
#elif defined(__APPLE__)
    ".dylib"
#endif
    ;


    wxString filename;
    bool next = dir.GetFirst(&filename,std::string("*")+dlext);
    while(next)
    {
        ret.push_back(filename.ToStdString());
        
        next = dir.GetNext(&filename);
    }

    return ret;
}


std::vector<EFunc> g_ExportFuncs;

void
LoadPlugins()
{
    std::vector<std::string> plugins = GetAllPlugins();

    for(int i=0;i<plugins.size();i++)
    {

        const char* libname = ("plugins/"+plugins[i]).c_str();

        DLH h = LoadLib(libname);
        
        if(!h)
        {
            printf("error: failed to load %s.\n",libname);
            return;
        }

        DLSYM sym = GetDLSym(h,"Register");

        if(!sym)
        {
            printf("error: (%s) Register function not found.\n",libname);
            return;
        }

        ((void (*)(void))sym)();

    }


    g_ExportFuncs = TMC_GetAllEFunc();

}


#endif //PLUGINS_H