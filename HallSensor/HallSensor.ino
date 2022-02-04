#include <EspMQTTClient.h>

#include <PubSubClient.h>

#define Hall_Sensor_D 16
#define TOPIC "HallSensor"
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


void setup() {
  Serial.begin(9600);
  pinMode(Hall_Sensor_D,INPUT);
}

void loop() {
   Val=digitalRead(Hall_Sensor_D);
   if(Val != StateChange){
    client.publish(TOPIC, String(Val));
    Serial.println(Val);
    StateChange = Val;
   }
    client.loop();
}
