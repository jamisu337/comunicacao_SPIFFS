// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <MFRC522.h>

// --- Mapeamento de Hardware ---
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define led_verde 8
#define led_vermelho 7
#define buzzer 6

// --- Variáveis Globais --- 
int tantativas = 0;

// --- Configurações Iniciais ---
void setup() {
  Serial.begin(9600);   // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522

  // --- componentes ---
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

} //end setup


// --- Loop Infinito ---
void loop() {
  
  // Verifica novos cartões
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Seleciona um dos cartões
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String conteudo = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  conteudo.toUpperCase();

  if (check_Tag(conteudo)) {
    Acesso_valido();
  } else {
    Acesso_invalido();
  }
}

bool check_Tag(String tag) {
  // Verifica a tag RFID cadastrada
  if (tag == "86 D0 17 7E") {
    return true;
  } else {
    return false;
  }
}

void Acesso_valido() {
  Serial.println("Acesso liberado!");
  digitalWrite(led_vermelho, LOW); // Desliga LED vermelho
  digitalWrite(led_verde, HIGH);   // Liga LED verde
  delay(10000);                    // Mantém LED verde ligado por 10 segundos
  digitalWrite(led_verde, LOW);    // Desliga LED verde
}

void Acesso_invalido() {
  Serial.println("Cartao invalido");
  tantativas++;

  if (tantativas >= 5) {
    sistema_block();
  } else {
    for (int i = 0; i < 3; i++) {
      digitalWrite(led_vermelho, HIGH); // Liga LED vermelho
      delay(500);
      digitalWrite(led_vermelho, LOW);  // Desliga LED vermelho
      delay(500);
    }
  }
}

void sistema_block() {
  Serial.println("SISTEMA BLOQUEADO");
  digitalWrite(led_vermelho, HIGH); // Liga LED vermelho
  tone(buzzer, 1000);               // Aciona o buzzer a 1000 Hz
  delay(30000);                     // Mantém buzzer ligado por 30 segundos
  noTone(buzzer);                   // Desliga o buzzer
  digitalWrite(led_vermelho, LOW);  // Desliga LED vermelho
  tantativas = 0;                   // Reinicia contagem de tentativas
}
