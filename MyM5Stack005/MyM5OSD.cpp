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

#include <M5Stack.h>
#include "SSCI_BME280.h"
#include "MyM5Stack.h"
#include "MyM5OSD.h"
#include "MyM5Utils.h"
#include "TFTClock.h"

/***********************************************************************
  Function : OSD
 ***********************************************************************/
#define OSD_X           240
#define OSD_Y             0
#define OSD_W            80
#define OSD_H           180
#define OSD_MENU_HEIGHT  10
#define OSD_CURSOR_X    OSD_X +  4
#define OSD_CURSOR_Y    OSD_Y + 20
#define OSD_MENU_X      OSD_X + 12
#define OSD_MENU_Y      OSD_Y + 20

#define OSD_BG TFT_BLACK //BG Color
#define OSD_FG TFT_GREEN //FG Color

#define MON_X          265       // Monitor Window
#define MON_Y          180
#define MON_W          320-MON_X
#define MON_H          240-MON_Y
#define MON_HEIGHT      10
#define MON_BATT_Y     MON_Y
#define MON_DHT12_T_Y  MON_Y + MON_HEIGHT
#define MON_DHT12_H_Y  MON_Y + MON_HEIGHT * 2
#define MON_BME280_T_Y MON_Y + MON_HEIGHT * 3
#define MON_BME280_H_Y MON_Y + MON_HEIGHT * 4
#define MON_BME280_P_Y MON_Y + MON_HEIGHT * 5

osd_menu_t osd_menu_id;
osd_menu_t osd_get_menu_id()
{
    return osd_menu_id;
}
int osd_set_menu_id(osd_menu_t menu_id)
{
    osd_menu_id = menu_id;
    return ERR_OK;
}

bool osd_fullscreen_draw;
bool osd_fullscreen_draw_get()
{
    return osd_fullscreen_draw;
}
void osd_fullscreen_draw_set(bool mode)
{
    osd_fullscreen_draw = mode;
}

int osd_open(osd_menu_t menu_id)
{
    M5.Lcd.setTextColor(OSD_FG);
    M5.Lcd.fillRect(OSD_X, OSD_Y, OSD_W, OSD_H, OSD_BG);
    M5.Lcd.drawCentreString("menu", 280, 0, 2);
    osd_set_menu_id(menu_id);
    osd_print_menu();
    osd_cursor(OSD_CURSOR_PRINT);
    osd_fullscreen_draw_set(false);
}
int osd_close()
{
    osd_set_menu_id(OSD_DISABLE);
    M5.Lcd.fillRect(OSD_X, OSD_Y, OSD_W, OSD_H, OSD_BG);
}

int osd_print_menu()
{
    M5.Lcd.setTextColor(OSD_FG);
    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_CLOSE);
    M5.Lcd.print("Close");

    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_QR_TWITTER_ADDR);
    M5.Lcd.print("QR(Twitter)");

    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_QR_QIITA_ADDR);
    M5.Lcd.print("QR(Qiita)");

    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_QR_GITHUB_ADDR);
    M5.Lcd.print("QR(GitHub)");

    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_BRIGHT_UP);
    M5.Lcd.print("Bright Up");
    
    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_BRIGHT_DOWN);
    M5.Lcd.print("Bright Down");

    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_LCD_WHITE);
    M5.Lcd.print("LCD_WHITE");

    M5.Lcd.setCursor(OSD_MENU_X, OSD_MENU_Y + OSD_MENU_HEIGHT * OSD_POWER_OFF);
    M5.Lcd.print("Power OFF");
}
int osd_cursor(bool mode)
{
    osd_menu_t menu_id;
    menu_id = osd_get_menu_id();
    M5.Lcd.setCursor(OSD_CURSOR_X, OSD_CURSOR_Y + OSD_MENU_HEIGHT * menu_id);
    if(mode)
    {
        M5.Lcd.setTextColor(OSD_FG);
    }
    else
    {
        M5.Lcd.setTextColor(OSD_BG);
    }
    M5.Lcd.print(">");
}

