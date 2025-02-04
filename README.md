# 🕷️ HIDCrawler 

![IMG_3902](https://github.com/user-attachments/assets/412ac46b-68ad-4b9e-ad29-01b2314a0be9)


**Capture USB keystrokes with real-time feedback**  
---

## 🌟 Features

- ⌨️ **HID Device Support** – Works with USB keyboards and compliant input devices.
- 💡 **LED Feedback** – Blinks an LED on each keypress.
- 📡 **Serial Monitor Output** – Streams typed characters to Serial Monitor.
- ⏳ **Debounce Logic** – Prevents duplicate keypress detection.
- 🔧 **Modifier Handling** – Supports shifted characters (e.g., `A`, `!`, `@`).

---

## 🛠️ Hardware Requirements

| Component               | Purpose                          | Optional? |
|-------------------------|----------------------------------|-----------|
| Arduino Board           | Uno/Leonardo with USB Host Shield| No        |
| USB Host Shield         | Enable USB device connectivity   | No        |

---

## 🔌 Installation

1. **Clone the Repository**  

2. **Install Dependencies**  
   - Install the [USB Host Shield Library 2.0](https://github.com/felis/USB_Host_Shield_2.0) via Arduino Library Manager.

3. **Upload the Code**  
   - Open `HIDCrawler.ino` in Arduino IDE.
   - Select your board (Arduino Leonardo).
   - Click **Upload** (➡️ icon).

---

## ⚡ Power Setup

⚠️ **High-Power Devices**  
If your HID device isn’t detected or behaves erratically:  
1. Solder the **5V points** on the USB Host Shield.  
2. Follow [this 5V mod tutorial](https://www.youtube.com/watch?v=1dxwU87GQVM).

---

## 🖥️ Usage

1. Open the **Serial Monitor** (`Ctrl+Shift+M`).  
2. Set baud rate to **115200**.  
3. Start typing! Keystrokes will appear like this:  
   ```plaintext
   Hello World!
   ```

4. The LED will blink briefly for each keypress.

---

## 📚 Dependencies

- [USB Host Shield Library 2.0](https://github.com/felis/USB_Host_Shield_2.0) by Oleg Mazurov  
  ![Version](https://img.shields.io/badge/Version-2.0-green)
