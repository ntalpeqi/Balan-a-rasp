#include "HX711.h"

HX711 cell(2, 3);
float tareValue = 0; // Variável para armazenar o valor da tara

void setup() {
  Serial.begin(9600);
}

long val = 0;
float count = 0;

void loop() {
  
  count = count + 1;
  
  // Use apenas um dos seguintes métodos de média
  //val = ((count-1)/count) * val    +  (1/count) * cell.read(); // média de longo prazo
  val = 0.5 * val    +   0.5 * cell.read(); // média recente
  //val = cell.read(); // leitura mais recente
  //val = cell.get_value(15);
  
  // Subtrair o valor da tara da leitura atual
  float weight = ((val - 8119523) / -58087.0f * 4) - tareValue;
  Serial.println(weight, 3); // Imprimir o peso

  if (Serial.available() > 0) {
    String input = Serial.readString(); // Ler a entrada do Serial Monitor
    input.trim(); // Remover espaços em branco extras
    
    if (input.equalsIgnoreCase("tara")) {
      // Se a entrada for "tara", definir o valor da tara como o valor atual da leitura
      tareValue = ((val - 8119523) / -58087.0f * 4);
    }
  }
}
