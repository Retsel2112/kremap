#include <HID-Settings.h>
#include <HID-Project.h>

#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

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

uint8_t qwerty_map[256] = {
  KEY_RESERVED,
  KEY_ERROR_ROLLOVER,
  KEY_POST_FAIL,
  KEY_ERROR_UNDEFINED,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_0,
  KEY_ENTER,
  KEY_ESC,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_SPACE,
  KEY_MINUS,
  KEY_EQUAL,
  KEY_LEFT_BRACE,
  KEY_RIGHT_BRACE,
  KEY_BACKSLASH,
  KEY_NON_US_NUM,
  KEY_SEMICOLON,
  KEY_QUOTE,
  KEY_TILDE,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_SLASH,
  KEY_CAPS_LOCK,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_PRINT,
  KEY_SCROLL_LOCK,
  KEY_PAUSE,
  KEY_INSERT,
  KEY_HOME,
  KEY_PAGE_UP,
  KEY_DELETE,
  KEY_END,
  KEY_PAGE_DOWN,
  KEY_RIGHT_ARROW,
  KEY_LEFT_ARROW,
  KEY_DOWN_ARROW,
  KEY_UP_ARROW,
  KEY_NUM_LOCK,
  KEYPAD_DIVIDE,
  KEYPAD_MULTIPLY,
  KEYPAD_SUBTRACT,
  KEYPAD_ADD,
  KEYPAD_ENTER,
  KEYPAD_1,
  KEYPAD_2,
  KEYPAD_3,
  KEYPAD_4,
  KEYPAD_5,
  KEYPAD_6,
  KEYPAD_7,
  KEYPAD_8,
  KEYPAD_9,
  KEYPAD_0,
  KEYPAD_DOT,
  KEY_NON_US,
  KEY_APPLICATION,
  KEY_POWER,
  KEY_PAD_EQUALS,
  KEY_F13,
  KEY_F14,
  KEY_F15,
  KEY_F16,
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24,
  KEY_EXECUTE,
  KEY_HELP,
  KEY_MENU2,
  KEY_SELECT,
  KEY_STOP,
  KEY_AGAIN,
  KEY_UNDO,
  KEY_CUT,
  KEY_COPY,
  KEY_PASTE,
  KEY_FIND,
  KEY_MUTE,
  KEY_VOLUME_UP,
  KEY_VOLUME_DOWN,
  KEY_LOCKING_CAPS_LOCK,
  KEY_LOCKING_NUM_LOCK,
  KEY_LOCKING_SCROLL_LOCK,
  KEYPAD_COMMA,
  KEYPAD_EQUAL_SIGN,
  KEY_INTERNATIONAL1,
  KEY_INTERNATIONAL2,
  KEY_INTERNATIONAL3,
  KEY_INTERNATIONAL4,
  KEY_INTERNATIONAL5,
  KEY_INTERNATIONAL6,
  KEY_INTERNATIONAL7,
  KEY_INTERNATIONAL8,
  KEY_INTERNATIONAL9,
  KEY_LANG1,
  KEY_LANG2,
  KEY_LANG3,
  KEY_LANG4,
  KEY_LANG5,
  KEY_LANG6,
  KEY_LANG7,
  KEY_LANG8,
  KEY_LANG9,
  KEY_ALTERNATE_ERASE,
  KEY_SYSREQ_ATTENTION,
  KEY_CANCEL,
  KEY_CLEAR,
  KEY_PRIOR,
  KEY_RETURN2,
  KEY_SEPARATOR,
  KEY_OUT,
  KEY_OPER,
  KEY_CLEAR_AGAIN,
  KEY_CRSEL_PROPS,
  KEY_EXSEL,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_PAD_00,
  KEY_PAD_000,
  KEY_THOUSANDS_SEPARATOR,
  KEY_DECIMAL_SEPARATOR,
  KEY_CURRENCY_UNIT,
  KEY_CURRENCY_SUB_UNIT,
  KEYPAD_LEFT_BRACE,
  KEYPAD_RIGHT_BRACE,
  KEYPAD_LEFT_CURLY_BRACE,
  KEYPAD_RIGHT_CURLY_BRACE,
  KEYPAD_TAB,
  KEYPAD_BACKSPACE,
  KEYPAD_A,
  KEYPAD_B,
  KEYPAD_C,
  KEYPAD_D,
  KEYPAD_E,
  KEYPAD_F,
  KEYPAD_XOR,
  KEYPAD_CARET,
  KEYPAD_PERCENT,
  KEYPAD_LESS_THAN,
  KEYPAD_GREATER_THAN,
  KEYPAD_AMPERSAND,
  KEYPAD_DOUBLEAMPERSAND,
  KEYPAD_PIPE,
  KEYPAD_DOUBLEPIPE,
  KEYPAD_COLON,
  KEYPAD_POUND_SIGN,
  KEYPAD_SPACE,
  KEYPAD_AT_SIGN,
  KEYPAD_EXCLAMATION_POINT,
  KEYPAD_MEMORY_STORE,
  KEYPAD_MEMORY_RECALL,
  KEYPAD_MEMORY_CLEAR,
  KEYPAD_MEMORY_ADD,
  KEYPAD_MEMORY_SUBTRACT,
  KEYPAD_MEMORY_MULTIPLY,
  KEYPAD_MEMORY_DIVIDE,
  KEYPAD_PLUS_MINUS,
  KEYPAD_CLEAR,
  KEYPAD_CLEAR_ENTRY,
  KEYPAD_BINARY,
  KEYPAD_OCTAL,
  KEYPAD_DECIMAL,
  KEYPAD_HEXADECIMAL,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_LEFT_CTRL,
  KEY_LEFT_SHIFT,
  KEY_LEFT_ALT,
  KEY_LEFT_GUI,
  KEY_RIGHT_CTRL,
  KEY_RIGHT_SHIFT,
  KEY_RIGHT_ALT,
  KEY_RIGHT_GUI,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED
};
uint8_t colemak_map[256] = {
  KEY_RESERVED,
  KEY_ERROR_ROLLOVER,
  KEY_POST_FAIL,
  KEY_ERROR_UNDEFINED,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_S,
  KEY_F,
  KEY_T,
  KEY_D,
  KEY_H,
  KEY_U,
  KEY_N,
  KEY_E,
  KEY_I,
  KEY_M,
  KEY_K,
  KEY_Y,
  KEY_SEMICOLON,
  KEY_Q,
  KEY_P,
  KEY_R,
  KEY_G,
  KEY_L,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_J,
  KEY_Z,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_0,
  KEY_ENTER,
  KEY_ESC,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_SPACE,
  KEY_MINUS,
  KEY_EQUAL,
  KEY_LEFT_BRACE,
  KEY_RIGHT_BRACE,
  KEY_BACKSLASH,
  KEY_NON_US_NUM,
  KEY_O,
  KEY_QUOTE,
  KEY_TILDE,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_SLASH,
  KEY_CAPS_LOCK,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_PRINT,
  KEY_SCROLL_LOCK,
  KEY_PAUSE,
  KEY_INSERT,
  KEY_HOME,
  KEY_PAGE_UP,
  KEY_DELETE,
  KEY_END,
  KEY_PAGE_DOWN,
  KEY_RIGHT_ARROW,
  KEY_LEFT_ARROW,
  KEY_DOWN_ARROW,
  KEY_UP_ARROW,
  KEY_NUM_LOCK,
  KEYPAD_DIVIDE,
  KEYPAD_MULTIPLY,
  KEYPAD_SUBTRACT,
  KEYPAD_ADD,
  KEYPAD_ENTER,
  KEYPAD_1,
  KEYPAD_2,
  KEYPAD_3,
  KEYPAD_4,
  KEYPAD_5,
  KEYPAD_6,
  KEYPAD_7,
  KEYPAD_8,
  KEYPAD_9,
  KEYPAD_0,
  KEYPAD_DOT,
  KEY_NON_US,
  KEY_APPLICATION,
  KEY_POWER,
  KEY_PAD_EQUALS,
  KEY_F13,
  KEY_F14,
  KEY_F15,
  KEY_F16,
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24,
  KEY_EXECUTE,
  KEY_HELP,
  KEY_MENU2,
  KEY_SELECT,
  KEY_STOP,
  KEY_AGAIN,
  KEY_UNDO,
  KEY_CUT,
  KEY_COPY,
  KEY_PASTE,
  KEY_FIND,
  KEY_MUTE,
  KEY_VOLUME_UP,
  KEY_VOLUME_DOWN,
  KEY_LOCKING_CAPS_LOCK,
  KEY_LOCKING_NUM_LOCK,
  KEY_LOCKING_SCROLL_LOCK,
  KEYPAD_COMMA,
  KEYPAD_EQUAL_SIGN,
  KEY_INTERNATIONAL1,
  KEY_INTERNATIONAL2,
  KEY_INTERNATIONAL3,
  KEY_INTERNATIONAL4,
  KEY_INTERNATIONAL5,
  KEY_INTERNATIONAL6,
  KEY_INTERNATIONAL7,
  KEY_INTERNATIONAL8,
  KEY_INTERNATIONAL9,
  KEY_LANG1,
  KEY_LANG2,
  KEY_LANG3,
  KEY_LANG4,
  KEY_LANG5,
  KEY_LANG6,
  KEY_LANG7,
  KEY_LANG8,
  KEY_LANG9,
  KEY_ALTERNATE_ERASE,
  KEY_SYSREQ_ATTENTION,
  KEY_CANCEL,
  KEY_CLEAR,
  KEY_PRIOR,
  KEY_RETURN2,
  KEY_SEPARATOR,
  KEY_OUT,
  KEY_OPER,
  KEY_CLEAR_AGAIN,
  KEY_CRSEL_PROPS,
  KEY_EXSEL,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_PAD_00,
  KEY_PAD_000,
  KEY_THOUSANDS_SEPARATOR,
  KEY_DECIMAL_SEPARATOR,
  KEY_CURRENCY_UNIT,
  KEY_CURRENCY_SUB_UNIT,
  KEYPAD_LEFT_BRACE,
  KEYPAD_RIGHT_BRACE,
  KEYPAD_LEFT_CURLY_BRACE,
  KEYPAD_RIGHT_CURLY_BRACE,
  KEYPAD_TAB,
  KEYPAD_BACKSPACE,
  KEYPAD_A,
  KEYPAD_B,
  KEYPAD_C,
  KEYPAD_D,
  KEYPAD_E,
  KEYPAD_F,
  KEYPAD_XOR,
  KEYPAD_CARET,
  KEYPAD_PERCENT,
  KEYPAD_LESS_THAN,
  KEYPAD_GREATER_THAN,
  KEYPAD_AMPERSAND,
  KEYPAD_DOUBLEAMPERSAND,
  KEYPAD_PIPE,
  KEYPAD_DOUBLEPIPE,
  KEYPAD_COLON,
  KEYPAD_POUND_SIGN,
  KEYPAD_SPACE,
  KEYPAD_AT_SIGN,
  KEYPAD_EXCLAMATION_POINT,
  KEYPAD_MEMORY_STORE,
  KEYPAD_MEMORY_RECALL,
  KEYPAD_MEMORY_CLEAR,
  KEYPAD_MEMORY_ADD,
  KEYPAD_MEMORY_SUBTRACT,
  KEYPAD_MEMORY_MULTIPLY,
  KEYPAD_MEMORY_DIVIDE,
  KEYPAD_PLUS_MINUS,
  KEYPAD_CLEAR,
  KEYPAD_CLEAR_ENTRY,
  KEYPAD_BINARY,
  KEYPAD_OCTAL,
  KEYPAD_DECIMAL,
  KEYPAD_HEXADECIMAL,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_LEFT_CTRL,
  KEY_LEFT_SHIFT,
  KEY_LEFT_ALT,
  KEY_LEFT_GUI,
  KEY_RIGHT_CTRL,
  KEY_RIGHT_SHIFT,
  KEY_RIGHT_ALT,
  KEY_RIGHT_GUI,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED,
  KEY_RESERVED
};

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


