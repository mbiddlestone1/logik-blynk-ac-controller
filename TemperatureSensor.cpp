#include "TemperatureSensor.h"

/******************************************************************************
 Private macros
******************************************************************************/

// Configuration.
#define SENSOR_IO_PIN (D2)

#define TEMP_PRECISION (9)  // Lowest resolution of 0.5 degree celcius increments.
#define TEMP_PIN (D2)

/******************************************************************************
 Public class member definitions
******************************************************************************/

TemperatureSensor::TemperatureSensor(void) : oneWireBus_(TEMP_PIN), temperatureSensorBus_(&oneWireBus_){
    offsetC_ = 0.0f;
    temperatureSensorBus_.begin();
    assert(temperatureSensorBus_.getDeviceCount() == 1);
    assert(temperatureSensorBus_.getAddress(
        temperatureSensorDeviceAddress_, 0
    ));

    temperatureSensorBus_.setResolution(
        temperatureSensorDeviceAddress_, TEMP_PRECISION
    );
    assert(
        temperatureSensorBus_.getResolution(temperatureSensorDeviceAddress_)
        == TEMP_PRECISION
    );
}

float32_t TemperatureSensor::ReadTemperatureC(void) {
    temperatureSensorBus_.requestTemperatures();
    return temperatureSensorBus_.getTempC(temperatureSensorDeviceAddress_) + offsetC_;
}

void TemperatureSensor::SetOffsetC(const float32_t newOffsetC) {
    offsetC_ = newOffsetC;
}
