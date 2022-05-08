#include <EspMQTTClient.h>

#include <PubSubClient.h>

#define Hall_Sensor_D 16
#define TOPIC "HallSensor"
int StateChange = 0;
int Val = 0;

EspMQTTClient client(
    "jack", //SSID
    "12345678", //PASSWORD
    "pitunnel.com", // MQTT Broker Server IP
    "pi",
    "raspberry",
    TOPIC, // Client Name | CAN be any string doesn't need to be same as topic
    14079
);
void onConnectionEstablished() {
    client.subscribe(TOPIC, [](const String & payload) {});
}

void setup() {
    pinMode(Hall_Sensor_D, INPUT);
}

void loop() {
    Val = digitalRead(Hall_Sensor_D);
    if (Val != StateChange) {
        client.publish(TOPIC, String(Val));
        StateChange = Val;
    }
    client.loop();
}
