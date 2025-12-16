#include <iostream>
#include <windows.h>
#include <clocale>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#undef max

using namespace std;


const float MAX_BASE_RATE = 1000000.0f;
const float MAX_BONUS_PERCENTAGE = 1000.0f;
const size_t MAX_JOB_NAME_LENGTH = 100;

class SalaryStrategy {
public:
    virtual float get_salary() = 0;
    virtual std::string get_job_type() = 0;
    virtual float get_base_rate() = 0;
    virtual float get_bonus_percentage() = 0;
};

class RegularWork : public SalaryStrategy {
private:
    std::string job_type;
    float base_rate;
public:
    RegularWork(std::string job, float rate) {
        job_type = job;
        base_rate = rate;
    }

    float get_salary() override {
        return base_rate;
    }

    std::string get_job_type() override {
        return job_type;
    }

    float get_base_rate() override {
        return base_rate;
    }

    float get_bonus_percentage() override {
        return 0.0f;
    }
};

class BonusWork : public SalaryStrategy {
private:
    std::string job_type;
    float base_rate;
    float bonus_percentage;
public:
    BonusWork(std::string job, float rate, float bonus) {
        job_type = job;
        base_rate = rate;
        bonus_percentage = bonus;
    }

    float get_salary() override {
        return base_rate * (1 + bonus_percentage / 100);
    }

    std::string get_job_type() override {
        return job_type;
    }

    float get_base_rate() override {
        return base_rate;
    }

    float get_bonus_percentage() override {
        return bonus_percentage;
    }
};

class SalaryDepartment {
private:
    static SalaryDepartment* instance;

    SalaryDepartment() {
        std::cout << "Отдел расчета зарплаты работает!\n";
    }

    std::vector<SalaryStrategy*> jobs;

public:
    static SalaryDepartment* getInstance() {
        if (instance == nullptr) {
            instance = new SalaryDepartment();
        }
        return instance;
    }

    void add_job(std::string job_type, float rate, int work_type, float bonus_percentage = 0) {
        job_type = trimString(job_type);

        if (job_type.empty()) {
            std::cout << "Ошибка! Название вида работы не может быть пустым!" << std::endl;
            return;
        }

        if (job_type.length() > MAX_JOB_NAME_LENGTH) {
            std::cout << "Ошибка! Название вида работы слишком длинное (максимум "
                << MAX_JOB_NAME_LENGTH << " символов)!" << std::endl;
            return;
        }

        if (rate <= 0) {
            std::cout << "Ошибка! Базовая ставка должна быть положительной!" << std::endl;
            return;
        }

        if (rate > MAX_BASE_RATE) {
            std::cout << "Ошибка! Базовая ставка слишком велика (максимум "
                << MAX_BASE_RATE << "руб)!" << std::endl;
            return;
        }

        switch (work_type) {
        case 0:
            jobs.push_back(new RegularWork(job_type, rate));
            std::cout << "Обычная работа '" << job_type << "' добавлена!\n";
            break;
        case 1:
            if (bonus_percentage < 0) {
                std::cout << "Ошибка! Процент надбавки не может быть отрицательным!\n";
                return;
            }
            if (bonus_percentage > MAX_BONUS_PERCENTAGE) {
                std::cout << "Ошибка! Процент надбавки слишком велик (максимум "
                    << MAX_BONUS_PERCENTAGE << "%)!\n";
                return;
            }
            if (bonus_percentage > 100) {
                std::cout << "Предупреждение: Процент надбавки превышает 100% ("
                    << bonus_percentage << "%)\n";
            }
            jobs.push_back(new BonusWork(job_type, rate, bonus_percentage));
            std::cout << "Работа с надбавкой '" << job_type << "' ("
                << bonus_percentage << "%) добавлена!\n";
            break;
        default:
            std::cout << "Ошибка! Неверный тип работы (допустимо: 0 или 1)!\n";
            break;
        }
    }

    void print_jobs() {
        if (jobs.empty()) {
            std::cout << "\nВидов работ пока нет.\n";
            return;
        }

        std::cout << "\n=== Список видов работ ===\n";
        std::cout << "Всего видов работ: " << jobs.size() << "\n";

        for (size_t i = 0; i < jobs.size(); i++) {
            std::cout << "\n" << (i + 1) << ". Вид работы: \"" << jobs[i]->get_job_type() << "\"" << std::endl;
            std::cout << "   Базовая ставка: " << jobs[i]->get_base_rate() << " руб" << std::endl;
            float bonus = jobs[i]->get_bonus_percentage();
            if (bonus > 0) {
                std::cout << "   Процент надбавки: " << bonus << "%" << std::endl;
                std::cout << "   Итоговая зарплата: " << jobs[i]->get_salary() << "руб" << std::endl;
            }
            else {
                std::cout << "   Надбавка: нет" << std::endl;
                std::cout << "   Итоговая зарплата: " << jobs[i]->get_salary() << "руб" << std::endl;
            }
        }
    }

    void average_salary() {
        if (jobs.empty()) {
            std::cout << "\nНет данных для расчета средней зарплаты.\n";
            return;
        }

        float total_salary = 0;
        float max_salary = -1;
        float min_salary = std::numeric_limits<float>::max();
        std::string max_job, min_job;

        for (size_t i = 0; i < jobs.size(); i++) {
            float salary = jobs[i]->get_salary();
            total_salary += salary;

            if (salary > max_salary) {
                max_salary = salary;
                max_job = jobs[i]->get_job_type();
            }

            if (salary < min_salary) {
                min_salary = salary;
                min_job = jobs[i]->get_job_type();
            }
        }

        float average = total_salary / jobs.size();
        std::cout << "\n=== Статистика зарплат ===\n";
        std::cout << "Средняя величина оплаты (с учетом надбавок): " << average << "руб.\n";
        std::cout << "Максимальная зарплата: " << max_salary << " руб. (" << max_job << ")\n";
        std::cout << "Минимальная зарплата: " << min_salary << " руб (" << min_job << ")\n";
        std::cout << "Общая сумма выплат: " << total_salary << " руб\n";
    }


