#include "StatisticsDialog.h"
#include <wx/statline.h>

wxBEGIN_EVENT_TABLE(StatisticsDialog, wxFrame)
EVT_CLOSE(StatisticsDialog::OnCloseWindow)
wxEND_EVENT_TABLE()

// статистика < 3 сотр
StatisticsDialog::StatisticsDialog(wxWindow* parent, const wxString& title, bool isBasicStats)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(500, 350))
{
    this->isBasicStats = isBasicStats;

    auto& employees = PayrollSystem::GetInstance().GetEmployees();

    double total = 0;
    for (const auto& emp : employees)
    {
        total += emp.GetSalary();
    }

    double avgSalary = employees.size() > 0 ? total / employees.size() : 0;

    wxString avgText = wxString::Format("%.2f руб.", avgSalary);
    wxString countText = wxString::Format("%d сотрудников", (int)employees.size());

    CreateInterface(avgText, countText);
    Show(true);
}

// стата больше 3
StatisticsDialog::StatisticsDialog(wxWindow* parent, const wxString& title, bool isBasicStats,
    const wxString& maxEmployee, const wxString& minEmployee)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(550, 450))
{
    this->isBasicStats = isBasicStats;

    auto& employees = PayrollSystem::GetInstance().GetEmployees();

    double total = 0;
    double maxSalary = 0;
    double minSalary = employees.size() > 0 ? employees[0].GetSalary() : 0;
    wxString maxEmpName = "";
    wxString minEmpName = "";

    for (const auto& emp : employees)
    {
        total += emp.GetSalary();

        if (emp.GetSalary() > maxSalary)
        {
            maxSalary = emp.GetSalary();
            maxEmpName = emp.GetFirstName() + " " + emp.GetLastName();
        }

        if (emp.GetSalary() < minSalary)
        {
            minSalary = emp.GetSalary();
            minEmpName = emp.GetFirstName() + " " + emp.GetLastName();
        }
    }

    double avgSalary = employees.size() > 0 ? total / employees.size() : 0;

    wxString avgText = wxString::Format("%.2f руб.", avgSalary);
    wxString countText = wxString::Format("%d сотрудников", (int)employees.size());
    wxString maxText = wxString::Format("%.2f руб. (%s)", maxSalary, maxEmpName);
    wxString minText = wxString::Format("%.2f руб. (%s)", minSalary, minEmpName);

    CreateInterface(avgText, countText, maxText, minText, maxEmpName, minEmpName);
    Show(true);
}

// интерфейс
void StatisticsDialog::CreateInterface(const wxString& avgSalary, const wxString& totalEmployees,
    const wxString& maxSalary, const wxString& minSalary,
    const wxString& maxEmployee, const wxString& minEmployee)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* lblTitle = new wxStaticText(panel, wxID_ANY, "Статистика отдела",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    lblTitle->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(lblTitle, 0, wxALL | wxEXPAND, 15);

    wxStaticLine* line1 = new wxStaticLine(panel, wxID_ANY);
    mainSizer->Add(line1, 0, wxEXPAND | wxLEFT | wxRIGHT, 15);

    wxStaticBox* basicBox = new wxStaticBox(panel, wxID_ANY, "Базовая статистика");
    wxStaticBoxSizer* basicSizer = new wxStaticBoxSizer(basicBox, wxVERTICAL);

    wxStaticText* lblCount = new wxStaticText(panel, wxID_ANY,
        "Всего сотрудников: " + totalEmployees);
    lblCount->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    basicSizer->Add(lblCount, 0, wxALL, 8);

    wxStaticText* lblAvg = new wxStaticText(panel, wxID_ANY,
        "Средняя зарплата: " + avgSalary);
    lblAvg->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    basicSizer->Add(lblAvg, 0, wxALL, 8);

    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 10);

    if (!isBasicStats)
    {
        wxStaticLine* line2 = new wxStaticLine(panel, wxID_ANY);
        mainSizer->Add(line2, 0, wxEXPAND | wxLEFT | wxRIGHT, 15);

        wxStaticBox* extBox = new wxStaticBox(panel, wxID_ANY, "Расширенная статистика");
        wxStaticBoxSizer* extSizer = new wxStaticBoxSizer(extBox, wxVERTICAL);

        wxStaticText* lblMax = new wxStaticText(panel, wxID_ANY,
            "Максимальная ЗП: " + maxSalary);
        lblMax->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        extSizer->Add(lblMax, 0, wxALL, 8);

        wxStaticText* lblMin = new wxStaticText(panel, wxID_ANY,
            "Минимальная ЗП: " + minSalary);
        lblMin->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        extSizer->Add(lblMin, 0, wxALL, 8);

        mainSizer->Add(extSizer, 0, wxEXPAND | wxALL, 10);
    }

    wxButton* btnClose = new wxButton(panel, wxID_CLOSE, "Закрыть",
        wxDefaultPosition, wxSize(120, 30));
    mainSizer->Add(btnClose, 0, wxALL | wxALIGN_CENTER, 15);

    btnClose->Bind(wxEVT_BUTTON, &StatisticsDialog::OnClose, this);

    panel->SetSizer(mainSizer);
    Centre();
}

// обработчик "закрыть"
void StatisticsDialog::OnClose(wxCommandEvent& evt)
{
    Close(true);
}

// крестик
void StatisticsDialog::OnCloseWindow(wxCloseEvent& evt)
{
    Destroy();
}