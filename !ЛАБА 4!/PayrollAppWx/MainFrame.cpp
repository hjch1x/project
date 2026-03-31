#include "MainFrame.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include "StatisticsDialog.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(101, MainFrame::OnAdd)
EVT_BUTTON(102, MainFrame::OnEdit)
EVT_BUTTON(103, MainFrame::OnDelete)
EVT_BUTTON(104, MainFrame::OnImport)
EVT_BUTTON(105, MainFrame::OnExport)
EVT_BUTTON(106, MainFrame::OnSort)
EVT_BUTTON(107, MainFrame::OnStatistics)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 500))
{
    wxPanel* panel = new wxPanel(this);

    grid = new wxGrid(panel, wxID_ANY);
    grid->CreateGrid(0, 4);
    grid->SetColLabelValue(0, "ID");
    grid->SetColLabelValue(1, "Имя");
    grid->SetColLabelValue(2, "Фамилия");
    grid->SetColLabelValue(3, "Зарплата");
    grid->SetColSize(0, 50);
    grid->SetColSize(1, 150);
    grid->SetColSize(2, 150);
    grid->SetColSize(3, 100);

    grid->EnableEditing(false);
    grid->DisableDragRowSize();
    grid->DisableDragColSize();
    grid->SetRowLabelSize(40);

    wxButton* btnAdd = new wxButton(panel, 101, "Добавить", wxDefaultPosition, wxSize(100, 30));
    wxButton* btnEdit = new wxButton(panel, 102, "Редактировать", wxDefaultPosition, wxSize(100, 30));
    wxButton* btnDelete = new wxButton(panel, 103, "Удалить", wxDefaultPosition, wxSize(100, 30));
    wxButton* btnImport = new wxButton(panel, 104, "Импорт", wxDefaultPosition, wxSize(100, 30));
    wxButton* btnExport = new wxButton(panel, 105, "Экспорт", wxDefaultPosition, wxSize(100, 30));
    wxButton* btnSort = new wxButton(panel, 106, "Сортировать", wxDefaultPosition, wxSize(100, 30));
    wxButton* btnStatistics = new wxButton(panel, 107, "Статистика", wxDefaultPosition, wxSize(100, 30));

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(btnAdd, 0, wxALL, 5);
    btnSizer->Add(btnEdit, 0, wxALL, 5);
    btnSizer->Add(btnDelete, 0, wxALL, 5);
    btnSizer->Add(btnImport, 0, wxALL, 5);
    btnSizer->Add(btnExport, 0, wxALL, 5);
    btnSizer->Add(btnSort, 0, wxALL, 5);
    btnSizer->Add(btnStatistics, 0, wxALL, 5);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER);

    panel->SetSizer(mainSizer);
    Centre();
    LoadDataToGrid();
}

void MainFrame::LoadDataToGrid()
{
    if (grid->GetNumberRows() > 0)
    {
        grid->DeleteRows(0, grid->GetNumberRows());
    }

    auto& employees = PayrollSystem::GetInstance().GetEmployees();

    for (size_t i = 0; i < employees.size(); i++)
    {
        grid->AppendRows(1);
        grid->SetCellValue(i, 0, wxString::Format("%d", employees[i].GetId()));
        grid->SetCellValue(i, 1, employees[i].GetFirstName());
        grid->SetCellValue(i, 2, employees[i].GetLastName());
        grid->SetCellValue(i, 3, wxString::Format("%.2f", employees[i].GetSalary()));
    }
}

void MainFrame::OnAdd(wxCommandEvent& evt)
{
    EditDialog dialog(this, "Добавить сотрудника", nullptr);
    if (dialog.ShowModal() == wxID_OK)
    {
        LoadDataToGrid();
    }
}

void MainFrame::OnEdit(wxCommandEvent& evt)
{
    int selectedRow = grid->GetGridCursorRow();
    if (selectedRow < 0)
    {
        wxMessageBox("Выберите сотрудника для редактирования", "Внимание", wxOK | wxICON_WARNING);
        return;
    }

    int id = wxAtoi(grid->GetCellValue(selectedRow, 0));
    Employee* emp = PayrollSystem::GetInstance().FindEmployee(id);

    if (emp)
    {
        EditDialog dialog(this, "Редактировать сотрудника", emp);
        if (dialog.ShowModal() == wxID_OK)
        {
            LoadDataToGrid();
        }
    }
}

void MainFrame::OnDelete(wxCommandEvent& evt)
{
    int selectedRow = grid->GetGridCursorRow();
    if (selectedRow < 0)
    {
        wxMessageBox("Выберите сотрудника для удаления", "Внимание", wxOK | wxICON_WARNING);
        return;
    }

    int result = wxMessageBox("Удалить выбранного сотрудника?", "Подтверждение", wxYES_NO | wxICON_QUESTION);
    if (result == wxYES)
    {
        int id = wxAtoi(grid->GetCellValue(selectedRow, 0));
        PayrollSystem::GetInstance().RemoveEmployee(id);
        LoadDataToGrid();
    }
}

void MainFrame::OnImport(wxCommandEvent& evt)
{
    wxFileDialog openFileDialog(this, "Импорт данных", "", "", "Text files (*.txt)|*.txt|All files (*.*)|*.*", wxFD_OPEN);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        try
        {
            PayrollSystem::GetInstance().LoadFromFile(openFileDialog.GetPath().ToStdString());
            LoadDataToGrid();
            wxMessageBox("Данные успешно загружены!", "Успех", wxOK | wxICON_INFORMATION);
        }
        catch (...)
        {
            wxMessageBox("Ошибка импорта", "Ошибка", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnExport(wxCommandEvent& evt)
{
    wxFileDialog saveFileDialog(this, "Экспорт данных", "", "", "Text files (*.txt)|*.txt", wxFD_SAVE);

    if (saveFileDialog.ShowModal() == wxID_OK)
    {
        try
        {
            PayrollSystem::GetInstance().SaveToFile(saveFileDialog.GetPath().ToStdString());
            wxMessageBox("Данные успешно сохранены!", "Успех", wxOK | wxICON_INFORMATION);
        }
        catch (...)
        {
            wxMessageBox("Ошибка экспорта", "Ошибка", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnSort(wxCommandEvent& evt)
{
    PayrollSystem::GetInstance().SortBySalary();
    LoadDataToGrid();

    int sortState = PayrollSystem::GetInstance().GetSortState();
    wxButton* btnSort = (wxButton*)FindWindow(106);

    if (btnSort)
    {
        if (sortState == 0)
            btnSort->SetLabel("Сортировать");
        else if (sortState == 1)
            btnSort->SetLabel("По убыванию ");
        else if (sortState == 2)
            btnSort->SetLabel("По возрастанию ");
    }
}

void MainFrame::OnStatistics(wxCommandEvent& evt)
{
    auto& employees = PayrollSystem::GetInstance().GetEmployees();
    int count = employees.size();

    if (count == 0)
    {
        wxMessageBox("Нет сотрудников для отображения статистики!", "Внимание", wxOK | wxICON_WARNING);
        return;
    }

    if (count <= 3)
    {
        StatisticsDialog* dialog = new StatisticsDialog(this, "Статистика отдела (Базовая)", true);
    }
    else
    {
        StatisticsDialog* dialog = new StatisticsDialog(this, "Статистика отдела (Расширенная)", false, "", "");
    }
}