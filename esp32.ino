#include <Arduino.h>
#include <FS.h>

int buttonPin = 2;  // Pino de entrada do botão
int ledPin = 13;    // Pino de saída do LED
bool ledState = false;  // Estado atual do LED

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  if(SPIFFS.begin()){
    File file = SPIFFS.open("/ledState.txt", "r");
    if(file){
      ledState = file.readStringUntil('\n').toInt();
      file.close();
      digitalWrite(ledPin, ledState);
    }
  }
}

void loop() {
  if(digitalRead(buttonPin) == HIGH){
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    File file = SPIFFS.open("/ledState.txt", "w");
    if(file){
      file.println(ledState);
      file.close();
    }

    delay(500);  // Debounce para evitar múltiplos acionamentos
  }
}
