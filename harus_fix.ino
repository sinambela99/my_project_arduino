/*********************************
 * Monitoring Suhu Kelembaban
 * Sensor : DHT11
 * Board  : Arduino Nano
 * Output : Serial (9600)
 * DOUT -> pin A1
*********************************/
#include <DHT.h> //library DHT
#define PIR_PIN 2
#define LED_PIN 6
#define kipas 5
#define kipas2 7
#define utama 9
#define DHTPIN A1 //pin DATA konek ke pin 4 Arduino
#define DHTTYPE DHT11 //tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE); //set sensor + koneksi pin
  long int
// jeda waktu indikator hidup ketika sensor PIR mendeteksi
// gerakan dalam milidetik
ledOn = 10000,
lastTime = -ledOn; // penghitung waktu (default led mati) 
                
void setup()
{
  Serial.begin(9600); //baud 9600
  dht.begin();
pinMode(LED_PIN, OUTPUT); // Atur pin 13 menjadi mode Output
pinMode(kipas, OUTPUT); // Atur pin 13 menjadi mode Output
pinMode(utama, OUTPUT); // Atur pin 13 menjadi mode Output
pinMode(kipas2, OUTPUT); // Atur pin 13 menjadi mode Output
pinMode(PIR_PIN, INPUT); // Atur pin 2 menjadi mode input 
digitalWrite(LED_PIN, HIGH); // matikan led indikator
digitalWrite(utama, HIGH);
delay(100); // Jeda sebentar
}

void loop() 
{
  float kelembaban = dht.readHumidity();
  float suhu = dht.readTemperature();

  Serial.print("Suhu=");  //kirim serial "Suhu"
  Serial.print(suhu);     //kirim serial nilai suhu   
  Serial.println("*C"); //Satuan Derajat Suhu
  Serial.print("Humi=");  //kirim serial "Humi"
  Serial.print(kelembaban);     //kirim serial nilai kelembaban  
  Serial.println("%RH");  //kirim serial "%RH"
  delay(1000);            //tunda 1 detik untuk pembacaan berikutnya

if ((suhu >= 32)&&(suhu < 35)){
digitalWrite(kipas, LOW);
digitalWrite(kipas2, HIGH);
}
else if ((suhu >= 35)&&(suhu < 60)){
digitalWrite(kipas, LOW);
digitalWrite(kipas2, LOW);  
}
else if (suhu < 32){
 digitalWrite(kipas, HIGH);
 digitalWrite(kipas2, HIGH);
 }
  //////////////////////////
if(digitalRead(PIR_PIN) == HIGH){    
lastTime = millis();
Serial.println("Motion Detected");
delay(100);
}else{
Serial.println("No Motion");
delay(100);
}
if ((millis() - lastTime) < ledOn){
digitalWrite(utama, LOW);  
digitalWrite(LED_PIN, LOW);
}
else if((millis() - lastTime) > ledOn) {
digitalWrite(LED_PIN, HIGH);
digitalWrite(utama, HIGH);
}
}
