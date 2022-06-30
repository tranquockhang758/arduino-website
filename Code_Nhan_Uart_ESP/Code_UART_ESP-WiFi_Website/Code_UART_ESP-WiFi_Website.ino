
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D5,D6);//RX,TX
String str = "";         
bool stringComplete = false;  

//===========================================Bien de hung gia tri doc ve tu serial
int s =0;//Bien de de lay do dai gia tri cua cam bien
int nghin,tram,chuc,donvi;
String Analog_LM35_1,Analog_LM35_2;
int Temp_LM35_1,Temp_LM35_2;
int Index1,Indexa,Indexb;
unsigned long last;
//===========================================


void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  str.reserve(200);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
}

void loop() {
  
  //Neu co ket noi WiFi ta doc du lieu gui qua tu Mega, Led o day de bao cho ta biet viec truyen du lieu va dua len server da ket thuc
  //  digitalWrite(13,HIGH);
  //  Read_Sensor_Data();
  //  Du lieu gui len server theo cau truc duong link sau :
  //  http://localhost:8080/unitop/PHP/back-end/lesson/projectmvc.vn/?mod=search&controller=index&action=get_product
  //  data = "&t1=" + String(Analog_LM35) + "&t2" + String(Analog_PT100);
  //  String url = "http://192.168.1.190:8080/projectmvc.vn/?mod=IOT_Scada&controller=index&action=index" + data;
  //  digitalWrite(13,LOW);
    digitalWrite(D1,HIGH);
    Read_Sensor_Data();
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
        delay(100);
}
void Read_Sensor_Data(){
  while(mySerial.available() >0) {
    // get the new byte:
    char inChar = (char)mySerial.read();   
    str += inChar;
    if (inChar == '\n') {
        stringComplete = true;
    }    
    }  
}
