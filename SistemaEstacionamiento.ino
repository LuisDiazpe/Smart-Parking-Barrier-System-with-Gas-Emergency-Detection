#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo barrera;

// CONFIGURACION DE PINES 

// LCD 16x2 (modo 4 bits) 
// RS -> 12
// E  -> 11
// D4 -> 5
// D5 -> 4
// D6 -> 3
// D7 -> 2

// Alimentacion a la protoboard
// 5V Arduino  -> riel + protoboard
// GND Arduino -> riel - protoboard
//
// Desde la protoboard:
// Servo rojo   -> +
// Servo marron -> -
// LCD VDD, A   -> +
// LCD VSS, RW, K -> -
// Control por teclado usando Monitor Serial
// 'a' -> Abrir barrera
// 'c' -> Cerrar barrera

// Configuración de pines del joystick
const int pinJoyY = A1;  // Eje Y del joystick

// Configuración del sensor ultrasónico HC-SR04
const int pinTrig = 8;
const int pinEcho = 10;

// Configuración del sensor MQ-135
const int pinMQ135 = A2;

// LEDs de estado
// LED azul   -> Acceso cerrado
// LED blanco -> Acceso permitido
const int LED_CERRADO = 6;
const int LED_ABIERTO = 7;

// Posiciones del servo
const int BARRERA_ABAJO = 60;   // Barrera cerrada
const int BARRERA_ARRIBA = 10;  // Barrera abierta

// Estado de la barrera
bool barreraAbierta = false;

// Indica si la barrera fue abierta por el sensor
bool aperturaPorSensor = false;

// Modo emergencia por gas
bool modoEmergencia = false;

// Distancia mínima para detectar vehículo (cm)
const int DISTANCIA_DETECCION = 15;

// Umbral de gas (ajustable)
const int GAS_PELIGRO = 300;

void setup() {
  lcd.begin(16, 2);
  barrera.attach(9);

  Serial.begin(9600);

  // Configurar LEDs
  pinMode(LED_CERRADO, OUTPUT);
  pinMode(LED_ABIERTO, OUTPUT);

  // Configurar HC-SR04
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  // Estado inicial
  barrera.write(BARRERA_ABAJO);
  digitalWrite(LED_CERRADO, HIGH);
  digitalWrite(LED_ABIERTO, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Estacionamiento");
  lcd.setCursor(0, 1);
  lcd.print("Sistema listo");
  delay(2000);
  lcd.clear();
}

// Funcion para medir distancia con HC-SR04
long medirDistancia() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  long duracion = pulseIn(pinEcho, HIGH, 25000);
  long distancia = duracion / 58;
  return distancia;
}

void loop() {
  int joyY = analogRead(pinJoyY);
  long distancia = medirDistancia();
  int gasValue = analogRead(pinMQ135);

  // MODO EMERGENCIA POR GAS
  if (gasValue >= GAS_PELIGRO) {
    modoEmergencia = true;

    if (!barreraAbierta) {
      barrera.write(BARRERA_ARRIBA);
      barreraAbierta = true;
    }

    digitalWrite(LED_CERRADO, LOW);
    digitalWrite(LED_ABIERTO, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("!!! PELIGRO !!!");
    lcd.setCursor(0, 1);
    lcd.print("GAS DETECTADO ");

    delay(300);
    return; // BLOQUEA TODO LO DEMAS
  }

  // Salir del modo emergencia
  if (modoEmergencia && gasValue < GAS_PELIGRO - 50) {
    modoEmergencia = false;
    lcd.clear();
  }

  //MOSTRAR DISTANCIA 
  lcd.setCursor(0, 0);
  lcd.print("Distancia:     ");
  lcd.setCursor(11, 0);
  lcd.print(distancia);
  lcd.print("cm");

  //ABRIR automatico por sensor
  if (distancia > 0 && distancia <= DISTANCIA_DETECCION && !barreraAbierta) {
    barrera.write(BARRERA_ARRIBA);
    barreraAbierta = true;
    aperturaPorSensor = true;

    digitalWrite(LED_CERRADO, LOW);
    digitalWrite(LED_ABIERTO, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("Vehiculo OK   ");
    delay(500);
  }

  //Cerrar automatico cuando se va
  if (distancia > 20 && barreraAbierta && aperturaPorSensor) {
    barrera.write(BARRERA_ABAJO);
    barreraAbierta = false;
    aperturaPorSensor = false;

    digitalWrite(LED_CERRADO, HIGH);
    digitalWrite(LED_ABIERTO, LOW);

    lcd.setCursor(0, 1);
    lcd.print("Barrera abajo ");
    delay(650);
  }

  //Abrir con joystick
  if (joyY > 800 && !barreraAbierta) {
    barrera.write(BARRERA_ARRIBA);
    barreraAbierta = true;
    aperturaPorSensor = false;

    digitalWrite(LED_CERRADO, LOW);
    digitalWrite(LED_ABIERTO, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("Manual abrir  ");
    delay(300);
  }

  //Cerrar con joystick
  if (joyY < 200 && barreraAbierta) {
    barrera.write(BARRERA_ABAJO);
    barreraAbierta = false;
    aperturaPorSensor = false;

    digitalWrite(LED_CERRADO, HIGH);
    digitalWrite(LED_ABIERTO, LOW);

    lcd.setCursor(0, 1);
    lcd.print("Barrera abajo ");
    delay(500);
  }

  //Codigo anterior para control por teclado usando serial monitor
  if (Serial.available() > 0) {
    char tecla = Serial.read();

    if (tecla == 'a' && !barreraAbierta) {
      barrera.write(BARRERA_ARRIBA);
      barreraAbierta = true;
      aperturaPorSensor = false;

      digitalWrite(LED_CERRADO, LOW);
      digitalWrite(LED_ABIERTO, HIGH);

      lcd.setCursor(0, 1);
      lcd.print("Teclado abrir ");
    }

    if (tecla == 'c' && barreraAbierta) {
      barrera.write(BARRERA_ABAJO);
      barreraAbierta = false;
      aperturaPorSensor = false;

      digitalWrite(LED_CERRADO, HIGH);
      digitalWrite(LED_ABIERTO, LOW);

      lcd.setCursor(0, 1);
      lcd.print("Teclado cerrar");
    }
  }
}
