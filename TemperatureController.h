
#ifndef TEMPERATURE_CONTROLLER_H
#define TEMPERATURE_CONTROLLER_H

#include "AcControls.h"
#include "stdtypes.h"
#include "TemperatureSensor.h"

class TemperatureController {
    private:
        TemperatureSensor temperatureSensor_;
        AcControls acControls_;
        float32_t currentTemperatureC_;
        float32_t temperatureSetpointC_;
        bool powered_;

        AcMode_t currentMode_;
        AcMode_t newMode_;
        FanSpeed_t fanSpeed_;

        float32_t fanDiffThresholdC_;
        float32_t offDiffThresholdC_;
        float32_t onDiffThresholdC_;
        bool fanPowerControl_;

        void ReadCurrentTemperatureC(void);

        void ControlFan(void);
        void ControlPower(void);

    public:
        TemperatureController(void);

        void Step(void);
        float32_t GetCurrentTemperatureC(void);

        void SetMode(AcMode_t);
        void SetFanSpeed(FanSpeed_t);

        void SetTemperatureSetpointC(float32_t);

        void SetFanDiffThresholdC(float32_t);
        void SetOffDiffThresholdC(float32_t);
        void SetOnDiffThresholdC(float32_t);
        void SetFanPowerControl(bool);
        void SetOffsetC(float32_t);
};

#endif
