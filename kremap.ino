#include <HID-Settings.h>
#include <HID-Project.h>

#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// Keymaps are hideous and now in a header to more easily ignore
#include "kbd_maps.h"

enum Layouts {
  QWERTY = 0,
  COLEMAK = 1,
};

int sensePin = 3;
int ledState = 0;
uint8_t LAYOUT = COLEMAK;
//uint8_t MAGIC_TOGGLE = KEY_ESC;
uint8_t MAGIC_TOGGLE = KEY_MENU2;
uint8_t CONSEC_MAGIC = 0;
uint8_t MAGIC_THRESH = 1;
uint8_t INTERRUPT_COUNT = 0;

void isrsignal()
{
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt > 100)
  {
    INTERRUPT_COUNT = INTERRUPT_COUNT + 1;
  }
  last_interrupt = interrupt_time;
  digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
  ledState = 1 - ledState;
}
KeyboardKeycode remap(uint8_t key)
{
  return KeyboardKeycode(key);
}
class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  if (LAYOUT == COLEMAK)
  {
    Keyboard.press(KeyboardKeycode(colemak_map[key]));
  } else {
    Keyboard.press(KeyboardKeycode(qwerty_map[key]));
  }

  uint8_t c = OemToAscii(mod, key);

  //if (c)
  //  OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
    if (afterMod.bmLeftCtrl) {
      Keyboard.press(KEY_LEFT_CTRL);
    } else {
      Keyboard.release(KEY_LEFT_CTRL);
    }
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
    if (afterMod.bmLeftShift) {
      Keyboard.press(KEY_LEFT_SHIFT);
    } else {
      Keyboard.release(KEY_LEFT_SHIFT);
    }
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
    if (afterMod.bmLeftAlt) {
      Keyboard.press(KEY_LEFT_ALT);
    } else {
      Keyboard.release(KEY_LEFT_ALT);
    }
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
    if (afterMod.bmLeftGUI) {
      Keyboard.press(KEY_LEFT_GUI);
    } else {
      Keyboard.release(KEY_LEFT_GUI);
    }
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
    if (afterMod.bmRightCtrl) {
      Keyboard.press(KEY_RIGHT_CTRL);
    } else {
      Keyboard.release(KEY_RIGHT_CTRL);
    }
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
    if (afterMod.bmRightShift) {
      Keyboard.press(KEY_RIGHT_SHIFT);
    } else {
      Keyboard.release(KEY_RIGHT_SHIFT);
    }
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
    if (afterMod.bmRightAlt) {
      Keyboard.press(KEY_RIGHT_ALT);
    } else {
      Keyboard.release(KEY_RIGHT_ALT);
    }
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
    if (afterMod.bmRightGUI) {
      Keyboard.press(KEY_RIGHT_GUI);
    } else {
      Keyboard.release(KEY_RIGHT_GUI);
    }
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  //Serial.print("UP ");
  //PrintKey(mod, key);
  if (LAYOUT == COLEMAK)
  {
    Keyboard.release(KeyboardKeycode(colemak_map[key]));
  } else {
    Keyboard.release(KeyboardKeycode(qwerty_map[key]));
  }
  if (key == MAGIC_TOGGLE) {
    CONSEC_MAGIC = CONSEC_MAGIC + 1;
  } else {
    CONSEC_MAGIC = 0;
  }
  if (CONSEC_MAGIC == MAGIC_THRESH) {
    CONSEC_MAGIC = 0;
    LAYOUT = 1 - LAYOUT; 
  }
  if (INTERRUPT_COUNT > 0)
  {
    char buffer[50];
    MAGIC_TOGGLE = key;
    MAGIC_THRESH = INTERRUPT_COUNT;
    INTERRUPT_COUNT = 0;
    CONSEC_MAGIC = 0;
    sprintf(buffer, "New settings %d presses of %02x", MAGIC_THRESH, MAGIC_TOGGLE);
    Serial.println(buffer);
  }
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("ASCII: ");
  Serial.println((char)key);
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial && millis()<5000); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Started");
  Keyboard.begin();

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );
  pinMode(sensePin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  INTERRUPT_COUNT = 0;
  attachInterrupt(digitalPinToInterrupt(sensePin), isrsignal, FALLING);
  HidKeyboard.SetReportParser(0, &Prs);
}

void loop()
{
  Usb.Task();
}


