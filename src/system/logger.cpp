#include "system/logger.h"
#include <Arduino.h>

#include "system/utils/array_size.h"

static char LOGGER_TIME_BUFF[30];

const __FlashStringHelper *LogLevelStrMap[] = {
    F(LOG_DEBUG_PREFIX_MSG),
    F(LOG_INFO_PREFIX_MSG),
    F(LOG_WARN_PREFIX_MSG),
    F(LOG_ERROR_PREFIX_MSG),
    F("[     ]: ")
};

const __FlashStringHelper *ColorAnsiCodeStr[] = {
    F("\033[1m"),
    F("\033[93m"),
    F("\033[91m"),
    F("\033[32m"),
    F("\033[0m")
};

const __FlashStringHelper *GetStringLogLevel(eLogLevel_t log_lv)
{
    if (log_lv >= ARRAY_SIZE(LogLevelStrMap)) {
        return F("[UNKNO]: ");
    }
    return LogLevelStrMap[(uint8_t)log_lv];
}

size_t KernelLogger::write(const uint8_t *buffer, size_t size)
{
    if (this->cout != nullptr)
        return this->cout->write(buffer, size);
    return 0;
}

size_t KernelLogger::write(uint8_t _byte)
{
    if (this->cout != nullptr)
        return this->cout->write(_byte);
    return 0;
}

void KernelLogger::setLogTime(Time_s time)
{
    memset(LOGGER_TIME_BUFF, 0x00, ARRAY_SIZE(LOGGER_TIME_BUFF));
    time.toCharArray(LOGGER_TIME_BUFF);
    printTime = true;
}

KernelLogger &KernelLogger::operator<<(LoggerIntBase_e base)
{
    this->intBase = base;
    return *this;
}

KernelLogger &KernelLogger::operator<<(LoggerFormatText_e ansiCode)
{
    if (ansiCode < ARRAY_SIZE(ColorAnsiCodeStr)) {
        (*this) << ColorAnsiCodeStr[ansiCode];
    }
    return *this;
}

KernelLogger &KernelLogger::operator<<(LoggerSpecialChar_t specialChar)
{
    switch (specialChar)
    {
    case EndLine:
        (*this) << LOGGER_TEXT_RESET << F("\n\r");
        break;
    }
    return *this;
}

KernelLogger &KernelLogger::operator<<(eLogLevel_t lvl)
{
    this->canLog = lvl >= this->log_lvl;
    (*this) << LOGGER_TEXT_RESET;
    if (printTime)
        (*this) << LOGGER_TEXT_BOLD << F("> ") << LOGGER_TIME_BUFF << F(" ");
    (*this) << GetStringLogLevel(lvl);
    return *this;
}

KernelLogger &KernelLogger::operator<<(const __FlashStringHelper *msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(const String &msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(const char msg[])
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(char msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(unsigned char msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg, (uint8_t)intBase);
    return *this;
}

KernelLogger &KernelLogger::operator<<(int msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(unsigned int msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(long msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(unsigned long msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(double msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

KernelLogger &KernelLogger::operator<<(const Printable &msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

void KernelLogger::log(const __FlashStringHelper *msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(const String &msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(const char msg[], eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(char msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(unsigned char msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(int msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(unsigned int msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(long msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(unsigned long msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(double msg, eLogLevel_t lvl, int precision)
{
    (*this) << lvl << msg << EndLine;
}

void KernelLogger::log(const Printable &msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}