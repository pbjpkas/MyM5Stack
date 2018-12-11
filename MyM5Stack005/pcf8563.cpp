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
#include "MyM5Utils.h"
#include "pcf8563.h"

/***********************************************************************
  Function : Real-time Clock PCF8563
 ***********************************************************************/
pcf8563_t *pcf8563_open()
{
    pcf8563_t *pcf8563;
    pcf8563 = (pcf8563_t*)malloc(sizeof(pcf8563_t));
    if( NULL == pcf8563 ){ return pcf8563; }
    
    //デバイス共通
    pcf8563->slave_addr     = 0xa2;
    pcf8563->send_stop      = true;

    //Timeレジスタ
    pcf8563->time_reg_addr  = 0x02;
    pcf8563->time_reg_bytes = 7;

    return pcf8563;
}
int pcf8563_close(pcf8563_t *pcf8563)
{
    if( NULL == pcf8563 ){ return ERR_NULL; }

    free(pcf8563);
    pcf8563 = NULL;

    return ERR_OK;
}
int pcf8563_print(pcf8563_t *pcf8563)
{
    if( NULL == pcf8563 ){ return ERR_NULL; }

    Serial.print(pcf8563->years);    Serial.print(" ");
    Serial.print(pcf8563->months);   Serial.print(" ");
    Serial.print(pcf8563->weekdays); Serial.print(" ");
    Serial.print(pcf8563->days);     Serial.print(" ");
    Serial.print(pcf8563->hours);    Serial.print(" ");
    Serial.print(pcf8563->minutes);  Serial.print(" ");
    Serial.print(pcf8563->seconds);  Serial.print("\r\n");

    return ERR_OK;
}
int pcf8563_get_time(pcf8563_t *pcf8563)
{
    wire_t *wire;
    byte   *wiredata;

    if( NULL == pcf8563 ){ return ERR_NULL; }

    wire = (wire_t*)malloc(sizeof(wire_t));
    if( NULL == wire )
    {
        return ERR_MALLOC;
    }

    wiredata  = (byte*)malloc(pcf8563->time_reg_bytes);
    if( NULL == wiredata )
    {
        free(wire);
        return ERR_MALLOC;
    }

    wire->slave_addr = pcf8563->slave_addr;
    wire->reg_addr   = pcf8563->time_reg_addr;
    wire->bytes      = pcf8563->time_reg_bytes;
    wire->data       = wiredata;
    wire->send_stop  = pcf8563->send_stop;
    wire->dbg_print  = false;
    wire->wait       = 0; //0ms

    wire_get(wire);
    
    pcf8563->vl       =   wire->data[0] & 0x80;
    pcf8563->seconds  = ((wire->data[0] & 0x70) >> 4)*10 + (wire->data[0] & 0x0f);
    pcf8563->minutes  = ((wire->data[1] & 0x70) >> 4)*10 + (wire->data[1] & 0x0f);
    pcf8563->hours    = ((wire->data[2] & 0x30) >> 4)*10 + (wire->data[2] & 0x0f);
    pcf8563->days     = ((wire->data[3] & 0x30) >> 4)*10 + (wire->data[3] & 0x0f);
    pcf8563->weekdays =   wire->data[4] & 0x07;
    pcf8563->months   = ((wire->data[5] & 0x10) >> 4)*10 + (wire->data[5] & 0x0f);
    pcf8563->years    = ((wire->data[6] & 0xf0) >> 4)*10 + (wire->data[6] & 0x0f);

    free(wiredata);
    free(wire);

    return ERR_OK;
}
int pcf8563_set_time(pcf8563_t *pcf8563)
{
    wire_t *wire;
    byte   *wiredata;

    if( NULL == pcf8563 ){ return ERR_NULL; }

    wire = (wire_t*)malloc(sizeof(wire_t));
    if( NULL == wire )
    {
        return ERR_MALLOC;
    }

    wiredata  = (byte*)malloc(pcf8563->time_reg_bytes);
    if( NULL == wiredata )
    {
        free(wire);
        return ERR_MALLOC;
    }

    wire->slave_addr = pcf8563->slave_addr;
    wire->reg_addr   = pcf8563->time_reg_addr;
    wire->bytes      = pcf8563->time_reg_bytes;
    wire->data       = wiredata;
    wire->send_stop  = pcf8563->send_stop;
    wire->dbg_print  = false;
    wire->wait       = 0; //0ms
    
    wire->data[0] = (pcf8563->seconds/10)<<4 | pcf8563->seconds%10; // seconds
    wire->data[1] = (pcf8563->minutes/10)<<4 | pcf8563->minutes%10; // minutes
    wire->data[2] = (pcf8563->hours  /10)<<4 | pcf8563->hours  %10; // hours
    wire->data[3] = (pcf8563->days   /10)<<4 | pcf8563->days   %10; // days
    wire->data[4] = 0x00;                                           // weekdays
    wire->data[5] = (pcf8563->months /10)<<4 | pcf8563->months %10; // months
    wire->data[6] = (pcf8563->years  /10)<<4 | pcf8563->years  %10; // years

    wire_set(wire);

    free(wiredata);
    free(wire);

    return ERR_OK;
}

