#pragma once
#include <string>

class Employee
{
private:
    int id;
    std::string firstName;
    std::string lastName;
    double baseSalary;

public:
    Employee(int empId, const std::string& first, const std::string& last, double salary = 0);

    int GetId() const;
    std::string GetFirstName() const;
    std::string GetLastName() const;
    double GetSalary() const;

    void SetName(const std::string& first, const std::string& last);
    void SetSalary(double salary);
    double CalculateSalary() const;
};