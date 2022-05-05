#include "system/utils/i2c.h"

#include <system/modules/logger.h>

static bool isI2CDeviceConnected(TwoWire &bus, uint8_t address)
{
    uint8_t error;
    bus.beginTransmission(address);
    error = bus.endTransmission();
    return error == 0;
}

void DumpI2Cdevices(TwoWire &bus)
{
    uint8_t row = 0;
    char buffer[3];

    bus.begin();
    logger << LOG_INFO << F("------------------- Scanning I2C Bus -------------------") << EndLine;
    logger << LOG_INFO << F("         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F") << EndLine;
    logger << LOG_INFO << F("    00: -- ");

    for (size_t i = 1; i < 0x80; i++)
    {
        if (i % 16 == 0)
        {
            row += 0x10;
            sprintf(buffer, "%02X", row);
            logger << EndLine << LOG_INFO << F("    ") << buffer << F(": ");
        }

        if (isI2CDeviceConnected(bus, i))
        {
            sprintf(buffer, "%02X", i);
            logger << LOGGER_TEXT_GREEN << buffer << LOGGER_TEXT_RESET << F(" ");
        }
        else
        {
            logger << F("-- ");
        }
    }
    logger << EndLine;
}