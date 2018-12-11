#ifndef _TFTCLOCK_H_
#define _TFTCLOCK_H_

/***********************************************************************
  Function Prototype : TFT Clock
 ***********************************************************************/
void tft_clock_analog_setup();
void tft_clock_analog_update();

/***********************************************************************
  Function Prototype : TFT Clock misc. function
 ***********************************************************************/
void tft_clock_ymd(void);
int  tft_clock_get_current_time();
void tft_clock_redraw();

#endif
