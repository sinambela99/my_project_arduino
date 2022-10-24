#define BLYNK_TEMPLATE_ID "TMPLX7K9-vpp"
#define BLYNK_DEVICE_NAME "SKM"
#define BLYNK_AUTH_TOKEN "x6Z5vMt3vsi0DIGyFXTx-Ujf0PYauyeU"

#define BLYNK_PRINT Serial
#include <PZEM004Tv30.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/* Hardware Serial3 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Apa";
char pass[] = "moonimoon"; // Jika tidak menggunakan password kosongkan ""
BlynkTimer timer;

PZEM004Tv30 pzem(&Serial2);

void sensor() {
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
    Blynk.virtualWrite(V0,power);
    Blynk.virtualWrite(V1,energy);
    Blynk.virtualWrite(V2,voltage);
    Blynk.virtualWrite(V3,current);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L,sensor);
}

void loop() {
    Blynk.run();
    timer.run();
}
