#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

HX711 cell(12,13);

const int buttonPin = 2;     // the number of the pushbutton pin for printing the weight
const int tarePin = 3;       // the number of the pushbutton pin for taring the scale
int buttonState = 0;         // variable for reading the pushbutton status
int tareState = 0;           // variable for reading the tare button status
float peso;
long val = 0;
float count = 0;
float tareOffset = 0.0;      // variable for storing the tare offset
String fala;

void setup() {
   pinMode(buttonPin, INPUT);
   pinMode(tarePin, INPUT);
   Serial.begin(9600);
   mySerial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("LPEQI - BALANCA");
  delay (150);

}

void ponto_para_virgula(String texto){
  texto.replace(".",",");
}

void loop() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("LPEQI - BALANCA");
  Serial.print("LPEQI - BALANCA");
  lcd.setCursor(0, 1);
  lcd.print(String(peso - tareOffset) + " g");
  Serial.print(String(peso - tareOffset) + " g");

  buttonState = digitalRead(buttonPin);
  tareState = digitalRead(tarePin);

  if (tareState == LOW) {
    tareOffset = peso;
    delay(200); // debounce
  }

  count = count + 1;
  delay(500);

  // Use only one of these
  //val = ((count-1)/count) * val    +  (1/count) * cell.read(); // take long term average
  //val = 0.5 * val    +   0.5 * cell.read(); // take recent average
  //peso =0.0;
  //val = cell.read(); // most recent reading
  val = cell.get_value(5);
  peso = ((val-8277610)/-396307.0f*226);

  
  // String fala; = String(peso)+" gramas";
  if (buttonState == LOW) {
    fala = String(String(peso - tareOffset) + " gramas");
    fala.replace(".",",");
    Serial.println(fala);
    mySerial.println(fala);
    delay(200);
  }
}
