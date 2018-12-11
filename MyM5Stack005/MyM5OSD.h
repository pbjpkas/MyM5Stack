/***********************************************************************
 * Copyright 2018 ka's@pbjpkas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***********************************************************************/

#ifndef _MYM5OSD_H_
#define _MYM5OSD_H_

#define MY_M5_DIR_UP   1 // Direction, UP
#define MY_M5_DIR_DOWN 2 // Direction, Down

#define OSD_CURSOR_PRINT true
#define OSD_CURSOR_ERASE false

enum osd_menu_tag
{
    OSD_DISABLE,
    OSD_CLOSE,
    OSD_QR_TWITTER_ADDR,
    OSD_QR_QIITA_ADDR,
    OSD_QR_GITHUB_ADDR,
    OSD_BRIGHT_UP,
    OSD_BRIGHT_DOWN,
    OSD_LCD_WHITE,
    OSD_POWER_OFF,

    OSD_MENU_NUM
};

typedef enum osd_menu_tag osd_menu_t;

osd_menu_t osd_get_menu_id();
int  osd_set_menu_id(osd_menu_t menu_id);
int  osd_open(osd_menu_t);
int  osd_close();
int  osd_print_menu();
int  osd_cursor(bool mode);
void osd_btnA();
void osd_btnB();
void osd_btnC();

/***********************************************************************
  Function Prototype : M5 LCD & Button
 ***********************************************************************/
int    m5lcd_menu();
void   m5lcd_brightness_step_set(int8_t step);
int8_t m5lcd_brightness_step_get();
void   m5lcd_brightness_updown(int direction);
void   m5lcd_qrDraw(char *str);
void   m5lcd_set_current_time_to_target_time();
void   m5lcd_monitor_update();
void   m5lcd_periodical_update_set(bool mode);
bool   m5lcd_periodical_update_get();
void   m5lcd_clear();
void   m5lcd_str(char *str);

int    m5btn_menu();

#endif
