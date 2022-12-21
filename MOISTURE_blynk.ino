#include <Wire.h> 
#define BLYNK_TEMPLATE_ID "TMPLX7K9-vpp"
#define BLYNK_DEVICE_NAME "SKM"
#define BLYNK_AUTH_TOKEN "x6Z5vMt3vsi0DIGyFXTx-Ujf0PYauyeU"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Pi Ware";
char pass[] = "29528677"; // Jika tidak menggunakan password kosongkan ""

const int pompa_air = 18;
const int pompa_ph = 5;
const int analogInPin = 34;  //sambungkan kabel hitam (output) ke pin A0
int sensorValue = 0;        
float outputValue = 0.0;      
const int  AOUT_PIN = 35; // ESP32 pin GIOP13 (ADC0) that connects to AOUT pin of moisture sensor
#define THRESHOLD 1000 // CHANGE YOUR THRESHOLD HERE

BlynkTimer timer;
int moistureLevel;

BLYNK_WRITE (V2)
{
 int buttonvalue = param.asInt();//assigning incoming value fromm v7 to a variabel
 digitalWrite(pompa_air, buttonvalue);
}

BLYNK_WRITE (V3)
{
 int buttonvalue2 = param.asInt();//assigning incoming value fromm v7 to a variabel
 digitalWrite(pompa_ph, buttonvalue2);
}

void sendSensor2()
{
  int value = analogRead(AOUT_PIN);
   if (value < THRESHOLD){
    Serial.println("The soil is DRY ");
    }
  else {
    Serial.println("The soil is WET ");
    }
    //////////////////////////////////
   sensorValue = analogRead(analogInPin);
  //rumus didapat berdasarkan datasheet 
  outputValue = (-0.0693*sensorValue)+7.3855; 
  Serial.print("pH sensor Tanah = ");
  Serial.println(outputValue);
   if  (outputValue < 7) {
   Serial.print("pH Terlalu rendah");
    }
  else{ 
    Serial.print("pH Normal");
  }
  Blynk.virtualWrite(V0, value);
  Blynk.virtualWrite(V1, outputValue);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(pompa_air, OUTPUT);
  pinMode(pompa_ph, OUTPUT);
  timer.setInterval(2000L, sendSensor2);
}

void loop() {
  Blynk.run();  
  timer.run();
}
