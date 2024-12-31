#ifndef POMODORO_H
#define POMODORO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>

/** 
 * Called when we want to start a pomodoro countdown (e.g. 25min=1500).
 * Typically:
 *   - Switch to Pause screen
 *   - Create 1-second lv_timer to decrement
 */
void start_pomodoro(int totalSeconds);

/** 
 * Called when we want to stop or reset the pomodoro.
 * Typically:
 *   - Cancel the lv_timer
 *   - Switch back to Start screen
 */
void stop_pomodoro(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // POMODORO_H
