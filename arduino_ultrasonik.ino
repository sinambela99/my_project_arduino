#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>

SoftwareSerial sim(10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trig 7//membuat triger pin ke kaki 3 arduino
#define echo 8//membuat echo pin ke kaki 4 arduino
int RELAY1 = 3; 
int RELAY2 = 4; 
int RELAY3 = 5; 
int RELAY4 = 6; 
long durasi;///membuat variabel durasi
int ketinggian_air;//membuat variabel jarak
int ketinggian_wadah = 50;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);//set komunikasi serial dengan baudrate sebesar 9600
lcd.begin();
  sim.begin(9600);
  delay(100);
  sim.println("AT");
  sim.println("AT+CMGF=1");
  sim.println("AT+CNMI=1,2,0,0,0");
pinMode(trig, OUTPUT);//set pin tigger sebagai output sensor
pinMode(echo, INPUT);// set pin echo sebagai input sensor

}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  digitalWrite(trig, LOW);//set trigger pin bernilai low 
  delayMicroseconds(5);//delay dalam microsecond sebesar 5 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  durasi = pulseIn(echo, HIGH);// membuat fungsi durasi sebagai pulsa yang masuk dengan nilai echo sebagai high
  ketinggian_air = ketinggian_wadah-durasi * 0.034 / 2; //rumus ketentuan jarak
  delay(200);
  sim.print("AT+CMGF=1\r\n");
  sim.print("AT+CMGS=\"");
  sim.print("081377358612");
  sim.print("\"\r\n");
  delay(3000);
  sim.print(ketinggian_air );
  sim.print(" cm");
  sim.write(0x1A);
  delay(3000);
  sim.println("AT+CMGD=3");
  delay(2000);//delay dalam 1 detik akan ditampilkan hasil dari variabel jarak
  Serial.print("ketinggian_air = ");
  Serial.print(ketinggian_air);//memunculkan dalam komunikasi serial nilai dari jarak dalam bentuk cm
  Serial.println(" cm");
  
  lcd.setCursor(0,1);
  lcd.print("Tinggi Air:");
  lcd.setCursor(11,1);
  lcd.print(ketinggian_air);
  lcd.setCursor(14,1);
  lcd.print("cm");
  delay(2000);
  
        if (ketinggian_air > 21 ){
        lcd.setCursor(0,0);
        lcd.print("Status Awas!");
        Serial.println("Status Banjir : Awas");  
        kirim("Status Banjir : Awas");
        pinMode(RELAY3, HIGH);
        pinMode(RELAY1, LOW);
        pinMode(RELAY2, LOW);
        delay(10000);} 
          
        else if ((ketinggian_air >= 16)&&(ketinggian_air < 21)){
        lcd.setCursor(0,0);
        lcd.print("Status Waspada!");
        Serial.println("Status Banjir : Waspada"); 
        kirim("Status Banjir : Waspada");
        pinMode(RELAY2, HIGH);
        pinMode(RELAY3, LOW);
        pinMode(RELAY1, LOW);
        delay(30000);}
        
        else if ((ketinggian_air >= 1 )&&(ketinggian_air < 16)){
        lcd.setCursor(0,0);
        lcd.print("Status Siaga 1!");
        Serial.println("Status Banjir : Siaga 1");
        kirim("Status Banjir : Siaga 1"); 
        pinMode(RELAY1, HIGH);
        pinMode(RELAY2, LOW);
        pinMode(RELAY3, LOW);
        delay(60000);} 
}

void kirim(String p){ 
  sim.print("AT+CMGF=1\r\n");
  delay(2000);
  sim.print("AT+CMGS=\"");
  sim.print("081377358612");
  sim.print("\"\r\n");
  delay(2000);
  sim.print(p);
  sim.write(0x1A);
  delay(2000);
  sim.println("AT+CMGD=3");
}
