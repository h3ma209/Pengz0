# Pengz0

Portable WiFi security research toolkit for **ESP8266**, with a **128×64 OLED** UI and two-button navigation. Built with **PlatformIO** and the **Arduino** framework.

Use only on networks and devices you own or have **explicit written permission** to test. Unauthorized access or interference with networks is illegal in many jurisdictions.

---

## What this project is

Pengz0 is a modular firmware framework for ethical security research on ESP8266 boards (e.g. ESP-12E / NodeMCU-style modules). You get a handheld-style menu system on an SSD1306 display, with WiFi features grouped under a clear menu tree. The codebase is organized by feature (`wifi/`, `menus/`, `bitmaps/`) so new modules (Radio, RFID, Remote) can plug into the same main menu later.

---

## Supported platform

| Item | Details |
|------|---------|
| **MCU** | ESP8266 (configured for **ESP-12E**) |
| **Framework** | Arduino (via PlatformIO) |
| **Build target** | `[env:esp12e]` in `platformio.ini` |
| **Display** | SSD1306 OLED, **128×64**, I2C address **0x3C** |
| **Serial monitor** | **9600** baud |

### Dependencies

- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) (`^2.5.13`)
- ESP8266 core libraries: `ESP8266WiFi`, `DNSServer`, `ESP8266WebServer`, `Wire`

---

## Hardware wiring

### Pin map

| Function | GPIO | NodeMCU label | Notes |
|----------|------|---------------|--------|
| **Next / scroll** | 14 | D5 | `INPUT_PULLUP`, active LOW |
| **Select** | 12 | D6 | `INPUT_PULLUP`, active LOW |
| **OLED SDA** | 4 | D2 | Default I2C data (ESP8266 `Wire`) |
| **OLED SCL** | 5 | D1 | Default I2C clock |

Button and display pin definitions live in `src/main.cpp` and `src/hardware.h`.

### Display layout

- Resolution: **128 × 64** pixels (`SCREEN_WIDTH` / `SCREEN_HEIGHT`)
- Driver: **Adafruit_SSD1306** over **I2C**
- Menu UI: text size 1, inverted highlight for selected row
- Full-screen **PROGMEM** bitmaps (128×64) for splash / status:
  - **Kawasaki** — shown when starting/stopping Fake AP and after WiFi scan in Clone AP flow
  - **Beeg Yoshi** — bitmap helper present (not used in main AP flow yet)
  - **DedSec** — asset in `src/bitmaps/` (not wired into menus yet)

### Suggested wiring (minimal)

```
ESP8266          SSD1306 (I2C)
  3.3V  --------  VCC
  GND   --------  GND
  D2    --------  SDA
  D1    --------  SCL

ESP8266          Buttons (other leg to GND)
  D5    --------  Next
  D6    --------  Select
```

Each button: one side to the GPIO above, other side to **GND** (internal pull-ups enabled in firmware).

---

## Features

### Implemented

| Feature | Description |
|---------|-------------|
| **Main menu** | WiFi, Radio, RFID, Remote, Settings — OLED list with Next/Select |
| **Fake AP** | Soft AP (`192.168.1.1`), DNS hijack (`* → AP`), captive-style HTTP on `/`, Android `/generate_204`, Apple `/hotspot-detect.html` |
| **Captive portal pages** | Selectable HTML templates: IQ Login, Google-style (Komar), MyKomar, Komar Cap — used when Fake AP starts |
| **Evil Twin menu** | Submenu: Clone AP, Captive Portal, Back |
| **Clone AP — scan** | Scans nearby SSIDs, lists them on OLED; sets `apSSID` from chosen network via `setAPSSID()` |
| **Clone AP — clone** | `cloneWiFiNetwork()` is a **stub** (SSID copy only; full AP clone not finished) |
| **Serial logging** | Debug output at 9600 baud across menus and WiFi actions |

### Menu placeholders (UI only)

These appear on the main menu but only log to Serial today — no driver logic yet:

- **Radio**
- **RFID**
- **Remote**
- **Settings** — screen exists; navigation handler is **not** called from `loop()` yet (commented in `main.cpp`)
- **WiFi → Scan Networks (TBD)**

### Not implemented (do not expect in firmware yet)

- **Deauthentication** attacks
- Full **evil twin** AP impersonation (beyond SSID string + Fake AP stack)
- **Captive portal ON/OFF** toggle (`captivePortalEnabled` is displayed but not switched in menu actions)

---

## Menu structure

```
Main Menu
├── WiFi
│   ├── Fake AP          → Turn On / Turn Off / Back
│   ├── Evil Twin
│   │   ├── Clone AP     → Scan → pick SSID → (sets AP SSID)
│   │   ├── Captive Portal → IQ / Google / MyKomar / Komar Cap / Back
│   │   └── Back
│   ├── Scan Networks (TBD)
│   └── Back
├── Radio      (placeholder)
├── RFID       (placeholder)
├── Remote     (placeholder)
└── Setting    (partial — navigation disabled in loop)
```

**Controls**

- **Next (D5):** move highlight down / wrap
- **Select (D6):** enter submenu or run action

---

## Project layout

```
Pengz0/
├── platformio.ini          # ESP-12E environment
├── src/
│   ├── main.cpp            # Pins, display, global menu state, loop
│   ├── hardware.h          # Button pin declarations
│   ├── menus/              # Main, WiFi, Settings UI
│   ├── wifi/
│   │   ├── fake_ap/        # Soft AP + DNS + web server
│   │   └── evil_twin/      # Clone AP, captive portal menus & portals
│   └── bitmaps/            # 128×64 OLED assets
├── lib/                    # PlatformIO local libs (optional)
└── test/                   # PlatformIO test stub
```

---

## Build and flash

Requires [PlatformIO](https://platformio.org/) (CLI or VS Code extension).

```bash
# Clone and enter project
cd Pengz0

# Build
pio run -e esp12e

# Upload (adjust port if needed)
pio run -e esp12e -t upload --upload-port /dev/cu.usbserial-XXXX

# Serial monitor
pio device monitor -b 9600
```

On first boot you should see the **Main Menu** on the OLED. Open **WiFi → Fake AP → Turn On** to bring up the soft AP (default SSID: `FakeAP` until changed by Clone AP).

---

## Configuration

| Setting | Location | Default |
|---------|----------|---------|
| AP SSID | `main.cpp` → `apSSID` | `"FakeAP"` |
| AP password | `apPassword` | `""` (open) |
| AP IP | `fake_ap.cpp` | `192.168.1.1` |
| Portal template | `captive_portal_menu.cpp` → `portalIndex` | IQ / Komar / MyKomar pages in `portals/portals.h` |
| Button pins | `main.cpp` | D5 = Next, D6 = Select |

---

## Roadmap (from current code)

- [ ] Wire **Settings** menu in `loop()`
- [ ] **Scan Networks** from WiFi menu (reuse `scanWiFiNetworks()`)
- [ ] Finish **clone WiFi network** (channel, encryption, beacon behavior)
- [ ] **Radio / RFID / Remote** modules
- [ ] Use **DedSec** / **Beeg Yoshi** bitmaps in boot or attack flows
- [ ] Optional: deauth / probe utilities (only where legal)

---

## Legal and ethics

This repository is for **education and authorized penetration testing**. You are responsible for complying with local laws and provider terms of service. The authors do not encourage illegal use.

---

## License

No license file is included in the repository yet. Add one if you plan to publish or accept contributions.
