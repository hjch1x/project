#pragma once
#include <vector>
#include <string>
#include "Employee.h"

class PayrollSystem
{
private:
    static PayrollSystem* instance;
    std::vector<Employee> employees;
    std::vector<Employee> originalEmployees;
    int nextEmployeeId;
    int sortState;  // 0 исх, 1 уб, 2 возр

    PayrollSystem();

public:
    static PayrollSystem& GetInstance();

    Employee& AddEmployee(const std::string& first, const std::string& last, double salary);
    Employee* FindEmployee(int id);
    void RemoveEmployee(int id);
    void UpdateEmployee(int id, const std::string& first, const std::string& last, double salary);

    void SaveToFile(const std::string& filename);
    void LoadFromFile(const std::string& filename);

    std::vector<Employee>& GetEmployees();
    void SortBySalary();
    int GetSortState();

    PayrollSystem(const PayrollSystem&) = delete;
    void operator=(const PayrollSystem&) = delete;
};