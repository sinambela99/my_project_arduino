//MOTOR DRIVER
//deklarasi pin motor driver
#define IN3 13 // deklarasi pin IN3 5
#define IN4 12 // deklarasi pin IN4 4
#define ENA 11 // deklarasi pin ENB 3
void setup() {
//MOTOR DRIVER
// Konfigurasi pin-pin sebagai Output
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(ENA, OUTPUT);
}    
void loop() {
//menjalankan konveyor
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite (ENA,150); 
}
