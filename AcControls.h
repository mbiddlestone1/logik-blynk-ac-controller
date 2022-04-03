
#ifndef AC_CONTROLS_H
#define AC_CONTROLS_H

#include "stdtypes.h"

#include <ir_Coolix.h>

typedef enum AcMode_e {
    AcMode_NONE = 0,
    AcMode_Off,
    AcMode_Fan,
    AcMode_Cool,
    AcMode_MAX
} AcMode_t;

typedef enum FanSpeed_e {
    FanSpeed_NONE = 0,
    FanSpeed_Min,
    FanSpeed_Max,
    FanSpeed_Auto,
    FanSpeed_MAX
} FanSpeed_t;

class AcControls {
    private:
        IRCoolixAC ac_;
        AcMode_t currentMode_;
        FanSpeed_t currentFanSpeed_;
        uint8_t currentTempC_;
        bool updateState_;

    public:
        AcControls(void);

        void Send(void);
        void SetMode(const AcMode_t newMode);
        void SetFan(const FanSpeed_t newFanSpeed, const bool force = false);
        void SetTempC(const uint8_t newTempC);
};

#endif