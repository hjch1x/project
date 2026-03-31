#pragma once
#include <wx/wx.h>
#include "Employee.h"
#include "PayrollSystem.h"

class StatisticsDialog : public wxFrame
{
public:
    // база
    StatisticsDialog(wxWindow* parent, const wxString& title, bool isBasicStats);

    // полная
    StatisticsDialog(wxWindow* parent, const wxString& title, bool isBasicStats,
        const wxString& maxEmployee, const wxString& minEmployee);

private:
    void CreateInterface(const wxString& avgSalary, const wxString& totalEmployees,
        const wxString& maxSalary = "", const wxString& minSalary = "",
        const wxString& maxEmployee = "", const wxString& minEmployee = "");

    void OnClose(wxCommandEvent& evt);
    void OnCloseWindow(wxCloseEvent& evt);  

    bool isBasicStats;

    wxDECLARE_EVENT_TABLE();
};