using System;

namespace PayrollApp
{
    //надбавка
    public class BonusWork : SalaryStrategy
    {
        private string jobType;
        private double baseRate;
        private double bonusPercentage;

        public BonusWork(string job, double rate, double bonus)
        {
            jobType = job;
            baseRate = rate;
            bonusPercentage = bonus;
        }

        public override double GetSalary() => baseRate * (1 + bonusPercentage / 100);
        public override string GetJobType() => jobType;
        public override double GetBaseRate() => baseRate;
        public override double GetBonusPercentage() => bonusPercentage;
    }
}