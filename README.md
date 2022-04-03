Controller for a Logik ac unit.

Switches off AC unit entirely when setpoint is reached to minimize losses when running on portable units (typical portable AC units continue to move the cold indoor air outdoors which causes warm air to be pulled in from outside due to the negative pressure that's created).

Required HW:
  * Wemos D1 Mini.
  * DS18B20 shield.
  * IR controller shield.

Required libraries:
  * Blynk.
  * IRremoteESP8266.
  * DallasTemperature.
  * OneWire.

Blynk IO can be deduced from looking at AcController.ino.