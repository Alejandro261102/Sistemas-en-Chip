// Definición de pines en la ESP32
/*const int niveles[] = {25, 26, 27};  // Pines que controlan las capas (niveles)
const int columnas[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};  // Pines para las columnas de LEDs
const int boton = 18;  // Pin del botón

void setup() {
    // Configurar pines de niveles como salida
    for (int i = 0; i < 3; i++) {
        pinMode(niveles[i], OUTPUT);
        digitalWrite(niveles[i], LOW);  // Apagar niveles
    }
    
    // Configurar pines de columnas como salida
    for (int i = 0; i < 9; i++) {
        pinMode(columnas[i], OUTPUT);
        digitalWrite(columnas[i], LOW);  // Apagar columnas
    }
    
    // Configurar botón como entrada
    pinMode(boton, INPUT_PULLUP);
}

// Encender un LED en una posición específica
void encenderLED(int nivel, int columna) {
    digitalWrite(niveles[nivel], HIGH);  // Activar el nivel
    digitalWrite(columnas[columna], HIGH);  // Activar la columna
    delay(100);
    digitalWrite(columnas[columna], LOW);  // Apagar la columna después del delay
    digitalWrite(niveles[nivel], LOW);  // Apagar el nivel
}

// Encender los 4 vértices
void encenderVertices() {
    int vertices[4][2] = {{0, 0}, {0, 8}, {2, 0}, {2, 8}}; // Niveles y columnas de los vértices
    for (int i = 0; i < 4; i++) {
        digitalWrite(niveles[vertices[i][0]], HIGH);
        digitalWrite(columnas[vertices[i][1]], HIGH);
    }
}

// Apagar todos los LEDs
void apagarTodo() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(niveles[i], LOW);
    }
    for (int i = 0; i < 9; i++) {
        digitalWrite(columnas[i], LOW);
    }
}

void loop() {
  if (digitalRead(boton) == LOW) {  // Si el botón está presionado
      apagarTodo();
      encenderVertices();
  } else {  // Si el botón no está presionado
        for (int nivel = 0; nivel < 3; nivel++) {
            for (int columna = 0; columna < 9; columna++) {
                encenderLED(nivel, columna);
                delay(100);
            }
        }
    }
}
*/

// Definición de pines en la ESP32
const int niveles[] = {25, 26, 27};  // Pines que controlan las capas (niveles)
const int columnas[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};  // Pines para las columnas de LEDs
const int boton = 18;  // Pin del botón

// Tiempos alto (TH) y bajo (TL) en milisegundos
const int TH = 180;
const int TL = 200;

void setup() {
    // Configurar pines de niveles como salida
    for (int i = 0; i < 3; i++) {
        pinMode(niveles[i], OUTPUT);
        digitalWrite(niveles[i], LOW);  // Apagar niveles
    }
    
    // Configurar pines de columnas como salida
    for (int i = 0; i < 9; i++) {
        pinMode(columnas[i], OUTPUT);
        digitalWrite(columnas[i], LOW);  // Apagar columnas
    }
    
    // Configurar botón como entrada
    pinMode(boton, INPUT);
}

// Encender un LED en una posición específica
void encenderLED(int nivel, int columna) {
    digitalWrite(niveles[nivel], HIGH);  // Activar el nivel
    digitalWrite(columnas[columna], HIGH);  // Activar la columna
    delay(TH);
    digitalWrite(columnas[columna], LOW);  // Apagar la columna después del delay
    digitalWrite(niveles[nivel], LOW);  // Apagar el nivel
    delay(TL);
}

// Encender los 8 vértices (esquinas) del cubo
void encenderVertices() {
    int vertices[8][2] = {
        {0, 0}, {0, 8}, {0, 2}, {0, 6},
        {2, 0}, {2, 8}, {2, 2}, {2, 6}
    };
    for (int i = 0; i < 8; i++) {
        digitalWrite(niveles[vertices[i][0]], HIGH);
        digitalWrite(columnas[vertices[i][1]], HIGH);
    }
}

// Apagar todos los LEDs
void apagarTodo() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(niveles[i], LOW);
    }
    for (int i = 0; i < 9; i++) {
        digitalWrite(columnas[i], LOW);
    }
}

void loop() {
    if (digitalRead(boton) == LOW) {  // Si el botón está presionado
        apagarTodo();
        encenderVertices();
    } else {  // Si el botón no está presionado
        for (int nivel = 0; nivel < 3; nivel++) {
            for (int columna = 0; columna < 9; columna++) {
                encenderLED(nivel, columna);
            }
        }
    }
}
