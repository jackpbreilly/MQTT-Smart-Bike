#include <TinyGPS++.h>

#include <SoftwareSerial.h>

#include <EspMQTTClient.h>

#include <PubSubClient.h>

#define TOPIC "GPS"

int RXPin = 4;
int TXPin = 5;
String loc;

int GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);
EspMQTTClient client(
  "jack", //SSID
  "12345678", //PASSWORD
  "pitunnel.com",  // MQTT Broker Server IP
  "pi",
  "raspberry",
  TOPIC,      // Client Name | CAN be any string doesn't need to be same as topic
  14079
);
void onConnectionEstablished() {
  client.subscribe(TOPIC, [] (   const String &payload){});
}


void setup() {
  Serial.begin(9600);

    // Start the software serial port at the GPS's default baud
    gpsSerial.begin(GPSBaud);
}

void loop() {
    while (gpsSerial.available() > 0)
        if (gps.encode(gpsSerial.read())) {
            if (gps.location.isValid()) {
                loc = String(gps.location.lat(),6) +","+ String(gps.location.lng(),6);
                    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
                client.publish(TOPIC, String(loc));
            }
            delay(1000);
        }
    client.loop();
}
