// Definición de pines en la ESP32
const int niveles[] = {25, 26, 27};  // Pines que controlan las capas (niveles)
const int columnas[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};  // Pines para las columnas de LEDs
const int LDR = 34;  // Pin para sensor LDR
const int selector[] = {18, 19, 21};  // Pines del selector de 3 bits
const int boton = 35; // Pin de botón para interrupción

// Matriz 3x3x3 para representar el estado de los LEDs (1 = encendido, 0 = apagado)
int cubo[3][3][3] = {0};

volatile int contadorInterrupciones = 0;
int ledsEncendidos = 0;
bool modoInterrupcion = false;
unsigned long ultimaInterrupcion = 0;

int pasoUnoPorUno = 0;

void IRAM_ATTR manejarInterrupcion() {
  if (millis() - ultimaInterrupcion > 1000) {
    contadorInterrupciones++;
    ultimaInterrupcion = millis();
    modoInterrupcion = true;
  }
}

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
  for (int i = 0; i < 3; i++) {
    pinMode(selector[i], INPUT_PULLUP);
  }
  pinMode(boton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(boton), manejarInterrupcion, FALLING);
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

void animacionUnoPorUnoPaso() {
  int z = pasoUnoPorUno / 9;
  int y = (pasoUnoPorUno % 9) / 3;
  int x = pasoUnoPorUno % 3;
  apagarCubo();
  cubo[z][y][x] = 1;
  mostrarCubo(100);
  pasoUnoPorUno++;
  if (pasoUnoPorUno >= 27) pasoUnoPorUno = 0;
}

void animacionVertices() {
  apagarCubo();
  int posiciones[8][3] = {
    {0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
    {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}
  };
  for (int i = 0; i < 8; i++) {
    int z = posiciones[i][0], y = posiciones[i][1], x = posiciones[i][2];
    cubo[z][y][x] = 1;
  }
  mostrarCubo(500);
}

void animacionTodosEncendidos() {
  for (int z = 0; z < 3; z++)
    for (int y = 0; y < 3; y++)
      for (int x = 0; x < 3; x++)
        cubo[z][y][x] = 1;
  mostrarCubo(500);
}

void animacionBordes() {
  apagarCubo();
  int bordes[] = {0, 1, 2, 3, 5, 6, 7, 8};
  for (int z = 0; z < 3; z++) {
    if (z == 1) {
      cubo[1][0][0] = 1; cubo[1][0][2] = 1;
      cubo[1][2][0] = 1; cubo[1][2][2] = 1;
    } else {
      for (int i = 0; i < 8; i++) {
        int idx = bordes[i];
        cubo[z][idx / 3][idx % 3] = 1;
      }
    }
  }
  mostrarCubo(500);
}

void animacionCaras() {
  apagarCubo();
  for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++) cubo[0][y][x] = 1;
  mostrarCubo(500);
  apagarCubo();
  for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++) cubo[2][y][x] = 1;
  mostrarCubo(500);
  apagarCubo();
  for (int z = 0; z < 3; z++) for (int y = 0; y < 3; y++) cubo[z][y][0] = 1;
  mostrarCubo(500);
  apagarCubo();
  for (int z = 0; z < 3; z++) for (int y = 0; y < 3; y++) cubo[z][y][2] = 1;
  mostrarCubo(500);
  apagarCubo();
  for (int z = 0; z < 3; z++) for (int x = 0; x < 3; x++) cubo[z][0][x] = 1;
  mostrarCubo(500);
  apagarCubo();
  for (int z = 0; z < 3; z++) for (int x = 0; x < 3; x++) cubo[z][2][x] = 1;
  mostrarCubo(500);
}

void animacionCapas() {
  for (int z = 0; z < 3; z++) {
    apagarCubo();
    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++) cubo[z][y][x] = 1;
    mostrarCubo(500);
  }
}

void animacionNavidad() {
  for (int i = 0; i < 10; i++) {
    apagarCubo();
    for (int j = 0; j < 6; j++) {
      int z = random(0, 3), y = random(0, 3), x = random(0, 3);
      cubo[z][y][x] = 1;
    }
    mostrarCubo(150);
    apagarCubo();
    for (int j = 0; j < 6; j++) {
      int z = random(0, 3), y = random(0, 3), x = random(0, 3);
      cubo[z][y][x] = 1;
    }
    mostrarCubo(150);
  }
}

int leerSelector() {
  int val = 0;
  for (int i = 0; i < 3; i++) {
    val |= (!digitalRead(selector[i])) << i;
  }
  return val;
}

int leerLDR() {
  int lectura = analogRead(LDR);
  int porcentaje = map(lectura, 0, 4095, 0, 100);
  return porcentaje / 15;
}

void manejarModoInterrupcion() {
  if (modoInterrupcion) {
    apagarCubo();
    int cantidad = contadorInterrupciones % 28;
    ledsEncendidos = cantidad;
    int ledActual = 0;
    for (int z = 0; z < 3; z++)
      for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++) {
          if (ledActual < cantidad) {
            cubo[z][y][x] = 1;
            ledActual++;
          }
        }
    mostrarCubo(500);
    Serial.print("Interrupciones acumuladas: ");
    Serial.println(contadorInterrupciones);
    if (ledsEncendidos >= 27) {
      contadorInterrupciones = 0;
    }
    modoInterrupcion = false;
  }
}

void loop() {
  manejarModoInterrupcion();
  if (!modoInterrupcion) {
    int nivel = 0;
    int sel = leerSelector();
    if (sel == 7) nivel = leerLDR();
    else nivel = sel;

    switch (nivel) {
      case 0: animacionUnoPorUnoPaso(); break;
      case 1: animacionVertices(); break;
      case 2: animacionTodosEncendidos(); break;
      case 3: animacionBordes(); break;
      case 4: animacionCaras(); break;
      case 5: animacionCapas(); break;
      case 6: animacionNavidad(); break;
      default: apagarCubo(); break;
    }
  }
}
