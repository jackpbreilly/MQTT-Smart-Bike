/* This code is to be used with KY-024 Hall effect sensor
 * It displays both Analog and Digital values given by the sensor
 * Refer to www.surtrtech.com for more details
 */

#define Hall_Sensor A0          //A0 used with analog output, D2 with digital output
#define Hall_Sensor_D 16
int StateChange = 0;

int Val=0;             //Here you can store both values, the Val2 can be boolean


void setup() {
  Serial.begin(9600);
  pinMode(Hall_Sensor_D,INPUT);
 

}

void loop() {
  
   Val=digitalRead(Hall_Sensor_D);
   if(Val != StateChange){
    Serial.println(Val);
    StateChange = Val;
   }
}
