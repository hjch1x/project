#include "EditDialog.h"
#include "PayrollSystem.h"
#include <wx/statline.h>

wxBEGIN_EVENT_TABLE(EditDialog, wxDialog)
EVT_BUTTON(1, EditDialog::OnSave)
wxEND_EVENT_TABLE()

// только буквы
bool IsOnlyLetters(const wxString& text)
{
    for (size_t i = 0; i < text.Length(); i++)
    {
        unsigned int c = text[i];

        // лат
        bool isLatin = (c >= 65 && c <= 90) || (c >= 97 && c <= 122);

        // кириллиц
        bool isCyrillic = (c >= 1040 && c <= 1071) || (c >= 1072 && c <= 1103) ||
            (c == 1025) || (c == 1105);

        // пробел и -
        bool isAllowed = (c == 32) || (c == 45);

        if (!isLatin && !isCyrillic && !isAllowed)
        {
            return false;
        }
    }
    return true;
}

// ток числа
bool IsOnlyNumber(const wxString& text, double& outValue)
{
    return text.ToDouble(&outValue);
}

EditDialog::EditDialog(wxWindow* parent, const wxString& title, Employee* employee)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(400, 250))
{
    currentEmployee = employee;
    isEditMode = (employee != nullptr);

    wxPanel* panel = new wxPanel(this);

    wxStaticText* lblFirstName = new wxStaticText(panel, wxID_ANY, "Имя:", wxPoint(20, 20));
    wxStaticText* lblLastName = new wxStaticText(panel, wxID_ANY, "Фамилия:", wxPoint(20, 60));
    wxStaticText* lblSalary = new wxStaticText(panel, wxID_ANY, "Зарплата:", wxPoint(20, 100));

    txtFirstName = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(130, 20), wxSize(200, 25));
    txtLastName = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(130, 60), wxSize(200, 25));
    txtSalary = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(130, 100), wxSize(200, 25));

    if (isEditMode && currentEmployee)
    {
        txtFirstName->SetValue(currentEmployee->GetFirstName());
        txtLastName->SetValue(currentEmployee->GetLastName());
        txtSalary->SetValue(wxString::Format("%.2f", currentEmployee->GetSalary()));
    }

    wxButton* btnSave = new wxButton(panel, 1, isEditMode ? "Сохранить" : "Добавить", wxPoint(130, 150), wxSize(90, 30));
    wxButton* btnCancel = new wxButton(panel, wxID_CANCEL, "Отмена", wxPoint(240, 150), wxSize(90, 30));

    Centre();
}

void EditDialog::OnSave(wxCommandEvent& evt)
{
    wxString firstName = txtFirstName->GetValue();
    wxString lastName = txtLastName->GetValue();
    wxString salaryStr = txtSalary->GetValue();

    // пустые поля
    if (firstName.Trim().IsEmpty())
    {
        wxMessageBox("Поле 'Имя' не может быть пустым!", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtFirstName->SetFocus();
        return;
    }

    if (lastName.Trim().IsEmpty())
    {
        wxMessageBox("Поле 'Фамилия' не может быть пустым!", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtLastName->SetFocus();
        return;
    }

    if (salaryStr.Trim().IsEmpty())
    {
        wxMessageBox("Поле 'Зарплата' не может быть пустым!", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtSalary->SetFocus();
        return;
    }

    // ток буквы имя
    if (!IsOnlyLetters(firstName))
    {
        wxMessageBox("Поле 'Имя' должно содержать только буквы!\nЦифры и специальные символы запрещены.", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtFirstName->SetFocus();
        txtFirstName->SelectAll();
        return;
    }

    // ток буквы фам
    if (!IsOnlyLetters(lastName))
    {
        wxMessageBox("Поле 'Фамилия' должно содержать только буквы!\nЦифры и специальные символы запрещены.", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtLastName->SetFocus();
        txtLastName->SelectAll();
        return;
    }

    // зп ток числа
    double salary;
    if (!IsOnlyNumber(salaryStr, salary))
    {
        wxMessageBox("Поле 'Зарплата' должно содержать только числа!\nМожно использовать точку или запятую для дробных значений.", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtSalary->SetFocus();
        txtSalary->SelectAll();
        return;
    }

    // зп >=0
    if (salary < 0)
    {
        wxMessageBox("Зарплата не может быть отрицательной!", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtSalary->SetFocus();
        txtSalary->SelectAll();
        return;
    }

    // зп макс 3 000 000
    if (salary > 3000000)
    {
        wxMessageBox("Зарплата не может превышать 3,000,000 руб.!\nМаксимальное значение: 3 миллиона рублей.", "Ошибка валидации", wxOK | wxICON_ERROR);
        txtSalary->SetFocus();
        txtSalary->SelectAll();
        return;
    }

    // если все гуд - сохр
    if (isEditMode && currentEmployee)
    {
        PayrollSystem::GetInstance().UpdateEmployee(
            currentEmployee->GetId(),
            firstName.ToStdString(),
            lastName.ToStdString(),
            salary
        );
    }
    else
    {
        PayrollSystem::GetInstance().AddEmployee(
            firstName.ToStdString(),
            lastName.ToStdString(),
            salary
        );
    }

    EndModal(wxID_OK);
}