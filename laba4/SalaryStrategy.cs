using System;

namespace PayrollApp
{
    // абстрактный класс стратегии расчёта зарплаты
    public abstract class SalaryStrategy
    {
        public abstract double GetSalary();
        public abstract string GetJobType();
        public abstract double GetBaseRate();
        public abstract double GetBonusPercentage();
    }
}