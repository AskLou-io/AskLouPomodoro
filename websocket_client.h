#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <WiFi.h>
#include <WebSocketsClient.h>
#include "pomodoro.h"   // to call start_pomodoro(...) / stop_pomodoro(...)

// If your UI objects (ui_Start, ui_Pause, etc.) need to be referenced here:
#include "ui.h"         // so we can switch screens by lv_scr_load(ui_Pause), etc.

/**
 * A simple WebSocket client wrapper that connects to a given ws:// address
 * and listens for commands: "start" or "stop".
 */
class WebsocketClientWrapper {
public:
    void begin(const char* wsAddress);
    void loop();
private:
    WebSocketsClient webSocket;
};

#endif // WEBSOCKET_CLIENT_H
