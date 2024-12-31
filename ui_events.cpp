#include <Arduino.h>
#include "lvgl.h"
#include "ui.h"
#include "ui_events.h"
#include "pomodoro.h" // so we can call start_pomodoro, stop_pomodoro

void PlayImgButton_event_cb(lv_event_t* e)
{
  if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
    Serial.println("User tapped Play => start 25-min");
    start_pomodoro(1500);
  }
}

void PauseImageButton_event_cb(lv_event_t* e)
{
  if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
    Serial.println("User tapped Pause => stop pomodoro");
    stop_pomodoro();
  }
}

void RewindImageButton_event_cb(lv_event_t* e)
{
  if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
    Serial.println("User tapped Rewind => go back to Start");
    lv_scr_load(ui_Start);
  }
}
