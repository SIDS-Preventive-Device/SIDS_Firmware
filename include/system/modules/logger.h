#if !defined(__CODE_GRAV_SYSTEM_LOGGER_H__)
#define __CODE_GRAV_SYSTEM_LOGGER_H__

#include <Print.h>

#include "config.h"
#include "system/utils/datetime.h"

// Local macro for prefix debug msg
#define LOG_DEBUG_PREFIX_MSG "[\033[1m\033[35mDEBUG\033[0m]: "
// Local macro for prefix info msg
#define LOG_INFO_PREFIX_MSG "[\033[1m\033[32mINFO \033[0m]: "
// Local macro for prefix warn msg
#define LOG_WARN_PREFIX_MSG "[\033[1m\033[93mWARN \033[0m]: "
// Local macro for prefix error msg
#define LOG_ERROR_PREFIX_MSG "[\033[1m\033[91mERROR\033[0m]: "

typedef enum
{
    // Log at debug level
    LOG_DEBUG,
    // Log at info level
    LOG_INFO,
    // Log at warning level
    LOG_WARN,
    // Log at error level
    LOG_ERROR,
    // Log at any level
    LOG_MASTER
} eLogLevel_t;

typedef enum
{
    EndLine
} LoggerSpecialChar_t;

typedef enum
{
    LOGGER_TEXT_BOLD,
    LOGGER_TEXT_YELLOW,
    LOGGER_TEXT_RED,
    LOGGER_TEXT_GREEN,
    LOGGER_TEXT_RESET
} LoggerFormatText_e;

typedef enum
{
    INT_BIN = 2,
    INT_OCT = 8,
    INT_DEC = 10,
    INT_HEX = 16
} LoggerIntBase_e;

const __FlashStringHelper *GetStringLogLevel(eLogLevel_t);

class KernelLogger : public Print
{
    eLogLevel_t log_lvl;

    Print *cout;

    bool canLog = false;
    bool printTime = false;
    LoggerIntBase_e intBase;

public:
    KernelLogger(Print *printOut)
        : cout(printOut), intBase(INT_DEC)
    {
    }

    void setLogOutput (Print *printOut)
    {
        this->cout = printOut;
    }

    size_t write(const uint8_t *buffer, size_t size);
    size_t write(uint8_t _byte);

    void setLogTime(Time_s);

    eLogLevel_t logLevel() const { return this->log_lvl; }
    void setLogLevel(const eLogLevel_t &_lvl) { this->log_lvl = _lvl; }

    KernelLogger &operator<<(LoggerIntBase_e);
    KernelLogger &operator<<(LoggerFormatText_e);
    KernelLogger &operator<<(LoggerSpecialChar_t);
    KernelLogger &operator<<(eLogLevel_t);
    KernelLogger &operator<<(const __FlashStringHelper *);
    KernelLogger &operator<<(const String &);
    KernelLogger &operator<<(const char[]);
    KernelLogger &operator<<(char);
    KernelLogger &operator<<(unsigned char);
    KernelLogger &operator<<(int);
    KernelLogger &operator<<(unsigned int);
    KernelLogger &operator<<(long);
    KernelLogger &operator<<(unsigned long);
    KernelLogger &operator<<(double);
    KernelLogger &operator<<(const Printable &);

    void log(const __FlashStringHelper *, eLogLevel_t);
    void log(const String &, eLogLevel_t);
    void log(const char[], eLogLevel_t);
    void log(char, eLogLevel_t);
    void log(unsigned char, eLogLevel_t, int = DEC);
    void log(int, eLogLevel_t, int = DEC);
    void log(unsigned int, eLogLevel_t, int = DEC);
    void log(long, eLogLevel_t, int = DEC);
    void log(unsigned long, eLogLevel_t, int = DEC);
    void log(double, eLogLevel_t, int = 2);
    void log(const Printable &, eLogLevel_t);
};

extern KernelLogger logger;

#endif // __CODE_GRAV_SYSTEM_LOGGER_H__
