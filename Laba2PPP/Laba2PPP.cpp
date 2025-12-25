#include <iostream>
#include <windows.h>
#include <clocale>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#undef max
#undef min

using namespace std;

enum class MenuOption {
    EXIT = 0,
    ADD_WORK_TYPE = 1,
    ADD_EMPLOYEE = 2,
    ADD_WORK_TO_EMPLOYEE = 3,
    CALCULATE_EMPLOYEE_SALARY = 4,
    CALCULATE_TOTAL_PAYOUT = 5,
    SHOW_EMPLOYEES = 6,
    SHOW_WORK_TYPES = 7,
    MAX_MENU_OPTION = 7
};

class WorkType {
private:
    string name;
    double paymentRate;

public:
    WorkType(const string& name, double paymentRate)
        : name(name), paymentRate(paymentRate) {}

    string getName() const { return name; }
    double getPaymentRate() const { return paymentRate; }
};

class CompletedWork {
private:
    WorkType* workType;
    double hours;

public:
    CompletedWork(WorkType* workType, double hours)
        : workType(workType), hours(hours) {}

    WorkType* getWorkType() const { return workType; }
    double getHours() const { return hours; }
    double getCost() const { return hours * workType->getPaymentRate(); }
};

class Employee {
private:
    int id;
    string firstName;
    string lastName;
    vector<CompletedWork*> completedWorks;

public:
    Employee(int id, const string& firstName, const string& lastName)
        : id(id), firstName(firstName), lastName(lastName) {}

    void addCompletedWork(CompletedWork* work) {
        completedWorks.push_back(work);
    }

    double calculateSalary() const {
        double total = 0.0;
        for (const auto& work : completedWorks) {
            total += work->getCost();
        }
        return total;
    }

    int getId() const { return id; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    vector<CompletedWork*> getCompletedWorks() const { return completedWorks; }

    ~Employee() {
        for (auto& work : completedWorks) {
            delete work;
        }
    }
};

class PayrollSystem {
private:
    static PayrollSystem* instance;
    vector<Employee*> employees;
    vector<WorkType*> workTypes;
    int nextEmployeeId;

    PayrollSystem() : nextEmployeeId(1) {
        cout << "Система расчета зарплаты создана!\n";
    }

public:
    PayrollSystem(const PayrollSystem&) = delete;
    PayrollSystem& operator=(const PayrollSystem&) = delete;

    static PayrollSystem* getInstance() {
        if (instance == nullptr) {
            instance = new PayrollSystem();
        }
        return instance;
    }

    void addWorkType(WorkType* workType) {
        workTypes.push_back(workType);
    }

    WorkType* findWorkTypeByName(const string& name) {
        for (auto& wt : workTypes) {
            if (wt->getName() == name) {
                return wt;
            }
        }
        return nullptr;
    }

    void addEmployee(Employee* employee) {
        employees.push_back(employee);
    }

    Employee* findEmployeeByLastName(const string& lastName) {
        for (auto& employee : employees) {
            if (employee->getLastName() == lastName) {
                return employee;
            }
        }
        return nullptr;
    }

    Employee* findEmployeeById(int id) {
        for (auto& employee : employees) {
            if (employee->getId() == id) {
                return employee;
            }
        }
        return nullptr;
    }

    double calculateTotalPayout() const {
        double total = 0.0;
        for (const auto& employee : employees) {
            total += employee->calculateSalary();
        }
        return total;
    }

    vector<Employee*> getEmployees() const { return employees; }
    vector<WorkType*> getWorkTypes() const { return workTypes; }
    int getNextEmployeeId() { return nextEmployeeId++; }

