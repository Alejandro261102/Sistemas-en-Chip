#include <BluetoothSerial.h> 
#include "DHT.h"

#define DHTPIN 14
#define DHTTYPE DHT11

BluetoothSerial SerialBT;

const int sensorPin = 36; // Pin del sensor FC-28

// Pines de los LEDs
const int ledVerde = 25;
const int ledAmarillo = 33;
const int ledRojo = 32;

// Valores del sensor FC-28 calibrados
const int valorSeco = 4095;   // Sensor en seco
const int valorHumedo = 1000; // Sensor en agua

DHT dht(DHTPIN, DHTTYPE);

// Variables para suavizado exponencial
float valorFiltrado = 0;
const float alpha = 0.1; // Entre 0.01 y 0.3 según qué tan suave quieres el filtro

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP-HUMEDAD");
  Serial.println("Esperando conexión Bluetooth...");

  dht.begin();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
}

void loop() {
  if (!SerialBT.hasClient()) {
    Serial.println("Esperando conexión...");
    delay(1000);
    return;
  }

  // Lectura y ajuste del DHT11
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Error leyendo del DHT11");
    return;
  }
  h = constrain(h, 0.0, 100.0); // Asegura que esté entre 0 y 100

  Serial.print("Humedad ambiental calibrada: ");
  Serial.println(h);

  // Lectura del sensor FC-28 con suavizado exponencial
  int lecturaActual = analogRead(sensorPin);
  valorFiltrado = alpha * lecturaActual + (1 - alpha) * valorFiltrado;

  // Cálculo del porcentaje de humedad del suelo
  float humedadSuelo = ((float)(valorSeco - valorFiltrado) / (valorSeco - valorHumedo)) * 100.0;
  humedadSuelo = constrain(humedadSuelo, 0.0, 100.0);

  // Envío por Bluetooth
  SerialBT.print("*A");
  SerialBT.print(h);
  SerialBT.print("*");
  SerialBT.print("*H");
  SerialBT.print(humedadSuelo, 2);
  SerialBT.print("*");

  // Envío por monitor serial (opcional)
  Serial.print("Valor filtrado FC-28: ");
  Serial.println((int)valorFiltrado);
  Serial.print("Humedad del suelo (%): ");
  Serial.println(humedadSuelo, 2);

  // Semáforo según humedad del suelo
  if (humedadSuelo >= 0 && humedadSuelo <= 60) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, HIGH);
  } else if (humedadSuelo > 60 && humedadSuelo <= 70) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarillo, HIGH);
    digitalWrite(ledRojo, LOW);
  } else if (humedadSuelo > 70 && humedadSuelo <= 80) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, LOW);
  } else if (humedadSuelo > 90 && humedadSuelo <= 100) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, HIGH);
  } else if (humedadSuelo > 80 && humedadSuelo <= 90) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarillo, HIGH);
    digitalWrite(ledRojo, LOW);
  }

  delay(500);
}

