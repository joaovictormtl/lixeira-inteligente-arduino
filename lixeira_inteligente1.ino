#include <LiquidCrystal.h>
#include <Servo.h>

// Display de Boas-Vindas
LiquidCrystal lcd(11, 6, 8, 5, 4, 2);

// Sensor de Abertura/Fechamento
#define trigAf 10
#define echoAf 9

// Sensor de Nível
#define trigN 13
#define echoN 12

// Distância para abertura
#define distancia_abertura 80

// Altura mínima para considerar cheia
const int limite_cheia = 15;

// LED de alerta
#define ledLixeiraCheia 7

// Buzzer de alerta
#define buzzer 3

Servo servo;
long cmDistancia, cmNivel;

void setup() {
  // quantidade linhas e colunas a serem utilizadas pelo display
  lcd.begin(16, 2); 
  
  // limpar a tela do display
  lcd.clear();
  
  // Configuração dos pinos
  pinMode(trigAf, OUTPUT);
  pinMode(echoAf, INPUT);
  pinMode(trigN, OUTPUT);
  pinMode(echoN, INPUT);
  pinMode(ledLixeiraCheia, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  servo.attach(A0);
  servo.write(0); // Inicia fechada
  Serial.begin(9600);
}

long lerSensor(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long microsec = pulseIn(echo, HIGH);
  return microsec / 58; // Convertendo para cm
}

void loop() {
  // Saudação
    lcd.setCursor(4, 0);
    lcd.print("Bom Dia!");
  
  // Lê sensor de abertura/fechamento
  cmDistancia = lerSensor(trigAf, echoAf);
  
  Serial.print("Distancia: ");
  Serial.print(cmDistancia);
  Serial.println(" cm");

  // Controle da tampa
  if(cmDistancia <= distancia_abertura) {
    Serial.println("Abrindo Lixeira");
    servo.write(90);
    delay(3000);
  } else {
    Serial.println("Fechando Lixeira");
    servo.write(0);
  }

  // Lê sensor de nível (com pequeno delay para evitar interferência)
  delay(50);
  cmNivel = lerSensor(trigN, echoN);
  
  Serial.print("Altura do lixo: ");
  Serial.print(cmNivel);
  Serial.println(" cm");

  // Controle do LED
  if(cmNivel <= limite_cheia) {
    Serial.println("A Lixeira esta cheia!");
    digitalWrite(ledLixeiraCheia, HIGH);
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(ledLixeiraCheia, LOW);
    digitalWrite(buzzer, LOW);
  }

  delay(100);
}