void osd_btnA()
{
    osd_menu_t menu_id;
    menu_id = osd_get_menu_id();
    switch(menu_id)
    {
        case OSD_DISABLE:
             //無視する
             break;
        case OSD_CLOSE:
             //無視する
             break;
        default:
             osd_cursor(OSD_CURSOR_ERASE);
             osd_set_menu_id( (osd_menu_t)( (int)menu_id-1 ) );
             osd_cursor(OSD_CURSOR_PRINT);
             break;
    }
}
void osd_btnB()
{
    osd_menu_t menu_id;
    menu_id = osd_get_menu_id();
    switch(menu_id)
    {
        case OSD_DISABLE:
             //無視する
             break;
        case OSD_POWER_OFF:
             //無視する
             break;
        default:
             osd_cursor(OSD_CURSOR_ERASE);
             osd_set_menu_id( (osd_menu_t)( (int)menu_id+1 ) );
             osd_cursor(OSD_CURSOR_PRINT);
             break;
    }
}
void osd_btnC()
{
    osd_menu_t menu_id;
    menu_id = osd_get_menu_id();
    switch(menu_id)
    {
        case OSD_DISABLE:
             osd_open(OSD_CLOSE);
             break;
        case OSD_CLOSE:
             osd_close();
             break;
        case OSD_QR_TWITTER_ADDR:
        case OSD_QR_QIITA_ADDR:
        case OSD_QR_GITHUB_ADDR:
             if( osd_fullscreen_draw_get() )
             {
                 osd_fullscreen_draw_set(false);
                 tft_clock_redraw();
                 m5lcd_periodical_update_set(true);
                 osd_open(menu_id);
             }
             else
             {
                 osd_fullscreen_draw_set(true);
                 m5lcd_periodical_update_set(false);
                 M5.Lcd.fillScreen(TFT_WHITE);
                 M5.Lcd.setCursor(1,1);
                 M5.Lcd.setTextColor(TFT_BLACK);
                 if(menu_id==OSD_QR_TWITTER_ADDR)
                 {
                     M5.Lcd.print("Twitter");
                     m5lcd_qrDraw("https://twitter.com");
                 }
                 if(menu_id==OSD_QR_QIITA_ADDR)
                 {
                     M5.Lcd.print("Qiita");
                     m5lcd_qrDraw("https://qiita.com");
                 }
                 if(menu_id==OSD_QR_GITHUB_ADDR)
                 {
                     M5.Lcd.print("GitHub");
                     m5lcd_qrDraw("https://github.com");
                 }
             }
             break;
        case OSD_BRIGHT_UP:
             m5lcd_brightness_updown(MY_M5_DIR_UP);
             break;
        case OSD_BRIGHT_DOWN:
             m5lcd_brightness_updown(MY_M5_DIR_DOWN);
             break;
        case OSD_LCD_WHITE:
             if( osd_fullscreen_draw_get() )
             {
                 osd_fullscreen_draw_set(false);
                 tft_clock_redraw();
                 m5lcd_periodical_update_set(true);
                 osd_open(menu_id);
             }
             else
             {
                 osd_fullscreen_draw_set(true);
                 m5lcd_periodical_update_set(false);
                 M5.Lcd.fillScreen(TFT_WHITE);
             }
            break;
        case OSD_POWER_OFF:
             M5.powerOFF();
             //M5.WiFi.OFF
             break;
    }
}

/***********************************************************************
  Function : M5 LCD & Button
 ***********************************************************************/
int8_t m5lcd_brightness_def[] = {1,2,4,8,16,32,64,96,128,160,192,224,255};
int8_t m5lcd_brightness_step;
void m5lcd_brightness_step_set(int8_t step)
{
    if( sizeof(m5lcd_brightness_def) <= step )
    {
        m5lcd_brightness_step = sizeof(m5lcd_brightness_def) -1;
    }
    else
    {
        m5lcd_brightness_step = step;
    }
    
    M5.Lcd.setBrightness( m5lcd_brightness_def[m5lcd_brightness_step] );
}
int8_t m5lcd_brightness_step_get()
{
    return m5lcd_brightness_step;
}
void m5lcd_brightness_updown(int direction)
{
    int8_t step;
    
    if(MY_M5_DIR_UP == direction)
    {
        step = m5lcd_brightness_step_get() + 1;
        m5lcd_brightness_step_set(step);
    }
    else
    {
        step = m5lcd_brightness_step_get() - 1;
        if( step < 0 ) step = 0;
        m5lcd_brightness_step_set(step);
    }
}

void m5lcd_qrDraw(char *str)
{
    M5.Lcd.qrcode(str, 50, 10, 220, 6);
}

