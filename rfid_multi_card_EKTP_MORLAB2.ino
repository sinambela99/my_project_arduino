#include <RFID.h> // kartu 
#include <SPI.h> // komunikasi serial periperal interface (microcontroller dan tele)
#include <ESP8266WiFi.h> // pemanggilan fungsi wifi pada esp 8266
#include <WiFiClientSecure.h> // fungsi pemanggilan 8266
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h> // sama sprti java script; utk protokol transfer data
#define IR D3 
int val = 0;

const char* ssid = "Apa";
const char* password = "moonimoon";

#define BOTtoken "5976701885:AAF3i7FDXzjKmmpux790TNzWsTOUl2Ijl_U"  // your Bot Token (Get from Botfather)
#define CHAT_ID "1288913327"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT); // gk wajib ; fungsi pemanggilan utk tele
#endif

const int merah = D0; // pin led ; sama sprti define
const int relay = D4;

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
  digitalWrite(relay, LOW);
  bot.sendMessage(chat_id,"Pintu Di buka Menggunakan Perintah Telegram");
  delay(2000);
  digitalWrite(relay, HIGH);
      }
    }   
  }
  

#define SS_PIN D2// sda pin rfid
#define RST_PIN D1

RFID rfid(SS_PIN,RST_PIN);

int serNum[5]; // serial number itu untuk menyimpan identitas dari kartu RFID yang digunakan sebanyak [5]

int card1[][5] =
{
  {115,30,207,46,140} // isi alamat card
};

int card2[][5] =
{
 {136,5,142,39,36} // isi alamat card 
};

int card3[][5] =
{
  {188,30,249,129,218}// isi alamat card
};


bool access_ = false; // jika benar, pintu akan terbuka; jika salah pintu tidak akan terbuka

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  rfid.init();
  
  pinMode(merah, OUTPUT); // bisa dibuat ke relay, led dan selenoid door
  pinMode(relay, OUTPUT); // bisa dibuat ke relay, led dan selenoid door
  pinMode(IR, INPUT);
  digitalWrite(merah, LOW);  
  digitalWrite(relay, HIGH); 
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
if(rfid.isCard())
  {
  if(rfid.readCardSerial()) // membaca serial number
  {
    Serial.print(rfid.serNum[0]);
    Serial.print(" ");
    Serial.print(rfid.serNum[1]);
    Serial.print(" ");
    Serial.print(rfid.serNum[2]);
    Serial.print(" ");
    Serial.print(rfid.serNum[3]);
    Serial.print(" ");
    Serial.print(rfid.serNum[4]);
    Serial.print(" ");

//--------------------card 1----------------------
      for(int x =0; x< sizeof (card1); x++) // awal code check card 1
      {
        for(int i = 0; i<sizeof(rfid.serNum); i++)
        {
          if(rfid.serNum[i] != card1 [x] [i])
          {
            access_ = false;
            break;
          }
          else{
            access_ = true;
            bot.sendMessage(CHAT_ID,"PINTU TERBUKA MENGGUNAKAN KARTU KARTU RFID");
          }
        }
      if (access_) break;
      } // untuk card satu

//---------------------card 2---------------------------
      for(int x =0; x< sizeof (card2); x++) // awal code check card 2
      {
        for(int i = 0; i<sizeof(rfid.serNum); i++)
        {
          if(rfid.serNum[i] != card2 [x] [i])
          {
            access_ = false;
            break;
          }
          else{
            access_ = true;
            bot.sendMessage(CHAT_ID,"PINTU TERBUKA MENGGUNAKAN KARTU KTP");
          }
        }
      if(access_) break;
      } // untuk card 2

//----------------------card 3----------------------------
     for(int x =0; x< sizeof (card3); x++) // awal code check card 3
      {
        for(int i = 0; i<sizeof(rfid.serNum); i++)
        {
          if(rfid.serNum[i] != card3 [x] [i])
          {
            access_ = false;
            break;
          }
          else{
            access_ = true;
            bot.sendMessage(CHAT_ID,"PINTU TERBUKA MENGGUNAKAN KARTU RFID");
          }
        }
      if(access_) break;
      } // untuk card 3
//--------------------------------------------------------------     
  }

if (access_) 
{
  Serial.println("BERHASIL");
  bot.sendMessage (CHAT_ID, "PINTU TERBUKA MENGGUNAKAN TELEGRAM"); 
  digitalWrite(relay,LOW);
  delay(2000);
  digitalWrite(relay,HIGH);
}

else 
{
  Serial.println("card tidak terdaftar");
  bot.sendMessage (CHAT_ID, "PINTU TIDAK TERBUKA");
  digitalWrite(merah, HIGH);
  delay(500);
  digitalWrite(merah, LOW);
  delay(500);
  digitalWrite(merah, HIGH);
  delay(500);
  digitalWrite(merah, LOW);
}
}
  rfid.halt(); // RFID selesai bekerja
  
val = digitalRead(IR); // read input value 
   if (val == HIGH)
   { // check if the input is HIGH
      digitalWrite(relay, HIGH); // turn LED OFF   
       Serial.println("Infrared Tidak Mendeteksi");
   } 
   else 
   { 
      digitalWrite(relay, LOW); // turn LED ON 
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
