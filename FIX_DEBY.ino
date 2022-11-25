#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#define trig D7//membuat triger pin ke kaki 3 arduino
#define echo D8//membuat echo pin ke kaki 4 arduino

long durasi;///membuat variabel durasi
int ketinggian_air;//membuat variabel jarak
int ketinggian_wadah = 150;
int level;

int relay= D0;//deklarasikan relay agar relay berfungsi

int LED1 = D3;//deklarasikan led agar led berfungsi
int LED2 = D4; 
int LED3 = D5; 
int LED4 = D6; 

const char* ssid = "Pi Ware";   // your network SSID (name) 
const char* password = "29528677";   // your network password


WiFiClient  client;

unsigned long myChannelNumber = 1952592;
const char * myWriteAPIKey = "11X67ZSB92KHFY7B";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

#define BOTtoken "5895515017:AAE1fxJVcZsXPFOOmcxWiYzSloSsTVIblrk"  // your Bot Token (Get from Botfather)
#define CHAT_ID "-1001575338555" 

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client2;
UniversalTelegramBot bot(BOTtoken, client2);
                       
int botRequestDelay = 500;
unsigned long lastTimeBotRan;

       

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
           
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

        
    if (text == "/start") {
      String welcome = "SELAMAT DATANG, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/STATUS_KETINGGIAN_AIR UNTUK MEMERIKSA KEADAAN KETINGGIAN AIR\n";
      welcome += "Informasi terkait level air Level 1 = 1cm - 50cm, Level 2 = 51cm -100cm, Level 3 = 101cm -150cm, Level 4 = 151cm -200cm \n";
      bot.sendMessage(chat_id, welcome, "");
    }
        
 if (text == "/STATUS_KETINGGIAN_AIR") {
  digitalWrite(trig, LOW);//set trigger pin bernilai low 
  delayMicroseconds(5);//delay dalam microsecond sebesar 5 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  durasi = pulseIn(echo, HIGH);// membuat fungsi durasi sebagai pulsa yang masuk dengan nilai echo sebagai high
  ketinggian_air = ketinggian_wadah-durasi * 0.034 / 2 ; //rumus ketentuan jarak
  delay(500);//delay dalam 1 detik akan ditampilkan hasil dari variabel jarak
 // Serial.print("jarak = ");
//  Serial.print(jarak);//memunculkan dalam komunikasi serial nilai dari jarak dalam bentuk cm
//  Serial.println(" cm");


      String hasilkair;
      hasilkair =   (String) "Ketinggian Air: " + ketinggian_air;
                    hasilkair += " cm"; 
                    bot.sendMessage(chat_id,hasilkair, "");
        ///////
        
        if ((ketinggian_air >= 101 )&&(ketinggian_air = 1)){
        Serial.println("Ketinggian Air : Level 3"); 
        bot.sendMessage(chat_id,"Status Ketinggian :  Level 3");
        }
        
        else if ((ketinggian_air >= 51)&&(ketinggian_air = 100)){
        Serial.println("Ketinggian Air : Level 2"); 
        bot.sendMessage(chat_id,"Status Ketinggian :  Level 2");
        }

        else if ((ketinggian_air >= 1)&&(ketinggian_air = 50)){
        Serial.println("Ketinggian Air : Level 1"); 
        bot.sendMessage(chat_id,"Status Ketinggian :  Level 1");
        }
      
    }

  }

 }


void setup() {
  Serial.begin(115200);  //Initialize serial
 
  pinMode(relay, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    
    pinMode(trig, OUTPUT);//set pin tigger sebagai output sensor
    pinMode(echo, INPUT);// set pin echo sebagai input sensor

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");          // get UTC time via NTP
    client2.setTrustAnchors(&cert);             // Add root certificate for api.telegram.org
  #endif
    
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
      // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
        
      } 
      Serial.println("\nConnected.");
    }
      Serial.println(WiFi.localIP());
}

void loop() {
  
    
  if ((millis() - lastTime) > timerDelay) {

    // Get a new temperature reading
  digitalWrite(trig, LOW);//set trigger pin bernilai low 
  delayMicroseconds(5);//delay dalam microsecond sebesar 5 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  durasi = pulseIn(echo, HIGH);// membuat fungsi durasi sebagai pulsa yang masuk dengan nilai echo sebagai high
  ketinggian_air = ketinggian_wadah-durasi * 0.034 / 2; //rumus ketentuan jarak
  delay(500);//delay dalam 1 detik akan ditampilkan hasil dari variabel jarak
  Serial.print("ketinggian_air = ");
  Serial.print(ketinggian_air);//memunculkan dalam komunikasi serial nilai dari jarak dalam bentuk cm
  Serial.println(" cm");
  
   
        
    int x = ThingSpeak.writeField(myChannelNumber, 1, ketinggian_air, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
       
    lastTime = millis();
  }
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
