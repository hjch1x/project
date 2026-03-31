using System;

namespace PayrollApp
{
    //обычная работа (без надбавки)
    public class RegularWork : SalaryStrategy
    {
        private string jobType;
        private double baseRate;

        public RegularWork(string job, double rate)
        {
            jobType = job;
            baseRate = rate;
        }

        public override double GetSalary() => baseRate;
        public override string GetJobType() => jobType;
        public override double GetBaseRate() => baseRate;
        public override double GetBonusPercentage() => 0.0;
    }
}