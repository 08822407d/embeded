#include "functions.h"


OneButton BtnEnter(PIN_BUTTON_2, true);
OneButton BtnBack(PIN_BUTTON_1, true);
Timer<1> BtnEnter_AckLongPressTimer;
Timer<1> BtnBack_AckLongPressTimer;
unsigned long BtnEnter_PressStartTime = 0;
unsigned long BtnBack_PressStartTime = 0;

uint8_t opt				= None;
int8_t volts_index		= 0;
int8_t tscale_index		= 0;
uint8_t	current_filter	= 1;

int btnok,
	btnpl,
	btnmn,
	btnbk;
bool menu				= false;
bool info				= true;
bool set_value			= false;
bool menu_action		= false;

uint8_t digital_wave_option	= 0; //0-auto | 1-analog | 2-digital data (SERIAL/SPI/I2C/etc)

int voltage_division[6] = { //screen has 4 divisions, 31 pixels each (125 pixels of height)
	550, //fullscreen 3.3V peak-peak
	500,
	375,
	180,
	100,
	50
};

/*each sample represents 1us (1Msps),
	 thus, the time division is the number
	 of samples per screen division
*/
float time_division[9] = { //screen has 4 divisions, 60 pixel each (240 pixel of width)
	10,
	25,
	50,
	100,
	250,
	500,
	1000,
	2500,
	5000
};
//,   //1Mhz    35ms of data (of 50ms possible)
//  10000,  //100khz  70ms/500ms
//  25000,  //100khz  175ms/500ms of data
//  50000,  //100khz  350ms/500ms of data
//  100000  //50khz   700ms/1000ms of data
//};


bool BtnEnter_AckLongPress(void *args) {
	btnok = 1;
#ifdef Enable_longPress_Continuous
	BtnEnter.reset();
#endif
	return true;
}
void IRAM_ATTR BtnEnter_CheckTicks() {
	// include all buttons here to be checked
	BtnEnter.tick(); // just call tick() to check the state.
}
// this function will be called when the button was pressed 1 time only.
void BtnEnter_SingleClick() {
	btnpl = 1;
	Serial.println("BtnEnter_SingleClick() detected.");
} // singleClick
void BtnEnter_PressStart() {
	// BtnEnter_PressStartTime = millis() - 1000; // as set in setPressMs()
	BtnEnter_AckLongPressTimer.at(millis() + BtnLongPressMs, BtnEnter_AckLongPress);
} // pressStart()
// this function will be called when the button was released after a long hold.
void BtnEnter_PressStop() {
	// btnok = 1;
} // pressStop()


bool BtnBack_AckLongPress(void *args) {
	btnbk = 1;
#ifdef Enable_longPress_Continuous
	BtnBack.reset();
#endif
	return true;
}
void IRAM_ATTR BtnBack_CheckTicks() {
	// include all buttons here to be checked
	BtnBack.tick(); // just call tick() to check the state.
}
// this function will be called when the button was pressed 1 time only.
void BtnBack_SingleClick() {
	btnmn = 1;
	Serial.println("BtnBack_SingleClick() detected.");
} // singleClick
void BtnBack_PressStart() {
	// BtnBack_PressStartTime = millis() - 1000; // as set in setPressMs()
	BtnBack_AckLongPressTimer.at(millis() + BtnLongPressMs, BtnBack_AckLongPress);
} // pressStart()
// this function will be called when the button was released after a long hold.
void BtnBack_PressStop() {
	// btnbk = 1;
} // pressStop()



void InitUserButton() {
	attachInterrupt(digitalPinToInterrupt(BtnEnter.pin()), BtnEnter_CheckTicks, FALLING);
	BtnEnter.setClickMs(100);
	BtnEnter.attachClick(BtnEnter_SingleClick);
	BtnEnter.setPressMs(BtnLongPressMs);
	BtnEnter.attachLongPressStart(BtnEnter_PressStart);
	BtnEnter.attachLongPressStop(BtnEnter_PressStop);

	attachInterrupt(digitalPinToInterrupt(BtnBack.pin()), BtnBack_CheckTicks, FALLING);
	BtnBack.setClickMs(100);
	BtnBack.attachClick(BtnBack_SingleClick);
	BtnBack.setPressMs(BtnLongPressMs);
	BtnBack.attachLongPressStart(BtnBack_PressStart);
	BtnBack.attachLongPressStop(BtnBack_PressStop);
}




void menu_handler() {
	button();
}

