# 🚗 Smart Parking Barrier System with Gas Emergency Detection

Sistema inteligente de control de acceso vehicular desarrollado con **Arduino UNO**, diseñado para simular el funcionamiento de una barrera automática de estacionamiento con **detección de vehículos**, **control manual**, y **modo de emergencia por presencia de gases peligrosos**.

---

## 🧠 Características principales

- ✅ Apertura y cierre automático mediante **sensor ultrasónico HC-SR04**
- 🎮 Control manual usando **joystick**
- ⌨️ Control alternativo mediante **teclado (Serial Monitor)**
- 🧯 **Detección de gases peligrosos** con sensor **MQ-135**
- 🚨 Modo emergencia:
  - Apertura inmediata de la barrera
  - Bloqueo de controles manuales
  - Mensaje de alerta en pantalla LCD
- 📟 Visualización de estado en **LCD 16x2**
- 💡 Indicadores LED:
  - LED azul → acceso cerrado
  - LED blanco → acceso permitido

---

## 🧩 Componentes utilizados

- Arduino UNO  
- Servo motor (barrera)
- Sensor ultrasónico HC-SR04  
- Sensor de gas MQ-135  
- Joystick analógico  
- Pantalla LCD 16x2  
- 2 LEDs + resistencias  
- Protoboard y cables  

---

## 🔌 Configuración de pines

### Servo
- Señal → Pin 9

### Sensor ultrasónico (HC-SR04)
- Trig → Pin 8
- Echo → Pin 10

### Sensor de gas (MQ-135)
- AO → A2
- VCC → 5V
- GND → GND

### Joystick
- VRy → A1
- VCC → 5V
- GND → GND

### LEDs
- LED azul (cerrado) → Pin 6
- LED blanco (abierto) → Pin 7

### LCD 16x2 (modo 4 bits)
- RS → 12  
- E  → 11  
- D4 → 5  
- D5 → 4  
- D6 → 3  
- D7 → 2  

---

## ⚙️ Funcionamiento del sistema

### 🚘 Detección de vehículo
Cuando el sensor ultrasónico detecta un vehículo a una distancia menor al umbral configurado, la barrera se abre automáticamente.

### 🎮 Control manual
El joystick permite:
- Empujar hacia arriba → abrir barrera
- Empujar hacia abajo → cerrar barrera

También se puede controlar desde el **Serial Monitor**:
- `'a'` → abrir
- `'c'` → cerrar

### 🧯 Modo emergencia por gas
Si el sensor MQ-135 detecta una concentración peligrosa de gas:
- La barrera se abre automáticamente
- Se bloquean todos los controles
- Se muestra un mensaje de **PELIGRO** en la pantalla LCD
- El sistema solo vuelve a la normalidad cuando el nivel de gas disminuye

---

## 📊 Calibración del sensor MQ-135

El valor de detección de gas se define a partir del nivel promedio de aire limpio, sumando un margen de seguridad.

Ejemplo:
```cpp
const int GAS_PELIGRO = aire_limpio + 150;
