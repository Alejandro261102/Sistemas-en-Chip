// Definición de pines en la ESP32
const int niveles[] = {25, 26, 27};  // Pines que controlan las capas (niveles)
const int columnas[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};  // Pines para las columnas de LEDs
const int sensorRTD = 34; // Ajusta según tu conexión

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
}

// Función para mapear lectura a porcentaje
int leerNivelRTD() {
  int lectura = analogRead(sensorRTD);
  int porcentaje = map(lectura, 0, 4095, 0, 100); // Asumiendo ADC de 12 bits
  return porcentaje / 15; // 7 niveles → 0 a 6
}

//Encender Leds uno por uno
void encenderLED() {
  for (int nivel = 0; nivel < 3; nivel++) {
    for (int columna = 0; columna < 9; columna++) {
      digitalWrite(niveles[nivel], HIGH);  // Activar el nivel
      digitalWrite(columnas[columna], HIGH);  // Activar la columna
      delay(TH);
      digitalWrite(columnas[columna], LOW);  // Apagar la columna después del delay
      digitalWrite(niveles[nivel], LOW);  // Apagar el nivel
      delay(TL);
    }
  }
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

void encenderTodos() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(niveles[i], HIGH);
    }
    for (int i = 0; i < 9; i++) {
        digitalWrite(columnas[i], HIGH);
    }
}

// Encender los 12 bordes del cubo
void encenderBordes() {
  int medio[] = {0, 2, 6, 8};
  int arribayabajo[] = {0, 1, 2, 3, 5, 6, 7, 8};
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(niveles[i], HIGH);
    if(i == 1){
      for (int j = 0; j < 4; j++) {
        digitalWrite(columnas[medio[j]], HIGH);
      }
    } else {
      for (int k = 0; k < 9; k++) {
       digitalWrite(columnas[arribayabajo[k]], HIGH);
      }
    }  
  }
}

// Encender una cara a la vez y cambiar
void encenderCaras() {
  // Cara 1: Inferior (nivel 0)
  digitalWrite(niveles[0], HIGH);
  for (int i = 0; i < 9; i++) {
    digitalWrite(columnas[i], HIGH);
  }
  delay(500);
  apagarTodo();

  // Cara 2: Superior (nivel 2)
  digitalWrite(niveles[2], HIGH);
  for (int i = 0; i < 9; i++) {
    digitalWrite(columnas[i], HIGH);
  }
  delay(500);
  apagarTodo();

  // Cara 3: Frontal (columnas 0,3,6)
  for (int i = 0; i < 3; i++) {
    digitalWrite(niveles[i], HIGH);
  }
  digitalWrite(columnas[0], HIGH);
  digitalWrite(columnas[3], HIGH);
  digitalWrite(columnas[6], HIGH);
  delay(500);
  apagarTodo();

  // Cara 4: Trasera (columnas 2,5,8)
  for (int i = 0; i < 3; i++) {
    digitalWrite(niveles[i], HIGH);
  }
  digitalWrite(columnas[2], HIGH);
  digitalWrite(columnas[5], HIGH);
  digitalWrite(columnas[8], HIGH);
  delay(500);
  apagarTodo();

  // Cara 5: Izquierda (columnas 0,1,2)
  for (int i = 0; i < 3; i++) {
    digitalWrite(niveles[i], HIGH);
  }
  digitalWrite(columnas[0], HIGH);
  digitalWrite(columnas[1], HIGH);
  digitalWrite(columnas[2], HIGH);
  delay(500);
  apagarTodo();

  // Cara 6: Derecha (columnas 6,7,8)
  for (int i = 0; i < 3; i++) {
    digitalWrite(niveles[i], HIGH);
  }
  digitalWrite(columnas[6], HIGH);
  digitalWrite(columnas[7], HIGH);
  digitalWrite(columnas[8], HIGH);
  delay(500);
  apagarTodo();
}

// Encender una capa inferior y cambiar
void encenderCapas() {
    for (int nivel = 0; nivel < 3; nivel++) {
        digitalWrite(niveles[nivel], HIGH);
        for (int i = 0; i < 9; i++) {
            digitalWrite(columnas[i], HIGH);
        }
        delay(500);
        digitalWrite(niveles[nivel], LOW);
        for (int i = 0; i < 9; i++) {
            digitalWrite(columnas[i], LOW);
        }
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

void navidad() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(niveles[i], HIGH);
    for (int j = 0; j < 9; j++) {
      digitalWrite(columnas[j], HIGH);
      delay(100);
      digitalWrite(columnas[j], LOW);
    }
    digitalWrite(niveles[i], LOW);
  }
  for (int i = 2; i >= 0; i--) {
    digitalWrite(niveles[i], HIGH);
    for (int j = 8; j >= 0; j--) {
      digitalWrite(columnas[j], HIGH);
      delay(100);
      digitalWrite(columnas[j], LOW);
    }
    digitalWrite(niveles[i], LOW);
  }
}

void loop() {
  int nivel = leerNivelRTD();
  switch(nivel) {
    case 0:
      apagarTodo();
      encenderLED();
      break;
    case 1:
      apagarTodo();
      encenderVertices();
      break;
    case 2:
      apagarTodo();
      encenderTodos();
      break;
    case 3:
      apagarTodo();
      encenderBordes();
      break;
    case 4:
      apagarTodo();
      encenderCaras();
      break;
    case 5:
      apagarTodo();
      encenderCapas();
      break;
    case 6:
      apagarTodo();
      navidad();
      break;
  }
  delay(100); // Pequeña pausa entre animaciones
}