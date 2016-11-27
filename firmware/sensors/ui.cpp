#include <EEPROM.h>
#include "ui.h"

bool diagnostics = false;

void main_ui(options_t& options) {
    uint8_t c, v;

    Serial.println();
    diagnostics = true;

    while(true) {
        if(Serial.available() == 0) {
            Serial.println();
            Serial.println("Current settings");
            Serial.println("----------------");
            Serial.print("[I] Device ID:                 "); Serial.println(options.id);
            Serial.print("[1] Radio channel:             "); Serial.println(options.channel);
            Serial.print("[2] Sensor address:            "); Serial.print(options.sen_addr[0]); Serial.print(" "); Serial.print(options.sen_addr[1]); Serial.print(" "); Serial.println(options.sen_addr[2]);
            Serial.print("[3] Hub address:               "); Serial.print(options.hub_addr[0]); Serial.print(" "); Serial.print(options.hub_addr[1]); Serial.print(" "); Serial.println(options.hub_addr[2]);
            Serial.print("[4] Readings interval (s):     "); Serial.println(options.interval);
            Serial.print("[5] VBAT correction:           "); Serial.println(options.vbat_correct, 4);
            Serial.print("[6] VREG correction:           "); Serial.println(options.vreg_correct, 4);
            Serial.print("[7] Enable BMP180:             "); Serial.println(options.flags & ENABLE_BMP180 ? 'Y' : 'N');
            Serial.print("[8] Enable TSL2561:            "); Serial.println(options.flags & ENABLE_TSL2561 ? 'Y' : 'N');
            Serial.print("[9] Enable HTU21:              "); Serial.println(options.flags & ENABLE_HTU21 ? 'Y' : 'N');
            Serial.print("[0] Altitude in meters:        "); Serial.println(options.altitude);
            Serial.println();
            Serial.println("[R] Reset to defaults");
            Serial.println("[S] Save settings");
            Serial.println("[X] Exit setup mode");
            Serial.println();
        }

        while(Serial.available() == 0);
        c = Serial.read();

        switch(c) {
            case 'i':
            case 'I':
                Serial.print("Enter device ID (0-255): ");
                options.id = Serial.parseInt();
                Serial.println();

                break;

            case '1':
                Serial.print("Enter radio channel (1-125): ");
                v = Serial.parseInt();
                Serial.println();

                if(v < 1 || v > 125) {
                    Serial.println("Invalid channel.");
                } else {
                    options.channel = v;
                }

                break;

            case '2':
                Serial.print("Enter sensor channel (three numbers 0-255): ");
                options.sen_addr[0] = Serial.parseInt();
                options.sen_addr[1] = Serial.parseInt();
                options.sen_addr[2] = Serial.parseInt();
                Serial.println();

                break;

            case '3':
                Serial.print("Enter hub channel (three numbers 0-255): ");
                options.hub_addr[0] = Serial.parseInt();
                options.hub_addr[1] = Serial.parseInt();
                options.hub_addr[2] = Serial.parseInt();
                Serial.println();

                break;

            case '4':
                Serial.print("Enter reading interval in seconds (>= 1): ");
                v = Serial.parseInt();
                Serial.println();

                if(v < 1) {
                    Serial.println("Invalid reading interval.");
                } else {
                    options.interval = v;
                }

                break;

            case '5':
                Serial.print("Enter VBAT correction: ");
                options.vbat_correct = Serial.parseFloat();
                Serial.println();

                break;

            case '6':
                Serial.print("Enter VREG correction: ");
                options.vreg_correct = Serial.parseFloat();
                Serial.println();

                break;

            case '7':
                options.flags ^= ENABLE_BMP180;
                break;

            case '8':
                options.flags ^= ENABLE_TSL2561;
                break;

            case '9':
                options.flags ^= ENABLE_HTU21;
                break;

            case '0':
                Serial.print("Enter altitude in meters: ");
                v = Serial.parseInt();
                Serial.println();

                options.altitude = v;
                break;

            case 'r':
            case 'R':
                options.id  = 1;
                options.channel = 100;
                options.sen_addr[0] = 0;
                options.sen_addr[1] = 0;
                options.sen_addr[2] = 1;
                options.hub_addr[0] = 1;
                options.hub_addr[1] = 0;
                options.hub_addr[2] = 0;
                options.interval = 16;
                options.vbat_correct = 1.0;
                options.vreg_correct = 1.0;
                options.flags = ENABLE_BMP180 | ENABLE_TSL2561 | ENABLE_HTU21;
                options.altitude = 0;

                Serial.println("Settings restored to defaults.");
                break;

            case 's':
            case 'S':
                Serial.println("Settings saved.");
                EEPROM.put(0, options);

                break;

            case 'x':
            case 'X':
                Serial.println("Exiting setup mode.");
                return;

            default:
                break;
        }
    }
}

void ui_setup(options_t& options) {
    EEPROM.get(0, options);

    Serial.begin(19200);
    Serial.setTimeout(5000);

    Serial.print("Press any key to setup and/or diagnostics");

    for(uint8_t i = 0; i < 10; i++) {
        if(Serial.available() > 0) {
            while(Serial.available() > 0) {
                Serial.read();
            }

            main_ui(options);
            break;
        }

        delay(500);
        Serial.print('.');
    }

    Serial.println();
    Serial.println("Starting normal loop.");
    Serial.end();
}

void ui_loop(const readings_t readings) {
    if(!diagnostics) {
        return;
    }

    Serial.begin(19200);

    Serial.print("Seq:               "); Serial.println(readings.seq);
    Serial.print("Flags:             "); Serial.println(readings.flags, 2);
    Serial.print("Status:            "); Serial.println(readings.status, 2);
    Serial.print("BMP180 temp.:      "); Serial.println(readings.bar_temp);
    Serial.print("BMP180 abs. pres.: "); Serial.println(readings.bar_pres_abs);
    Serial.print("BMP180 rel. pres.: "); Serial.println(readings.bar_pres_rel);
    Serial.print("TSL2561 Light:     "); Serial.println(readings.light_lux);
    Serial.print("HTU21 temp.:       "); Serial.println(readings.htu_temp);
    Serial.print("HTU21 hum.:        "); Serial.println(readings.htu_hum);
    Serial.print("VBAT:              "); Serial.println(readings.vbat);
    Serial.print("VREG:              "); Serial.println(readings.vreg);
    Serial.println("--------------------");

    Serial.end();
}