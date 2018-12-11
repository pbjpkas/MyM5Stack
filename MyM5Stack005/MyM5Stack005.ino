/***********************************************************************
 * MyM5Stack
 * 2018-07-01 by ka's
 *
 * 2018-07-04 import Command Line Interface from arduinoLeonardoToolsV2
 *            https://qiita.com/pbjpkas/items/97dbf835b0aab6725e94
 *
 * 2018-07-16 add PCF8563 RTC Module and wire read/write
 *            https://qiita.com/pbjpkas/items/cc3bf43a5f9fd2c3415d
 *
 * 2018-07-22 add M5.Lcd.* Command Operation Function
 *            https://qiita.com/pbjpkas/items/62f625e21312560c26ce
 *
 ***********************************************************************/
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

#include <driver/adc.h>
#include <M5Stack.h>
#include <Wire.h>
#include "SSCI_BME280.h"
#include "MyM5Stack.h"
#include "MyM5Utils.h"
#include "MyM5OSD.h"
#include "TFTClock.h"
#include "pcf8563.h"


/***********************************************************************
  Function Prototype : setup and loop
 ***********************************************************************/
extern SSCI_BME280 bme280;
void setup();
void loop();

/***********************************************************************
  Function Prototype : Command Mode
 ***********************************************************************/
#define CMD_QUIT             1
#define CMD_OK          ERR_OK
#define CMD_BUF_LENGTH      64 // 63+1
#define CMD_MAX_LENGTH      64 // 63+1
#define ARG_MAX_LENGTH      64 // 63+1

void cmd_print_help(void);
void cmd_print_ver(void);
int  cmd_wire_get(void);
int  cmd_wire_set(void);
int  cmd_execute(char *buf);
void cmd_rx_data(void);

/***********************************************************************
  Function : setup and loop
 ***********************************************************************/
// the setup routine runs once when M5Stack starts up
void setup(){

    // Initialize the M5Stack object
    M5.begin();
    M5.Speaker.setVolume(0);
    Serial.begin(115200);
//    WiFi.mode(WIFI_OFF); // WiFi off

    Wire.begin();

    bme280.begin(); // BME280の初期化
    
    // Battery Voltage
    pinMode(36, INPUT);

    m5lcd_brightness_step_set(6);
    M5.setWakeupButton(BUTTON_C_PIN);

    osd_set_menu_id(OSD_DISABLE);

    m5lcd_periodical_update_set(true);
    tft_clock_get_current_time();
    tft_clock_analog_setup();
    tft_clock_ymd();
}

// the loop routine runs over and over again forever
void loop() {

    if( m5lcd_periodical_update_get() )
    {
        m5lcd_monitor_update();
        tft_clock_analog_update();
    }

    if(Serial.available()) {
        cmd_rx_data();
        tft_clock_get_current_time();
        tft_clock_analog_setup();
        tft_clock_ymd();
        tft_clock_analog_update();
        m5lcd_set_current_time_to_target_time();
    }

    if(M5.BtnA.wasPressed())
    {
        osd_btnA();
    }
    if(M5.BtnB.wasPressed())
    {
        osd_btnB();
    }
    if(M5.BtnC.wasPressed())
    {
        osd_btnC();
    }
    M5.update();

}

/***********************************************************************
  Function : Command Mode
 ***********************************************************************/
