/*********************************
 * Monitoring Suhu Kelembaban
 * Sensor : DHT11
 * Board  : Arduino Nano
 * Output : Serial (9600)
 * DOUT -> pin 4
*********************************/

#include <LiquidCrystal_I2C.h>
#include <DHT.h> //library DHT
#define DHTPIN 4 //pin DATA konek ke pin 4 Arduino
#define DHTTYPE DHT11 //tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE); //set sensor + koneksi pin
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600); //baud 9600
  dht.begin();
  lcd.begin();

}

void loop() 
{
  float kelembaban = dht.readHumidity();
  float suhu = dht.readTemperature();
 
  Serial.print("Suhu=");  //kirim serial "Suhu"
  lcd.clear();
  delay (10);
  lcd.setCursor(0, 0);
  lcd.print("Temp =");  //kirim serial "Suhu"
  lcd.print(suhu);
  Serial.print(suhu);     //kirim serial nilai suhu   
  lcd.print((char)223); // menampilkan karakter derajat pada LCD
  lcd.print("C"); //Satuan Derajat Suhu
  Serial.println("*C"); //Satuan Derajat Suhu
  
  lcd.setCursor(0, 1);
  lcd.print("Humi =");     //kirim serial nilai kelembaban  
  lcd.print(kelembaban);
  lcd.print("%RH");  //kirim serial "%RH"
  Serial.print("Humi=");  //kirim serial "Humi"
  Serial.print(kelembaban);     //kirim serial nilai kelembaban  
  Serial.println("%RH");  //kirim serial "%RH"
  delay(1000);            //tunda 1 detik untuk pembacaan berikutnya
}
