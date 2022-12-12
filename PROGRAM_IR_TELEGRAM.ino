#include <ESP8266WiFi.h> // pemanggilan fungsi wifi pada esp 8266
#include <WiFiClientSecure.h> // fungsi pemanggilan 8266
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h> // sama sprti java script; utk protokol transfer data
#define IR D3  
int val = 0; // nyimpan logika IR dimulai dgn 0 (low)

const char* ssid = "Pi Ware";
const char* password = "29528677";

#define BOTtoken "5976701885:AAF3i7FDXzjKmmpux790TNzWsTOUl2Ijl_U"  // your Bot Token (Get from Botfather)
#define CHAT_ID "1288913327"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT); // gk wajib ; fungsi pemanggilan utk tele
#endif

const int merah = D0; // pin led ; sama sprti define
const int hijau = D4;

WiFiClientSecure client; // wifi yg udah terhubung ke internet dipanggil oleh client
UniversalTelegramBot bot(BOTtoken, client); // tele terhubung melalui client ke wifi yg sudah terhubung internet
                       
int botRequestDelay = 50; // delay bot
unsigned long lastTimeBotRan; // variable kapan tele terakhir berjalan

// float ;lebih kecil
// int ; kecil
// long int ; sedang
// unsigned long ; besar
// unsigned log int ; lebih besar


//Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) { // void = fungsi baru
  Serial.println("handleNewMessages"); // menampilkan 
  Serial.println(String(numNewMessages)); 

  for (int i=0; i<numNewMessages; i++) { // variable utk memulai menerima pesan
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id); // mencocokkan chat_id dengan bot
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
           
    // Print the received message 
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;


    // perintah utk menjalankan di bot harus dengan (/)
    
    if (text == "/start") { 
      String welcome = "SELAMAT DATANG, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/BUKA_PINTU \n";
      bot.sendMessage(chat_id, welcome, "");
    } 
    
  if (text == "/BUKA_PINTU") {
  digitalWrite(hijau, LOW);
  bot.sendMessage(chat_id,"Pintu Di buka Menggunakan Perintah Telegram");
  delay(2000);
  digitalWrite(hijau, HIGH);
      }
    }   
 }
 
  


void setup()
{
  Serial.begin(115200);

  pinMode(merah, OUTPUT); // bisa dibuat ke relay, led dan selenoid door
  pinMode(hijau, OUTPUT); // bisa dibuat ke relay, led dan selenoid door
  pinMode(IR, INPUT);
  digitalWrite(merah, LOW);  
  digitalWrite(hijau, HIGH); 
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");          // get UTC time via NTP
    client.setTrustAnchors(&cert);             // Add root certificate for api.telegram.org
  #endif
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}


void loop()
{
val = digitalRead(IR); // read input value 
   if (val == HIGH)
   { // check if the input is HIGH
      digitalWrite(hijau, HIGH); // turn LED OFF   
       Serial.println("Infrared Tidak Mendeteksi");
   } 
   else 
   { 
      digitalWrite(hijau, LOW); // turn LED ON 
       Serial.println("Infrared  Mendeteksi");
       bot.sendMessage(CHAT_ID,"Pintu Di buka Menggunakan Infrared");
   } 


  
    if (millis() > lastTimeBotRan + botRequestDelay)  { // pengiriman pesan ke tele
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
