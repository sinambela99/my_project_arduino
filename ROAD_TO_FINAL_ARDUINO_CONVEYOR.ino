#include <Wire.h>
#include <Servo.h>
Servo servo;

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define IN1 13 // deklarasi pin IN3 5
#define IN2 12 // deklarasi pin IN4 4
#define ENA 11 // deklarasi pin ENB 3
#define IN1_M A4 // deklarasi pin IN1 Pemotong Cabai Merah
#define IN2_M A5 // deklarasi pin IN2 Pemotong Cabai Merah
#define ENA_M 9 // deklarasi pin ENA Pemotong Cabai Merah
#define IN3_H A1 // deklarasi pin IN1 Pemotong Cabai Merah
#define IN4_H A2 // deklarasi pin IN2 Pemotong Cabai Merah
#define ENB_H 10 // deklarasi pin ENA Pemotong Cabai Merah
#define IR 3

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() 
{
  servo.attach(2); //D2
  servo.write(62);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(ENA, OUTPUT);
pinMode(IN1_M, OUTPUT);
pinMode(IN2_M, OUTPUT);
pinMode(ENA_M, OUTPUT);
pinMode(IN3_H, OUTPUT);
pinMode(IN4_H, OUTPUT);
pinMode(ENB_H, OUTPUT);    
 pinMode(IR, INPUT);
  // Setting output
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting sensorout
  pinMode(sensorOut, INPUT);
  
  // Setting Frekuensi
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite (ENA,HIGH); 
  digitalWrite (ENA_M,HIGH);
  digitalWrite (ENB_H,HIGH); 
  // Begins serial LCD
  Serial.begin(9600);
}

void loop() {
if(digitalRead (IR)== LOW) STOP();
if(digitalRead (IR)== HIGH) START();

  // Setting RED Color
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);

  redFrequency = pulseIn(sensorOut, LOW);
  redColor = map(redFrequency, 68, 114, 255,0);
  delay(10);
  
  // Setting GREEN Color
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, 118, 157, 255, 0);
  delay(10);
 

  if(redColor > greenColor && redColor > blueColor){
      Serial.println("Warna Merah");
      digitalWrite (ENB_H,LOW); 
      digitalWrite (ENA_M,HIGH);
      delay(11000);
      servo.write(62); //prosedur penulisan data PWM ke motor servo
  }
  
  if(greenColor > redColor && greenColor > blueColor){
    Serial.println("Warna Hijau"); 
    servo.write(15);
    digitalWrite (ENA_M,LOW);
    digitalWrite (ENB_H,HIGH);
    delay(11000);
    servo.write(62); //prosedur penulisan data PWM ke motor servo 
  }
}
void STOP(){
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN1_M, LOW);
digitalWrite(IN2_M, LOW);
digitalWrite(IN3_H, LOW);
digitalWrite(IN4_H, LOW);
delay (2000);
}

void START(){
digitalWrite(IN1, LOW);    
digitalWrite(IN2, HIGH);
digitalWrite(IN1_M, LOW);
digitalWrite(IN2_M, HIGH);
digitalWrite(IN3_H, LOW);
digitalWrite(IN4_H, HIGH);
}
