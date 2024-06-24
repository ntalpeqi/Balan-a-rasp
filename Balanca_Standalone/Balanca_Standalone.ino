#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <TMRpcm.h>
#include "HX711.h"

#define DOUT 2  // Define o pino DOUT do módulo HX711
#define CLK 3   // Define o pino CLK do módulo HX711
#define BUTTON_PIN 4  // Pino do botão
#define SD_CS_PIN 10  // Pino CS do módulo SD
#define SPEAKER_PIN 9  // Pino conectado ao PAM8403

HX711 cell(DOUT, CLK);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inicializa o LCD I2C

TMRpcm tmrpcm;  // Inicializa o módulo de reprodução de áudio

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Falha ao inicializar o cartão SD!");
    return;
  }

  tmrpcm.speakerPin = SPEAKER_PIN;
}

void loop() 
{
  if (digitalRead(BUTTON_PIN) == LOW) {
    float peso = lerPeso();  // Leitura do sensor de peso
    displayLCD(peso);
    reproduzirAudio(peso);
    imprimirNoSerial(peso);
    delay(1000);  // Evitar leituras rápidas e confusas
  }
}

float lerPeso() {
  long val = cell.get_value(10);
  return (val - 8399085) / 188192.0f * 16;

}

void displayLCD(float peso) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Peso: ");
  lcd.print(peso, 2);  // Exibe o peso com 2 casas decimais
  lcd.setCursor(0, 1);
  lcd.print("Gramas");
  Serial.println(peso); // Exibe o peso no monitor serial
}

void reproduzirAudio(float peso) {
  int parteInteira = (int)peso;
  int parteDecimal = int((peso - parteInteira) * 100);

  reproduzirNumero(parteInteira);
  tmrpcm.play("virgula.wav");
  reproduzirNumero(parteDecimal);
  tmrpcm.play("gramas.wav");
}

void reproduzirNumero(int numero) {
  char nomeArquivo[20];
  sprintf(nomeArquivo, "%d.wav", numero);
  tmrpcm.play(nomeArquivo);
}

void imprimirNoSerial(float peso) {
  Serial.print("Peso: ");
  Serial.println(peso, 2);
}
