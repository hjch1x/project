#pragma once
#include <wx/wx.h>
#include "Employee.h"

class EditDialog : public wxDialog
{
public:
    EditDialog(wxWindow* parent, const wxString& title, Employee* employee);

private:
    wxTextCtrl* txtFirstName;
    wxTextCtrl* txtLastName;
    wxTextCtrl* txtSalary;
    Employee* currentEmployee;
    bool isEditMode;

    void OnSave(wxCommandEvent& evt);

    wxDECLARE_EVENT_TABLE();
};