
#include "wxdefs.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/glcanvas.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <terrain.h>


class View;
class ViewCanvas;
class Configs;

View* view;
Configs* configs;

class EditorApp: public wxApp
{
    public:
    virtual bool OnInit();
};

class EditorFrame: public wxFrame
{
    public:
    EditorFrame();
};

class View: public wxPanel
{
    ViewCanvas* canvas;
    public:
    View(wxWindow* parent);
};

class ViewCanvas: public wxGLCanvas
{
    wxGLContext* glcontext;
    public:
    ViewCanvas(wxWindow* parent);
    void Paint(wxPaintEvent& event);


    DECLARE_EVENT_TABLE();
};
BEGIN_EVENT_TABLE(ViewCanvas,wxGLCanvas)
    EVT_PAINT(ViewCanvas::Paint)
END_EVENT_TABLE()


class Configs: public wxPanel
{
    wxSlider* color_r;
    public:
    Configs(wxWindow* parent);

    void FloatSlider(wxPropertyGridEvent& event);

    wxDECLARE_EVENT_TABLE();
};
BEGIN_EVENT_TABLE(Configs,wxPanel)
END_EVENT_TABLE()

wxIMPLEMENT_APP(EditorApp);

ViewCanvas::ViewCanvas(wxWindow* parent)
: wxGLCanvas(parent,-1,(const int*)__null,wxDefaultPosition,wxDefaultSize)
{
    glcontext = new wxGLContext(this);
}


bool start=false;
void ViewCanvas::Paint(wxPaintEvent& event)
{
    SetCurrent(*glcontext);
    wxPaintDC(this);
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    if(!start)
    {
        TMC_CalculateMaps();
        start=true;
    }

    TMC_Draw();

    SwapBuffers();
}

bool EditorApp::OnInit()
{
    EditorFrame* e= new EditorFrame();
    e->Show(true);
    SetTopWindow(e);
    return true;
}

View::View(wxWindow* parent)
: wxPanel(parent,wxID_ANY)
{
    wxGridSizer* sizer = new wxGridSizer(1,1,0,0);
    SetSizer(sizer);
    canvas = new ViewCanvas(this);
    sizer->Add(canvas,1,wxEXPAND|wxALL);
    sizer->Layout();
}

EditorFrame::EditorFrame()
: wxFrame(NULL,-1,"Terrain Maker",wxPoint(50,50),wxSize(800,600))
{
    wxMenuBar* menu = new wxMenuBar();
    menu->Append(new wxMenu(),"File");
    SetMenuBar(menu);

    SetStatusBar(new wxStatusBar(this));

    wxSplitterWindow* splitter = new wxSplitterWindow(this);

    view = new View(splitter);
    configs = new Configs(splitter);
    splitter->SplitVertically(view,configs);
}

void
Configs::FloatSlider(wxPropertyGridEvent& event)
{
    view->Refresh();
    
}


Configs::Configs(wxWindow* parent)
: wxPanel(parent)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxPropertyGrid* prop = new wxPropertyGrid(this);
    
    sizer->Add(prop,1,wxEXPAND | wxALL);
    
    wxPropertyCategory* base = new wxPropertyCategory("Configs:");
    prop->Append(base);
    
    wxPropertyCategory* position = new wxPropertyCategory("Perlin:");
    base->AppendChild(position);
    position->AppendChild(new wxIntProperty("Seed:","_perlin_seed"));
    position->AppendChild(new wxFloatProperty("Redius:","_radius"));

    SetSizer(sizer);
    sizer->Layout();
    


    prop->Bind(wxEVT_PG_CHANGED,&Configs::FloatSlider,this);
}