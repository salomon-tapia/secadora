#include <HX711.h>
#include <max6675.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Ticker.h>
#include <Servo.h>

#define PIN_TRIG 2
#define PIN_ECHO 3
#define PIN_POT A2
#define PIN_LED 4
#define PIN_MOT 5

Servo mot;
MAX6675 thermocouple(5, 6, 7);
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 balanza;

int value_led=0;
int value_mot=0;
float value_pot= -1.0;

void fnDistancia(){
  float pesoF;
  //distancia = getDistance(PIN_TRIG,PIN_ECHO);
  //distancia =analogRead(PIN_POT);
  float peso = balanza.get_units(20) - 1.65;
  pesoF = (int)peso;
  if (pesoF != -1){
    Serial.println("pes:" + String(pesoF,0));    
  }  
}
Ticker ticDistancia( fnDistancia,1000);

void fnPotenc(){
  float temF;
  int pot = analogRead(PIN_POT);
  //temF = thermocouple.readCelsius();
  //Serial.println("potAntes: " + String(pot));  
  if (pot != value_pot){
    value_pot = pot;
    Serial.println("tem:" + String(value_pot,0));  
  }  
}
Ticker ticPotenc( fnPotenc,500);

void fnActuadores(String cad){
  int pos;
  String label,value;
  cad.trim();
  cad.toLowerCase();
  pos = cad.indexOf(':');
  label= cad.substring(0,pos);
  value= cad.substring(pos+1);

  if (value_mot != label.toInt()){
    value_mot = label.toInt();
    //Serial.println("tem:" + );
    lcd.setCursor(0,0);
    lcd.print("Tempe: " + String(value_mot));
  }
  
  
  lcd.setCursor(0,1);
  lcd.print("Tiempo: " + value);
  //Serial.println(cad);
/*
  if (label.equals("mot")){
    if(value_mot != value.toInt()){
      value_mot = value.toInt();  
      //mot.write(value_mot); 
      lcd.setCursor(0,1);
      //lcd.print("Temp: " + (String)value_mot);
    }    
  }
  
  if (label.equals("led")){
    if(value_led != value.toInt()){
      value_led = value.toInt();  
      //digitalWrite(PIN_LED,value_led);
      lcd.setCursor(0,1);
      //lcd.print("led" + (String)value_led);
    }
  }
  */
}



void setup() {
  Serial.begin(9600);
  delay(30);
  // config pines
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED, OUTPUT);
  mot.attach(PIN_MOT);

  //Initial values
  digitalWrite(PIN_LED,value_led);
  mot.write(value_mot);

  balanza.begin(A1, A0);
  balanza.set_scale(-414);
  balanza.tare(20);

  lcd.init();
  lcd.backlight();
  lcd.print("Tkinter");

  ticDistancia.start();
  ticPotenc.start();
}



void loop() {
  ticDistancia.update();
  ticPotenc.update();
  /*
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: " + (String)ktc.readCelsius());
  delay(500);   */
  if(Serial.available()){
    fnActuadores(Serial.readString());
  }
}