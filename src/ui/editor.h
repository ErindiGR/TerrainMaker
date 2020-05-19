#ifndef EDITOR_H
#define EDITOR_H


#include <wx/splitter.h>

#include "view.h"
#include "config.h"

class EditorFrame
: public wxFrame
{
    public:
    EditorFrame();
};



EditorFrame::EditorFrame()
: wxFrame(NULL,-1,wxT("Terrain Maker"),wxPoint(50,50),wxSize(800,600))
{
    SetStatusBar(new wxStatusBar(this));

    wxSplitterWindow* splitter = 
    new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_LIVE_UPDATE);

    g_View = new View(splitter);
    g_View->Show();
    g_ConfigsPanel = new ConfigsPanel(splitter);
    g_ConfigsPanel->Show();
    splitter->SplitVertically(g_View,g_ConfigsPanel,400);
    splitter->Layout();
}


#endif //EDIOTR_H