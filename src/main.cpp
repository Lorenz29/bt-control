#include "BluetoothSerial.h" // Biblioteca para Bluetooth Clásico Serial
#include "DHT.h"             // Biblioteca para el sensor DHT

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT; // Objeto para manejar la comunicación Bluetooth Serial

// Define el pin donde está conectado el LED incorporado
const int ledPin = 2;

// Define el pin donde está conectado el sensor DHT22
const int dhtPin = 32; // Puedes cambiar esto al pin GPIO que estés usando

// Define el tipo de sensor DHT (DHT22)
const int dhtType = DHT22;

// Crea un objeto DHT
DHT dht(dhtPin, dhtType);

// Define el intervalo de tiempo entre lecturas del sensor (en milisegundos)
const long intervaloLectura = 2000; // Leer cada 2 segundos
unsigned long ultimoTiempoLectura = 0;

void setup() {
  Serial.begin(115200); // Inicia el monitor serial para depuración
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegúrate que el LED empiece apagado

  // Inicia el sensor DHT
  dht.begin();
  Serial.println("Sensor DHT22 iniciado.");

  // Inicia Bluetooth Serial con un nombre de dispositivo visible
  SerialBT.begin("ESP32_Control_DHT"); // Cambia el nombre para diferenciarlo (opcional)
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
    } else if (incomingChar == 'T' || incomingChar == 't') {
      // Si se recibe 'T' o 't', envía la lectura de temperatura
      float temperatura = dht.readTemperature();
      if (isnan(temperatura)) {
        Serial.println("Error al leer la temperatura!");
        SerialBT.println("Error al leer la temperatura!");
      } else {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" *C");
        SerialBT.print("Temperatura: ");
        SerialBT.print(temperatura);
        SerialBT.println(" *C");
      }
    } else if (incomingChar == 'H' || incomingChar == 'h') {
      // Si se recibe 'H' o 'h', envía la lectura de humedad
      float humedad = dht.readHumidity();
      if (isnan(humedad)) {
        Serial.println("Error al leer la humedad!");
        SerialBT.println("Error al leer la humedad!");
      } else {
        Serial.print("Humedad: ");
        Serial.print(humedad);
        Serial.println(" %");
        SerialBT.print("Humedad: ");
        SerialBT.print(humedad);
        SerialBT.println(" %");
      }
    } else if (incomingChar == 'S' || incomingChar == 's') {
      // Si se recibe 'S' o 's', envía ambas lecturas (Temperatura y Humedad)
      float temperatura = dht.readTemperature();
      float humedad = dht.readHumidity();

      if (isnan(temperatura) || isnan(humedad)) {
        Serial.println("Error al leer temperatura y/o humedad!");
        SerialBT.println("Error al leer temperatura y/o humedad!");
      } else {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.print(" *C, Humedad: ");
        Serial.print(humedad);
        Serial.println(" %");
        SerialBT.print("Temperatura: ");
        SerialBT.print(temperatura);
        SerialBT.print(" *C, Humedad: ");
        SerialBT.print(humedad);
        SerialBT.println(" %");
      }
    } else {
      SerialBT.print("Comando '");
      SerialBT.print(incomingChar);
      SerialBT.println("' no reconocido.");
    }
  }

  // Lectura periódica del sensor (opcional, puedes basarte solo en comandos)
  if (millis() - ultimoTiempoLectura >= intervaloLectura) {
    ultimoTiempoLectura = millis();
    // Puedes descomentar el siguiente bloque si quieres que la ESP32 envíe
    // las lecturas periódicamente sin necesidad de un comando.
    /*
    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();

    if (!isnan(temperatura) && !isnan(humedad)) {
      Serial.print("Temperatura: ");
      Serial.print(temperatura);
      Serial.print(" *C, Humedad: ");
      Serial.print(humedad);
      Serial.println(" % (Lectura periódica)");
      SerialBT.print("Temperatura: ");
      SerialBT.print(temperatura);
      SerialBT.print(" *C, Humedad: ");
      SerialBT.print(humedad);
      SerialBT.println(" % (Lectura periódica)");
    } else {
      Serial.println("Error al leer temperatura y/o humedad (Lectura periódica)!");
      SerialBT.println("Error al leer temperatura y/o humedad (Lectura periódica)!");
    }
    */
  }

  delay(20); // Pequeña pausa
}