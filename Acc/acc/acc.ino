
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include<Wire.h>
#include <EspMQTTClient.h>
#include <PubSubClient.h>

#define TOPIC "Accelerometer"

const int MPU_addr = 0x68;

Adafruit_MPU6050 mpu;
int16_t axis_X, axis_Y, axis_Z;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;


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
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
    // Try to initialize!
    if (!mpu.begin()) {
        delay(10);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    delay(100);
}
void loop() {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true);
    axis_X = Wire.read() << 8 | Wire.read();
    axis_Y = Wire.read() << 8 | Wire.read();
    axis_Z = Wire.read() << 8 | Wire.read();
    int xAng = map(axis_X, minVal, maxVal, -90, 90);
    int yAng = map(axis_Y, minVal, maxVal, -90, 90);
    int zAng = map(axis_Z, minVal, maxVal, -90, 90);

    x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
    z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent( & a, & g, & temp);

    client.publish(TOPIC, String(String(y) + "," + String(a.acceleration.x) + "," + String(temp.temperature)));
    delay(500);
    client.loop();

}
