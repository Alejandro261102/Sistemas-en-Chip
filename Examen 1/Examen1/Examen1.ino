// Definición de pines en la ESP32
const int niveles[] = {25, 26, 27};  // Pines que controlan las capas (niveles)
const int columnas[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};  // Pines para las columnas de LEDs
const int LDR = 34;  // Pin para sensor LDR

// Matriz 3x3x3 para representar el estado de los LEDs (1 = encendido, 0 = apagado)
int cubo[3][3][3] = {0};

void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(niveles[i], OUTPUT);
    digitalWrite(niveles[i], LOW);
  }
  for (int i = 0; i < 9; i++) {
    pinMode(columnas[i], OUTPUT);
    digitalWrite(columnas[i], LOW);
  }
}

void mostrarCubo(int tiempo) {
  unsigned long start = millis();
  while (millis() - start < tiempo) {
    for (int z = 0; z < 3; z++) {
      digitalWrite(niveles[z], HIGH);
      for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
          int index = y * 3 + x;
          digitalWrite(columnas[index], cubo[z][y][x] ? HIGH : LOW);
        }
      }
      delay(2);
      digitalWrite(niveles[z], LOW);
    }
  }
}

void apagarCubo() {
  for (int z = 0; z < 3; z++)
    for (int y = 0; y < 3; y++)
      for (int x = 0; x < 3; x++)
        cubo[z][y][x] = 0;
}

int leerLDR() {
  int lectura = analogRead(LDR);
  int porcentaje = map(lectura, 0, 4095, 0, 100);
  return porcentaje;
}

void girarVertical(int cara, int velocidad) {
  int orden[8][2] = {{0,0},{0,1},{0,2},{1,2},{2,2},{2,1},{2,0},{1,0}};
  for (int i = 0; i < 8; i++) {
    apagarCubo();
    int x = orden[i][0];
    int z = orden[i][1];
    cubo[z][cara][x] = 1;
    mostrarCubo(velocidad);
  }
}

void loop() {
  int velocidad = 500 - leerLDR() * 4;
  if (velocidad < 50) velocidad = 50;
  Serial.print("Velocidad: ");
  Serial.println(velocidad);
  girarVertical(1, velocidad);  // Cara vertical elegida
}
