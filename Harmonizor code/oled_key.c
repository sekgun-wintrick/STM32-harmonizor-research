/*
 * oled_key.c
 *
 *  Created on: Aug 29, 2026
 *      Author: phlwi
 */
#include "oled_key.h"

void update_screen_key(uint32_t state){
	switch (state){
	case 0:
		SSD1306_Fill(0);
		SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("C", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 1:
		SSD1306_Fill(0);
		SSD1306_GotoXY (43,23); // goto 10, 10
		SSD1306_Puts ("C#", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
    case 2:
		SSD1306_Fill(0);
		SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("D", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 3:
		SSD1306_Fill(0);
		SSD1306_GotoXY (43,23); // goto 10, 10
		SSD1306_Puts ("D#", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 4:
		SSD1306_Fill(0);
		SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("E", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 5:
		SSD1306_Fill(0);
		SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("F", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 6:
		SSD1306_Fill(0);
		SSD1306_GotoXY (43,23); // goto 10, 10
		SSD1306_Puts ("F#", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 7:
		SSD1306_Fill(0);
		SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("G", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 8:
		SSD1306_Fill(0);
		SSD1306_GotoXY (43,23); // goto 10, 10
		SSD1306_Puts ("G#", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 9:
		SSD1306_Fill(0);
	    SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("A", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 10:
		SSD1306_Fill(0);
		SSD1306_GotoXY (43,23); // goto 10, 10
		SSD1306_Puts ("A#", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	case 11:
		SSD1306_Fill(0);
		SSD1306_GotoXY (55,23); // goto 10, 10
		SSD1306_Puts ("B", &Font_16x26, 1);
		SSD1306_UpdateScreen();
		break;
	}
}


