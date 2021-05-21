#include <BleKeyboard.h>
#include <M5Atom.h>
#include "common.h"

const std::string DEVICE_NAME("RoBoHoN Mute Button");
const std::string DEVICE_MANUFACTURER("M5Stack");

const CRGB CRGB_BLE_CONNECTED(0x00, 0x00, 0xf0);
// 数値は緑だが Lite では赤く光る https://github.com/m5stack/M5Atom/issues/5
const CRGB CRGB_BLE_DISCONNECTED(0x00, 0xf0, 0x00);

const CRGB CRGB_MUTE(0xf0, 0xf0, 0xf0);
const CRGB CRGB_UNMUTE(0xf0, 0xf0, 0x00);

const bool ENABLE_SERIAL = true;
const bool ENABLE_I2C = false;
const bool ENABLE_DISPLAY = true;

const uint8_t N_VOL_DOWNS_TO_MUTE = 7;
const uint8_t N_VOL_UPS_TO_UNMUTE = 2;

BleKeyboard bleKeyboard(DEVICE_NAME, DEVICE_MANUFACTURER);
bool isBleConnected = false;
bool isMuted = false;

extern void mute(void);
extern void unmute(void);

void setup(void)
{
    bleKeyboard.begin();
    M5.begin(ENABLE_SERIAL, ENABLE_I2C, ENABLE_DISPLAY);
    M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
}

void loop(void)
{
    M5.update();
    if (bleKeyboard.isConnected())
    {
        if (!isBleConnected)
        {
            M5.dis.drawpix(0, CRGB_BLE_CONNECTED);
            isBleConnected = true;
            SERIAL_PRINTLN("Connected");
            delay(1000);
            M5.dis.drawpix(0, CRGB_UNMUTE);
            SERIAL_PRINTLN("Unmuted");
        }
        if (M5.Btn.wasPressed())
        {
            if (isMuted) {
                unmute();
                isMuted = false;
                SERIAL_PRINTLN("Unmuted");
            }
            else
            {
                mute();
                isMuted = true;
                SERIAL_PRINTLN("Muted");
            }
        }
    } else {
        if (isBleConnected) {
            M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
            isBleConnected = false;
            SERIAL_PRINTLN("Disconnected");
        }
    }
}

void mute(void)
{
    for (uint8_t c = 0; c < N_VOL_DOWNS_TO_MUTE; ++c) {
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
        delay(200);
    }
    M5.dis.drawpix(0, CRGB_MUTE);
}

void unmute(void) {
    for (uint8_t c = 0; c < N_VOL_UPS_TO_UNMUTE + 1; ++c)
    {
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
        delay(c == 0 ? 500 : 200);
    }
    M5.dis.drawpix(0, CRGB_UNMUTE);
}