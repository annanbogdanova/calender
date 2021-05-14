#include <ctime>
#include <iostream>
#include <locale>
#include <string>
const int DAYS_IN_A_WEEK = 7;
const int LEFT_BLOCK_SIZE = 15;         // можно регулировать размер блока слева, где названия месяцев и номер года

using namespace std;

int increment_date(tm * t) {    // увеличивает дату на один день так, чтобы она осталась корректной
    t->tm_mday++;
    int current_month = t->tm_mon;
    mktime(t);
    return current_month != t->tm_mon;  // возвращает 0, если месяц сменился
}

string format_date(const char * format, tm * t) {   // моя обертка вокруг C-ной функции strftime, которая возвращает строку из STL
    char mbstr[100];
    strftime(mbstr, sizeof(mbstr), format, t);
    string result(mbstr);
    return result;
}

string calendar(int year) {     // принимает номер года распечатывает календарь
    string str_year = to_string(year);
    string title_string = str_year;
    for (int i = 0; i < LEFT_BLOCK_SIZE + 1 - str_year.size(); ++i) {
        title_string += " ";
    }
    tm t{};
    for (int i = 0; i < 6 * DAYS_IN_A_WEEK; ++i)  {      // создается строчка со списком дней недели (на 6 недель)
        increment_date(&t);
        title_string += format_date("%a", &t) + " ";
    }
    title_string += "\n";
    tm d{};
    d.tm_year = year - 1900;
    mktime(&d);     // в этот момент d хранит дату 31 декабря года year - 1900 - 1, поэтому цикл начинается с инкремента
    string result = title_string;
    do {
        if (increment_date(&d)) {       // инкремент и проверка на смену месяца
            string new_month = format_date("\n%B", &d);
            result += new_month;
            for (int i = 0; i < (LEFT_BLOCK_SIZE + 2 - new_month.size()); ++i) {
                result += " ";
            }
            result += '|';
            for (int j = 0; j < (d.tm_wday == 0 ? 6 : d.tm_wday - 1); ++j) {
                result += "    ";
            }
            result += format_date("%e", &d) + "  ";
            increment_date(&d);
            }
        result += format_date("%e", &d) + "  ";
    } while (!(d.tm_mday == 31 && d.tm_mon == 11));     // проверка на 31 декабря в конце, чтобы все не заглохло на первой итерации
    result += "\n";
    return result;
}

 
int main() {
    cout << calendar(2021);
}