void button() {
	if ( btnok == 1 || btnbk == 1 || btnpl == 1 || btnmn == 1)
	{
		menu_action = true;
	}
	if (menu == true)
	{
		if (set_value) {
			switch (opt) {
				case Vdiv:
					if (btnpl == 1) {
						volts_index++;
						if (volts_index >= sizeof(voltage_division) / sizeof(*voltage_division)) {
							volts_index = 0;
						}
						btnpl = 0;
					}
					else if (btnmn == 1) {
						volts_index--;
						if (volts_index < 0) {
							volts_index = sizeof(voltage_division) / sizeof(*voltage_division) - 1;
						}
						btnmn = 0;
					}

					v_div = voltage_division[volts_index];
					break;

				case Sdiv:
					if (btnmn == 1) {
						tscale_index++;
						if (tscale_index >= sizeof(time_division) / sizeof(*time_division)) {
							tscale_index = 0;
						}
						btnmn = 0;
					}
					else if (btnpl == 1) {
						tscale_index--;
						if (tscale_index < 0) {
							tscale_index = sizeof(time_division) / sizeof(*time_division) - 1;
						}
						btnpl = 0;
					}

					s_div = time_division[tscale_index];
					break;

				case Offset:
					if (btnmn == 1) {
						offset += 0.1 * (v_div * 4) / 3300;
						btnmn = 0;
					}
					else if (btnpl == 1) {
						offset -= 0.1 * (v_div * 4) / 3300;
						btnpl = 0;
					}

					if (offset > 3.3)
						offset = 3.3;
					if (offset < -3.3)
						offset = -3.3;

					break;

				case TOffset:
					if (btnpl == 1)
					{
						toffset += 0.1 * s_div;
						btnpl = 0;
					}
					else if (btnmn == 1)
					{
						toffset -= 0.1 * s_div;
						btnmn = 0;
					}

					break;

				default:
					break;

			}
			if (btnbk == 1)
			{
				set_value = 0;
				btnbk = 0;
			}
		}
		else
		{
			if (btnpl == 1)
			{
				opt++;
				if (opt > Single)
				{
					opt = 1;
				}
				Serial.print("option : ");
				Serial.println(opt);
				btnpl = 0;
			}
			if (btnmn == 1)
			{
				opt--;
				if (opt < 1)
				{
					opt = 9;
				}
				Serial.print("option : ");
				Serial.println(opt);
				btnmn = 0;
			}
			if (btnbk == 1)
			{
				hide_menu();
				btnbk = 0;
			}
			if (btnok == 1) {
				switch (opt) {
					case Autoscale:
						auto_scale = !auto_scale;
						break;

					case Vdiv:
						set_value = true;
						break;

					case Sdiv:
						set_value = true;
						break;

					case Offset:
						set_value = true;
						break;

					case Stop:
						stop = !stop;
						//Serial.print("Stop : ");
						//Serial.println(stop);
						set_value = false;
						break;

					case TOffset:
						set_value = true;
						//set_value = false;
						break;

					case Single:
						single_trigger = true;
						set_value = false;
						break;

					case Reset:
						offset = 0;
						v_div = 550;
						s_div = 10;
						tscale_index = 0;
						volts_index = 0;
						break;

					case Probe:
						break;

					case Mode:
						digital_wave_option++;
						if (digital_wave_option > 2)
							digital_wave_option = 0;
						break;

					case Filter:
						current_filter++;
						if (current_filter > 3)
							current_filter = 0;
						break;

					default:
						break;

				}

				btnok = 0;
			}
		}
	}
	else
	{
		if (btnok == 1)
		{
			opt = 1;
			show_menu();
			btnok = 0;
		}
		if (btnbk == 1)
		{
			if (info == true)
			{
				hide_all();
			}
			else
			{
				info = true;
			}
			btnbk = 0;
		}
		if (btnpl == 1) {
			volts_index++;
			if (volts_index >= sizeof(voltage_division) / sizeof(*voltage_division)) {
				volts_index = 0;
			}
			btnpl = 0;
			v_div = voltage_division[volts_index];
		}
		if (btnmn == 1) {
			tscale_index++;
			if (tscale_index >= sizeof(time_division) / sizeof(*time_division)) {
				tscale_index = 0;
			}
			btnmn = 0;
			s_div = time_division[tscale_index];
		}
	}

}

void hide_menu() {
	menu = false;
}

void hide_all() {
	menu = false;
	info = false;
}

void show_menu() {
	menu = true;
}

String strings_vdiv() {
	return "";
}

String strings_sdiv() {
	return "";
}

String strings_offset() {
	return "";
}

String strings_toffset() {
	return "";
}

String strings_freq() {
	return "";
}

String strings_peak() {
	return "";
}

String strings_vmax() {
	return "";
}

String strings_vmin() {
	return "";
}

String strings_filter() {
	return "";
}