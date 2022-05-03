#if !defined(__CODE_GRAV_SYSTEM_UTILS_DATETIME_H__)
#define __CODE_GRAV_SYSTEM_UTILS_DATETIME_H__

#include <Arduino.h>
#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

// Difference between local hour to universal hour
#define DIFF_LOCAL_UTC_HOURS 6U

const char monthNames[] PROGMEM = "Error\0January\0February\0March\0April\0May\0June\0July\0August\0September\0October\0November\0December";
const byte monthNameIndex[] = {0, 6, 14, 23, 29, 35, 39, 44, 49, 56, 66, 74, 83};
const char monthNamesShort[] PROGMEM = "Err\0Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec";
const char dayNames[] PROGMEM = "Error\0Sunday\0Monday\0Tuesday\0Wednesday\0Thursday\0Friday\0Saturday";
const byte dayNameIndex[] = {0, 6, 13, 20, 28, 38, 47, 54};
const char dayNamesShort[] PROGMEM = "Err\0Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";

struct Time_s;

class DateTime
{
  private:
    int _year, _month, _day, _hour, _minute, _second, _millisecond;
    int parse(char number);
    int parse(char* number, int characters);

    String& getProgMemString(const char *progMemString, byte index);

  public:
    enum Period { Year, Month, Day, Hour, Minute, Second, Millisecond };
    enum DayOfWeek { Error, Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
    enum TimeSource { Compiler, NMEA };
    
    DateTime();
    DateTime(char* date, char* time, DateTime::TimeSource source);
    DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
    DateTime(Time_s &time);

    Time_s get();

    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    int millisecond() const;
    
    int hourTens() const;
    int hourUnits() const;
    int minuteTens() const;
    int minuteUnits() const;

    void add(int interval, Period period);
    int daysInMonth();
    byte monthFromString(char* string);
    unsigned long totalMilliseconds() const;
    DateTime::DayOfWeek dayOfWeek() const;
    DateTime toLocal();
    boolean isLeapYear() const;
    boolean isApproximatelyEqualTo(const DateTime &other) const;
    
    boolean isEarlierThan(const DateTime &other) const;
    boolean isEqualTo(const DateTime &other) const;
    
    static void getDaylightSavingsDates(DateTime forDate, DateTime &utcStartDate, DateTime &utcEndDate);

    boolean operator == (const DateTime &other) const;
    boolean operator < (const DateTime &other) const;
    boolean operator <= (const DateTime &other) const;
    boolean operator > (const DateTime &other) const;
    boolean operator >= (const DateTime &other) const;
    
    String& toString();
    String& monthToString();
    String& monthToString(int month);
    String& monthToShortString();
    String& monthToShortString(int month);
    String& dayOfWeekToString();
    String& dayOfWeekToString(DayOfWeek day);
    String& dayOfWeekToShortString();
    String& dayOfWeekToShortString(DayOfWeek day);
};
typedef class DateTime DateTime;


struct Time_s {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  
  Time_s() { }
  Time_s(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
  Time_s(DateTime time);

  String toString() const;
  void toCharArray(char *buff) const;
  DateTime toDateTime() const;
  Time_s toUtcTime() const;
  bool operator==(Time_s&);
};
typedef struct Time_s Time_s;

#endif // __CODE_GRAV_SYSTEM_UTILS_DATETIME_H__
