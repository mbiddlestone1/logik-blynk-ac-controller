#include <iostream>

#define BLYNK_PRINT Serial  ///< Must be defined before importing BlynkSimple...

#include <BlynkSimpleEsp8266.h>

#include "AcControls.h"
#include "stdtypes.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

using namespace std;

/******************************************************************************
 Private macros
******************************************************************************/

#define SERIAL_BAUD_RATE 115200

/// The rate at which to update the controller state when there is no input from
/// the user.
#define UPDATE_RATE_MS 60e3

/// The Blynk virtual pin on which to return the current temperature in degrees
/// celsius.
#define BLYNK_CURRENT_TEMP_PIN (V3)

/******************************************************************************
 Private data
******************************************************************************/

// Blynk authentication token.
char auth[] = "";

// WiFi credentials.
char ssid[] = "";
char pass[] = "";

/******************************************************************************
 Private class definitions
******************************************************************************/

/// Blynk controlled AC controller.
class AcController : public TemperatureController {
  private:
    bool stateUpdateRequested_ = false;
    uint32_t nextUpdateTimeMs_ = UINT32_MAX;

    /// Switch off the ESP8266's onboard LED.
    void SwitchOffOnBoardLed(void) {
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH);
    }

  public:
    AcController(void) {
      SwitchOffOnBoardLed();
    }

    /// Forces the controller to update its state.
    /// To be called whenever there is a user input.
    void UpdateState(void) {
      stateUpdateRequested_ = true;
    }

    /// Flag that the controller state has been updated.
    void StateUpdated(void) {
      stateUpdateRequested_ = false;
    }

    /// Actions performed for each control cycle.
    void Step(void) {
      clog << "*****Loop step*****\n";
      while((millis() < nextUpdateTimeMs_) && !stateUpdateRequested_) {
        Blynk.run();
      }
      nextUpdateTimeMs_ = millis() + (uint32_t)UPDATE_RATE_MS;

      TemperatureController::Step();
      Blynk.virtualWrite(
        BLYNK_CURRENT_TEMP_PIN, TemperatureController::GetCurrentTemperatureC()
      );
      StateUpdated();
    }
} acController_;


/******************************************************************************
 Public function definitions
******************************************************************************/

void setup(void) {
  Serial.begin(SERIAL_BAUD_RATE);
  Blynk.begin(auth, ssid, pass);
}

void loop(void) {
  acController_.Step();
}

/// Called on first startup or whenever the internet connection was lost and
/// re-established. Calls each of the BLYNK_WRITE functions to make sure
/// internal state matches what is set in the app.
BLYNK_CONNECTED() {
    Blynk.syncAll();
}

// BLYNK_WRITE must use stringification of the given parameter as defining
// macros with descriptive names does not work.

/// V0 = Temperature controller mode.
BLYNK_WRITE(V0) {
  int16_t pinValue = param.asInt();
  AcMode_t newMode;
  switch (pinValue)
  {
    case 1:
      newMode = AcMode_Off;
      break;
    case 2:
      newMode = AcMode_Fan;
      break;
    case 3:
      newMode = AcMode_Cool;
      break;
    default:
      assert(false);
  }
  acController_.SetMode(newMode);
  acController_.UpdateState();
}

/// V1 = Temperature controller fan speed.
BLYNK_WRITE(V1) {
  int16_t pinValue = param.asInt();
  FanSpeed_t newFanSpeed;
  switch (pinValue)
  {
    case 1:
      newFanSpeed = FanSpeed_Min;
      break;
    case 2:
      newFanSpeed = FanSpeed_Max;
      break;
    case 3:
      newFanSpeed = FanSpeed_Auto;
      break;
    default:
      assert(false);
  }
  acController_.SetFanSpeed(newFanSpeed);
  acController_.UpdateState();
}

/// V2 = Temperature controller setpoint (degrees C).
BLYNK_WRITE(V2) {
  acController_.SetTemperatureSetpointC(param.asFloat());
  acController_.UpdateState();
}

/// V4 = Temperature sensor offset (degrees C).
BLYNK_WRITE(V4) {
  acController_.SetOffsetC(param.asFloat());
  acController_.UpdateState();
}

/// V5 = Temperature controller fan diff threshold (degrees C).
/// Below <measured temperature> + <fan diff threshold> the fan speed will
/// decrease to min if the fan speed is set to auto.
BLYNK_WRITE(V5) {
  acController_.SetFanDiffThresholdC(param.asFloat());
  acController_.UpdateState();
}

/// V6 = Temperature controller off diff threshold (degrees C).
/// Below <measured temperature> + <off diff threshold> the AC will switch off.
BLYNK_WRITE(V6) {
  acController_.SetOffDiffThresholdC(param.asFloat());
  acController_.UpdateState();
}

/// V7 = Temperature controller on diff threshold (degrees C).
/// Above <measured temperature> + <on diff threshold> the AC will switch on.
BLYNK_WRITE(V7) {
  acController_.SetOnDiffThresholdC(param.asFloat());
  acController_.UpdateState();
}

/// V8 = Temperature controller fan power control.
/// If set to true the schmitt trigger for power which acts on the AC in cool
/// mode will also work in fan mode.
BLYNK_WRITE(V8) {
  acController_.SetFanPowerControl(param.asInt() == 1);
  acController_.UpdateState();
}
