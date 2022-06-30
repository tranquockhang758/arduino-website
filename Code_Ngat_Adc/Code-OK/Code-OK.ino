#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <avr/io.h>
#include <avr/interrupt.h>
#define R25 10000
#define beta 3950
#define T25 298.15
#define R1 10000
long int x1 = 0, x2 = 0, x3 = 0;
double Voltage_LM35,Temp_LM35;
double RNTC,Temp_NTC,Vout;
int main(void){
  adc_init();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("hello, world!");
  while(1){
      Serial.print("Temp_LM35:");
      Serial.print(Temp_LM35);
      Serial.println(" Do C"); 
      Serial.print("Temp_NTC:");
      Serial.print(Temp_NTC);
      Serial.println(" Do C"); 

  }
  return 0;
}
//float Thermistor(int Vo) { 
//  logRt = log(10000.0*((1024.0/Vo-1))); 
//  T = (1.0 / (A + B*logRt + C*logRt*logRt*logRt)); 
//}
// We get the temperature value in Kelvin from this Stein-Hart equation Tc = T - 273.15; 
// Convert Kelvin to Celcius Tf = (Tc * 1.8) + 32.0; // Convert Kelvin to Fahrenheit return T; } 
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
      Voltage_LM35 = x1 * 5.0 /1023.0;
      Temp_LM35 = Voltage_LM35 * 100.0;
      ADMUX = 0x41;
      break;
    case 0x41:
      x2 = x;
      Vout = x2 * 5.0 / 1023.0;
      RNTC = R1 * ((5.0f/Vout) -1); 
      Temp_NTC = (1.0f/(log(RNTC/R25)/beta + 1/T25) - 273.15);//log(x) tra ve ln(x)
      ADMUX = 0x40;
      break;
    default: 
      break;  
  }
  ADCSRA |= 1<<ADSC;
}
