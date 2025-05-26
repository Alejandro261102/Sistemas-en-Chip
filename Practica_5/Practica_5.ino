
// Pines
#define LDR_PIN 23         // Pin donde está conectado el LDR

// Pines del motor DC (puente H)
#define MOTOR_IN1 4
#define MOTOR_IN2 2
#define MOTOR_ENA 15

// Pines de los displays de 7 segmentos
#define DISPLAY1_A 36
#define DISPLAY1_B 39
#define DISPLAY1_C 34
#define DISPLAY1_D 35
#define DISPLAY1_E 32
#define DISPLAY1_F 33
#define DISPLAY1_G 25

#define DISPLAY2_A 22
#define DISPLAY2_B 21
#define DISPLAY2_C 19
#define DISPLAY2_D 18
#define DISPLAY2_E 5
#define DISPLAY2_F 17
#define DISPLAY2_G 16

#define DISPLAY3_A 26
#define DISPLAY3_B 27
#define DISPLAY3_C 14
#define DISPLAY3_D 12
#define DISPLAY3_E 13
#define DISPLAY3_F 9
#define DISPLAY3_G 10

// Variables
volatile int revoluciones = 0;
unsigned long tiempoInicio = 0;
const int tiempoMedicion = 15000;  // 15 segundos

// Mapas de segmentos para los números del 0 al 9
const uint8_t numeros[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
};

// Interrupción para contar revoluciones
void IRAM_ATTR contarRevolucion() {
    revoluciones++;
}

// Configuración inicial
void setup() {
    Serial.begin(115200);
    pinMode(LDR_PIN, INPUT);

    // Configuración de los pines del motor
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_ENA, OUTPUT);

    // Configuración de los pines de los displays
    for (int pin = DISPLAY1_A; pin <= DISPLAY3_G; pin++) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    // Interrupción en el flanco de subida (cuando el láser pasa)
    attachInterrupt(digitalPinToInterrupt(LDR_PIN), contarRevolucion, RISING);
}

// Función para mostrar un número en un display específico
void mostrarNumeroEnDisplay(int numero, int displayBasePin) {
    for (int i = 0; i < 7; i++) {
        digitalWrite(displayBasePin + i, numeros[numero][i]);
    }
}

// Función para mostrar un número de 3 dígitos en los displays
void mostrarNumero(int numero) {
    int centenas = (numero / 100) % 10;
    int decenas = (numero / 10) % 10;
    int unidades = numero % 10;

    mostrarNumeroEnDisplay(centenas, DISPLAY1_A);
    mostrarNumeroEnDisplay(decenas, DISPLAY2_A);
    mostrarNumeroEnDisplay(unidades, DISPLAY3_A);
}

// Función para controlar el motor
void controlarMotor(bool adelante, int velocidad) {
    analogWrite(MOTOR_ENA, velocidad);  // Control de velocidad
    if (adelante) {
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
    } else {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
    }
}

// Bucle principal
void loop() {
    revoluciones = 0;
    tiempoInicio = millis();

    // Encender el motor hacia adelante al 50% de velocidad
    controlarMotor(true, 128);

    // Esperar 15 segundos
    while (millis() - tiempoInicio < tiempoMedicion) {
        // Esperando medición
    }

    // Apagar el motor
    controlarMotor(false, 0);

    // Calcular RPM
    int rpm = (revoluciones * 60) / 15;
    Serial.print("Revoluciones: ");
    Serial.println(revoluciones);
    Serial.print("RPM: ");
    Serial.println(rpm);

    // Mostrar RPM en los displays
    mostrarNumero(rpm);

    delay(1000);  // Pausa para la siguiente medición
}
