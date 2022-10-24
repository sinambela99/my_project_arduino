int RELAY1 = D3; 
int RELAY2 = D4; 
int RELAY3 = D5; 
int RELAY4 = D6; 

void setup() {
  // put your setup code here, to run once:
pinMode(RELAY1, OUTPUT);
pinMode(RELAY2, OUTPUT);
pinMode(RELAY3, OUTPUT);
pinMode(RELAY4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(RELAY1, HIGH); 
digitalWrite(RELAY2, HIGH);
digitalWrite(RELAY3, HIGH);
digitalWrite(RELAY4, HIGH);
delay (2000);
digitalWrite(RELAY1, LOW); 
digitalWrite(RELAY2, LOW); 
digitalWrite(RELAY3, LOW); 
digitalWrite(RELAY4, LOW); 
delay (1000);
} 
