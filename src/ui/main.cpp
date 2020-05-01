
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
class ConfigsPanel;

View* view;
ConfigsPanel* configsPanel;

class EditorApp
: public wxApp
{
    public:
    virtual bool OnInit();
};

class EditorFrame
: public wxFrame
{
    public:
    EditorFrame();
};

class View
: public wxPanel
{
    ViewCanvas* canvas;
    public:
    View(wxWindow* parent);

};

class ViewCanvas
: public wxGLCanvas
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


class ConfigsPanel: public wxPanel
{
    wxSlider* color_r;
    public:
    ConfigsPanel(wxWindow* parent);

    void Change(wxPropertyGridEvent& event);

    wxDECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(ConfigsPanel,wxPanel)
END_EVENT_TABLE()

wxIMPLEMENT_APP(EditorApp);

ViewCanvas::ViewCanvas(wxWindow* parent)
: wxGLCanvas(parent,-1,(const int*)__null,wxDefaultPosition,wxDefaultSize)
{
    glcontext = new wxGLContext(this);
}


void
ViewCanvas::Paint(wxPaintEvent& event)
{
    SetCurrent(*glcontext);
    wxPaintDC(this);
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_view_angle++;

    g_aspect_ratio = GetSize().x/(float)GetSize().y;

    TMC_Draw();


    SwapBuffers();
}

bool
EditorApp::OnInit()
{
    EditorFrame* e = new EditorFrame();
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
    canvas->Show();
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

    wxSplitterWindow* splitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition,
    wxDefaultSize,wxSP_LIVE_UPDATE);

    view = new View(splitter);
    view->Show();
    configsPanel = new ConfigsPanel(splitter);
    configsPanel->Show();
    splitter->SplitVertically(view,configsPanel,400);
    splitter->Layout();
}

void
ConfigsPanel::Change(wxPropertyGridEvent& event)
{

    if(event.GetPropertyName() == "_perlin_high_blur")
    {
        g_highf_blur = event.GetValue().GetInteger();
        TMC_GenHFMap();
    }
    
    if(event.GetPropertyName() == "_perlin_low_blur")
    {
        g_lowf_blur = event.GetValue().GetInteger();
        TMC_GenLFMap();
    }

    
    if(event.GetPropertyName() == "_view_dist")
        g_view_distance = event.GetValue().GetDouble();
    else if(event.GetPropertyName() == "_view_angle")
        g_view_angle = event.GetValue().GetDouble();
    else if(event.GetPropertyName() == "_height")
        g_height = event.GetValue().GetDouble();

    //only way to refresh the splitter
    wxSplitterWindow* s = (wxSplitterWindow*)view->GetParent();
    s->SetSashPosition(s->GetSashPosition()+1);
}


ConfigsPanel::ConfigsPanel(wxWindow* parent)
: wxPanel(parent)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxPropertyGrid* prop = new wxPropertyGrid(this);
    sizer->Add(prop,1,wxEXPAND | wxALL);
    
    wxPropertyCategory* base = new wxPropertyCategory("ConfigsPanel:");
    prop->Append(base);
    
    wxPropertyCategory* position = new wxPropertyCategory("Perlin:");
    base->AppendChild(position);
    position->AppendChild(new wxIntProperty("High Blur:","_perlin_high_blur"));
    position->AppendChild(new wxIntProperty("Low Blur:","_perlin_low_blur"));
    position->AppendChild(new wxIntProperty("Seed:","_perlin_seed"));
    position->AppendChild(new wxFloatProperty("Height:","_height"));
    position->AppendChild(new wxFloatProperty("View Distance:","_view_dist"));
    position->AppendChild(new wxFloatProperty("View Angle:","_view_angle"));

    prop->Bind(wxEVT_PG_CHANGED,&ConfigsPanel::Change,this);



    SetSizer(sizer);
    sizer->Layout();

}