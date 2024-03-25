#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// here the configuration of your wifi network
const char* ssid = "***"; // name
const char* password = "***"; // password

/* Devices for this program: just change the IP

"devices": {
		"led rojo": {
			"ip": "your arduino's IP",
			"type": "output"
		},
		"led verde": {
			"ip": "your arduino's IP",
			"type": "output"
		},
		"led azul": {
			"ip": "your arduino's IP",
			"type": "output"
		}
	}

*/

// this must be port 80, since http runs on that port
WebServer server(80);  // Create a web server on port 80

///////////////////////// Here Your Global Variables ////////////////////

const int redPin = 14;    // Pin connected to the red LED
const int greenPin = 15;  // Pin connected to the green LED
const int bluePin = 16;   // Pin connected to the blue LED

int redPinValue = 0;    // initial values
int greenPinValue = 0;  // initial values
int bluePinValue = 0;   // initial values

/////////////////////////////////////////////////////////////////////////

void setup() {
  /////////////////////// Here Your Pin Configurations //////////////////

  // the configurations
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // the initial values
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);

  ///////////////////////////////////////////////////////////////////////
  
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

    /////////////////////// Here Your Pin Actions /////////////////////////

    // Access the received data
    String alias = doc["alias"];
    String type = doc["type"];
    // this is the final message
    String message = "";
    
    // for each device ...
    if (alias == "led rojo") {
      // first check the type
      if (type == "output") {
        // get the value
        String value_str = doc["value"];
        // RGB builtin led has Common Anode
        redPinValue = -1 * value_str.toInt() + 255;
        // finally set the value
        analogWrite(redPin, redPinValue);
        // and set the answer message
        if (redPinValue == 255) {
          message = "Led Rojo apagado";
        } else {
          message = "Led Rojo encendido";
        }
      } else {
        if (redPinValue == 255) {
          message = "Led Rojo está apagado";
        } else {
          message = "Led Rojo está endendido";
        }
      }
    }

    if else (alias == "led verde") {
      // first check the type
      if (type == "output") {
        // get the value
        String value_str = doc["value"];
        // RGB builtin led has Common Anode
        greenPinValue = -1 * value_str.toInt() + 255;
        // finally set the value
        analogWrite(greenPin, greenPinValue);
        // and set the answer message
        if (greenPinValue == 255) {
          message = "Led Verde apagado";
        } else {
          message = "Led Verde encendido";
        }
      } else {
        if (greenPinValue == 255) {
          message = "Led Verde está apagado";
        } else {
          message = "Led Verde está endendido";
        }
      }
    }

    if else (alias == "led azul") {
      // first check the type
      if (type == "output") {
        // get the value
        String value_str = doc["value"];
        // RGB builtin led has Common Anode
        bluePinValue = -1 * value_str.toInt() + 255;
        // finally set the value
        analogWrite(bluePin, bluePinValue);
        // and set the answer message
        if (bluePinValue == 255) {
          message = "Led Azul apagado";
        } else {
          message = "Led Azul encendido";
        }
      } else {
        if (bluePinValue == 255) {
          message = "Led Azul está apagado";
        } else {
          message = "Led Azul está endendido";
        }
      }
    }
    
    else {
      message = "Dispositivo no reconocido: " + alias;
    }

    ///////////////////////////////////////////////////////////////////////

    // Send a response with the message
    server.send(200, "text/plain", message);
  } else {
    server.send(400, "text/plain", "No JSON data found");
  }
}