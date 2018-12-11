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

#ifndef _MYM5UTILS_H_
#define _MYM5UTILS_H_

#define BUF_LENGTH    64 // 63+1
#define ERR_OK         0
#define ERR_INVALID   -1 // 不正
#define ERR_NULL      -2 // 引数がNULL
#define ERR_MALLOC    -3 // mallocの戻り値がNULL

/***********************************************************************
  Function Prototype : Serial
 ***********************************************************************/
int serial_getstr(char *buf);
int serial_getnum(int type);

/***********************************************************************
  Function Prototype : Wire(I2C)
 ***********************************************************************/
typedef struct {
    byte slave_addr; // デバイスのスレーブアドレス
    byte reg_addr;   // 読み書きするレジスタのアドレス
    byte bytes;      // 読み書きするバイト数
    byte *data;      // 読み書きするデータを格納するメモリのポインタ
    bool send_stop;  // send stopフラグ
    bool dbg_print;  // デバッグ用のUART出力フラグ
    int  wait;       // wait(ms)
}wire_t;

int wire_get(wire_t *wire);
int wire_set(wire_t *wire);

#endif
