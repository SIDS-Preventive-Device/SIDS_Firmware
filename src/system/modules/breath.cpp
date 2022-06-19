#include "system/modules/breath.h"
#include "system/modules/logger.h"
#include "system/utils/linkedlist.h"
#include "system/utils/array_size.h"
#include "system/utils/matrix.h"
#include <Arduino.h>

const __FlashStringHelper* BreathStatusStrTable[BREATH_DELIM] = {
    F("DETECTED"),
    F("NOT DETECTED"),
    F("INITIALIZING"),
};

const size_t BreathStatusStrTableSize = ARRAY_SIZE(BreathStatusStrTable); 

const __FlashStringHelper* GetBreathStatusStr (BreathStatus_e st) {
    if (st >= BREATH_DELIM) {
        return F("UNKNOWN");
    }
    return BreathStatusStrTable[(uint8_t)st];
}

BreathStatus_e isBreathMovementDetected(OrientationData_t &orientation)
{
    static LinkedList<Vector3D_t> AccelerometerSamples;
    static const size_t MaxSamples = 10;
    static BreathStatus_e lastResult = BREATH_INITIALIZING;
    static unsigned long st = 0;
    static unsigned long end = 0;


    AccelerometerSamples.add(orientation.acceleration);

    if (AccelerometerSamples.size() >= MaxSamples) {
        Vector3D_t integrated;
        Matrix<3, 1, int16_t> vector;
        integrated.x = 0;
        integrated.y = 0;
        integrated.z = 0;

        for (size_t i = 0; i < (AccelerometerSamples.size() - 1); i++)
        {
            Vector3D_t current = AccelerometerSamples.get(i);
            Vector3D_t next = AccelerometerSamples.get(i + 1);
            integrated.x += (next.x - current.x) / (2.0f);
            integrated.y += (next.y - current.y) / (2.0f);
            integrated.z += (next.z - current.z) / (2.0f);
        }
        integrated.x /= AccelerometerSamples.size();
        integrated.y /= AccelerometerSamples.size();
        integrated.z /= AccelerometerSamples.size();
        vector = integrated.toMatrix();
        AccelerometerSamples.clear();

        logger << LOG_INFO << F("Acceleration vector: ") << vector << F(", magnitude: ") << vector.magnitude() << EndLine;

        lastResult = BREATH_NOT_DETECTED;
        if (vector.magnitude() > 10) {
            lastResult = BREATH_DETECTED;
        }

        if (lastResult == BREATH_NOT_DETECTED) {
            logger << LOG_WARN << F("Breath not detected!") << EndLine;
        }
    }

    return lastResult;
}