    ~PayrollSystem() {
        cout << "\nДеструктор PayrollSystem\n";
        for (auto& employee : employees) {
            delete employee;
        }
        cout << "Удалено сотрудников: " << employees.size() << endl;
        for (auto& workType : workTypes) {
            delete workType;
        }
        cout << "Удалено видов работ: " << workTypes.size() << endl;
    }
};

PayrollSystem* PayrollSystem::instance = nullptr;

string trimString(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

template<typename T>
bool safeInput(T& value, const string& prompt, T minVal = (numeric_limits<T>::min)(),
    T maxVal = (numeric_limits<T>::max)()) {
    while (true) {
        cout << prompt;

        string input;
        getline(cin, input);

        if (cin.eof()) {
            cout << "Обнаружен EOF. Выход.\n";
            return false;
        }

        if (input.empty()) {
            cout << "Ошибка: Ввод не может быть пустым. Попробуйте снова.\n";
            continue;
        }

        bool validInput = true;
        bool hasDecimalPoint = false;
        bool hasSign = false;

        for (size_t i = 0; i < input.size(); ++i) {
            char c = input[i];

            if (i == 0 && (c == '-' || c == '+')) {
                hasSign = true;
                continue;
            }

            if (!isdigit(c)) {
                if (typeid(T) == typeid(double) && c == '.' && !hasDecimalPoint) {
                    hasDecimalPoint = true;
                }
                else {
                    validInput = false;
                    break;
                }
            }
        }

        if (!validInput) {
            cout << "Ошибка: Неверный формат числа. Используйте только цифры"
                << (typeid(T) == typeid(double) ? " и точку для дробной части" : "")
                << ". Попробуйте снова.\n";
            continue;
        }

        stringstream ss(input);
        T temp;

        if (!(ss >> temp)) {
            cout << "Ошибка: Не удалось преобразовать в число. Попробуйте снова.\n";
            continue;
        }

        string remaining;
        if (ss >> remaining) {
            cout << "Ошибка: В строке обнаружены лишние символы. Попробуйте снова.\n";
            continue;
        }

        if (temp < minVal || temp > maxVal) {
            cout << "Ошибка: Число должно быть в диапазоне от " << minVal
                << " до " << maxVal << ". Попробуйте снова.\n";
            continue;
        }

        value = temp;
        return true;
    }
}

void addWorkType(PayrollSystem* system) {
    string name;
    double rate;

    try {
        cout << "Введите название вида работ: ";
        getline(cin, name);

        name = trimString(name);
        if (name.empty()) {
            cout << "Ошибка! Название не может быть пустым!" << endl;
            return;
        }

        if (!safeInput(rate, "Введите ставку оплаты за час (руб.): ", 0.01, 1000000.0)) {
            return;
        }

    }
    catch (...) {
        cout << "Что-то не так, попробуйте заново" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    system->addWorkType(new WorkType(name, rate));
    cout << "Вид работ '" << name << "' добавлен!\n";
}

void addEmployee(PayrollSystem* system) {
    string firstName;
    string lastName;

    try {
        cout << "Введите имя сотрудника: ";
        getline(cin, firstName);

        firstName = trimString(firstName);
        if (firstName.empty()) {
            cout << "Ошибка! Имя не может быть пустым!" << endl;
            return;
        }

        cout << "Введите фамилию сотрудника: ";
        getline(cin, lastName);

        lastName = trimString(lastName);
        if (lastName.empty()) {
            cout << "Ошибка! Фамилия не может быть пустой!" << endl;
            return;
        }

    }
    catch (...) {
        cout << "Что-то не так, попробуйте заново" << endl;
        return;
    }

    int id = system->getNextEmployeeId();
    system->addEmployee(new Employee(id, firstName, lastName));
    cout << "Сотрудник " << lastName << " " << firstName << " добавлен с ID: " << id << "\n";
}

void addWorkToEmployee(PayrollSystem* system) {
    if (system->getWorkTypes().empty()) {
        cout << "Сначала добавьте виды работ!\n";
        return;
    }

    string lastName;
    try {
        cout << "Введите фамилию сотрудника: ";
        getline(cin, lastName);

        lastName = trimString(lastName);
        if (lastName.empty()) {
            cout << "Ошибка! Фамилия не может быть пустой!" << endl;
            return;
        }

    }
    catch (...) {
        cout << "Что-то не так, попробуйте заново" << endl;
        return;
    }

    Employee* employee = system->findEmployeeByLastName(lastName);
    if (!employee) {
        cout << "Сотрудник с фамилией '" << lastName << "' не найден!\n";
        return;
    }

    cout << "Доступные виды работ:\n";
    int index = 1;
    for (auto& workType : system->getWorkTypes()) {
        cout << index++ << ". " << workType->getName()
            << " (" << workType->getPaymentRate() << " руб./час)\n";
    }

    int choice;
    if (!safeInput(choice, "Выберите вид работ: ", 1, static_cast<int>(system->getWorkTypes().size()))) {
        return;
    }

    choice = choice - 1;

    double hours;
    if (!safeInput(hours, "Введите количество отработанных часов: ", 0.01, 10000.0)) {
        return;
    }

    WorkType* selectedWorkType = system->getWorkTypes()[choice];
    employee->addCompletedWork(new CompletedWork(selectedWorkType, hours));

    cout << "Работа '" << selectedWorkType->getName()
        << "' (" << hours << " часов) успешно добавлена сотруднику "
        << employee->getLastName() << " " << employee->getFirstName() << "!\n";
}

void calculateEmployeeSalary(PayrollSystem* system) {
    string lastName;
    try {
        cout << "Введите фамилию сотрудника: ";
        getline(cin, lastName);

        lastName = trimString(lastName);
        if (lastName.empty()) {
            cout << "Ошибка! Фамилия не может быть пустой!" << endl;
            return;
        }

    }
    catch (...) {
        cout << "Что-то не так, попробуйте заново" << endl;
        return;
    }

    Employee* employee = system->findEmployeeByLastName(lastName);

    if (employee) {
        double salary = employee->calculateSalary();
        cout << "Зарплата сотрудника " << employee->getLastName()
            << " " << employee->getFirstName() << ": " << salary << " руб.\n";

        auto works = employee->getCompletedWorks();
        if (!works.empty()) {
            cout << "Детализация:\n";
            for (const auto& work : works) {
                cout << "  - " << work->getWorkType()->getName()
                    << ": " << work->getHours() << " часов = "
                    << work->getCost() << " руб.\n";
            }
        }
        else {
            cout << "У сотрудника пока нет выполненных работ.\n";
        }
    }
    else {
        cout << "Сотрудник с фамилией '" << lastName << "' не найден!\n";
    }
}

void showEmployees(PayrollSystem* system) {
    auto employees = system->getEmployees();
    if (employees.empty()) {
        cout << "Сотрудников нет.\n";
        return;
    }

    cout << "Список сотрудников:\n";
    for (const auto& employee : employees) {
        cout << "ID: " << employee->getId() << ", "
            << employee->getLastName() << " " << employee->getFirstName()
            << ", Зарплата: " << employee->calculateSalary() << " руб.\n";
    }
    cout << "Всего сотрудников: " << employees.size() << "\n";
}

void showWorkTypes(PayrollSystem* system) {
    auto workTypes = system->getWorkTypes();
    if (workTypes.empty()) {
        cout << "Видов работ нет.\n";
        return;
    }

    cout << "Список видов работ:\n";
    for (const auto& workType : workTypes) {
        cout << "- " << workType->getName()
            << " (" << workType->getPaymentRate() << " руб./час)\n";
    }
    cout << "Всего видов работ: " << workTypes.size() << "\n";
}

int getMenuChoice() {
    int choice;
    while (true) {
        cout << "Выберите пункт меню: ";

        string input;
        getline(cin, input);

        if (cin.eof()) {
            return static_cast<int>(MenuOption::EXIT);
        }

        if (input.empty()) {
            cout << "Ошибка: Ввод не может быть пустым. Попробуйте снова.\n";
            continue;
        }

        bool validInput = true;
        for (char c : input) {
            if (!isdigit(c)) {
                validInput = false;
                break;
            }
        }

        if (!validInput) {
            cout << "Ошибка: Введите число от 0 до 7. Попробуйте снова.\n";
            continue;
        }

        stringstream ss(input);
        if (!(ss >> choice)) {
            cout << "Ошибка: Не удалось преобразовать в число. Попробуйте снова.\n";
            continue;
        }

        string remaining;
        if (ss >> remaining) {
            cout << "Ошибка: В строке обнаружены лишние символы. Попробуйте снова.\n";
            continue;
        }

        if (choice < static_cast<int>(MenuOption::EXIT) ||
            choice > static_cast<int>(MenuOption::MAX_MENU_OPTION)) {
            cout << "Ошибка: Пункт меню должен быть от 0 до "
                << static_cast<int>(MenuOption::MAX_MENU_OPTION)
                << ". Попробуйте снова.\n";
            continue;
        }

        return choice;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    locale::global(locale("Russian_Russia.1251"));

    PayrollSystem* system = PayrollSystem::getInstance();

    cout << "=== Система расчета зарплаты предприятия ===\n";

    while (true) {
        cout << "\n=== Меню ===\n";
        cout << "1. Добавить вид работ\n";
        cout << "2. Добавить сотрудника\n";
        cout << "3. Добавить работу сотруднику\n";
        cout << "4. Рассчитать зарплату сотрудника\n";
        cout << "5. Общая сумма выплат всем сотрудникам\n";
        cout << "6. Показать всех сотрудников\n";
        cout << "7. Показать все виды работ\n";
        cout << "0. Выход\n";

        int choice = getMenuChoice();

        if (choice == static_cast<int>(MenuOption::EXIT)) {
            break;
        }

        switch (static_cast<MenuOption>(choice)) {
        case MenuOption::ADD_WORK_TYPE:
            addWorkType(system);
            break;

        case MenuOption::ADD_EMPLOYEE:
            addEmployee(system);
            break;

        case MenuOption::ADD_WORK_TO_EMPLOYEE:
            addWorkToEmployee(system);
            break;

        case MenuOption::CALCULATE_EMPLOYEE_SALARY:
            calculateEmployeeSalary(system);
            break;

        case MenuOption::CALCULATE_TOTAL_PAYOUT: {
            double total = system->calculateTotalPayout();
            cout << "Общая сумма выплат всем сотрудникам: " << total << " руб.\n";
            break;
        }

        case MenuOption::SHOW_EMPLOYEES:
            showEmployees(system);
            break;

        case MenuOption::SHOW_WORK_TYPES:
            showWorkTypes(system);
            break;

        default:
            break;
        }
    }

    cout << "Выход из программы.\n";
    delete system;
    return 0;
}