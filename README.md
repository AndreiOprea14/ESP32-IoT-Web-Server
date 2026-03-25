#  ESP32 Dual-Interface Web Server (Wi-Fi & Bluetooth)

This project turns an ESP32 microcontroller into a standalone, asynchronous web server that hosts and serves a structured JSON database (in this case, a collection of art pieces and their image URLs). 

I built this to explore IoT connectivity, specifically focusing on creating a dual-communication architecture and dynamic network provisioning.

##  Key Features
* **Dual Control Interfaces:** You can interact with the ESP32 either through standard HTTP REST requests (over Wi-Fi) or via Serial text commands (over Bluetooth).
* **Dynamic Wi-Fi Provisioning:** No hardcoded Wi-Fi passwords! You can scan for available networks and send the SSID/Password remotely to connect the board on the fly.
* **Asynchronous Web Server:** The server doesn't block the main loop while handling multiple client requests.
* **JSON Parsing:** Uses `ArduinoJson` to efficiently search the internal database and serve specific entries on demand.

##  Tech Stack & Libraries
* **Hardware:** ESP32 Development Board
* **Language:** C++ (Arduino IDE)
* **Libraries used:** * `WiFi.h` & `BluetoothSerial.h` (Core connectivity)
  * `ESPAsyncWebServer` (For non-blocking HTTP handling)
  * `ArduinoJson` (For parsing and formatting API responses)

##  The API Endpoints (Wi-Fi)
If the ESP32 is connected to the local network, you can hit these endpoints from your browser or Postman:
* `GET /getNetworks` -> Scans the area and returns a JSON list of available Wi-Fi networks and their signal strengths.
* `POST /connect` -> Accepts `ssid` and `password` parameters to dynamically connect the board to a network.
* `GET /dataList` -> Returns the complete JSON payload containing all the art pieces.
* `GET /dataEntry?id=X` -> Searches the database and returns the details for a specific object ID.

##  Bluetooth Commands
If Wi-Fi goes down (or isn't configured yet), the ESP32 listens via Bluetooth (`ESP32_BT`) for these serial text commands:
* `CONNECT_TO_WIFI <ssid> <password>`
* `GET_DATA`
* `GET_DETAILS <id>`


This project was a great dive into how IoT devices manage remote configuration and handle data formatting for client applications.
