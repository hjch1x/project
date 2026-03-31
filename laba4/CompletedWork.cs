using System;

namespace PayrollApp
{
    //выполненная работа
    public class CompletedWork
    {
        private SalaryStrategy workType;
        private int hours;

        public CompletedWork(SalaryStrategy work, int hrs)
        {
            workType = work;
            hours = hrs;
        }

        public int GetHours() => hours;
        public double GetCost() => workType.GetSalary() * hours;
        public SalaryStrategy GetWorkType() => workType;
    }
}