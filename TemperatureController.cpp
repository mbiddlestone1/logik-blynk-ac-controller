
#include "TemperatureController.h"

#include <iostream>
using namespace std;

/******************************************************************************
 Private macros
******************************************************************************/

#define MIN_AC_SETPOINT (17u)

// Default state
#define DEFAULT_POWERED false
#define DEFAULT_MODE AcMode_Off
#define DEFAULT_FAN_SPEED FanSpeed_Auto
#define DEFAULT_FAN_DIFF_THRESHOLD_C 2.0f
#define DEFAULT_OFF_DIFF_THRESHOLD_C -1.0f
#define DEFAULT_ON_DIFF_THRESHOLD_C 0.5
#define DEFAULT_FAN_POWER_CONTROL true

/******************************************************************************
 Private class member definitions
******************************************************************************/

void TemperatureController::ReadCurrentTemperatureC(void) {
    currentTemperatureC_ = temperatureSensor_.ReadTemperatureC();
}

void TemperatureController::ControlPower(void) {
    if (currentTemperatureC_ <= (temperatureSetpointC_ + offDiffThresholdC_)) {
        clog << "Temperatre setpoint reached. Switching off AC\n";
        powered_ = false;
        acControls_.SetMode(AcMode_Off);
    }
    else if (currentTemperatureC_ >= (temperatureSetpointC_ + onDiffThresholdC_)) {
        clog << "Temperature setpoint below current. Switching AC on\n";
        powered_ = true;
        acControls_.SetMode(currentMode_);
    }
}

void TemperatureController::ControlFan(void) {
    if (!powered_) {}
    else if (fanSpeed_ != FanSpeed_Auto) {
        acControls_.SetFan(fanSpeed_);
    }
    else if (currentTemperatureC_ <= (temperatureSetpointC_ + fanDiffThresholdC_)) {
        clog << "Temperature close to setpoint. Switching fan speed to min\n";
        acControls_.SetFan(FanSpeed_Min);
    }
    else {
        clog << "Temperature far above from setpoint. Switching fan speed to max\n";
        acControls_.SetFan(FanSpeed_Max);
    }
}

/******************************************************************************
 Public class member definitions
******************************************************************************/

TemperatureController::TemperatureController(void) {
    acControls_.SetTempC(MIN_AC_SETPOINT);
    currentTemperatureC_ = (float32_t)MIN_AC_SETPOINT;
    SetTemperatureSetpointC((float32_t)MIN_AC_SETPOINT);
    powered_ = false;
    currentMode_ = DEFAULT_MODE;
    newMode_ = DEFAULT_MODE;
    fanSpeed_ = DEFAULT_FAN_SPEED;
    fanDiffThresholdC_ = DEFAULT_FAN_DIFF_THRESHOLD_C;
    offDiffThresholdC_ = DEFAULT_OFF_DIFF_THRESHOLD_C;
    onDiffThresholdC_ = DEFAULT_ON_DIFF_THRESHOLD_C;
    fanPowerControl_ = DEFAULT_FAN_POWER_CONTROL;
}

void TemperatureController::Step(void) {
    if (newMode_ != currentMode_) {
        currentMode_ = newMode_;
        acControls_.SetMode(currentMode_);
        powered_ = true;
    }

    ReadCurrentTemperatureC();
    if (((currentMode_ == AcMode_Fan) && fanPowerControl_)
        || (currentMode_ == AcMode_Cool))
    {
        // Power control (switches AC on/off based on temperature) called
        // for both fan and cool mode but is switchable for fan mode.
        ControlPower();
    }

    if (currentMode_ != AcMode_Off)
    {
        ControlFan();
    }

    acControls_.Send();
}

float32_t TemperatureController::GetCurrentTemperatureC(void) {
    return currentTemperatureC_;
}

void TemperatureController::SetMode(const AcMode_t newMode) {
    newMode_ = newMode;
}

void TemperatureController::SetFanSpeed(const FanSpeed_t newFanSpeed) {
    fanSpeed_ = newFanSpeed;
}

void TemperatureController::SetTemperatureSetpointC(const float32_t newSetpointC) {
    temperatureSetpointC_ = newSetpointC;
}

void TemperatureController::SetFanDiffThresholdC(const float32_t newThresholdC) {
    fanDiffThresholdC_ = newThresholdC;
}

void TemperatureController::SetOffDiffThresholdC(const float32_t newThresholdC) {
    offDiffThresholdC_ = newThresholdC;
}

void TemperatureController::SetOnDiffThresholdC(const float32_t newThresholdC) {
    onDiffThresholdC_ = newThresholdC;
}

void TemperatureController::SetFanPowerControl(const bool newFanPowerControl) {
    fanPowerControl_ = newFanPowerControl;
}

void TemperatureController::SetOffsetC(const float32_t newOffset) {
    temperatureSensor_.SetOffsetC(newOffset);
}
