#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define BLYNK_TEMPLATE_ID "TMPLjvtl2Km6"
#define BLYNK_DEVICE_NAME "SMART ENERGY METER YOHANES"
#define BLYNK_AUTH_TOKEN "7xv3jeQFmhtrDIh-K8yHSEgZVGNsuqNm"
#define BLYNK_PRINT Serial

#include <PZEM004Tv30.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/* Hardware Serial3 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
const int relay = 2;
float biaya;
float hargaperkwh = 1444.7;

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Apa";
char pass[] = "moonimoon"; // Jika tidak menggunakan password kosongkan ""


PZEM004Tv30 pzem(&Serial2);
WidgetLCD lcd2(V6);

BLYNK_WRITE (V7)
{
 int buttonvalue = param.asInt();//assigning incoming value fromm v7 to a variabel
 if (buttonvalue = 1){
 digitalWrite(relay, LOW);
 pzem.resetEnergy();
}

}

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  lcd.begin();
}

void loop() {
    float voltage = pzem.voltage();
    if(!isnan(voltage)){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    float current = pzem.current();
    if(!isnan(current)){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    if(!isnan(power)){
        Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }

    float energy = pzem.energy();
    if(!isnan(energy)){
        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
         Blynk.logEvent("informasi_listrik","PEMAKAIAN SEKITAR 100% (0.2)");
    } else {
        Serial.println("Error reading energy");
    }

    float frequency = pzem.frequency();
    if(!isnan(frequency)){
        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
    } else {
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    if(!isnan(pf)){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }
    Serial.println();
    delay(2000);
    
  lcd.setCursor(0,0);
  lcd.print("SEM YOHANES");
  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.setCursor(7,1);
  lcd.print("W");
  lcd.setCursor(2,1);
  lcd.print(power);
  lcd.setCursor(9,1);
  lcd.print("E:");
  lcd.setCursor(17,1);
  lcd.print("kWh");
  lcd.setCursor(11,1);
  lcd.print(energy);
  lcd.setCursor(0,2);
  lcd.print("V:");
  lcd.setCursor(8,2);
  lcd.print("V");
  lcd.setCursor(2,2);
  lcd.print(voltage);
  lcd.setCursor(10,2);
  lcd.print("I:");
  lcd.setCursor(12,2);
  lcd.print(current);
  lcd.setCursor(16,2);
  lcd.print("A");
  lcd.setCursor(0,3);
  lcd.print("f:");
  lcd.setCursor(7,3);
  lcd.print("Hz");
  lcd.setCursor(2,3);
  lcd.print(frequency);
  lcd.setCursor(10,3);
  lcd.print("Pf:");
  lcd.setCursor(14,3);
  lcd.print(pf);
  delay (2000);
  lcd.clear();
  delay (10);


   biaya = energy * hargaperkwh ;
   Serial.print ("Harga:");
   Serial.print ("Rp ");
   Serial.print (biaya);
   Serial.println (",-");
  

   lcd2.clear();
   lcd2.print(0, 0, "Harga:");
   lcd2.print(6, 0, "Rp ");
   lcd2.print(8, 0, biaya);
   lcd2.print(14, 0, ",-");
   
    Blynk.virtualWrite(V0,power);
    Blynk.virtualWrite(V1,energy);
    Blynk.virtualWrite(V2,voltage);
    Blynk.virtualWrite(V3,current);
    Blynk.virtualWrite(V4,frequency);
    Blynk.virtualWrite(V5,pf);
    Blynk.run();

    if(energy>=0.200) {
    digitalWrite(relay,LOW);}   
    else{
    digitalWrite(relay, HIGH);}
}
