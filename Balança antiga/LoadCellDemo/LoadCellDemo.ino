#include "HX711.h"

HX711 cell(2, 3);

void setup() {
  Serial.begin(9600);
}

long val = 0;
float count = 0;

void loop() {
  count = count + 1;
  
  // Use only one of these
  //val = ((count-1)/count) * val    +  (1/count) * cell.read(); // take long term average
  //val = 0.5 * val    +   0.5 * cell.read(); // take recent average
  //val = cell.read(); // most recent reading
  val = cell.get_value (10);
  
  Serial.println (((val-8119523)/-58087.0f*4),4);
}
