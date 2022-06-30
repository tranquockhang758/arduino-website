
#include <avr/io.h>
#include <avr/interrupt.h>

#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define R25 10000
#define beta 3950
#define T25 298.15
#define R1 10000
long int x1 = 0, x2 = 0, x3 = 0;
double Voltage_LM35_1,Temp_LM35_1;
double Voltage_LM35_2,Temp_LM35_2;
int Temp_Out_LM35_1;int Temp_Out_LM35_2;
//double RNTC,Temp_NTC,Vout;
String Data_Send = "";
long doam;
long cai1 = 100;
long cai2 = 500;
unsigned long last;
int main(void){
  adc_init();
  Temp_Out_LM35_1 = Temp_Out_LM35_2 = 0;
  Serial.begin(9600);
  Serial3.begin(115200);
  lcd.begin(16,2);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  while(1){    
      digitalWrite(13,HIGH);      
      Send_Data(Temp_Out_LM35_1 ,Temp_Out_LM35_2);
      digitalWrite(13,LOW);
  }
  return 0;
}


//===========================================================================Ngat ADC
void adc_init(void){
  ADMUX = 0x40;
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  SREG |= 0b10000000;
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADIE);
  ADCSRA |= 1<<ADSC;
//  sei();
}
ISR(ADC_vect){
  uint16_t x = ADCW;
  switch(ADMUX){
    case 0x40:
      x1 = x;
      Voltage_LM35_1 = x1 * 5.0 /1023.0;
      Temp_LM35_1 = Voltage_LM35_1 * 100;
      Temp_LM35_1 = Temp_LM35_1 *100;
      Temp_Out_LM35_1 = Temp_LM35_1;
      ADMUX = 0x41;
      break;
    case 0x41:
      x2 = x;
      Voltage_LM35_2 = x2 * 5.0 / 1023.0;
//      RNTC = R1 * ((5.0f/Vout) -1); 
//      Temp_NTC = (1.0f/(log(RNTC/R25)/beta + 1/T25) - 273.15);//log(x) tra ve ln(x)
      Temp_LM35_2 = Voltage_LM35_2 * 100;
      Temp_LM35_2 = Temp_LM35_2 *100;
      Temp_Out_LM35_2 = Temp_LM35_1;
      ADMUX = 0x40;
      break;
    default: 
      break;  
  }
  ADCSRA |= 1<<ADSC;
}
////=====================================================================================


//================================================Code truyen ?40a50b60c
void Send_Data(float Temp_Out_LM35_1 , float Temp_Out_LM35_2){
  Data_Send = '?' + String(Temp_Out_LM35_1) + 'a' + String(Temp_Out_LM35_2) + 'b';
  Serial3.println(Data_Send);//Gui du lieu di
  Serial.println(Data_Send);
  Serial3.flush();//Ham nay la ham de arduino gui du lieu xong moi duoc lam viec khac
}
