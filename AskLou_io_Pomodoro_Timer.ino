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

//I'm specifically loading these libraries in the order they need to for startup
#include <TFT_eSPI.h>            // TFT_eSPI Library for rendering to a TFT screen
#include <lvgl.h>                // LVGL Graphics library
#define USE_TFT_ESPI_LIBRARY
#include "lv_xiao_round_screen.h"
#include "ui.h"                  // SquareLine UI
#include "ui_events.h"           // events for play/pause image buttons

// If you want local lv_conf.h, you might do: 
// #define LV_CONF_INCLUDE_SIMPLE

// 1) If you want to define the WS URL in this file:
#define WS_ADDRESS "ws://192.168.0.36:8081" 
// Or whichever IP/port you want

WebsocketClientWrapper websocketClient;

/*******************************************************
 * setup()
 *******************************************************/
//Declare your screen resolutions here
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;

//Declare your coordinate plane here, to pass these values back to LVGL
lv_coord_t touchX,touchY;

//Build your display buffers
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}


/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX = 0, touchY = 0;
    

    chsc6x_get_xy((lv_coord_t*)&touchX, (lv_coord_t*)&touchY); //Use the Zephyr CHSC6X library, which is included in the lv_xiao_round_screen.h
    bool touched = (touchX != 0 || touchY != 0); // Example condition for checking if the screen is touched
    
    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;  
    }
    else
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

       Serial.print( "Data x " );
       Serial.println( touchX );

       Serial.print( "Data y " );
       Serial.println( touchY );
    }
}

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
  
  tft.begin(); //Initialize the TFT library
  tft.setRotation(0); // Determine the orientation of the display. 0 = USB on the right, 1 = USB on top, etc.
  //Draw buffers
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );
  
  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  /*I don't use the below lines, but if your code is working with them, then idk keep cooking*/
  lv_xiao_disp_init();
  lv_xiao_touch_init(); // assume your hardware has touch
  /*I don't use the above lines, but if your code is working with them, then idk keep cooking*/

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
