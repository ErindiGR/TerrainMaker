#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/filedlg.h>

#include <core.h>
#include <export.h>


class View* g_View;
class ConfigsPanel* g_ConfigsPanel;

#include "plugins.h"
#include "editor.h"




class EditorApp
: public wxApp
{
    public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(EditorApp);


#define EXPORT_ID 27700

void OnExport(wxCommandEvent& event)
{
    for(int i=0;i<g_ExportFuncs.size();i++)
        if(event.GetId() == EXPORT_ID+i)
        {
            wxFileDialog* expwin = new wxFileDialog(g_View,
            std::string("Save ")+g_ExportFuncs[i].m_menu_name,"",
            g_ExportFuncs[i].m_filename,g_ExportFuncs[i].m_extension,wxFD_SAVE);

            if(expwin->ShowModal() == wxID_CANCEL)
                return;

            TMC_SetPath(expwin->GetPath().ToStdString());

            g_ExportFuncs[i].m_export_func();
        }
}


bool
EditorApp::OnInit()
{
    EditorFrame* e = new EditorFrame();

    LoadPlugins();

    wxMenuBar* menu = new wxMenuBar();
    
    wxMenu* menu_file = new wxMenu();
    wxMenu* menu_export = new wxMenu();
    
    menu_file->AppendSubMenu(menu_export,wxT("Export"));

    for(int i=0;i<g_ExportFuncs.size();i++)
    {
        menu_export->Append(EXPORT_ID+i,g_ExportFuncs[i].m_menu_name,g_ExportFuncs[i].m_description);
    }
    menu_export->Bind(wxEVT_MENU,OnExport);
    
    menu->Append(menu_file,wxT("File"));

    e->SetMenuBar(menu);
    e->Show(true);
    SetTopWindow(e);

    return true;
}
