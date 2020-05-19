#ifndef VIEW_H
#define VIEW_H

#include <wx/glcanvas.h>



#define REFRESH_TIME 16

class View
: public wxPanel
{
    

    public:
    View(wxWindow* parent);
    class ViewCanvas* canvas;
    wxTimer refreshTimer;

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
    glClearColor(0.1,0.1,0.5,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TMC_SetAspectRatio(GetSize().x/(float)GetSize().y);


    TMC_Draw();


    SwapBuffers();
}

void
OnRefreshTimer(wxTimerEvent& event)
{
    g_View->canvas->Refresh();
    g_View->refreshTimer.Start(REFRESH_TIME);
}


View::View(wxWindow* parent)
: wxPanel(parent,wxID_ANY),
refreshTimer(this,wxID_ANY)
{
    wxGridSizer* sizer = new wxGridSizer(1,1,0,0);
    SetSizer(sizer);
    canvas = new ViewCanvas(this);
    canvas->Show();
    sizer->Add(canvas,1,wxEXPAND|wxALL);
    sizer->Layout();

    this->Bind(wxEVT_TIMER,&OnRefreshTimer);
    refreshTimer.Start(REFRESH_TIME);
}

#endif //VIEW_H