void m5lcd_monitor_batt()
{
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setCursor(MON_X, MON_BATT_Y);
    M5.Lcd.printf("V :%1.3f", analogRead(36)*5.0/4096);
    dacWrite(25,0);
}
int m5lcd_monitor_dht12()
{
    byte     i;
    wire_t   *wire;
    int      send_stop;

    wire = (wire_t*)malloc(sizeof(wire_t));
    if( NULL == wire )
    {
        return ERR_MALLOC;
    }
    
    wire->slave_addr = 0xb9;
    wire->reg_addr   = 0x00;
    wire->bytes      = 5;
    wire->send_stop  = true;
    wire->wait       = 0;

    wire->data = (byte*)malloc(wire->bytes);
    if( NULL == wire->data )
    {
        Serial.print("ERR_MALLOC\r\n");
        free(wire);
        return ERR_MALLOC;
    }

    wire->dbg_print = false;

    wire_get(wire);
    if(wire->data[4] == (wire->data[0] + wire->data[1] + wire->data[2] + wire->data[3]))
    {
        M5.Lcd.setTextColor(TFT_GREEN);
        M5.Lcd.setCursor(MON_X, MON_DHT12_T_Y);
        M5.Lcd.printf("Ta:%d.%d", wire->data[2], wire->data[3]);
        M5.Lcd.setCursor(MON_X, MON_DHT12_H_Y);
        M5.Lcd.printf("Ha:%d.%d", wire->data[0], wire->data[1]);
    }
    else
    {
        Serial.print(F("m5lcd_monitor_dht12 checksum error.\r\n"));
        Serial.printf("%02x %02x %02x %02x %02x\r\n", wire->data[0], wire->data[1], wire->data[2], wire->data[3], wire->data[4]);
    }

    free(wire->data);
    free(wire);

    return ERR_OK;
}
int m5lcd_monitor_bme280()
{
    extern SSCI_BME280 bme280;
    double temp_act = 0.0, press_act = 0.0,hum_act=0.0;

    bme280.readData(&temp_act, &press_act, &hum_act);

    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setCursor(MON_X, MON_BME280_T_Y);
    M5.Lcd.printf("T :%3.2f", temp_act);
    M5.Lcd.setCursor(MON_X, MON_BME280_H_Y);
    M5.Lcd.printf("H :%3.2f", hum_act);
    M5.Lcd.setCursor(MON_X, MON_BME280_P_Y);
    M5.Lcd.printf("P :%4.1f\r\n", press_act);
}

static uint32_t targetTime = 0; // for next 1 second timeout
void m5lcd_set_current_time_to_target_time()
{
    targetTime = millis();
}
void m5lcd_monitor_update()
{
    if (targetTime < millis()) {
        targetTime += 1000;

        M5.Lcd.fillRect(MON_X, MON_BATT_Y,     MON_W, MON_HEIGHT,     TFT_BLACK);
        m5lcd_monitor_batt();
        M5.Lcd.fillRect(MON_X, MON_BME280_T_Y, MON_W, MON_HEIGHT * 3, TFT_BLACK);
        m5lcd_monitor_bme280();
        M5.Lcd.fillRect(MON_X, MON_DHT12_T_Y,  MON_W, MON_HEIGHT * 2, TFT_BLACK);
        m5lcd_monitor_dht12();
    }
}

bool m5lcd_periodical_update;
void m5lcd_periodical_update_set(bool mode)
{
    m5lcd_periodical_update = mode;
}
bool m5lcd_periodical_update_get()
{
    return m5lcd_periodical_update;
}

void m5lcd_clear()
{
    M5.Lcd.fillScreen(TFT_BLACK);
}

void m5lcd_str(char *str)
{
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
    M5.Lcd.drawCentreString(str,160,120,4);
}

