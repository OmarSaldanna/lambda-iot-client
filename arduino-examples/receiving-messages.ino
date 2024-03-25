#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "***"; // Name of the wifi network
const char* password = "***"; // password

/* Devices for this program: just change the IP

  "devices": {
		"monitor serial": {
			"ip": "your arduino's IP",
			"type": "output"
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

    // check the device alias 
    if (alias == "monitor serial") {
      // print the message on the serial monitor
      Serial.print("Incoming Message: ");
      Serial.println(message);
      // set the answer, you can change it as you want
      // this message is the one you see in the Lambda chat
      answer = "Mensaje Recibido";
    } else {
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