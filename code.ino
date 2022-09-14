 #include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

const char* ssid = "AndroidAP90D0";
const char* password = "sindhuri";

int led = 14;
int ir = 12;
int c=0;
int h = 0;
int m=0;
int s=0;
int val=0;
int timer=0;
int timer1=0;

const char* host = "maker.ifttt.com";

void setup()
{
    Serial.begin(115200);
    Serial.println("Email from Node Mcu");
    delay(100);
    delay(1000);
    pinMode(led, OUTPUT);
    connectWiFi();
    pinMode (ir, INPUT); // sensor pin INPUT
    lcd.begin(16,2);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Capsule Remainder");
    delay(5000);
}

void loop()
{  
      while((!(WiFi.status() == WL_CONNECTED)))
      {
          connectWiFi();
      }              
      WiFiClient client;
      const int httpPort = 80;  
      if (!client.connect(host, httpPort))
      {  
         Serial.println("connection failed");  
         return;
      }
      if (Serial.available() > 0 && c!=3) {
          val = Serial.parseInt();
          delay(5);
          if(c==0)
          {
            h=val;
            c=c+1;
          }
          else if(c==1)
          {
              m=val;
              c=c+1;
          }
          else if(c==2)
          {
              s=val;
              c=c+1;
          }
      }
      else if(c==3)
      {
          timer=(((h*60)+m)*60)+s;
          Serial.println(timer);
          while(timer!=0)
          {
              timer=timer-1;
              delay(1000);
          }
          if(timer==0)
          {
            lcd.clear();
            lcd.setCursor(3,0);
            lcd.print("Take Medicine");
            digitalWrite(led, 1);
            String url = "/trigger/ESP_CALL/json/with/key/d9G-bvcycZGa5-DIMzHUe-";
            client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
            delay(1000);
            while(true)
            {
              int statusSensor = digitalRead(ir);
              Serial.println(statusSensor);
              if (statusSensor == 1){
                  digitalWrite(led, 0); // LED LOW
                  break;
              }  
            }
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Close Pill Box");
            while(true)
            {
              int statusSensor = digitalRead(ir);
              Serial.println(statusSensor);
              if (statusSensor == 0){
                  digitalWrite(led, 0); // LED LOW
                  break;
              }
            }  
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Capsule Remainder");
          }
      }
      while((!(WiFi.status() == WL_CONNECTED)))
      {
        connectWiFi();
      }              
}
void connectWiFi()
{
  int i=0;
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while((!(WiFi.status() == WL_CONNECTED)))
  {
    i++;
    delay(300);
    if( i>10 )    
    {
     connectWiFi();
    }
   }  
   Serial.println("");
   Serial.println("WiFi connected");
}