    std::string trimString(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");

        if (start == std::string::npos) {
            return "";
        }

        return str.substr(start, end - start + 1);
    }

    ~SalaryDepartment() {
        std::cout << "\n    Деструктор SalaryDepartment    " << std::endl;
        for (auto job : jobs) {
            delete job;
        }
        std::cout << "Удалено видов работ: " << jobs.size() << std::endl;
    }
};

SalaryDepartment* SalaryDepartment::instance = nullptr;


bool safe_getline(std::string& value, const std::string& prompt, size_t max_length = MAX_JOB_NAME_LENGTH) {
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);


        if (value.empty()) {
            std::cout << "Ошибка! Ввод не может быть пустым!" << std::endl;
            continue;
        }


        if (value.length() > max_length) {
            std::cout << "Ошибка! Слишком длинный ввод (максимум "
                << max_length << " символов)!" << std::endl;
            continue;
        }

        return true;
    }
}


template<typename T>
bool safe_input(T& value, const std::string& prompt, T min_val, T max_val) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);


        if (input.empty()) {
            std::cout << "Ошибка! Ввод не может быть пустым!" << std::endl;
            continue;
        }

        try {

            if constexpr (std::is_same<T, int>::value) {
                value = std::stoi(input);
            }
            else if constexpr (std::is_same<T, float>::value) {
                value = std::stof(input);
            }
            else if constexpr (std::is_same<T, double>::value) {
                value = std::stod(input);
            }
            else {

                std::stringstream ss(input);
                ss >> value;
                if (ss.fail() || !ss.eof()) {
                    throw std::invalid_argument("Некорректный ввод");
                }
            }

            if (value < min_val) {
                std::cout << "Ошибка! Значение не может быть меньше " << min_val << "." << std::endl;
                return false;
            }


            if (value > max_val) {
                std::cout << "Ошибка! Значение не может быть больше " << max_val << "." << std::endl;
                return false;
            }

            return true;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Ошибка! Неверный формат числа. Введите числовое значение." << std::endl;
        }
        catch (const std::out_of_range&) {
            std::cout << "Ошибка! Введенное число слишком большое." << std::endl;
        }
        catch (...) {
            std::cout << "Ошибка! Некорректный ввод." << std::endl;
        }
    }
}


bool safe_input_int_with_options(int& value, const std::string& prompt,
    const std::vector<int>& valid_values) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Ошибка! Ввод не может быть пустым!" << std::endl;
            continue;
        }

        try {
            value = std::stoi(input);


            if (std::find(valid_values.begin(), valid_values.end(), value) == valid_values.end()) {
                std::cout << "Ошибка! Допустимые значения: ";
                for (size_t i = 0; i < valid_values.size(); i++) {
                    std::cout << valid_values[i];
                    if (i < valid_values.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "." << std::endl;
                return false;
            }

            return true;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Ошибка! Неверный формат числа. Введите целое число." << std::endl;
        }
        catch (const std::out_of_range&) {
            std::cout << "Ошибка! Введенное число слишком большое." << std::endl;
        }
    }
}


bool get_confirmation(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);


        input = SalaryDepartment::getInstance()->trimString(input);
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "0" || input == "да" || input == "y" || input == "yes" || input == "д") {
            return true;
        }
        else if (input == "нет" || input == "n" || input == "no" || input == "н") {
            return false;
        }
        else if (!input.empty()) {

            return false;
        }
        else {
            std::cout << "Ошибка! Ввод не может быть пустым!" << std::endl;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::locale::global(std::locale("Russian_Russia.1251"));

    SalaryDepartment* department = SalaryDepartment::getInstance();

    std::cout << "\n=== Система расчета зарплаты предприятия ===\n";
    std::cout << "Добавьте виды работ в отдел расчета зарплаты\n" << std::endl;

    bool continue_input = true;

    while (continue_input) {
        std::string job_type;
        float rate;
        int work_type;
        float bonus_percentage = 0;


        safe_getline(job_type, "\nНазвание вида работы: ");


        if (!safe_input(rate, "Базовая ставка оплаты: ", 0.01f, MAX_BASE_RATE)) {
            continue;
        }


        vector<int> valid_work_types = { 0, 1 };
        if (!safe_input_int_with_options(work_type,
            "Введите тип работы (Обычная - 0, С надбавкой - 1): ",
            valid_work_types)) {
            continue;
        }


        if (work_type == 1) {

            if (!safe_input(bonus_percentage, "Процент надбавки: ", 0.0f, MAX_BONUS_PERCENTAGE)) {
                continue;
            }
        }

        department->add_job(job_type, rate, work_type, bonus_percentage);


        continue_input = !get_confirmation("\nЭто был последний вид работы? (да - 0, нет - любой другой символ): ");
    }

    std::cout << "\n=== Итоговая информация ===\n";
    department->print_jobs();
    department->average_salary();

    delete department;

    std::cout << "\nПрограмма завершена. Нажмите Enter для выхода...";
    std::cin.get();
    return 0;
}