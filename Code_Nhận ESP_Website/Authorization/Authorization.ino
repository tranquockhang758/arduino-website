//============================================================Thu vien ESP Ket noi Web
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
//============================================================Thu vien ESP Ket noi Web


//============================================================Bien de hung gia tri doc ve tu serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D5,D6);//RX,TX
int s =0;//Bien de de lay do dai gia tri cua cam bien
int nghin,tram,chuc,donvi;
String Analog_LM35_1,Analog_LM35_2;
int Temp_LM35_1,Temp_LM35_2;
int Index1,Indexa,Indexb;
unsigned long last;
//============================================================




//=============================================================Bien de doc UART
String str = "";         
bool stringComplete = false;  
String data = "";
int t1 ,t2,t3;
String url = "";
//==============================================================



void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  str.reserve(200);
  t1 = t2 = t3 = 30;

  
  //===========================================Led bao truyen-nhan
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  //===========================================Led bao truyen-nhan

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Ut", "tritam1615");

}

void loop() { 
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    
    //====================================================Doc va tach du lieu UART
        Read_Sensor_Data()
        digitalWrite(D1,LOW);
        if(stringComplete){
            Index1 = str.indexOf("?");Indexa = str.indexOf("a");Indexb = str.indexOf("b");
            Analog_LM35_1 = str.substring(Index1 + 1,Indexa);
            Temp_LM35_1 = Analog_LM35_1.toInt();
            Analog_LM35_2 = str.substring(Indexa+1,Indexb);
            Temp_LM35_2 = Analog_LM35_2.toInt();
            Serial.print("Nhiet do 1:");
            Serial.println(Temp_LM35_1);
            Serial.print("Nhiet do 2 la:");
            Serial.println(Temp_LM35_2);
            stringComplete = false;
            str = "";
        }
        digitalWrite(D1,LOW);
   //====================================================Doc va tach du lieu UART

   
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    data = "&t1=" + String(Temp_LM35_1) + "&t2=" + String(Temp_LM35_2);
    
    url = "http://192.168.1.190:8080/projectmvc_Ismart.vn/?mod=IOT_Scada&controller=index&action=index"+String(data);
    Serial.println(url);
    
      if(http.begin(client,url)){
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          Serial.print("[HTTP] GET...\n");
          int httpCode = http.GET();
          // Neu ket noi duoc voi Server
            if (httpCode > 0) {
              //Neu nhan duoc phan hoi tu server       
              Serial.printf("[HTTP] GET... code: %d\n", httpCode);
              // file found at server
              if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                  String payload = http.getString();
                  Serial.println(payload);  
                  //Ki tu 0 la quy dinh cua C
                  //int I00 = payload[  payload.indexOf("I00")+6] -'0';
              }
            }
          //=====================================Neu khong ket noi duoc voi server  httpCode<0
            else{
              Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }    
            http.end();
      }
      else{
        Serial.println("Unable to connect Server\n");
      }
    }
    delay(10000);    
}

void Read_Sensor_Data(){
  while(mySerial.available()>0){
    char inChar = (char)mySerial.read();   
    str += inChar;
    if (inChar == '\n') {
        stringComplete = true;
    }   
  }
}
