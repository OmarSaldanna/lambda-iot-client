#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "****"; // Name of the wifi network
const char* password = "*****"; // password

/* Devices for this program

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
  // begin the serial port
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Serial Messages");

  // Define the GET request handler
  server.on("/iot", handleGetData);
}

void loop() {
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
      // set the answer
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