#include "pomodoro.h"
#include "ui.h" // so we can do lv_scr_load(ui_Pause), etc.
#include <Arduino.h>

// For demonstration, we use a static lv_timer:
static lv_timer_t* s_pomodoroTimer = nullptr;
static int s_remainingSecs = 0;
static const int POMODORO_DURATION = 1500; // 25 minutes

// Forward-declared
static void pomodoro_timer_cb(lv_timer_t* timer);

void start_pomodoro(int totalSeconds)
{
    Serial.printf("start_pomodoro(%d)\n", totalSeconds);

    // If a timer is running, kill it
    if(s_pomodoroTimer) {
      lv_timer_del(s_pomodoroTimer);
      s_pomodoroTimer = nullptr;
    }
    s_remainingSecs = totalSeconds;

    // Switch to Pause screen
    lv_scr_load(ui_Pause);

    // For clarity, set the PauseArc and PauseTimerLabel to initial state
    // e.g. 25:00 and Arc = 0
    lv_label_set_text(ui_PauseTimerLabel, "25:00");
    lv_arc_set_value(ui_PauseArc, 0);

    // Create a 1-second timer
    s_pomodoroTimer = lv_timer_create(pomodoro_timer_cb, 1000, NULL);
}

void stop_pomodoro(void)
{
    Serial.println("stop_pomodoro() called");

    // Cancel the timer
    if(s_pomodoroTimer) {
      lv_timer_del(s_pomodoroTimer);
      s_pomodoroTimer = nullptr;
    }
    s_remainingSecs = 0;

    // Switch back to Start screen
    lv_scr_load(ui_Start);
    // Reset StartArc and TimerLabel if you like:
    lv_label_set_text(ui_TimerLabel, "00:00");
    lv_arc_set_value(ui_StartArc, 0);
}

// Timer callback => decrement s_remainingSecs
static void pomodoro_timer_cb(lv_timer_t* timer)
{
    if(s_remainingSecs > 0) {
        s_remainingSecs--;

        // Update arc & label on Pause screen
        // Example: the arc is from 0..100 => 
        //   used = (POMODORO_DURATION - s_remainingSecs)
        //   pct  = used * 100 / POMODORO_DURATION
        int used = (POMODORO_DURATION - s_remainingSecs);
        int pct  = (used * 100) / POMODORO_DURATION;
        lv_arc_set_value(ui_PauseArc, pct);

        // Format mm:ss
        int mm = s_remainingSecs / 60;
        int ss = s_remainingSecs % 60;
        char buf[8];
        snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);
        lv_label_set_text(ui_PauseTimerLabel, buf);
    } else {
        // Time’s up => Switch to Rewind
        lv_scr_load(ui_Rewind);

        // For Rewind screen, set 00:00 or arc=100
        lv_label_set_text(ui_RewindTimerLabel, "00:00");
        lv_arc_set_value(ui_RewindArc, 100);

        // Cancel the timer
        lv_timer_del(s_pomodoroTimer);
        s_pomodoroTimer = nullptr;
    }
}
