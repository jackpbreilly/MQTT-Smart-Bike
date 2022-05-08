#include <EspMQTTClient.h>
#include <PubSubClient.h>
#include <Wire.h>

#include "MAX30105.h"
#include "heartRate.h"

#define TOPIC "HeartRateSensor"

MAX30105 particleSensor;


EspMQTTClient client(
  "jack", //SSID
  "123456789", //PASSWORD
  "pitunnel.com",  // MQTT Broker Server IP
  "pi",
  "raspberry",
  TOPIC,      // Client Name | CAN be any string doesn't need to be same as topic
  14079
);
void onConnectionEstablished() {
  client.subscribe(TOPIC, [] (   const String &payload){});
}

long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;

void setup()
{
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    while (1);
  }
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);
    client.publish(TOPIC, String(beatsPerMinute));
  }
    delay(500);                  // waits for a second

  client.loop();
}
