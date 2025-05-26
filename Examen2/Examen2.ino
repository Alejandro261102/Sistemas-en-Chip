#include <BluetoothSerial.h> 

BluetoothSerial SerialBT;

const int ledVerde = 25;
const int ledAmarillo = 33;
const int ledRojo = 32;
const int ledAzul = 26; // Supongo un cuarto LED, cambia el pin si es necesario
const int boton = 27;   // Asigna el pin correcto de tu botón

int contador = 0;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP-HUMEDAD");
  Serial.println("Esperando conexión Bluetooth...");

  pinMode(boton, INPUT_PULLUP);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
}

void loop() {
  if (!SerialBT.hasClient()) {
    Serial.println("Esperando conexión...");
    delay(1000);
    return;
  }

  bool buttonState = digitalRead(boton);

  // Detección  flanco descendente (presión de botón)
  if (lastButtonState == HIGH && buttonState == LOW) {
    contador++;
    if (contador > 4) contador = 1;

    String msg = "Boton presionado. Encendiendo " + String(contador) + " led(s)";
    Serial.println(msg);
    SerialBT.println(msg);
  }
  lastButtonState = buttonState;

  // Control de LEDs según el contador
  digitalWrite(ledVerde, contador >= 1 ? HIGH : LOW);
  digitalWrite(ledAmarillo, contador >= 2 ? HIGH : LOW);
  digitalWrite(ledRojo, contador >= 3 ? HIGH : LOW);
  digitalWrite(ledAzul, contador >= 4 ? HIGH : LOW);

  delay(50); // Antirrebote simple
}

