void setup() {
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

AsyncWebServer server(80);
BluetoothSerial SerialBT;

const char* jsonData = R"rawliteral(
{
  "data": [
    {
objectID: 437307,
title: "Poplars, Éragny",
imgUrl: https://bogdanflorea.ro/images/met/437307.png
},
{
objectID: 437651,
title: "Saint John the Evangelist",
imgUrl: https://bogdanflorea.ro/images/met/437651.png
},
{
objectID: 437517,
title: "An Early Summer Morning in the Forest of Fontainebleau",
imgUrl: https://bogdanflorea.ro/images/met/437517.png
},
{
objectID: 435884,
title: "Still Life with Jar, Cup, and Apples",
imgUrl: https://bogdanflorea.ro/images/met/435884.png
},
{
objectID: 436413,
title: "Christ Bearing the Cross",
imgUrl: https://bogdanflorea.ro/images/met/436413.png
},
{
objectID: 436187,
title: "A Dragoon on Horseback",
imgUrl: https://bogdanflorea.ro/images/met/436187.png
},
{
objectID: 437447,
title: "Captain George K. H. Coussmaker (1759–1801)",
imgUrl: https://bogdanflorea.ro/images/met/437447.png
},
{
objectID: 435990,
title: "A Woman Gathering Faggots at Ville-d'Avray",
imgUrl: https://bogdanflorea.ro/images/met/435990.png
},
{
objectID: 437433,
title: "The Farm at Les Collettes, Cagnes",
imgUrl: https://bogdanflorea.ro/images/met/437433.png
},
{
objectID: 437001,
title: "The Conquest of Naples by Charles of Durazzo",
imgUrl: https://bogdanflorea.ro/images/met/437001.png
},
{
objectID: 436677,
title: "Paying the Hostess",
imgUrl: https://bogdanflorea.ro/images/met/436677.png
},
{
objectID: 437802,
title: "Neptune and the Winds",
imgUrl: https://bogdanflorea.ro/images/met/437802.png
},
{
objectID: 437472,
title: "Arches in Ruins",
imgUrl: https://bogdanflorea.ro/images/met/437472.png
},
{
objectID: 436638,
title: "Jacob Willemsz van Veen (1456–1535), the Artist's Father",
imgUrl: https://bogdanflorea.ro/images/met/436638.png
},
{
objectID: 435952,
title: "Portrait of a Man",
imgUrl: https://bogdanflorea.ro/images/met/435952.png
},
{
objectID: 437917,
title: "Portrait of a Woman",
imgUrl: https://bogdanflorea.ro/images/met/437917.png
},
{
objectID: 437157,
title: "Christ in the Wilderness",
imgUrl: https://bogdanflorea.ro/images/met/437157.png
},
{
objectID: 437074,
title: "A Cavalry Engagement",
imgUrl: https://bogdanflorea.ro/images/met/437074.png
},
{
objectID: 437585,
title: "Market by the Seashore",
imgUrl: https://bogdanflorea.ro/images/met/437585.png
},
{
objectID: 436345,
title: "Still Life with Strawberries",
imgUrl: https://bogdanflorea.ro/images/met/436345.png
}
  ]
}
)rawliteral";

String teamId = "";

void handleGetNetworks(AsyncWebServerRequest *request);
void handleConnect(AsyncWebServerRequest *request);
void handleDataListRequest(AsyncWebServerRequest *request);
void handleDataEntryRequest(AsyncWebServerRequest *request);
void handleBluetoothCommands();

void scanWiFiNetworks() {
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(")");
      delay(10);
    }
  }
}

void connectToWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleGetNetworks(AsyncWebServerRequest *request) {
  if (request->hasParam("teamId")) {
    teamId = request->getParam("teamId")->value();
    int n = WiFi.scanNetworks();
    DynamicJsonDocument doc(4096);
    JsonArray networks = doc.to<JsonArray>();
    for (int i = 0; i < n; ++i) {
      JsonObject network = networks.createNestedObject();
      network["ssid"] = WiFi.SSID(i);
      network["strength"] = WiFi.RSSI(i);
      network["encryption"] = WiFi.encryptionType(i);
      network["teamId"] = teamId;
    }
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  } else {
    request->send(400, "application/json", "{\"error\": \"Missing teamId parameter\"}");
  }
}

void handleConnect(AsyncWebServerRequest *request) {
  if (request->hasParam("ssid") && request->hasParam("password")) {
    String ssid = request->getParam("ssid")->value();
    String password = request->getParam("password")->value();

    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    DynamicJsonDocument doc(1024);
    doc["ssid"] = ssid;
    doc["connected"] = (WiFi.status() == WL_CONNECTED);
    doc["teamId"] = teamId;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  } else {
    request->send(400, "application/json", "{\"error\": \"Missing ssid or password parameter\"}");
  }
}

void handleDataListRequest(AsyncWebServerRequest *request) {
  request->send(200, "application/json", jsonData);
}

void handleDataEntryRequest(AsyncWebServerRequest *request) {
  if (request->hasParam("id")) {
    int id = request->getParam("id")->value().toInt();
    DynamicJsonDocument doc(4096); 
    DeserializationError error = deserializeJson(doc, jsonData);
    if (error) {
      request->send(500, "application/json", "{\"error\": \"Internal server error\"}");
      return;
    }
    for (JsonObject item : doc["data"].as<JsonArray>()) {
      if (item["objectID"] == id) {
        String response;
        serializeJson(item, response);
        request->send(200, "application/json", response);
        return;
      }
    }
    request->send(404, "application/json", "{\"error\": \"Data not found\"}");
  } else {
    request->send(400, "application/json", "{\"error\": \"Invalid request\"}");
  }
}

void handleBluetoothCommands() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    Serial.println("Received command: " + command);

    if (command.startsWith("CONNECT_TO_WIFI")) {
      int ssidStart = command.indexOf(' ') + 1;
      int ssidEnd = command.indexOf(' ', ssidStart);
      String ssid = command.substring(ssidStart, ssidEnd);
      String password = command.substring(ssidEnd + 1);
      connectToWiFi(ssid.c_str(), password.c_str());
    } else if (command.startsWith("GET_DATA")) {
      SerialBT.println(jsonData);
    } else if (command.startsWith("GET_DETAILS")) {
      int idStart = command.indexOf(' ') + 1;
      int id = command.substring(idStart).toInt();

      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, jsonData);
      if (error) {
        SerialBT.println("{\"error\": \"Internal server error\"}");
        return;
      }
      for (JsonObject item : doc["data"].as<JsonArray>()) {
        if (item["objectID"] == id) {
          String response;
          serializeJson(item, response);
          SerialBT.println(response);
          return;
        }
      }
      SerialBT.println("{\"error\": \"Data not found\"}");
    }
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); 

  scanWiFiNetworks();

  connectToWiFi(ssid, password);

  server.on("/getNetworks", HTTP_GET, handleGetNetworks);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/dataList", HTTP_GET, handleDataListRequest);
  server.on("/dataEntry", HTTP_GET, handleDataEntryRequest);

  server.begin();
}

void loop() {
  handleBluetoothCommands();
}