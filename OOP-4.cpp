#include <iostream>

class Date {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    bool isOurEra;

public:
    // Конструктор по умолчанию
    Date() : year(1960), month(1), day(1), hour(0), minute(0), second(0), isOurEra(true) {}

    Date(int year, int month, int day, int hour, int minute, int second) {
        this->year = year;
        this->month = month;
        this->day = day;
        this->hour = hour;
        this->minute = minute;
        this->second = second;
        this->isOurEra = (year >= 0) ? true : false;
    }
    // Методы add и subtract
    Date add(int years, int months, int days, int hours, int minutes, int seconds) const;
    Date subtract(int years, int months, int days, int hours, int minutes, int seconds) const;

    // Перегрузка операторов
    bool operator==(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    Date& operator=(const Date& other);
    Date operator+(const Date& other) const;
    Date operator-(const Date& other) const;
    Date& operator+=(const Date& other);
    Date& operator-=(const Date& other);

    friend std::ostream& operator<<(std::ostream& os, const Date& date);

private:
    // Вспомогательные методы
    bool isLeapYear(int year) const;
    int daysInMonth(int year, int month) const;
    void normalizeDate();
};

Date Date::add(int years, int months, int days, int hours, int minutes, int seconds) const {
    Date newDate = *this;

    // Добавление лет и месяцев
    newDate.year += years;
    newDate.month += months;
    newDate.normalizeDate();  // Нормализация после добавления месяцев

    // Добавление дней
    newDate.day += days;
    while (newDate.day > newDate.daysInMonth(newDate.year, newDate.month)) {
        newDate.day -= newDate.daysInMonth(newDate.year, newDate.month);
        newDate.month++;
        newDate.normalizeDate();  // Нормализация после добавления дней
    }

    // Добавление часов, минут и секунд
    newDate.hour += hours;
    newDate.minute += minutes;
    newDate.second += seconds;
    newDate.normalizeDate();  // Нормализация после добавления времени

    return newDate;
}

Date Date::subtract(int years, int months, int days, int hours, int minutes, int seconds) const {
    Date newDate = *this;

    // Вычитание лет и месяцев
    newDate.year -= years;
    newDate.month -= months;
    newDate.normalizeDate();  // Нормализация после вычитания месяцев

    // Вычитание дней
    newDate.day -= days;
    while (newDate.day <= 0) {
        newDate.month--;
        if (newDate.month <= 0) {
            newDate.year--;
            newDate.month += 12;
        }
        newDate.day += newDate.daysInMonth(newDate.year, newDate.month);
        newDate.normalizeDate();  // Нормализация после вычитания дней
    }

    // Вычитание часов, минут и секунд
    newDate.hour -= hours;
    newDate.minute -= minutes;
    newDate.second -= seconds;
    newDate.normalizeDate();  // Нормализация после вычитания времени

    return newDate;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day &&
        hour == other.hour && minute == other.minute && second == other.second &&
        isOurEra == other.isOurEra;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    if (minute != other.minute) return minute < other.minute;
    return second < other.second;
}

bool Date::operator>(const Date& other) const {
    return !(*this < other || *this == other);
}

Date& Date::operator=(const Date& other) {
    if (this != &other) {
        year = other.year;
        month = other.month;
        day = other.day;
        hour = other.hour;
        minute = other.minute;
        second = other.second;
        isOurEra = other.isOurEra;
    }
    return *this;
}

Date Date::operator+(const Date& other) const {
    return add(other.year, other.month, other.day, other.hour, other.minute, other.second);
}

Date Date::operator-(const Date& other) const {
    return subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);
}

Date& Date::operator+=(const Date& other) {
    *this = *this + other;
    return *this;
}

Date& Date::operator-=(const Date& other) {
    *this = *this - other;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    std::string era = date.isOurEra ? "AD" : "BC";
    os << date.hour << ":" << date.minute << ":" << date.second << " - " << date.day << "." << date.month << "." << date.year << " " << era;
    return os;
}

bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int year, int month) const {
    switch (month) {
    case 2:
        return isLeapYear(year) ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        return 31;
    }
}

void Date::normalizeDate() {
    while (month <= 0) {
        year--;
        month += 12;
    }
    while (month > 12) {
        year++;
        month -= 12;
    }

    while (day <= 0) {
        month--;
        if (month <= 0) {
            year--;
            month += 12;
        }
        day += daysInMonth(year, month);
    }

    while (day > daysInMonth(year, month)) {
        day -= daysInMonth(year, month);
        month++;
        if (month > 12) {
            year++;
            month -= 12;
        }
    }

    while (hour < 0) {
        day--;
        if (day <= 0) {
            month--;
            if (month <= 0) {
                year--;
                month += 12;
            }
            day += daysInMonth(year, month);
        }
        hour += 24;
    }

    while (hour >= 24) {
        day++;
        if (day > daysInMonth(year, month)) {
            month++;
            if (month > 12) {
                year++;
                month -= 12;
            }
            day = 1;
        }
        hour -= 24;
    }

    while (minute > 60) {
        hour++;
        if (hour >= 24) {
            day++;
            if (day > daysInMonth(year, month)) {
                month++;
                if (month > 12) {
                    year++;
                    month -= 12;
                }
                day = 1;
            }
            hour -= 24;
        }
        minute -= 60;
    }

    while (minute < 0) {
        hour--;
        if (hour < 0) {
            day--;
            if (day <= 0) {
                month--;
                if (month <= 0) {
                    year--;
                    month += 12;
                }
                day += daysInMonth(year, month);
            }
            hour += 24;
        }
        minute += 60;
    }

    while (second > 60) {
        minute++;
        if (minute > 60) {
            hour++;
            if (hour >= 24) {
                day++;
                if (day > daysInMonth(year, month)) {
                    month++;
                    if (month > 12) {
                        year++;
                        month -= 12;
                    }
                    day = 1;
                }
                hour -= 24;
            }
            minute -= 60;
        }
        second -= 60;
    }

    while (second < 0) {
        minute--;
        if (minute < 0) {
            hour--;
            if (hour < 0) {
                day--;
                if (day <= 0) {
                    month--;
                    if (month <= 0) {
                        year--;
                        month += 12;
                    }
                    day += daysInMonth(year, month);
                }
                hour += 24;
            }
            minute += 60;
        }
        second += 60;
    }
}

int main() {
    // Пример использования
    Date date_default;
    Date date1(2000, 11, 30, 2, 5, 6);

    std::cout << date_default << std::endl;  // Выведет: 1960 1-1 0:0:0 AD

    Date date_new = date_default.add(1, 3, 0, 0, 0, 0);

    std::cout << (date_default - date1) << std::endl;

    std::cout << date_new << std::endl;  // Выведет: 1961 4-1 0:0:0 AD

    return 0;
}

