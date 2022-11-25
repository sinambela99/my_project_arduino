#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define BLYNK_TEMPLATE_ID "TMPLnBHEfOSZ"
#define BLYNK_DEVICE_NAME "SMART HOME HANS"
#define BLYNK_AUTH_TOKEN "YqLkT8p5-kdO393srSVTCL8O0GOXF5ui"
#define BLYNK_PRINT Serial

#include <RBDdimmer.h>//
#include <PZEM004Tv30.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#define USE_SERIAL  Serial
#define outputPin  32 
#define zerocross  34 // for bo ards with CHANGEBLE input pins
dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
//dimmerLamp dimmer(outputPin); //initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero
int buttonvalue;
int outVal = 0;
int dim_val ;
int flag = 0;
WidgetLCD lcd2(V11);
BlynkTimer timer;
float biaya;
float hargaperkwh = 1444.7;
bool led_set[2];
long timer_start_set[2] = {0xFFFF, 0xFFFF};
long timer_stop_set[2] = {0xFFFF, 0xFFFF};
unsigned char weekday_set[2];

/* Hardware Serial3 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
int RELAY1 = 13; 
int RELAY2 = 12; 
int RELAY3 = 14; 
int relay5 = 35; 

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Pi Ware";
char pass[] = "29528677"; // Jika tidak menggunakan password kosongkan ""

PZEM004Tv30 pzem(&Serial2);
RTC_DS1307 rtc;

long rtc_sec;
unsigned char day_of_week;

bool led_status[2];
bool update_blynk_status[2];
bool led_timer_on_set[2];

BLYNK_WRITE (V13)
{
 buttonvalue = 1;
}

// #########################################################################################################
// LED 1
BLYNK_WRITE (V12)
{
 int buttonvalue = param.asInt();//assigning incoming value fromm v7 to a variabel
 if (buttonvalue = 1){
 digitalWrite(RELAY1, HIGH);
}
}

BLYNK_WRITE(V7)
{
  int val = param.asInt();

  if ( led_timer_on_set[0] == 0 )
    led_set[0] = val;
  else
    update_blynk_status[0] = 1;
}

// #########################################################################################################
// LED 2
BLYNK_WRITE(V8)
{
  int val = param.asInt();

  if ( led_timer_on_set[1] == 0 )
    led_set[1] = val;
  else
    update_blynk_status[1] = 1;  
}

// #########################################################################################################
// Timer 1
BLYNK_WRITE(V9) 
{
  unsigned char week_day;
 
  TimeInputParam t(param);

  if (t.hasStartTime() && t.hasStopTime() ) 
  {
    timer_start_set[0] = (t.getStartHour() * 60 * 60) + (t.getStartMinute() * 60) + t.getStartSecond();
    timer_stop_set[0] = (t.getStopHour() * 60 * 60) + (t.getStopMinute() * 60) + t.getStopSecond();
    
    Serial.println(String("Start Time: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
                   
    Serial.println(String("Stop Time: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
                   
    for (int i = 1; i <= 7; i++) 
    {
      if (t.isWeekdaySelected(i)) 
      {
        week_day |= (0x01 << (i-1));
        Serial.println(String("Day ") + i + " is selected");
      }
      else
      {
        week_day &= (~(0x01 << (i-1)));
      }
    } 

    weekday_set[0] = week_day;
  }
  else
  {
    timer_start_set[0] = 0xFFFF;
    timer_stop_set[0] = 0xFFFF;
  }
}

// #########################################################################################################
// Timer 2
BLYNK_WRITE(V10) 
{
unsigned char week_day;
 
  TimeInputParam t(param);

  if (t.hasStartTime() && t.hasStopTime() ) 
  {
    timer_start_set[1] = (t.getStartHour() * 60 * 60) + (t.getStartMinute() * 60) + t.getStartSecond();
    timer_stop_set[1] = (t.getStopHour() * 60 * 60) + (t.getStopMinute() * 60) + t.getStopSecond();
    
    Serial.println(String("Start Time: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
                   
    Serial.println(String("Stop Time: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
                   
    for (int i = 1; i <= 7; i++) 
    {
      if (t.isWeekdaySelected(i)) 
      {
        week_day |= (0x01 << (i-1));
        Serial.println(String("Day ") + i + " is selected");
      }
      else
      {
        week_day &= (~(0x01 << (i-1)));
      }
    } 

    weekday_set[1] = week_day;
  }
  else
  {
    timer_start_set[1] = 0xFFFF;
    timer_stop_set[1] = 0xFFFF;
  }
}

// #########################################################################################################
BLYNK_WRITE(InternalPinRTC) 
{
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  unsigned long blynkTime = param.asLong(); 
  
  if (blynkTime >= DEFAULT_TIME) 
  {
    setTime(blynkTime);

    day_of_week = weekday();
  
    if ( day_of_week == 1 )
      day_of_week = 7;
    else
      day_of_week -= 1; 
    
    rtc_sec = (hour()*60*60) + (minute()*60) + second();
   
    Serial.println(blynkTime);
    Serial.println(String("RTC Server: ") + hour() + ":" + minute() + ":" + second());
    Serial.println(String("Day of Week: ") + weekday()); 
  }
}

// #########################################################################################################
BLYNK_CONNECTED() 
{
  Blynk.sendInternal("rtc", "sync"); 
}

// #########################################################################################################
void checkTime() 
{
  Blynk.sendInternal("rtc", "sync"); 
}

// #########################################################################################################
void led_mng()
{
  bool time_set_overflow;
  bool led_status_buf[2];
  
  for (int i=0; i<2; i++)
  {
    led_status_buf[i] = led_status[i];
    time_set_overflow = 0;
    
    if ( timer_start_set[i] != 0xFFFF && timer_stop_set[i] != 0xFFFF)
    {
      if ( timer_stop_set[i] < timer_start_set[i] ) time_set_overflow = 1;

      if ((((time_set_overflow == 0 && (rtc_sec >= timer_start_set[i]) && (rtc_sec < timer_stop_set[i])) ||
        (time_set_overflow  && ((rtc_sec >= timer_start_set[i]) || (rtc_sec < timer_stop_set[i])))) && 
        (weekday_set[i] == 0x00 || (weekday_set[i] & (0x01 << (day_of_week - 1) )))) )
        {
          led_timer_on_set[i] = 1;
        }
        else
          led_timer_on_set[i] = 0;
    }
    else
      led_timer_on_set[i] = 0;

    if ( led_timer_on_set[i] )
    {
      led_status[i] = 1;
      led_set[i] = 0;
    }
    else
    {
      led_status[i] = led_set[i];
    }

    if ( led_status_buf[i] != led_status[i] )
      update_blynk_status[i] = 1;  
  }
    // HARDWARE CONTROL
  digitalWrite(14, led_status[0]);  
  digitalWrite(12, led_status[1]);  
}

// #########################################################################################################
void blynk_update()
{
  if ( update_blynk_status[0] )
  {
      update_blynk_status[0] = 0;
      Blynk.virtualWrite(V7, led_status[0]);
  }  

  if ( update_blynk_status[1] )
  {
      update_blynk_status[1] = 0;
      Blynk.virtualWrite(V8, led_status[1]);
  } 
}

BLYNK_WRITE(V6)
{
  outVal = param.asInt(); // assigning incoming value from pin V1 to a variable
  dim_val = map(outVal,0, 50, 0, 50);
  dimmer.setPower(dim_val);
  Blynk.virtualWrite(V6, dim_val);
  Serial.print("V6 Slider value is: ");
  Serial.println(dim_val);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  timer.setInterval(10000L, checkTime);
  lcd.begin();
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
  digitalWrite(RELAY1,HIGH);
  Wire.begin();
  rtc.begin();
  if (! rtc.isrunning())
  {
  Serial.println("RTC is Not running");
  }
}

void loop() {
  DateTime now = rtc.now();
 // Serial.println(now,DEC);
 Serial.print(now.day());
 Serial.print("/");
 Serial.print(now.month());
 Serial.print("/");
 Serial.print(now.year());
 Serial.print(" ");
 Serial.print(now.hour());
 Serial.print(":");
 Serial.print(now.minute());
 Serial.print(":");
 Serial.println(now.second());
  delay(1000);


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
        Blynk.virtualWrite(V13, HIGH);
    } else {
        Blynk.virtualWrite(V13, LOW);
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
    
    lcd.setCursor(0,0);
  lcd.print("SMART HOME HANS");
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
  delay (1000);
  lcd.clear();
  delay (10);
  
    Serial.println();
    Blynk.virtualWrite(V0,power);
    Blynk.virtualWrite(V1,energy);
    Blynk.virtualWrite(V2,voltage);
    Blynk.virtualWrite(V3,current);
    Blynk.virtualWrite(V4,frequency);
    Blynk.virtualWrite(V5,pf);
    Blynk.run();
    
  timer.run();
  led_mng();
  blynk_update();
  
   if(current>=5) {
   digitalWrite(RELAY1,LOW);
 }
 
 biaya = energy * hargaperkwh ;
   Serial.print ("Harga:");
   Serial.print ("Rp ");
   Serial.print (biaya);
   Serial.println (",-");
   
    lcd2.clear();
   lcd2.print(0, 0, "Electricity Cost");
   lcd2.print(0, 1, "Rp ");
   lcd2.print(4, 1, biaya);
   lcd2.print(14, 1, ",-");

   if (now.day() == 30 && now.hour()==01 && now.minute() == 01)
{
pzem.resetEnergy();
}
}
