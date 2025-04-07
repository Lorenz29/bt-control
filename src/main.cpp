#include "BluetoothSerial.h" // Biblioteca para Bluetooth Clásico Serial

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT; // Objeto para manejar la comunicación Bluetooth Serial

// Define el pin donde está conectado el LED incorporado
// En muchas placas Lolin32/D32 es el GPIO 2
const int ledPin = 2;

void setup() {
  Serial.begin(115200); // Inicia el monitor serial para depuración
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegúrate que el LED empiece apagado

  // Inicia Bluetooth Serial con un nombre de dispositivo visible
  // Puedes cambiar "ESP32_Control" por el nombre que prefieras
  SerialBT.begin("ESP32_Control");
  Serial.println("Dispositivo Bluetooth iniciado. Listo para emparejar!");
  Serial.println("Puedes conectar ahora desde tu app Android.");
}

void loop() {
  // Revisa si hay datos recibidos a través de Bluetooth
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read(); // Lee el caracter recibido

    Serial.print("Recibido por Bluetooth: ");
    Serial.println(incomingChar);

    // Procesa el comando recibido
    if (incomingChar == '1') {
      digitalWrite(ledPin, HIGH); // Enciende el LED
      Serial.println("LED Encendido");
      SerialBT.println("Comando recibido: LED Encendido"); // Envía respuesta a Android
    } else if (incomingChar == '0') {
      digitalWrite(ledPin, LOW); // Apaga el LED
      Serial.println("LED Apagado");
      SerialBT.println("Comando recibido: LED Apagado"); // Envía respuesta a Android
    } else {
      SerialBT.print("Comando '");
      SerialBT.print(incomingChar);
      SerialBT.println("' no reconocido.");
    }
  }

  // Puedes agregar otras tareas aquí si es necesario,
  // pero evita usar delays largos para no bloquear la recepción Bluetooth.
  delay(20); // Pequeña pausa
}