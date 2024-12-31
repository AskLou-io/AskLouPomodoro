/*******************************************************
 * AskLouPomodoro.ino
 * 
 * Integrates:
 *   - Pomodoro countdown logic
 *   - UI from SquareLine (ui_Start, ui_Pause, ui_Rewind)
 *   - Touch events (ui_events.cpp)
 *   - WebSocket commands via "websocket_client.h"
 *******************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include "credentials.h"         // has ssid, password
#include "pomodoro.h"            // has start_pomodoro(...) / stop_pomodoro(...)
#include "websocket_client.h"    // our WebsocketClientWrapper
#include "ui.h"                  // SquareLine UI
#include "ui_events.h"           // events for play/pause image buttons

// If you want local lv_conf.h, you might do: 
// #define LV_CONF_INCLUDE_SIMPLE

#define USE_TFT_ESPI_LIBRARY
#include "lv_xiao_round_screen.h"
#include <lvgl.h>

// 1) If you want to define the WS URL in this file:
#define WS_ADDRESS "ws://192.168.0.36:8081" 
// Or whichever IP/port you want

WebsocketClientWrapper websocketClient;

/*******************************************************
 * setup()
 *******************************************************/
void setup()
{
  Serial.begin(115200);
  delay(300);

  // ---- Connect WiFi ----
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // ---- Initialize LVGL + Display + Touch ----
  lv_init();
  lv_xiao_disp_init();
  lv_xiao_touch_init(); // assume your hardware has touch

  // ---- Initialize SquareLine UI (loads ui_Start by default) ----
  ui_init();

  // ---- Attach event callbacks for your image buttons ----
  lv_obj_add_event_cb(ui_PlayImgButton,    PlayImgButton_event_cb,    LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_PauseImageButton, PauseImageButton_event_cb,  LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_RewindImageButton,RewindImageButton_event_cb, LV_EVENT_CLICKED, NULL);

  // ---- Start WebSocket ----
  websocketClient.begin(WS_ADDRESS);

  Serial.println("Setup done!");
}

/*******************************************************
 * loop()
 *******************************************************/
void loop()
{
  // Let LVGL do drawing, handle animations, etc.
  lv_timer_handler();
  delay(5);

  // WebSocket loop
  websocketClient.loop();
}
