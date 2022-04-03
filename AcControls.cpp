
#include "AcControls.h"

#include <iostream>
#include <ir_Coolix.h>

using namespace std;

/******************************************************************************
 Private macros
******************************************************************************/

#define IR_SEND_PIN (D6)

// Default state
#define DEFAULT_MODE AcMode_Off
#define DEFAULT_FAN_SPEED FanSpeed_Min
#define DEFAULT_CURRENT_TEMP 17

// Corresponds to the DEFAULT_FAN_SPEED
#define DEFAULT_COOLIX_FAN_SPEED kCoolixFanMin

/******************************************************************************
 Public class member definitions
******************************************************************************/

AcControls::AcControls(void) : ac_(IR_SEND_PIN) {
    currentMode_ = DEFAULT_MODE;
    currentFanSpeed_ = DEFAULT_FAN_SPEED;
    currentTempC_ = DEFAULT_CURRENT_TEMP;

    ac_.begin();
    ac_.setFan(DEFAULT_COOLIX_FAN_SPEED);
    ac_.off();
    ac_.send();
};

void AcControls::Send(void) {
    if (updateState_) {
        clog << "Sending any updated state to AC\n";
        
        ac_.send();
        updateState_ = false;
    }
};

void AcControls::SetMode(const AcMode_t newMode) {
    assert((newMode > AcMode_NONE) && (newMode < AcMode_MAX));

    if (newMode != currentMode_) {
        clog << "Setting AC mode to " << newMode << "\n";
        
        switch (newMode) {
            case AcMode_Off:
                ac_.off();
                break;
            case AcMode_Fan:
                ac_.setMode(kCoolixFan);
                SetFan(currentFanSpeed_, true);
                break;
            case AcMode_Cool:
                ac_.setMode(kCoolixCool);
                ac_.setTemp(currentTempC_);
                SetFan(currentFanSpeed_, true);
                break;
            default:
                cerr << "Invalid AC mode\n";
                assert(false);
        };
        currentMode_ = newMode;
        updateState_ = true;
    }
}

void AcControls::SetFan(const FanSpeed_t newFanSpeed, const bool force) {
    assert((newFanSpeed > FanSpeed_NONE) && (newFanSpeed < FanSpeed_MAX));

    if ((currentFanSpeed_ != newFanSpeed) || force) {
        clog << "Setting fan speed to " << newFanSpeed << "\n";

        uint8_t kCoolixFanSpeed;
        switch (newFanSpeed) {
            case FanSpeed_Min:
                kCoolixFanSpeed = kCoolixFanMin;
                break;
            case FanSpeed_Max:
                kCoolixFanSpeed = kCoolixFanMax;
                break;
            case FanSpeed_Auto:
                kCoolixFanSpeed = kCoolixFanAuto;
                break;
            default:
                cerr << "Invalid fan speed\n";
                assert(false);
        }

        if (currentMode_ != AcMode_Off) {
            ac_.setFan(kCoolixFanSpeed);
            currentFanSpeed_ = newFanSpeed;
            updateState_ = true;
        }
    }
}

void AcControls::SetTempC(const uint8_t newTempC)
{
    assert((newTempC >= 17u) && (newTempC <= 30u));
    if (newTempC != currentTempC_) {
        clog << "Setting temperature setpoint to " << newTempC << "C\n";
        ac_.setTemp(newTempC);
        updateState_ = true;
    }
}