void cmd_print_help(void)
{
    Serial.print(F("Available Command:\r\n"));
    Serial.print(F("help, ?     : print Help Messages\r\n"));
    Serial.print(F("ver         : print Version Information\r\n"));
    Serial.print(F("typesize    : print type's size\r\n"));
    Serial.print(F("wireget     : get data from I2C device\r\n"));
    Serial.print(F("wireset     : set data to   I2C device\r\n"));
    Serial.print(F("pcf8563     : Real-time Clock PCF8563 function\r\n"));
    Serial.print(F("  pcf8563 print - print clock value\r\n"));
    Serial.print(F("  pcf8563 set   - set   clock value\r\n"));
    Serial.print(F("m5lcd       : M5Stack LCD function\r\n"));
    Serial.print(F("  m5lcd menu  - M5Stack LCD Menu\r\n"));
    Serial.print(F("  m5lcd clear - M5Stack LCD Clear\r\n"));
    Serial.print(F("  m5lcd str <strings> - M5Stack LCD print strings\r\n"));
    Serial.print(F("m5btn       : M5Stack Btn function\r\n"));
    Serial.print(F("m5adc1      : M5Stack ADC1 function\r\n"));
    Serial.print(F("quit, exit  : Quit Command Control Mode\r\n"));
}
void cmd_print_ver(void)
{
    Serial.print("This is ");
    Serial.print(__FILE__);
    Serial.print(" ");
    Serial.print("Build at ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.print(__TIME__);
    Serial.print("\r\n");
}

int cmd_wire_get(void)
{
    byte i;
    wire_t *wire;
    int send_stop;

    wire = (wire_t*)malloc(sizeof(wire_t));
    if( NULL == wire )
    {
        return ERR_MALLOC;
    }
    
    Serial.print(F(    "Slave Addr         : 0x")); wire->slave_addr = serial_getnum(HEX);
    Serial.print(F("\r\nReg   Addr         : 0x")); wire->reg_addr   = serial_getnum(HEX);
    Serial.print(F("\r\nBytes              : 0d")); wire->bytes      = serial_getnum(DEC);
    Serial.print(F("\r\nSend Stop (0 or 1) : "  )); send_stop        = serial_getnum(DEC);
    Serial.print(F("\r\nwait(ms)           : "  )); wire->wait       = serial_getnum(DEC);
    if(send_stop)
    {
        wire->send_stop = true;
    }
    else
    {
        wire->send_stop = false;
    }
    Serial.print("\r\n");

    wire->data = (byte*)malloc(wire->bytes);
    if( NULL == wire->data )
    {
        Serial.print("ERR_MALLOC\r\n");
        free(wire);
        return ERR_MALLOC;
    }

    wire->dbg_print = false;

    wire_get(wire);
    for(i=0; i<wire->bytes; i++)
    {
        Serial.print("0x" ); Serial.print(wire->data[i], HEX);
        Serial.print(" 0d"); Serial.print(wire->data[i], DEC);
        Serial.print(" 0b"); Serial.print(wire->data[i], BIN);
        Serial.print("\r\n");
    }
    
    free(wire->data);
    free(wire);

    return ERR_OK;
}
int cmd_wire_set(void)
{
    byte i;
    wire_t *wire;

    wire = (wire_t*)malloc(sizeof(wire_t));
    if( NULL == wire )
    {
        return ERR_MALLOC;
    }

    Serial.print(F(    "Slave Addr : 0x")); wire->slave_addr = serial_getnum(HEX);
    Serial.print(F("\r\nReg   Addr : 0x")); wire->reg_addr   = serial_getnum(HEX);
    Serial.print(F("\r\nBytes      : 0d")); wire->bytes      = serial_getnum(DEC);
    Serial.print(F("\r\n"));

    wire->data = (byte*)malloc(wire->bytes);
    if( NULL == wire->data )
    {
        Serial.print(F("ERR_MALLOC\r\n"));
        free(wire);
        return ERR_MALLOC;
    }
    for(i=0; i< wire->bytes; i++)
    {
        Serial.print(F("Data       : 0x"));
        wire->data[i] = serial_getnum(HEX);
        Serial.print(F("\r\n"));
    }
    
    wire->dbg_print = false;

    wire_set(wire);

    free(wire->data);
    free(wire);

    return ERR_OK;
}

int cmd_execute(char *buf)
{
    int i, x, y;
    unsigned int ux;
    int return_val = CMD_OK;
    char cmd[CMD_MAX_LENGTH];
    char arg1[ARG_MAX_LENGTH];
    char arg2[ARG_MAX_LENGTH];

    strcpy(cmd, "");
    strcpy(arg1, "");
    strcpy(arg2, "");
    sscanf(buf, "%s %s %s", &cmd, &arg1, &arg2);

    if     (strcmp(cmd, "help")==0){ cmd_print_help(); }
    else if(strcmp(cmd, "?"   )==0){ cmd_print_help(); }
    else if(strcmp(cmd, "ver" )==0){ cmd_print_ver();  }

    else if((strcmp(cmd, "quit")==0) or (strcmp(cmd, "exit")==0))
    {
        tft_clock_redraw();
        return CMD_QUIT;
    }

    else if(strcmp(cmd, "m5adc1")==0)
    {
        int val;
        val = analogRead(36);
        Serial.printf("%d, %1.3f\r\n", val, val*5.0/4096);
    }

    else if(strcmp(cmd, "m5lcd")==0)
    {
        if(strcmp(arg1, "menu")==0)
        {
            m5lcd_menu();
        }
        else if(strcmp(arg1, "clear")==0)
        {
            m5lcd_clear();
        }
        else if(strcmp(arg1, "str")==0)
        {
            m5lcd_str(&arg2[0]);
        }
        else
        {
            return ERR_INVALID;
        }
    }

    else if(strcmp(cmd, "m5btn")==0)
    {
        m5btn_menu();
    }

    else if(strcmp(cmd, "wireget")==0)
    {
        return_val = cmd_wire_get();
    }
    else if(strcmp(cmd, "wireset")==0)
    {
        return_val = cmd_wire_set();
    }

    else if(strcmp(cmd, "pcf8563")==0)
    {
        if(strcmp(arg1, "print")==0)
        {
            int i;
            pcf8563_t *pcf8563;
            pcf8563 = pcf8563_open();
            if( NULL == pcf8563 ){ return ERR_NULL; }

            for(i=0; i<10; i++)
            {
                pcf8563_get_time(pcf8563);
                pcf8563_print(pcf8563);
                delay(1000);
            }

            pcf8563_close(pcf8563);
        }
        else if(strcmp(arg1, "set")==0)
        {
            pcf8563_t *pcf8563;
            pcf8563 = pcf8563_open();
            if( NULL == pcf8563 ){ return ERR_NULL; }
    
            Serial.print(F(    "YY : ")); pcf8563->years   = serial_getnum(DEC);
            Serial.print(F("\r\nMM : ")); pcf8563->months  = serial_getnum(DEC);
            Serial.print(F("\r\nDD : ")); pcf8563->days    = serial_getnum(DEC);
            Serial.print(F("\r\nhh : ")); pcf8563->hours   = serial_getnum(DEC);
            Serial.print(F("\r\nmm : ")); pcf8563->minutes = serial_getnum(DEC);
            Serial.print(F("\r\nss : ")); pcf8563->seconds = serial_getnum(DEC);
            Serial.print(F("\r\n"));
            pcf8563_set_time(pcf8563);
    
            pcf8563_close(pcf8563);
        }
        else
        {
            return ERR_INVALID;
        }
    }

    else if(strcmp(cmd, "typesize")==0)
    {
        Serial.print(F(    "char      : ")); Serial.print(F(sizeof(char)));
        Serial.print(F("\r\nshort     : ")); Serial.print(F(sizeof(short)));
        Serial.print(F("\r\nint       : ")); Serial.print(F(sizeof(int)));
        Serial.print(F("\r\nlong      : ")); Serial.print(F(sizeof(long)));
        Serial.print(F("\r\nlong long : ")); Serial.print(F(sizeof(long long)));
        Serial.print(F("\r\nfloat     : ")); Serial.print(F(sizeof(float)));
        Serial.print(F("\r\ndouble    : ")); Serial.print(F(sizeof(double)));
        Serial.print(F("\r\n"));
    }

    else
    {
        return ERR_INVALID;
    }

    return return_val;

}
void cmd_rx_data(void)
{
    int i;
    int return_val = CMD_OK;
    char buf[CMD_BUF_LENGTH];

    /* モード切替時の "Hit any key" のキー操作を捨てる */
    while(Serial.available()){ Serial.read(); }

    Serial.print(F("\r\n### Command Mode. ###\r\n"));
    Serial.print(F("### Hit ? to help.###\r\n"));
    Serial.print(F("$"));

    i=0;
    while(1)
    {
        if(Serial.available())
        {
            buf[i] = Serial.read();
            Serial.print(buf[i]); //echo-back

            if ( (buf[i] == 0x08) or (buf[i] == 0x7f) ) //BackSpace, Delete
            {
                buf[i] = '\0';
                if(i) i--;
            }
            else if( (buf[i] == '\r') or (buf[i] == '\n') )
            {
                Serial.print( F("\r\n") );
                buf[i] = '\0';
                return_val = cmd_execute(&buf[0]);
                for(i=0; i<CMD_BUF_LENGTH; i++) buf[i] = '\0';
                i=0;

                if(return_val == CMD_QUIT)
                {
                    Serial.print(F("### Quit Command Mode. ###\r\n"));
                    return;
                }
                else if(return_val == ERR_INVALID)
                {
                    Serial.print(F("?\r\n"));
                    Serial.print(F("$"));
                }
                else
                {
                    Serial.print(F("OK\r\n$"));
                }
            }
            else
            {
                i++;
                if(i>=CMD_BUF_LENGTH)
                {
                    Serial.print(F("### CMD BUFFER FULL, CLEAR. ###\r\n"));
                    for(i=0; i<CMD_BUF_LENGTH; i++) buf[i] = '\0';
                    i=0;
                }
            }
        }
    }// while
}
