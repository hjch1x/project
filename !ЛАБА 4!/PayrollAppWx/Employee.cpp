#include "Employee.h"

Employee::Employee(int empId, const std::string& first, const std::string& last, double salary)
    : id(empId), firstName(first), lastName(last), baseSalary(salary)
{
}

int Employee::GetId() const { return id; }
std::string Employee::GetFirstName() const { return firstName; }
std::string Employee::GetLastName() const { return lastName; }
double Employee::GetSalary() const { return baseSalary; }

void Employee::SetName(const std::string& first, const std::string& last)
{
    firstName = first;
    lastName = last;
}

void Employee::SetSalary(double salary)
{
    baseSalary = salary;
}

double Employee::CalculateSalary() const
{
    return baseSalary;
}