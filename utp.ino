#include <Ethernet.h>
#include <SPI.h>

#include <DHT.h>
#include <DHT_U.h>

// Configuracion del Ethernet Shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xEE}; // Direccion MAC
byte ip[] = { 192,168,1,65}; // Direccion IP del Arduino
byte server[] = { 192,168,1,43 }; // Direccion IP del servidor

EthernetClient client; 
int temperatura;
int sensor = 2;


DHT dht(sensor,DHT11);

void setup() {
   Serial.begin(9600);
   dht.begin();
  Ethernet.begin(mac, ip); // Inicializamos el Ethernet Shield
  delay(1000); // Esperamos 1 segundo de cortesia
}

void loop() {
  
  temperatura = dht.readTemperature();
  
  //Display in Serial Monitor
  Serial.print(temperatura); //Return temperature to Monitor
  Serial.println(" oC");
  // Proceso de envio de muestras al servidor
  Serial.println("Connecting...");
  if (client.connect(server, 8080)>0) {  // Conexion con el servidor
    client.print("GET /firebase/sensor/1/"); // Enviamos los datos por GET
    client.print(temperatura);
    client.println(" HTTP/1.0");
    client.println("User-Agent: Arduino 1.0");
    client.println();
    Serial.println("Conectado");
  } else {
    Serial.println("Fallo en la conexion");
  }
  if (!client.connected()) {
    Serial.println("Disconnected!");
  }
  client.stop();
  client.flush();
  delay(6000); // Espero un minuto antes de tomar otra muestra
}
