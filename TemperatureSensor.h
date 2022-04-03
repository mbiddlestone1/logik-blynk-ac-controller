
#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <DallasTemperature.h>
#include <OneWire.h>

#include "stdtypes.h"

class TemperatureSensor {
    private:
        OneWire oneWireBus_;
        DallasTemperature temperatureSensorBus_;
        DeviceAddress temperatureSensorDeviceAddress_;
        float32_t offsetC_;

    public:
        TemperatureSensor(void);

        float32_t ReadTemperatureC(void);
        void SetOffsetC(float32_t);
};

#endif
