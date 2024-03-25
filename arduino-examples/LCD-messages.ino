// needed libraries for instance the web server
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// libraries requiered for LCD display
#include <Wire.h> // this is normally installed by default
// this one should be the arduino oficial:
// https://github.com/johnrickman/LiquidCrystal_I2C
// just download the .zip and include that zip as a library
#include <LiquidCrystal_I2C.h>

// this line sets the dimentions of the display
// most common dimentions are 20x4 and 16x2
LiquidCrystal_I2C lcd(0x27,16,2);

// here the configuration of your wifi network
const char* ssid = "***"; // name
const char* password = "***"; // password

/* Devices for this program

  "devices": {
    "display lcd": {
      "ip": "your arduino's IP",
      "type": "output"
    }
  }

*/

// this must be port 80, since http runs on that port
WebServer server(80); // create a web server on port 80
// if you desire to change the port, you will have to
// change some code on the lambda-iot-client app.

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
  
  // init the lcd display
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  // print the ip on the display
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  
  // Start the server
  server.begin();
  // Define the GET request handler
  server.on("/iot", handleGetData);
}

void loop() {
  // while true, keep the server listening
  server.handleClient();
}

// function used to handle data
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
    if (alias == "display lcd") {
      // print the message on the serial monitor
      Serial.print("Incoming Message: ");
      Serial.println(message);
      // change the value of the lcd
      lcd.clear() // clear the lcd
      // print the incomming message
      lcd.setCursor(0,0); // set the cursor on the first line
      lcd.print(message);
      // print again the ip
      lcd.setCursor(1,0); // set the cursor on the second line
      lcd.print(WiFi.localIP()); // print again the IP
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