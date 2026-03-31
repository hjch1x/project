#include "PayrollSystem.h"
#include <fstream>
#include <sstream>
#include <algorithm>

PayrollSystem* PayrollSystem::instance = nullptr;

PayrollSystem::PayrollSystem() : nextEmployeeId(1), sortState(0)
{
}

PayrollSystem& PayrollSystem::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new PayrollSystem();
    }
    return *instance;
}

Employee& PayrollSystem::AddEmployee(const std::string& first, const std::string& last, double salary)
{
    employees.emplace_back(nextEmployeeId++, first, last, salary);
    originalEmployees = employees;
    sortState = 0;
    return employees.back();
}

Employee* PayrollSystem::FindEmployee(int id)
{
    for (auto& emp : employees)
    {
        if (emp.GetId() == id)
            return &emp;
    }
    return nullptr;
}

void PayrollSystem::RemoveEmployee(int id)
{
    auto it = std::remove_if(employees.begin(), employees.end(),
        [id](const Employee& emp) { return emp.GetId() == id; });
    employees.erase(it, employees.end());
    originalEmployees = employees;
    sortState = 0;
}

void PayrollSystem::UpdateEmployee(int id, const std::string& first, const std::string& last, double salary)
{
    Employee* emp = FindEmployee(id);
    if (emp)
    {
        emp->SetName(first, last);
        emp->SetSalary(salary);
        originalEmployees = employees;
    }
}

void PayrollSystem::SaveToFile(const std::string& filename)
{
    std::ofstream file(filename);
    for (const auto& emp : employees)
    {
        file << "EMP," << emp.GetId() << "," << emp.GetFirstName() << ","
            << emp.GetLastName() << "," << emp.GetSalary() << "\n";
    }
    file.close();
}

void PayrollSystem::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type, idStr, first, last, salaryStr;

        std::getline(ss, type, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, first, ',');
        std::getline(ss, last, ',');
        std::getline(ss, salaryStr, ',');

        if (type == "EMP")
        {
            int id = std::stoi(idStr);
            double salary = std::stod(salaryStr);
            Employee emp(id, first, last, salary);
            employees.push_back(emp);

            if (id >= nextEmployeeId)
                nextEmployeeId = id + 1;
        }
    }
    file.close();
    originalEmployees = employees;
    sortState = 0;
}

std::vector<Employee>& PayrollSystem::GetEmployees()
{
    return employees;
}

void PayrollSystem::SortBySalary()
{
    sortState = (sortState + 1) % 3;

    if (sortState == 0)
    {
        employees = originalEmployees;
    }
    else if (sortState == 1)
    {
        std::sort(employees.begin(), employees.end(),
            [](const Employee& a, const Employee& b) {
                return a.GetSalary() > b.GetSalary();
            });
    }
    else if (sortState == 2)
    {
        std::sort(employees.begin(), employees.end(),
            [](const Employee& a, const Employee& b) {
                return a.GetSalary() < b.GetSalary();
            });
    }
}

int PayrollSystem::GetSortState()
{
    return sortState;
}