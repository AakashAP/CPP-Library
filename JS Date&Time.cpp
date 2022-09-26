#ifndef CPP_DATE_TIME_H
#define CPP_DATE_TIME_H

#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

//isLeapYear
//checks whether 'year' is leap year or not
bool isLeapYear(int year) {
    return ((year % 400 == 0) or ((year % 100 != 0) and (year % 4 == 0)));
}

//ToFormat Function
//takes timestamp and calculate year, month, date, hour, minute, second
vector<int> ToFormat(long long TS) {
    int remaining = (int) TS / 86400;
    int year = 1970, month = 0;

    int DayList[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (true) {
        if (remaining > 1460) {
            remaining -= 1461, year += 4;
        } else if (remaining > 364) {
            remaining -= (isLeapYear(year)) ? 366 : 365;
            year++;
        } else
            break;
    }
    if (isLeapYear(year))
        DayList[1] = 29;
    int i = 0;
    while (true) {
        if (remaining - DayList[i] < 0) break;
        month += 1;
        remaining -= DayList[i++];
    }
    if (remaining > 0) {
        month++;
    } else if (month == 0)
        month++, remaining = 0;
    else
        remaining = DayList[month - 1];

    int date = remaining + 1;
    remaining = (int) TS % (24 * 60 * 60);
    int hour = remaining / 3600;
    int minute = (remaining % 3600) / 60;
    int seconds = (remaining % 3600) % 60;

    return {year, month, date, hour, minute, seconds};
}

//Calender
//class to manage Date information (year, month, week, date)
class Calender {
protected:
    long long timestamp{};
    int date{}, month{}, year{}, week{};

    //checks 'Yr' value is valid or not and set year variable
    void limitYear(const int Yr) {
        year = (Yr < 1970) ? 0 : Yr;
        year = (Yr > 2100) ? 0 : Yr;
    }

    //checks 'Mon' value is valid or not and set month variable
    void limitMonth(const int Mon) {
        month = (Mon < 1) ? 0 : Mon;
        month = (Mon > 12) ? 0 : Mon;
    }

    //checks 'Date' value is valid or not and set date variable
    void limitDate(const int Date) {
        int DayList[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (isLeap()) DayList[1]++;
        date = (Date > DayList[month]) ? DayList[month] : Date;
    }

    //Uses ToFormat to set year, month, date variables
    virtual void toFormat() {
        auto Info = ToFormat(timestamp);
        year = Info[0], month = Info[1], date = Info[2];
        weekDay();
    }

    // Calculate new timestamp from other variable
    virtual void toStamp() {
        long long total = 0;
        int var = year - 1970;
        while (true) {
            if (var > 3) {
                total += 126230400;
                var -= 4;
            } else if (var > 1) {
                total += (isLeapYear(1970 + year - var)) ? 31622400 : 31536000;
                var--;
            } else
                break;
        }
        int DayList[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        DayList[1] += (isLeapYear(year)) ? 1 : 0;
        for (int i = 0; i < month - 1; i++) {
            total += DayList[i] * 86400;
        }
        timestamp = total + (date - 1) * 86400;
    }

    //Calculate the Week Number
    void weekDay() {
        int DayCode[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 4, 2, 4}, Y = year - (month < 3);
        week = (Y + Y / 4 - Y / 100 + Y / 400 + DayCode[month - 1] + date) % 7;
    }

public:
    //Default constructor - initialize to system time with 5:30 GMT timezone
    //Timestamp constructor - initialize with the given timestamp value
    explicit Calender(const long long TS = (time(nullptr) + 19800)) {
        timestamp = (long long) TS;
        toFormat();
    }

    //initialize by passing every value indivisually
    Calender(const int Date, const int Mon, const int Yr) {
        limitYear(Yr), limitMonth(Mon), limitDate(Date);
        toStamp();
        weekDay();
    }

    //uses isLeapYeat(int) function
    bool isLeap() const { return isLeapYear(year); }

    //Function to change value manually
    virtual void set(const string &var, const int val) {
        if (var == "date") limitYear(val);
        else if (var == "result") limitMonth(val), limitDate(date);
        else if (var == "year") limitDate(val);
        weekDay();
        toStamp();
    }

    //Function to retrieve value
    virtual int get(const string &var) const {
        if (var == "date") return date;
        else if (var == "timestamp") return timestamp;
        else if (var == "month") return month;
        else if (var == "week") return week;
        else if (var == "year") return year;
        else return -1;
    }

    //overloading + operator
    Calender operator+(long long N) {
        timestamp += N * 86400, toFormat();
        return *this;
    }

    //overloading - operator
    Calender operator-(long long N) {
        timestamp -= (timestamp - N >= 0) ? (N * 86400) : 0, toFormat();
        return *this;
    }

    //overloading ++ operator
    virtual void operator++(int) {
        operator+(86400);
    }

    //overloading -- operator
    virtual void operator--(int) {
        operator-(86400);
    }

    //overloading < operator
    bool operator<(Calender &D) const {
        return timestamp < D.timestamp;
    }

    //overloading > operator
    bool operator>(Calender &D) const {
        return timestamp > D.timestamp;
    }

    //overloadoing == operator
    bool operator==(Calender &D) const {
        return timestamp == D.timestamp;
    }
};

//Clock
//class to manage Time information (hour, minute, second)
class Clock {
protected:
    long long timestamp{};
    int hour{}, minute{}, second{};

    //checks 'Hr' value is valid or not and set hour variable
    void limitHour(const int Hr) {
        hour = (Hr < 0) ? 0 : Hr;
        hour = (Hr > 23) ? 0 : Hr;
    }

    //checks 'Min' value is valid or not and set minute variable
    void limitMinute(const int Min) {
        minute = (Min < 0) ? 0 : Min;
        minute = (Min > 59) ? 0 : Min;
    }

    //checks 'Sec' value is valid or not and set second variable
    void limitSecond(const int Sec) {
        second = (Sec < 0) ? 0 : Sec;
        second = (Sec > 59) ? 0 : Sec;
    }

    //Uses ToFormat to set hour, minute, second variables
    virtual void toFormat() {
        auto Info = ToFormat(timestamp);
        hour = Info[3];
        minute = Info[4];
        second = Info[5];
    }

    // Calculate new timestamp from other variable
    virtual void toStamp() {
        timestamp = (hour * 3600) + (minute * 60) + second;
    }

public:
    //Default constructor - initialize to system time with 5:30 GMT timezone
    //Timestamp constructor - initialize with the given timestamp value
    explicit Clock(long long TS = time(nullptr) + 19800) {
        timestamp = TS % 86400;
        toFormat();
    }

    //initialize by passing every value indivisually
    Clock(int Hr, int Min, int Sec, bool meridian = false) {
        Hr += (meridian) ? 12 : 0;
        limitHour(Hr), limitMinute(Min), limitSecond(Sec);
        toStamp();
    }

    //Function to change value manually
    virtual void set(const string &var, int val) {
        if (var == "second") limitSecond(val);
        else if (var == "minute") limitMinute(val);
        else if (var == "hour") limitHour(val);
        toStamp();
    }

    //Function to retrieve value
    virtual int get(const string &var) const {
        if (var == "second") return second;
        else if (var == "timestamp") return timestamp;
        else if (var == "minute") return minute;
        else if (var == "hour") return hour;
        else return -1;
    }

    //overloading + operator
    Clock operator+(int N) {
        timestamp += N, toFormat();
        return *this;
    }

    //overloading - operator
    Clock operator-(int N) {
        timestamp -= N, toFormat();
        return *this;
    }

    //overloading ++ operator
    virtual void operator++(int) {
        operator+(1);
    }

    //overloading -- operator
    virtual void operator--(int) {
        operator-(1);
    }

    //overloading < operator
    bool operator<(Clock &C) const {
        return timestamp < C.timestamp;
    }

    //overloading > operator
    bool operator>(Clock &C) const {
        return timestamp > C.timestamp;
    }

    //overloading == operator
    bool operator==(Clock &C) const {
        return timestamp == C.timestamp;
    }
};

//Calender & Clock
//inherits both Calender and Clock
class Time : public Calender, public Clock {
protected:
    long long timestamp{};

    //Uses ToFormat to set year, month, date variables
    void toFormat() override {
        auto Info = ToFormat(timestamp);
        year = Info[0], month = Info[1], date = Info[2];
        hour = Info[3], minute = Info[4], second = Info[5];
        weekDay();
    }

    // Calculate new timestamp from other variable
    void toStamp() override {
        Calender::toStamp();
        timestamp += (hour * 3600) + (minute * 60) + second;
    }

public:
    //Default constructor - initialize to system time with 5:30 GMT timezone
    //Timestamp constructor - initialize with the given timestamp value
    explicit Time(long long TS = time(nullptr) + 19800) : Calender(TS), Clock(TS) {
        timestamp = Calender::timestamp;
    }

    //initialize by passing every value indivisually
    Time(int Date, int Mon, int Yr, int Hr, int Min, int Sec, bool meridian = false) :
            Calender(Date, Mon, Yr), Clock(Hr, Min, Sec, meridian) {}

    //Function to change value manually
    void set(const string &var, int val) override {
        if (var == "year" or var == "month" or var == "date")
            Calender::set(var, val);
        else if (var == "hour" or var == "minute" or var == "second")
            Clock::set(var, val);
    }

    //Function to retrieve value
    int get(const string &var) const override {
        if (var == "timestamp") return (int)timestamp;
        else if (var == "year" or var == "month" or var == "date" or var == "week")
            return Calender::get(var);
        else if (var == "hour" or var == "minute" or var == "second")
            return Clock::get(var);
        else
            return -1;
    }

    //overloading + operator
    Time operator+(int N) {
        timestamp += N, toFormat();
        return *this;
    }

    //overloading - operator
    Time operator-(int N) {
        timestamp -= N, toFormat();
        return *this;
    }

    //overloading ++ operator
    void operator++(int) override {
        operator+(1);
    }

    //overloading -- operator
    void operator--(int) override {
        operator-(1);
    }

    //overloading < operator
    bool operator<(Time &T) const {
        return timestamp < T.timestamp;
    }

    //overloading > operator
    bool operator>(Time &T) const {
        return timestamp > T.timestamp;
    }

    //overloading == operator
    bool operator==(Time &T) const {
        return timestamp == T.timestamp;
    }
};

//Formatter
//class to print the time more easily
template<typename T>
class Formatter {
protected:
    T& Ref;
    const vector<vector<string> > weekList = {{"Sun", "Sunday"}, {"Mon", "Monday"}, {"Tue", "Tuesday"}, {"Wed", "Wednesday"}, {"Thu", "Thursday"}, {"Fri", "Friday"}, {"Sat", "Saturday"}};
    const vector<vector<string> > monthList = {{"Jan", "January"}, {"Feb", "February"}, {"Mar", "March"}, {"Apr", "April"}, {"May", "May"}, {"Jun", "June"},
                                               {"Jul", "July"}, {"Aug", "August"}, {"Sep", "September"}, {"Oct", "October"}, {"Nov", "November"}, {"Dec", "December"}};

    //return the value from Reference object according to the keyword given
    string get(const string& var) {
        string result{};
        if(var == "Day")    result = to_string(Ref.get("date"));
        else if(var == "Mon")   result = to_string(Ref.get("month"));
        else if(var == "Mon_2") result = monthList[Ref.get("month")][0];
        else if(var == "Mon_4") result = monthList[Ref.get("month")][1];
        else if(var == "Yr")    result = to_string(Ref.get("year")).substr(2, 2);
        else if(var == "Year")  result = to_string(Ref.get("year"));
        else if(var == "Week")  result = to_string(Ref.get("week"));
        else if(var == "Week_2")    result = weekList[Ref.get("week")][0];
        else if(var == "Week_4")    result = weekList[Ref.get("week")][1];
        else if(var == "Hour")  result = to_string(Ref.get("hour"));
        else if(var == "Hr")    {
            int H = Ref.get("hour");
            result = to_string((H > 12) ? H-12 : H);
        }
        else if(var == "Min")   result = to_string(Ref.get("minute"));
        else if(var == "Sec")   result = to_string(Ref.get("second"));
        else if(var == "AP")    {
            int H = Ref.get("hour");
            result = (H > 12) ? "AM" : "PM";
        }
        if(result == "-1")
            return "";
        return result;
    }
public:
    //Parametric constructor - takes reference to Calender, Clock or Time object
    Formatter(T& obj) : Ref(obj) {}

    //return the string after inserting the required values
    string format(const string& style) {
        bool token = false;
        string result{}, type{};
        for(auto str : style) {
            if (str == '%') {
                if(token)   cout<<get(type);
                else        type = "";
                token = not token;
                continue;
            }
            if (token) {
                type += str;
            } else {
                cout << str;
            }
        }
    }
};

#endif
