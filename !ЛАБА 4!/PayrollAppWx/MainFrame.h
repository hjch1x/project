#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include "Employee.h"
#include "PayrollSystem.h"
#include "EditDialog.h"
#include "StatisticsDialog.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    wxGrid* grid;

    void LoadDataToGrid();
    void OnAdd(wxCommandEvent& evt);
    void OnEdit(wxCommandEvent& evt);
    void OnDelete(wxCommandEvent& evt);
    void OnImport(wxCommandEvent& evt);
    void OnExport(wxCommandEvent& evt);
    void OnSort(wxCommandEvent& evt);
    void OnStatistics(wxCommandEvent& evt);

    wxDECLARE_EVENT_TABLE();
};