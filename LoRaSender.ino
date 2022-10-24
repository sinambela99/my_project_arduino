#include <SPI.h>
#include <LoRa.h>

#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h> 
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(); 

float acceleration_x, acceleration_y, acceleration_z, acceleration; 
float calibration_x, calibration_y, calibration_z; 
float skalarichter,magnitude=0; 
int flag=0;

void calibration(){ 
 sensors_event_t event; 
 accel.getEvent(&event); 
 calibration_x = event.acceleration.x; 
 calibration_y = event.acceleration.y; 
 calibration_z = event.acceleration.z; 
}
void read_acceleration(){ 
 sensors_event_t event; 
 accel.getEvent(&event); 
 acceleration_x = abs(event.acceleration.x-calibration_x); 
 acceleration_y = abs(event.acceleration.y-calibration_y); 

 acceleration_z = abs(event.acceleration.z-calibration_z); 
 float xymax = max(acceleration_x,acceleration_y); 
 acceleration = max(xymax,acceleration_z)-0.04; 
 if(acceleration<0)acceleration=0; 
}

void convert_skalarichter(){ 
 if(acceleration>0 && acceleration<0.017){ 
 magnitude=1; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=0.017 && acceleration<0.045){ 
 magnitude=2; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=0.045 && acceleration<0.14){ 
 magnitude=3; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=0.14 && acceleration<0.39){ 
 magnitude=4; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=0.39 && acceleration<0.92){ 
 magnitude=5; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=0.92 && acceleration<1.8){ 
 magnitude=6; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=1.8 && acceleration<3.4){ 
 magnitude=7; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=3.4 && acceleration<6.5){ 
 magnitude=8; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=6.5 && acceleration<12.4){ 
 magnitude=9; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }
 if(acceleration>=12.4){ 
 magnitude=10; 
 skalarichter = ((log(acceleration)) + 1.6 * (log(30)) - 0.15); 
 }  
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
   pinMode(13,OUTPUT); 
 if(!accel.begin()) 
 {
 Serial.println("ADXL345 no detected!"); 
 }
 
 accel.setRange(ADXL345_RANGE_16_G); 
 delay(1000);
 calibration(); 
}

void loop() {
  Serial.print("Sending packet: ");
  // send packet
  LoRa.beginPacket();
  LoRa.print("Skalarichter_");
  LoRa.print(skalarichter);
  LoRa.endPacket();

  read_acceleration(); 
  convert_skalarichter(); 
 /*
 Serial.print(acceleration_x); Serial.print(" "); 
 Serial.print(acceleration_y); Serial.print(" "); 
http://digilib.mercubuana.ac.id/
27
 Serial.print(acceleration_z); Serial.print(" "); 
 Serial.println(acceleration); 
 */ 
 /*
 Serial.print(acceleration); Serial.print(" "); 
 Serial.print(magnitude); Serial.print(" "); 
 Serial.println(skalarichter); 
 */ 
 Serial.println(skalarichter); 
 if(skalarichter>5) { 
 flag=1; 
 }
 if(flag==1) 
 {
 digitalWrite(13,HIGH); 
 } 
  delay(5000);
}
