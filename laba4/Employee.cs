using System;
using System.Collections.Generic;

namespace PayrollApp
{
    public class Employee
    {
        private int id;
        private string firstName;
        private string lastName;
        private double baseSalary;
        private List<CompletedWork> completedWorks;

        public Employee(int empId, string first, string last, double salary = 0)
        {
            id = empId;
            firstName = first;
            lastName = last;
            baseSalary = salary;
            completedWorks = new List<CompletedWork>();
        }

        public void AddCompletedWork(CompletedWork work)
        {
            completedWorks.Add(work);
        }

        public double CalculateSalary()
        {
            return baseSalary;
        }

        public int GetId() => id;
        public string GetFirstName() => firstName;
        public string GetLastName() => lastName;
        public List<CompletedWork> GetCompletedWorks() => completedWorks;

        public void SetName(string first, string last)
        {
            firstName = first;
            lastName = last;
        }

        public void SetSalary(double salary)
        {
            baseSalary = salary;
        }

        public double GetSalary() => baseSalary;
    }
}