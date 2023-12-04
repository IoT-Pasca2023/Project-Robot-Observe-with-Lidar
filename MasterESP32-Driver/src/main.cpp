#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Haykull";       
const char* password = "asd12321"; 

// Set your Gateway IP address
IPAddress StaticIP(172, 20, 10, 9);
IPAddress gateway(172, 20, 10, 1);
IPAddress subnet(255, 255, 255, 240);
IPAddress dns(172, 20, 10, 1);

// Create an instance of the web server
AsyncWebServer server(80);

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <title></title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .button {
        background-color: #2f4468;
        border: none;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
      img {  width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
    </style>
  </head>
  <body>
    <table>
      <tr>
        <td colspan="3" align="center">
          <button class="button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Forward</button>
        </td>
      </tr>
      <tr>
        <td align="center">
          <button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Left</button>
        </td>
        <td align="center">
          <button class="button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');">Stop</button>
        </td>
        <td align="center">
          <button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Right</button>
        </td>
      </tr>
      <tr>
        <td colspan="3" align="center">
          <button class="button" onmousedown="toggleCheckbox('backward');" ontouchstart="toggleCheckbox('backward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Backward</button>
        </td>
      </tr>
    </table>
    <script>
      function toggleCheckbox(x) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/action?go=" + x, true);
        xhr.onreadystatechange = function() {
          if (xhr.readyState == 4 && xhr.status == 200) {
            console.log("Command sent to Arduino:", x);
          }
        };
        xhr.send();
      }
      window.onload = function() {
        document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
      };
    </script>
  </body>
</html>
)rawliteral";

void handleRoot(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", INDEX_HTML);
}

void handleAction(AsyncWebServerRequest *request) {
  String param = request->getParam("go")->value();
  // Serial.print("Sending command to Arduino: ");
  Serial.println(param);

  // Error handling
  if (Serial.availableForWrite() <= 0) {
    Serial.println("Error sending data to Arduino");
    request->send(500, "text/plain", "Error");
  } else {
    request->send(200, "text/plain", "OK");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (WiFi.config(StaticIP, gateway, subnet, dns) == false) {
    Serial.println("WiFi configuration failed.");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Route for root URL
  server.on("/", HTTP_GET, handleRoot);
  server.on("/action", HTTP_GET, handleAction);

  server.begin();
}

void loop() {
}
