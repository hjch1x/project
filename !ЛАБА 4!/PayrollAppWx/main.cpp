#include <wx/wx.h>
#include "MainFrame.h"

class PayrollApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

bool PayrollApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MainFrame* frame = new MainFrame("Система расчёта зарплаты");
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(PayrollApp);