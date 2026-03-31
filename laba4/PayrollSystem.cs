using System;
using System.Collections.Generic;
using System.IO;

namespace PayrollApp
{
    //система расчёта зарплатысинглтон
    public class PayrollSystem
    {
        private static PayrollSystem instance;
        private List<Employee> employees;
        private List<SalaryStrategy> jobTypes;
        private int nextEmployeeId;

        private PayrollSystem()
        {
            employees = new List<Employee>();
            jobTypes = new List<SalaryStrategy>();
            nextEmployeeId = 1;
        }

        public static PayrollSystem GetInstance()
        {
            if (instance == null)
            {
                instance = new PayrollSystem();
            }
            return instance;
        }

        public Employee AddEmployee(string first, string last, double salary = 0)
        {
            Employee emp = new Employee(nextEmployeeId++, first, last, salary);
            employees.Add(emp);
            return emp;
        }

        public Employee FindEmployee(int id)
        {
            foreach (var emp in employees)
            {
                if (emp.GetId() == id) return emp;
            }
            return null;
        }

        public void RemoveEmployee(int id)
        {
            Employee emp = FindEmployee(id);
            if (emp != null)
            {
                employees.Remove(emp);
            }
        }

        public void UpdateEmployee(int id, string first, string last, double salary)
        {
            Employee emp = FindEmployee(id);
            if (emp != null)
            {
                emp.SetName(first, last);
                emp.SetSalary(salary);
            }
        }

        public void AddJobType(string name, double rate, int workType, double bonus = 0)
        {
            if (workType == 0)
            {
                jobTypes.Add(new RegularWork(name, rate));
            }
            else
            {
                jobTypes.Add(new BonusWork(name, rate, bonus));
            }
        }

        public SalaryStrategy GetJobType(int index)
        {
            if (index >= 0 && index < jobTypes.Count)
            {
                return jobTypes[index];
            }
            return null;
        }

        public void SaveToFile(string filename)
        {
            using (StreamWriter writer = new StreamWriter(filename))
            {
                foreach (var emp in employees)
                {
                    writer.WriteLine($"EMP,{emp.GetId()},{emp.GetFirstName()},{emp.GetLastName()},{emp.GetSalary()}");
                }
            }
        }

        public void LoadFromFile(string filename)
        {
            if (!File.Exists(filename))
            {
                throw new FileNotFoundException("Файл не найден");
            }

            using (StreamReader reader = new StreamReader(filename))
            {
                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();
                    string[] parts = line.Split(',');

                    if (parts[0] == "EMP" && parts.Length >= 5)
                    {
                        int id = int.Parse(parts[1]);
                        string first = parts[2];
                        string last = parts[3];
                        double salary = double.Parse(parts[4]);

                        Employee emp = new Employee(id, first, last, salary);
                        employees.Add(emp);

                        if (id >= nextEmployeeId)
                        {
                            nextEmployeeId = id + 1;
                        }
                    }
                }
            }
        }

        public double GetAverageSalary()
        {
            if (employees.Count == 0) return 0;
            double total = 0;
            foreach (var emp in employees)
            {
                total += emp.CalculateSalary();
            }
            return total / employees.Count;
        }

        public double GetTotalPayout()
        {
            double total = 0;
            foreach (var emp in employees)
            {
                total += emp.CalculateSalary();
            }
            return total;
        }

        public List<Employee> GetEmployees() => employees;
        public List<SalaryStrategy> GetJobTypes() => jobTypes;

        public void Clear()
        {
            employees.Clear();
            nextEmployeeId = 1;
        }
    }
}