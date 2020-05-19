#ifndef CONFIG_H
#define CONFIG_H

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class ConfigsPanel
: public wxPanel
{
    public:
    
    ConfigsPanel(wxWindow* parent);

    void Change(wxPropertyGridEvent& event);

    wxDECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(ConfigsPanel,wxPanel)
END_EVENT_TABLE()



void
ConfigsPanel::Change(wxPropertyGridEvent& event)
{

    for(int i=0;i<4;i++)
        if(event.GetPropertyName() == (std::string("_freq")+std::to_string(i)+"_blur").c_str())
        {
            TMC_SetFreqMapBlur(event.GetValue().GetInteger(),i);
            return;
        }

    for(int i=0;i<4;i++)
        if(event.GetPropertyName() == (std::string("_freq")+std::to_string(i)+"_mul").c_str())
        {
            TMC_SetFreqMapMultiplier(event.GetValue().GetDouble(),i);
            return;
        }
    
    if(event.GetPropertyName() == "_view_dist")
        TMC_SetViewDistance(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_view_angle")
        TMC_SetViewAngleSpeed(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_mheight")
        TMC_SetMeshHeight(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_mwidth")
        TMC_SetMeshWidth(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_nerosions")
        TMC_SetNumErosions(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_erosion_steps")
        TMC_SetErosionSteps(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_resolution")
        TMC_SetResultResolution(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_tessellation")
        TMC_SetTessellation(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_erosion_amount")
        TMC_SetErosionAmount(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_erosion_max")
        TMC_SetErosionMax(event.GetValue().GetDouble());
    else if(event.GetPropertyName() == "_erosion_dep")
        TMC_SetErosionAndDeposit(event.GetValue().GetBool());
    else if(event.GetPropertyName() == "_freq_normalize")
        TMC_SetFreqNormalize(event.GetValue().GetBool());
    else if(event.GetPropertyName() == "_freq_view")
        TMC_SetFreqView(event.GetValue().GetBool());
    else
        printf("%s is not implemented.\n",event.GetPropertyName().c_str().AsChar());

}

void
OnGenerate(wxCommandEvent& event)
{
    TMC_GenHeightMap();
    TMC_GenErosionMap();
    TMC_GenMesh();
    TMC_GenColorMap();

}

ConfigsPanel::ConfigsPanel(wxWindow* parent)
: wxPanel(parent)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);


    wxPropertyGrid* prop = new wxPropertyGrid(this);
    sizer->Add(prop,1,wxEXPAND | wxALL);

    //type the initial value on the constructr.
    //not time to create get functions just for initial value synchronization
    
    prop->Append(new wxIntProperty("Resolution:","_resolution",256));
    prop->Append(new wxIntProperty("Tessellation:","_tessellation",256));
   
    prop->Append(new wxIntProperty("FreqMap0 Blur:","_freq0_blur",4));
    prop->Append(new wxIntProperty("FreqMap1 Blur:","_freq1_blur",6));
    prop->Append(new wxIntProperty("FreqMap2 Blur:","_freq2_blur",12));
    prop->Append(new wxIntProperty("FreqMap3 Blur:","_freq3_blur",24));

    prop->Append(new wxFloatProperty("FreqMap0 Mul:","_freq0_mul",0.5));
    prop->Append(new wxFloatProperty("FreqMap1 Mul:","_freq1_mul",0.25));
    prop->Append(new wxFloatProperty("FreqMap2 Mul:","_freq2_mul",0.125));
    prop->Append(new wxFloatProperty("FreqMap3 Mul:","_freq3_mul",0.0625));

    prop->Append(new wxBoolProperty("FreqMap Normalize:","_freq_normalize",false));
    prop->Append(new wxBoolProperty("FreqMap View:","_freq_view",false));

    prop->Append(new wxIntProperty("Num Erosions:","_nerosions",5000));
    prop->Append(new wxIntProperty("Erosion Steps:","_erosion_steps",64));
    prop->Append(new wxFloatProperty("Erosion Amount:","_erosion_amount",0.8f));
    prop->Append(new wxFloatProperty("Max Erosion:","_erosion_max",0.02f));
    prop->Append(new wxBoolProperty("Erosion & Deposit:","_erosion_dep",false));

    prop->Append(new wxFloatProperty("Mesh Height:","_mheight",50));
    prop->Append(new wxFloatProperty("Mesh Width:","_mwidth",50));

    prop->Append(new wxFloatProperty("View Distance:","_view_dist",50));
    prop->Append(new wxFloatProperty("View Angle Speed:","_view_angle",0.2f));

    prop->Bind(wxEVT_PG_CHANGED,&ConfigsPanel::Change,this);

    
    wxButton* gen = new wxButton(this,wxID_ANY,"Generate");

    gen->Bind(wxEVT_BUTTON,OnGenerate);

    sizer->Add(gen);


    SetSizer(sizer);
    sizer->Layout();

}

#endif //CONFIG_H