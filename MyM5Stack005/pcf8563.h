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

#ifndef _PCF8563_H_
#define _PCF8563_H_

/***********************************************************************
  Function Prototype : Real-time Clock PCF8563
 ***********************************************************************/
typedef struct {
    byte slave_addr;               // 0xA2(Write), 0xA3(Read)
    bool send_stop;

    /* Control and status registers */
    byte control_status_reg_addr;  // 0x00
    byte control_status_reg_bytes; // 2bytes
    byte control_status_1;
    byte control_status_2;
    
    /* Time and date registers */
    byte time_reg_addr;            // 0x02
    byte time_reg_bytes;           // 7bytes(0x02-0x08)
    byte vl;                       // VL_seconds register, bit7
    byte seconds;                  // VL_seconds register, bit6-0
    byte minutes;
    byte hours;
    byte days;
    byte weekdays;
    byte century;                  // Century_months register, bit7
    byte months;                   // Century_months register, bit4-0
    byte years;

    /* Alarm registers */
    byte alarm_reg_addr;           // 0x09
    byte alarm_reg_bytes;          // 4bytes
    byte minute_alarm;
    byte hour_alarm;
    byte day_alarm;
    byte weekday_alarm;

    /* CLKOUT control register */
    byte clkout_reg_addr;          // 0x0d
    byte clkout_reg_bytes;         // 1byte
    byte clkout_control;

    /* Timer registers */
    byte timer_reg_addr;           // 0x0e
    byte timer_reg_bytes;          // 2bytes
    byte timer_control;
    byte timer;
}pcf8563_t;

pcf8563_t *pcf8563_open();
int pcf8563_close(pcf8563_t *pcf8563);
int pcf8563_print(pcf8563_t *pcf8563);
int pcf8563_get_time(pcf8563_t *pcf8563);
int pcf8563_set_time(pcf8563_t *pcf8563);

#endif

