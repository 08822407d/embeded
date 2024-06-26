/**
 * @file DAC2_GP8413.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 Hat DAC2 Test
 * @version 0.1
 * @date 2024-01-09
 *
 *
 * @Hardwares: M5StickCPlus2 + Hat DAC2(GP8413)
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * DFRobot_GP8XXX: https://github.com/DFRobot/DFRobot_GP8XXX
 */
#include <M5StickCPlus2.h>

#include "includes.hpp"


// range is 0~10000mv
void setDacVoltage(uint16_t vol, uint8_t ch) {
	uint16_t setting_vol = 0;
	if (vol > 10000) {
		vol = 10000;
	}
	if (ch > 1) ch = 1;
	setting_vol = (int16_t)((float)vol / 10000.0f * 32767.0f);
	if (setting_vol > 32767) {
		setting_vol = 32767;
	}
	GP8413.setDACOutVoltage(setting_vol, ch);
}

void AllOutputCtl(uint16_t vol) {
	// StickCP2.Display.fillRect(0, 0, StickCP2.Display.width(), 30,
	// 						  vol > 0 ? GREEN : ORANGE);
	// StickCP2.Display.drawString("OUTPUT " + String(vol) + "mv",
	// 							StickCP2.Display.width() / 2, 0);
	// set channel0
	setDacVoltage(vol, 0);
	// set channel1
	setDacVoltage(vol, 1);
}

void setup(void) {
	auto cfg = M5.config();

	StickCP2.begin(cfg);
	StickCP2.Display.setRotation(3);
	// StickCP2.Display.setTextDatum(top_center);
	StickCP2.Display.setTextColor(GREEN);
	// StickCP2.Display.setFont(&fonts::FreeSansBoldOblique12pt7b);
	StickCP2.Display.setTextSize(3);
	// StickCP2.Display.drawString("DAC2", StickCP2.Display.width() / 2,
	// 							StickCP2.Display.height() / 2 - 20);
	Wire.end();
	Wire.begin(0, 26, 5000000UL);

	while (GP8413.begin() != 0) {
		Serial.println("Init Fail!");
		StickCP2.Display.drawString("Init Fail!", StickCP2.Display.width() / 2,
									StickCP2.Display.height() / 2);
		delay(1000);
	}
	StickCP2.Display.clear();
	// StickCP2.Display.drawString("DAC2", StickCP2.Display.width() / 2,
	// 							StickCP2.Display.height() / 2 - 20);
	GP8413.setDACOutRange(GP8413.eOutputRangeVCC);
	// StickCP2.Display.drawString("BtnA En/Dis Output",
	// 							StickCP2.Display.width() / 2,
	// 							StickCP2.Display.height() / 2 + 20);

	AllOutputCtl(0);

	// init_timer();
	// start_timer(10);
}

bool output = false;

void loop(void) {
	// StickCP2.update();
	// if (StickCP2.BtnA.wasClicked()) {
	// 	output = !output;
	// 	if (output) {
	// 		AllOutputCtl(3100);
	// 	} else {
	// 		AllOutputCtl(0);
	// 	}
	// }

	// StickCP2.Display.setCursor(4, 4);
	// StickCP2.Display.printf("0x%4X", Wave.Buffer[Wave.Buff_idx]);
	
	// delay(1);
	sendToDac(&Wave);

	// StickCP2.Display.clear();
}