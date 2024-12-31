#include "websocket_client.h"
#include <Arduino.h>
#include "pomodoro.h" // start_pomodoro(...), stop_pomodoro(...)
#include "ui.h"       // If you want to do lv_scr_load(...) from here

void WebsocketClientWrapper::begin(const char* wsAddress)
{
    Serial.print("[WS] begin => ");
    Serial.println(wsAddress);

    // parse the host + port from "ws://"
    String address(wsAddress);
    int colonIndex = address.indexOf(':', 5); // after "ws://"
    if(colonIndex < 0) {
      Serial.println("[WS] Invalid address, e.g. ws://192.168.0.36:8081");
      return;
    }
    String host = address.substring(5, colonIndex);
    int port    = address.substring(colonIndex + 1).toInt();

    webSocket.begin(host.c_str(), port, "/");
    webSocket.onEvent([this](WStype_t type, uint8_t *payload, size_t length) {
        switch(type) {
            case WStype_CONNECTED:
                Serial.println("[WS] Connected to server");
                webSocket.sendTXT("ESP32 connected");
                break;

            case WStype_DISCONNECTED:
                Serial.println("[WS] Disconnected");
                break;

            case WStype_TEXT: {
                String message((char*)payload, length);
                message.trim();
                Serial.printf("[WS] Received: '%s'\n", message.c_str());

                if(message.equalsIgnoreCase("start")) {
                    start_pomodoro(1500);
                    // If you prefer, do: lv_scr_load(ui_Pause);
                }
                else if(message.equalsIgnoreCase("stop")) {
                    stop_pomodoro();
                    // lv_scr_load(ui_Start);
                }
                else {
                    Serial.printf("[WS] Unrecognized command: %s\n", message.c_str());
                }
                break;
            }
            default:
                // handle WStype_BIN, WStype_ERROR, etc. if needed
                break;
        }
    });
}

void WebsocketClientWrapper::loop()
{
    webSocket.loop();
}
