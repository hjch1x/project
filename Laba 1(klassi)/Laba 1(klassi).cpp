#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class otdel {
private:
    int kolvo_rab;
    string name;
    double zp_hour;
    double nalog;
    int norma;
public:
    double raschettotal() {
        if (norma <= 0 || zp_hour <= 0 || kolvo_rab <= 0 || nalog <= 0) { 
            throw runtime_error("Некорректные данные для расчета!");
        }
        double zp = norma * zp_hour;
        double nalogs1 = zp * (nalog / 100.0);
        double total = nalogs1 * kolvo_rab;
        return total;
    }

    void set_norma(int n) {
        if (n <= 0) {
            throw invalid_argument("Норма часов должна быть больше нуля!");
        }
        norma = n;
    }

    void set_data(int y, string t, double x, double c) {
        if (y <= 0) {
            throw invalid_argument("Количество работников должно быть больше нуля!");
        }
        if (t.empty()) {
            throw invalid_argument("Название предприятия не может быть пустым!");
        }
        if (x <= 0) {
            throw invalid_argument("Оплата за час должна быть больше нуля!");
        }
        if (c <= 0) { 
            throw invalid_argument("Налог должен быть больше нуля!");
        }
        if (c > 100) {
            throw invalid_argument("Налог не может превышать 100%!");
        }

        kolvo_rab = y;
        name = t;
        zp_hour = x;
        nalog = c;
    }

    void get_info() {
        cout << "Наименование предприятия: " << name << endl;
        cout << "Число работников: " << kolvo_rab << endl;
        cout << "Оплата за час: " << zp_hour << endl;
        cout << "Подоходный налог: " << nalog << "%" << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "RU");

    try {
        otdel school;
        school.set_norma(30);
        school.set_data(10, "Ромашка", 7000, 30); 
        school.get_info();

        double total = school.raschettotal();
        cout << "Общая выплата по подоходному налогу: " << total << " руб" << endl;
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}