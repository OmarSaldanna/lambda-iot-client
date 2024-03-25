#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// library requiered to use the DHT11, downloaded from
// https://github.com/adidax/dht11
// just download the .zip and include that zip as a library
#include <dht11.h>
// this is the digital pin connected to the sensor
#define DHT11PIN 4
// instance the sensor
dht11 DHT11;

// variables for temperature and humidity
float temp, hum;

// here the configuration of your wifi network
const char* ssid = "Asadero Cien"; // name
const char* password = "buenprovecho"; // password

/* Devices for this program: just change the IP

  "devices": {
    "humedad": {
      "ip": "your arduino's IP",
      "type": "input"
    },
    "temperatura": {
      "ip": "your arduino's IP",
      "type": "input"
    }
  }

*/

// this must be port 80, since http runs on that port
WebServer server(80);  // Create a web server on port 80

void setup() {
  // connect to Wi-Fi
  WiFi.begin(ssid, password);
  // start the wifi connection process
  Serial.print("Conectando a wifi");
  while (WiFi.status() != WL_CONNECTED) {
    // try to connect every 500 milis
    Serial.print(".");
    delay(500);
  }
  // send a message of succesfully connected
  Serial.println("Conexion exitosa");
  
  // begin the serial monitor
  Serial.begin(115200);
  // Print the IP address
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // Start the server
  server.begin();
  // Define the GET request handler
  server.on("/iot", handleGetData);
}

void loop() {
  // while true, keep the server listening
  server.handleClient();
}

void handleGetData() {
  // Check if the request has JSON data
  if (server.hasArg("plain") && server.arg("plain") != "") {
    String jsonString = server.arg("plain");
    // uncomment the next prints to see what the Arduino receives
    // Serial.println("Request recibida:");
    // Serial.println(jsonString);
    // Example with a 200-byte allocation:
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, jsonString);

    /////////////////////// Here Your Actions /////////////////////////

    // Access the received data
    String alias = doc["alias"];
    String message = doc["value"];
    
    // instance the answer message
    String answer = "";

    // make the sensor reccord
    int chk = DHT11.read(DHT11PIN);
    // and save the results
    temp = (float)DHT11.temperature;
    hum = (float)DHT11.humidity;

    // for humidity
    if (alias == "humedad") {
      // set the answer, you can change it as you want
      // this message is the one you see in the Lambda chat
      answer = "La humedad es de " + String(hum, 1) + "%";
    } 

    // for temperature
    else if (alias == "temperatura") {
      // set the answer, you can change it as you want
      // this message is the one you see in the Lambda chat
      answer = "La temperatura es de " + String(temp, 1) + "°C";
    } 
     else {
      // set the answer
      answer = "Dispositivo no reconocido" + alias;
      // and display it on the monitor
      Serial.println(answer);
    }

    ///////////////////////////////////////////////////////////////////////

    // Send a response with the message
    server.send(200, "text/plain", answer);
  } else {
    server.send(400, "text/plain", "No JSON data found");
  }
}