// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: asklouTimer

#include "ui.h"

void ui_Start_screen_init(void)
{
ui_Start = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Start, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_StartArc = lv_arc_create(ui_Start);
lv_obj_set_width( ui_StartArc, 225);
lv_obj_set_height( ui_StartArc, 210);
lv_obj_set_x( ui_StartArc, 8 );
lv_obj_set_y( ui_StartArc, 1 );
lv_obj_set_align( ui_StartArc, LV_ALIGN_CENTER );
lv_arc_set_value(ui_StartArc, 50);

lv_obj_set_style_arc_color(ui_StartArc, lv_color_hex(0x3BB77E), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_StartArc, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_StartArc, lv_color_hex(0x3BB77E), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_StartArc, 255, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_TimerLabel = lv_label_create(ui_Start);
lv_obj_set_width( ui_TimerLabel, 39);
lv_obj_set_height( ui_TimerLabel, LV_SIZE_CONTENT);   /// 25
lv_obj_set_x( ui_TimerLabel, 1 );
lv_obj_set_y( ui_TimerLabel, -6 );
lv_obj_set_align( ui_TimerLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_TimerLabel,"00:00");

ui_PlayImgButton = lv_imgbtn_create(ui_Start);
lv_imgbtn_set_src(ui_PlayImgButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_play_button_png, NULL);
lv_obj_set_height( ui_PlayImgButton, 42);
lv_obj_set_width( ui_PlayImgButton, LV_SIZE_CONTENT);  /// 1
lv_obj_set_x( ui_PlayImgButton, 0 );
lv_obj_set_y( ui_PlayImgButton, 64 );
lv_obj_set_align( ui_PlayImgButton, LV_ALIGN_CENTER );


}