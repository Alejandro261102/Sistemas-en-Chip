// Definición de pines en la ESP32
const int niveles[] = {25, 26, 27};  // Pines que controlan las capas (niveles)
const int columnas[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};  // Pines para las columnas de LEDs
const int boton1 = 18;  // Pin del primer botón
const int boton2 = 19;  // Pin del segundo botón

// Matriz 3x3x3 para representar el estado de los LEDs (1 = encendido, 0 = apagado)
int cubo[3][3][3] = {0};

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
    
    // Configurar botones como entrada
    pinMode(boton1, INPUT_PULLUP);
    pinMode(boton2, INPUT_PULLUP);
}

// Mostrar el contenido del cubo usando multiplexado
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
            delay(2);  // Delay corto para persistencia visual
            digitalWrite(niveles[z], LOW);
        }
    }
}

// Apagar todos los LEDs del cubo lógico
void apagarCubo() {
    for (int z = 0; z < 3; z++) {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                cubo[z][y][x] = 0;
            }
        }
    }
}

// Animaciones corregidas usando la matriz cubo[][][]
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
            cubo[1][0][0] = 1; cubo[1][0][2] = 1; cubo[1][2][0] = 1; cubo[1][2][2] = 1;
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
    // Cara Inferior
    apagarCubo();
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            cubo[0][y][x] = 1;
    mostrarCubo(500);

    // Cara Superior
    apagarCubo();
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            cubo[2][y][x] = 1;
    mostrarCubo(500);

    // Cara Frontal (x=0)
    apagarCubo();
    for (int z = 0; z < 3; z++)
        for (int y = 0; y < 3; y++)
            cubo[z][y][0] = 1;
    mostrarCubo(500);

    // Cara Trasera (x=2)
    apagarCubo();
    for (int z = 0; z < 3; z++)
        for (int y = 0; y < 3; y++)
            cubo[z][y][2] = 1;
    mostrarCubo(500);

    // Cara Izquierda (y=0)
    apagarCubo();
    for (int z = 0; z < 3; z++)
        for (int x = 0; x < 3; x++)
            cubo[z][0][x] = 1;
    mostrarCubo(500);

    // Cara Derecha (y=2)
    apagarCubo();
    for (int z = 0; z < 3; z++)
        for (int x = 0; x < 3; x++)
            cubo[z][2][x] = 1;
    mostrarCubo(500);
}

void animacionCapas() {
    for (int z = 0; z < 3; z++) {
        apagarCubo();
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++)
                cubo[z][y][x] = 1;
        mostrarCubo(500);
    }
}

void loop() {
    int estado = digitalRead(boton1) * 2 + digitalRead(boton2);

    switch (estado) {
        case 0:
            animacionBordes();
            break;
        case 1:
            animacionTodosEncendidos();
            break;
        case 2:
            animacionCaras();
            break;
        case 3:
            animacionCapas();
            break;
    }
}