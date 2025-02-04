#include <Usb.h>
#include <usbhub.h>
#include <hidboot.h>

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb, 1);

const int LED_PIN = 13;
const unsigned long DEBOUNCE_DELAY = 100;

class KeyboardParser : public HIDReportParser {
public:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        uint8_t currentKeys[6] = {0};
        bool keyPressed = false;

        for (uint8_t i = 0; i < 6; i++) {
            currentKeys[i] = buf[i + 2];
            if (currentKeys[i] != 0) keyPressed = true;
        }

        if (keyPressed) {
            ledOnTime = millis();
            digitalWrite(LED_PIN, HIGH);
            
            for (uint8_t i = 0; i < 6; i++) {
                uint8_t keycode = currentKeys[i];
                if (keycode != 0 && !wasPressed(keycode)) {
                    char key = getKey(keycode, buf[0]);
                    if (key != 0 && bufferIndex < BUFFER_SIZE - 1) {
                        keyBuffer[bufferIndex++] = key;
                    }
                }
            }
        }

        memcpy(prevKeyCodes, currentKeys, sizeof(prevKeyCodes));
    }

    void flushBuffer() {
        if (bufferIndex > 0) {
            keyBuffer[bufferIndex] = '\0';
            Serial.print(keyBuffer);
            bufferIndex = 0;
        }
    }

    void updateLed() {
        if (digitalRead(LED_PIN) && (millis() - ledOnTime > DEBOUNCE_DELAY)) {
            digitalWrite(LED_PIN, LOW);
        }
    }

private:
    static const uint8_t BUFFER_SIZE = 64;
    char keyBuffer[BUFFER_SIZE];
    volatile uint8_t bufferIndex = 0;
    uint8_t prevKeyCodes[6] = {0};
    unsigned long ledOnTime = 0;

    bool wasPressed(uint8_t keycode) {
        for (uint8_t i = 0; i < 6; i++) {
            if (prevKeyCodes[i] == keycode) return true;
        }
        return false;
    }

    char getKey(uint8_t keycode, uint8_t modifiers) {
    const char normalKeys[] = {
        0,   0,   0,   0,   // 0x00-0x03: No Event, ErrorRollOver, POSTFail, ErrorUndefined
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 0x04-0x0D
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 0x0E-0x17
        'u', 'v', 'w', 'x', 'y', 'z',                     // 0x18-0x1D
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', // 0x1E-0x27
        '\n', 0,   '\b','\t',' ', '-', '=', '[', ']','\\', // 0x28-0x31 (Enter, Esc, Bksp, Tab, Space)
        '#', ';', '\'','`', ',', '.', '/',                 // 0x32-0x38
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x39-0x42 (CapsLock, F1-F10)
        0,   0,   0,   0,   0,   '-', 0,   '5', 0,   '+', // 0x43-0x4C (F11, F12, PS, SL, PB, KP-, KP+, etc.)
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', // 0x4D-0x56 (Keypad numbers)
        '\n','\\', 0,   0,   0,   0,   0,   0,   0,   0,   // 0x57-0x60 (KP Ent, KP \)
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x61-0x6A 
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0    // 0x6B-0x73
    };

        if (keycode >= sizeof(normalKeys)) return 0;
        return (modifiers & 0x22) ? shiftKeys[keycode] : normalKeys[keycode];
    }

    const char shiftKeys[128] = {
    0,   0,   0,   0,    // 0x00-0x03
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', // 0x04-0x0D
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', // 0x0E-0x17
    'U', 'V', 'W', 'X', 'Y', 'Z',                     // 0x18-0x1D
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', // 0x1E-0x27
    '\n', 0,   '\b','\t',' ', '_', '+', '{', '}', '|', // 0x28-0x31
    '~', ':', '"', '~', '<', '>', '?',                // 0x32-0x38
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x39-0x42
    0,   0,   0,   0,   0,   '-', 0,   '5', 0,   '+', // 0x43-0x4C
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', // 0x4D-0x56
    '\n','|',  0,   0,   0,   0,   0,   0,   0,   0,   // 0x57-0x60
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x61-0x6A
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0    // 0x6B-0x73
    };
};

KeyboardParser parser;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    Serial.begin(115200);
    while (!Serial);
    
    if (Usb.Init() == -1) {
        Serial.println("USB Error");
        while (1);
    }
    HidKeyboard.SetReportParser(0, &parser);
}

void loop() {
    Usb.Task();
    parser.updateLed();
    parser.flushBuffer();
}
