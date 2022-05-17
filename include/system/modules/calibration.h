#if !defined(__CODE_GRAV_SYSTEM_MODULES_CALIBRATION_H__)
#define __CODE_GRAV_SYSTEM_MODULES_CALIBRATION_H__

#include <stdint.h>
#include "system/utils/types.h"
#include "system/kernel/nvstorage.h"
#include "system/modules/logger.h"
#include "system/utils/linkedlist.h"

template <typename Tvariable>
class VariableCalibration
{
protected:
    Tvariable calibrationResults;
    const char* id;
public:
    VariableCalibration(const char* id) : id(id) { }

    Tvariable getResults() const { return calibrationResults; }
    const char* getId () const { return id; }

    virtual bool calibrate();

    virtual ~VariableCalibration() { }
};

template <typename Tvariable>
class VariableCalibrationNvram {
public:
    static Tvariable Restore(VariableCalibration<Tvariable> &calibration)
    {
        Blob_t offsetsData;
        bool status = OsKernel::NvVariableGet(calibration.getId(), &offsetsData);
        if (!status) {
            logger << LOG_WARN << F("Can't load stored calibrations!") << EndLine;
        }
        return *((Tvariable*)offsetsData.pData);
    }

    static bool Save(VariableCalibration<Tvariable> &calibration)
    {
        Tvariable rslts = calibration.getResults();
        return OsKernel::NvVariableSet(calibration.getId(),
            Blob_t{.size = sizeof(Tvariable), .pData = &rslts});
    }
};

template <typename Tvariable>
struct AverageOffsetsCalibrationsConfig_t {

    uint16_t measureIntervalMs;
    uint16_t measuringTimeS;

    void *pContext;

    void (*preMeasures) (void *pContext);
    Tvariable (*onMeasure) (void *pContext);
    void (*postMeasures) (void *pContext);
    Tvariable (*processResults) (void *pContext, LinkedList<Tvariable>* measures);
};

template <typename Tvariable>
class AverageOffsetsCalibrations : public VariableCalibration<Tvariable>
{
    AverageOffsetsCalibrationsConfig_t<Tvariable> config;
public:
    AverageOffsetsCalibrations(const char* id) : VariableCalibration<Tvariable>(id) { }

    void setConfig(AverageOffsetsCalibrationsConfig_t<Tvariable> config) {
        this->config = config;
    }

    bool calibrate() {
        LinkedList<Tvariable> measures;
        esp_timer_handle_t updateSensorsTimer_h;
        TickType_t xLastWakeTime;
        TaskHandle_t self = xTaskGetCurrentTaskHandle ();

        esp_timer_create_args_t updateSensorTimerConfig = {
            .callback = [](TaskHandle_t self) -> void {
                logger.print('.');
                xTaskNotify(self, 0x00, eNoAction);
            },
            .arg = self,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "CalibrationUpdate"
        };

        logger << LOG_INFO << F("Measuring for ") << config.measuringTimeS << F("s") << EndLine;

        if (this->config.preMeasures) {
            this->config.preMeasures(this->config.pContext);
        }

        esp_timer_create (&updateSensorTimerConfig, &updateSensorsTimer_h);
        esp_timer_start_once (updateSensorsTimer_h, config.measuringTimeS * 1000000UL);

        while (xTaskNotifyWait(0, 0, NULL, 0) != pdPASS) {
            xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(config.measureIntervalMs));

            measures.add(this->config.onMeasure(this->config.pContext));
        }

        logger << EndLine;
        esp_timer_delete(updateSensorsTimer_h);

        if (this->config.postMeasures) {
            this->config.postMeasures(this->config.pContext);
        }

        this->calibrationResults = this->config.processResults(this->config.pContext, &measures);

        measures.clear();

        return true;
    }
};

extern AverageOffsetsCalibrations<Vector3D_t> GiroscopeCalibration;
extern AverageOffsetsCalibrations<Vector3D_t> AccelerometerCalibration;
extern AverageOffsetsCalibrations<Vector3D_t> MagnetometerCalibration;

#endif // __CODE_GRAV_SYSTEM_MODULES_CALIBRATION_H__