int m5lcd_menu()
{
    int      ret;
    uint8_t  select = 0;
    char     buf[BUF_LENGTH];

    boolean  wrap;
    int16_t  x, y, x0, y0, x1, y1, x2, y2;
    int16_t  w, h, radius, r, delta;
    int      dX, poY, font;
    
    uint8_t  ubrightness, ucornername, usize;
    uint16_t ucolor, ubg;
    uint16_t ux, uy, ux0, uy0;

    while( select != 99 )
    {
        Serial.print("----------------\r\n");
        Serial.print(" 1.setBrightness\r\n");
        Serial.print(" 2.drawPixel\r\n");
        Serial.print(" 3.drawLine\r\n");
        Serial.print(" 4.fillRect\r\n");
        Serial.print(" 5.fillScreen\r\n");
        Serial.print(" 6.drawCircle\r\n");
        Serial.print(" 7.drawCircleHelper\r\n");
        Serial.print(" 8.fillCircle\r\n");
        Serial.print(" 9.fillCircleHelper\r\n");
        Serial.print("10.drawTriangle\r\n");
        Serial.print("11.fillTriangle\r\n");
        Serial.print("12.drawRoundRect\r\n");
        Serial.print("13.fillRoundRect\r\n");
//        Serial.print("14.drawBitmap    (Unimplemented)\r\n");
//        Serial.print("15.drawRGBBitmap (Unimplemented)\r\n");
        Serial.print("16.drawChar\r\n");
        Serial.print("17.setCursor\r\n");
        Serial.print("18.setTextColor\r\n");
        Serial.print("19.setTextColor /w bgcolor\r\n");
        Serial.print("20.setTextSize\r\n");
        Serial.print("21.setTextWrap\r\n");
        Serial.print("23.print\r\n");
        Serial.print("25.drawCentreString\r\n");
        Serial.print("26.drawRightString\r\n");
//        Serial.print("27.drawJpg     (Unimplemented)\r\n");
//        Serial.print("28.drawJpgFile (Unimplemented)\r\n");
//        Serial.print("29.drawBmpFile (Unimplemented)\r\n");
        Serial.print("30.qrcode\r\n");
        Serial.print("99.exit\r\n");
        Serial.print("select : ");
        select = serial_getnum(DEC);
        Serial.print("\r\n");
        switch(select)
        {
            case 1:
                Serial.print("Brightness(0-255) : "); ubrightness = serial_getnum(DEC); Serial.print("\r\n");
                M5.Lcd.setBrightness(ubrightness);
                break;
            case 2:
                Serial.print("x     : ");   x      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y     : ");   y      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.drawPixel(x, y, ucolor);
                break;
            case 3:
                Serial.print("x0    : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0    : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("x1    : ");   x1     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y1    : ");   y1     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.drawLine(x0, y0, x1, y1, ucolor);
                break;
            case 4:
                Serial.print("x     : ");   x      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y     : ");   y      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("w     : ");   w      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("h     : ");   h      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.fillRect(x, y, w, h, ucolor);
                break;
            case 5:
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.fillScreen(ucolor);
                break;
            case 6:
                Serial.print("x0    : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0    : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("r     : ");   r      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.drawCircle(x0, y0, r, ucolor);
                break;
            case 7:
                Serial.print("x0         : ");   x0          = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0         : ");   y0          = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("r          : ");   r           = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("cornername : ");   ucornername = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color      : 0x"); ucolor      = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.drawCircleHelper(x0, y0, r, ucornername, ucolor);
                break;
            case 8:
                Serial.print("x0    : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0    : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("r     : ");   r      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.fillCircle(x0, y0, r, ucolor);
                break;
            case 9:
                Serial.print("x0         : ");   x0          = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0         : ");   y0          = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("r          : ");   r           = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("cornername : ");   ucornername = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("delta      : ");   delta       = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color      : 0x"); ucolor      = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.fillCircleHelper(x0, y0, r, ucornername, delta, ucolor);
                break;
            case 10:
                Serial.print("x0    : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0    : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("x1    : ");   x1     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y1    : ");   y1     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("x2    : ");   x2     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y2    : ");   y2     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.drawTriangle(x0, y0, x1, y1, x2, y2, ucolor);
                break;
            case 11:
                Serial.print("x0    : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0    : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("x1    : ");   x1     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y1    : ");   y1     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("x2    : ");   x2     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y2    : ");   y2     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.fillTriangle(x0, y0, x1, y1, x2, y2, ucolor);
                break;
            case 12:
                Serial.print("x0     : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0     : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("w      : ");   w      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("h      : ");   h      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("radius : ");   radius = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color  : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.drawRoundRect(x0, y0, w, h, radius, ucolor);
                break;
            case 13:
                Serial.print("x0     : ");   x0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0     : ");   y0     = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("w      : ");   w      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("h      : ");   h      = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("radius : ");   radius = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("color  : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.fillRoundRect(x0, y0, w, h, radius, ucolor);
                break;
            case 16:
                Serial.print("x     : ");   ux     = serial_getnum(DEC);     Serial.print("\r\n");
                Serial.print("y     : ");   uy     = serial_getnum(DEC);     Serial.print("\r\n");
                Serial.print("char  : ");   ret    = serial_getstr(&buf[0]); Serial.print("\r\n");
                Serial.print("color : 0x"); ucolor = serial_getnum(HEX);     Serial.print("\r\n");
                Serial.print("bg    : 0x"); ubg    = serial_getnum(HEX);     Serial.print("\r\n");
                Serial.print("size  : ");   usize  = serial_getnum(DEC);     Serial.print("\r\n");
                M5.Lcd.drawChar(ux, uy, buf[0], ucolor, ubg, usize);
                break;
            case 17:
                Serial.print("x0 : "); ux0 = serial_getnum(DEC); Serial.print("\r\n");
                Serial.print("y0 : "); uy0 = serial_getnum(DEC); Serial.print("\r\n");
                M5.Lcd.setCursor(ux0, uy0);
                break;
            case 18:
                Serial.print("TextColor : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.setTextColor(ucolor);
                break;
            case 19:
                Serial.print("TextColor : 0x"); ucolor = serial_getnum(HEX); Serial.print("\r\n");
                Serial.print("bg        : 0x"); ubg    = serial_getnum(HEX); Serial.print("\r\n");
                M5.Lcd.setTextColor(ucolor, ubg);
                break;
            case 20:
                Serial.print("TextSize : "); usize = serial_getnum(DEC); Serial.print("\r\n");
                M5.Lcd.setTextSize(usize);
                break;
            case 21:
                Serial.print("TextWrap (0 or 1): "); wrap = serial_getnum(DEC); Serial.print("\r\n");
                M5.Lcd.setTextWrap(wrap);
                break;
            case 23:
                Serial.print("str : "); ret = serial_getstr(&buf[0]); Serial.print("\r\n");
                M5.Lcd.print(buf);
                break;
            case 25:
                Serial.print("str  : "); ret  = serial_getstr(&buf[0]); Serial.print("\r\n");
                Serial.print("dX   : "); dX   = serial_getnum(DEC);     Serial.print("\r\n");
                Serial.print("poY  : "); poY  = serial_getnum(DEC);     Serial.print("\r\n");
                Serial.print("font : "); font = serial_getnum(DEC);     Serial.print("\r\n");
                M5.Lcd.drawCentreString(buf, dX, poY, font);
                break;
            case 26:
                Serial.print("str  : "); ret  = serial_getstr(&buf[0]); Serial.print("\r\n");
                Serial.print("dX   : "); dX   = serial_getnum(DEC);     Serial.print("\r\n");
                Serial.print("poY  : "); poY  = serial_getnum(DEC);     Serial.print("\r\n");
                Serial.print("font : "); font = serial_getnum(DEC);     Serial.print("\r\n");
                M5.Lcd.drawRightString(buf, dX, poY, font);
                break;
            case 30:
                Serial.print("str : "); ret = serial_getstr(&buf[0]); Serial.print("\r\n");
                M5.Lcd.qrcode(buf, 50, 10, 220, 6);
                break;
            case 99:
                break;
            default:
                break;
        }
    }
    return 0;
}

int m5btn_menu()
{
    uint8_t  select = 0;

    while( select != 99 )
    {
        Serial.print("----------------\r\n");
        Serial.print(" 1.BtnA/B/C.read\r\n");
        Serial.print("99.exit\r\n");
        Serial.print("select : ");
        select = serial_getnum(DEC);
        Serial.print("\r\n");
        switch(select)
        {
            case 1:
                M5.Lcd.fillScreen(0xffff); // white
                M5.Lcd.setCursor(10, 10); M5.Lcd.print("BtnA:"); M5.Lcd.println( M5.BtnA.read() );
                M5.Lcd.setCursor(10, 30); M5.Lcd.print("BtnB:"); M5.Lcd.println( M5.BtnB.read() );
                M5.Lcd.setCursor(10, 50); M5.Lcd.print("BtnC:"); M5.Lcd.println( M5.BtnC.read() );
                break;
            case 99:
                break;
            default:
                break;
        }
    }
    return 0;
}
