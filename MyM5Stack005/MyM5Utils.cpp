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
#include "MyM5Utils.h"

/***********************************************************************
  Function : Serial
 ***********************************************************************/
int serial_getstr(char *buf)
{
    int i;

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
                for(i; i<BUF_LENGTH; i++) buf[i] = '\0';
                return ERR_OK;
            }
            else
            {
                i++;
                if(i>=BUF_LENGTH)
                {
                    Serial.print("### BUFFER FULL, CLEAR. ###\r\n");
                    for(i=0; i<BUF_LENGTH; i++) buf[i] = '\0';
                    i=0;
                    return ERR_INVALID;
                }
            }
        }
    }// while
}

int serial_getnum(int type)
{
    int  i;
    int  return_val;
    char buf[BUF_LENGTH];

    if( DEC == type || HEX == type )
    {
        return_val = serial_getstr(&buf[0]);
        if( ERR_OK != return_val )
        {
            return return_val;
        }

        if( DEC == type ){ sscanf(buf, "%d", &i); }
        if( HEX == type ){ sscanf(buf, "%x", &i); }
        return i;
    }
    else
    {
        return ERR_INVALID;
    }
}

/***********************************************************************
  Function : Wire(I2C)
 ***********************************************************************/
int wire_get(wire_t *wire)
{
    byte     i, i_max;
    byte     received_num;
    uint32_t target_time;
    
    if( NULL == wire ){ return ERR_NULL; }

    if( wire->dbg_print )
    {
        Serial.print("!!! wire_get head !!!\r\n");
        Serial.print(    "Slave Addr   : 0x"); Serial.print(wire->slave_addr, HEX);
        Serial.print("\r\nReg   Addr   : 0x"); Serial.print(wire->reg_addr,   HEX);
        Serial.print("\r\nBytes        : 0d"); Serial.print(wire->bytes,      DEC);
        Serial.print("\r\nSend Stop    : "  ); Serial.print(wire->send_stop,  DEC);
        Serial.print("\r\n");
    }
    
    Wire.beginTransmission(wire->slave_addr>>1);
    Wire.write(wire->reg_addr);
    Wire.endTransmission(wire->send_stop);

    if( wire->wait )
    {
        Serial.printf("wait :%d\n",wire->wait);
        Serial.printf("mills:%d\n",millis());
        target_time = millis() + wire->wait;
        while(1)
        {
            if(target_time < millis()) break;
        }
        Serial.printf("mills:%d\n",millis());
    }

    received_num = Wire.requestFrom(wire->slave_addr>>1, wire->bytes);
    if( wire->dbg_print )
    {
        Serial.print("received num : 0d");
        Serial.print(received_num, DEC);
        Serial.print("\r\n");
    }

    if( Wire.available() < wire->bytes )
    {
        i_max = Wire.available();
    }
    else
    {
        i_max = wire->bytes;
    }

    for(i=0; i<i_max; i++)
    {
        wire->data[i] = Wire.read();
        if( wire->dbg_print )
        {
            Serial.print("0x" ); Serial.print(wire->data[i], HEX);
            Serial.print(" 0d"); Serial.print(wire->data[i], DEC);
            Serial.print(" 0b"); Serial.print(wire->data[i], BIN);
            Serial.print("\r\n");
        }
    }

    if( wire->dbg_print )
    {
        Serial.print("!!! wire_get tail !!!\r\n");
    }

    return received_num;
}

int wire_set(wire_t *wire)
{
    int i;
    
    if( NULL == wire ){ return ERR_NULL; }

    if( wire->dbg_print )
    {
        Serial.print("!!! wire_set head !!!\r\n");
        Serial.print(    "Slave Addr : 0x"); Serial.print(wire->slave_addr, HEX);
        Serial.print("\r\nReg   Addr : 0x"); Serial.print(wire->reg_addr,   HEX);
        Serial.print("\r\nBytes      : 0d"); Serial.print(wire->bytes,      DEC);
        for(i=0; i<wire->bytes; i++)
        {
            Serial.print("\r\nData       : 0x");
            Serial.print(wire->data[i], HEX);
        }
        Serial.print("\r\n");
    }

    Wire.beginTransmission(wire->slave_addr>>1);
    Wire.write(wire->reg_addr);
    for(i=0; i<wire->bytes; i++)
    {
        Wire.write(wire->data[i]);
    }
    Wire.endTransmission();

    if( wire->dbg_print )
    {
        Serial.print("!!! wire_set tail !!!\r\n");
    }

    return ERR_OK;
}
