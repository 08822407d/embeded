/**
 * @file imu.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 get IMU data
 * @version 0.1
 * @date 2023-12-19
 *
 *
 * @Hardwares: M5StickCPlus2
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5StickCPlus2: https://github.com/m5stack/M5StickCPlus2
 */

#include <M5StickCPlus2.h>
#include "M5HatBugC2.h"

M5HatBugC bugc;

void setup() {
    Serial.begin(115200);
    while (!bugc.begin(&Wire, BUGC_DEFAULT_I2C_ADDR, 0, 26, 400000U)) {
        Serial.println("Couldn't find BugC");
        delay(1000);
    }
    bugc.setAllMotorSpeed(0, 0, 0, 0);
}

void loop(void) {
    // Read the battery voltage (only BugC2 support)
    uint16_t bat_voltage = bugc.getRawAdc12Bit();
    Serial.printf("Battery: %.2fV\r\n",
                  ((float)bat_voltage / 4095.0 * 3.3) * 2.960784);

    bugc.setAllLedColor(0xff0000, 0x0000ff);
    delay(500);
    bugc.setAllLedColor(0x0000ff, 0xff0000);
    delay(500);
    bugc.setAllMotorSpeed(50, 50, 50, 50);
    delay(100);
    bugc.setAllMotorSpeed(-50, -50, -50, -50);
    delay(100);
    bugc.setAllMotorSpeed(0, 0, 0, 0);
    delay(100);
    bugc.move(MOVE_LEFT, 50);
    delay(100);
    bugc.move(MOVE_FORWARD, 50);
    delay(100);
    bugc.move(MOVE_RIGHT, 50);
    delay(100);
    bugc.move(MOVE_BACKWARD, 50);
    delay(100);
    bugc.move(MOVE_STOP);
    delay(100);
}