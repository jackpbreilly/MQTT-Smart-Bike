#include <Wire.h>
#include <ds3231.h>

#include <EspMQTTClient.h>

#include <PubSubClient.h>

#define Hall_Sensor_D 16
#define TOPIC "RTC"
int StateChange = 0;
int Val=0;

EspMQTTClient client(
  "", //SSID
  "", //PASSWORD
  "192.168.0.43",  // MQTT Broker Server IP
  TOPIC      // Client Name | CAN be any string doesn't need to be same as topic
);
void onConnectionEstablished() {
  client.subscribe(TOPIC, [] (   const String &payload){onMessageReceived(payload);});
}
void onMessageReceived(const String& message) { 
  Serial.println(String(message)); // Print messages sent to subscribed topic to serial monitor
}

 
struct ts t; 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
}
 
void loop() {
  DS3231_get(&t);
  String time = "";
  time.concat(t.hour); time.concat(":"); time.concat(t.min); time.concat(":");  time.concat(t.sec);
  client.publish(TOPIC, time);
  Serial.println(time);
 
  delay(1000);
  client.loop();
}