/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * BRICOLEMON atsamd51 as core boar, based on the work of gran_central_m4
 */

#ifndef ARDUINO_GRAND_CENTRAL_M4
  #error "Oops! Select 'Adafruit Grand Central M4' in 'Tools > Board.'"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BRICOLEMON V1.0, Lemoncrest & BricoGeek colaboration."
#endif


//
//
// Opción para habilitar la EEPROM emulation de la que dispone el chip ARM, para poder habilitarlo antes hay que setear los fuses con https://github.com/GMagician/SAMD51-SmartEEprom-Manager/blob/master/SEEManager/SEEManager.ino
// La capacidad se autoconfigura sola leyendo estos fuses. 
//
#define FLASH_EEPROM_EMULATION


//This its another option to emulate an EEPROM, but its more efficient to dont loose the data the first One.
//#define SDCARD_EEPROM_EMULATION


//
// BLTOUCH
//

#define SERVO0_PIN         33 // Pin for the pwm of the bltouch



//
// Limit Switches
//
#define X_MIN_PIN                             10
#define Y_MIN_PIN                             11
#define Z_MIN_PIN                             12
#define E_MIN_PIN                             48

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     12
#endif

//
// Steppers
//
#define X_STEP_PIN                             3  // Mega/Due:54 - AGCM4:67
#define X_DIR_PIN                             22  // Mega/Due:55 - AGCM4:68
#define X_ENABLE_PIN                          26

#define Y_STEP_PIN                             4  // Mega/Due:60 - AGCM4:73
#define Y_DIR_PIN                             23  // Mega/Due:61 - AGCM4:74
#define Y_ENABLE_PIN                          27  // Mega/Due:56 - AGCM4:69

#define Z_STEP_PIN                            5
#define Z_DIR_PIN                             24
#define Z_ENABLE_PIN                          28  // Mega/Due:62 - AGCM4:54


#define E0_STEP_PIN                            2
#define E0_DIR_PIN                            25
#define E0_ENABLE_PIN                         29

#define E1_STEP_PIN                           13
#define E1_DIR_PIN                            46
#define E1_ENABLE_PIN                         47


//
// Temperature Sensors, falta el sensor de la cama.
//


#define TEMP_0_PIN                            1
#define TEMP_1_PIN                            2
//#define TEMP_BED_PIN                          3



//
// Heaters / Fans
//
#define HEATER_0_PIN                           6
#define HEATER_1_PIN                          45
#define HEATER_BED_PIN                         7


#define FAN_PIN                                8
#define FAN1_PIN                               9

//
// Misc. Functions
//
#define SDSS                                  53



//Poer supply pin
//#ifndef PS_ON_PIN
//  #define PS_ON_PIN                           39
//#endif



//
//Define of Serial ADDRESS
//
/*
#define  X_SLAVE_ADDRESS 0b00
#define  Y_SLAVE_ADDRESS 0b01
#define  Z_SLAVE_ADDRESS 0b10
#define E0_SLAVE_ADDRESS 0b11
#define E1_SLAVE_ADDRESS 0b00
*/

//Se debe definir el puerto 0 como -1, para que coja el USB y luego el port2 como 1, para que coja el Serial2.
//El serial 1 está definido ya de por sí como el 0/1, luego es añadir un serial, marlin lo hace por nosotros con el
//#define SERIAL_PORT_2 1. Igualmente adafruit indica como crear más serial mediante los pin-demux y el SERCOM.




#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  #define E1_HARDWARE_SERIAL Serial2
  

  #define TMC_BAUD_RATE 115200
  //
  // Software serial
  //

  #define  X_SLAVE_ADDRESS 0
  #define  Y_SLAVE_ADDRESS 1
  #define  Z_SLAVE_ADDRESS 2
  #define E0_SLAVE_ADDRESS 3
  #define E1_SLAVE_ADDRESS 0

    #define X_SERIAL_TX_PIN   0
    #define X_SERIAL_RX_PIN   1

    #define Y_SERIAL_TX_PIN   0
    #define Y_SERIAL_RX_PIN   1

    #define Z_SERIAL_TX_PIN   0
    #define Z_SERIAL_RX_PIN   1

    #define E0_SERIAL_TX_PIN  0
    #define E0_SERIAL_RX_PIN  1

    #define E1_SERIAL_TX_PIN  18
    #define E1_SERIAL_RX_PIN  19




#endif



//////////////////////////
// LCDs and Controllers //
//////////////////////////
/*
Esquema de los expansion BricoLemon

                EXP1
               --------
        VCC   | .    . | GND
        LCD7  | .    . | LCDD6
        LCD5  | .    .   LCDD4
        LCDRS | .    . | LCDDE
  BTN_ENCODER | .    . | BEEPER
               --------

                EXP2
               --------
        KILL  | .    . | GND
        RESET | .    . | SD_DETECT
        MOSI  | .    .   BTN_EN1
        SS    | .    . | BTN_EN2
        SCLK  | .    . | MISO
               --------

Tabla de pines dentro de la bricolemon para configuración

    LCD---
      LCD4  36
      LCD5  37
      LCD6  38
      LCD7  39
      LCDRS 34
      LCDDE 35
    ENCODER---
      BTN_EN1     42
      BTN_EN2     43
      BTN_ENCODER 40
    SPI---
      MISO  50
      SS    53
      SCKL  52
      MOSI  51
    BEEPER
      BEEP  41
    SD-CARD---
      SD_DETECT 44
    KILL_PIN 49

*/




#if HAS_SPI_LCD

  //
  // LCD Display output pins
  //
  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

    // TO TEST
    // #define LCD_PINS_RS         49   // CS chip select /SS chip slave select
    // #define LCD_PINS_ENABLE     51   // SID (MOSI)
    // #define LCD_PINS_D4         52   // SCK (CLK) clock

  #elif BOTH(NEWPANEL, PANEL_ONE)

    // TO TEST
    // #define LCD_PINS_RS         40
    // #define LCD_PINS_ENABLE     42
    // #define LCD_PINS_D4         57     // Mega/Due:65 - AGCM4:57
    // #define LCD_PINS_D5         58     // Mega/Due:66 - AGCM4:58
    // #define LCD_PINS_D6         44
    // #define LCD_PINS_D7         56     // Mega/Due:64 - AGCM4:56

  #else

    #if ENABLED(CR10_STOCKDISPLAY)

      // TO TEST
      // #define LCD_PINS_RS       27
      // #define LCD_PINS_ENABLE   29
      // #define LCD_PINS_D4       25

      #if DISABLED(NEWPANEL)
        // TO TEST
        // #define BEEPER_PIN      37
      #endif

    #elif ENABLED(ZONESTAR_LCD)

      // TO TEST
      // #define LCD_PINS_RS       56     // Mega/Due:64 - AGCM4:56
      // #define LCD_PINS_ENABLE   44
      // #define LCD_PINS_D4       55     // Mega/Due:63 - AGCM4:55
      // #define LCD_PINS_D5       40
      // #define LCD_PINS_D6       42
      // #define LCD_PINS_D7       57     // Mega/Due:65 - AGCM4:57

    #else

      #if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
        // TO TEST
        // #define LCD_PINS_DC     25   // Set as output on init
        // #define LCD_PINS_RS     27   // Pull low for 1s to init
        // DOGM SPI LCD Support
        // #define DOGLCD_CS       16
        // #define DOGLCD_MOSI     17
        // #define DOGLCD_SCK      23
        // #define DOGLCD_A0       LCD_PINS_DC
      #else
        #define LCD_PINS_RS                   34
        #define LCD_PINS_ENABLE               35
        #define LCD_PINS_D4                   36
        #define LCD_PINS_D5                   37
        #define LCD_PINS_D6                   38
      #endif

      #define LCD_PINS_D7                     39

      #if DISABLED(NEWPANEL)
        #define BEEPER_PIN                    41
      #endif

    #endif

    #if DISABLED(NEWPANEL)
      // Buttons attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK                     38
      //#define SHIFT_LD                      42
      //#define SHIFT_OUT                     40
      //#define SHIFT_EN                      17
    #endif

  #endif





  //
  // LCD Display input pins
  //
  #if ENABLED(NEWPANEL)

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

      #define BEEPER_PIN                      41

      #if ENABLED(CR10_STOCKDISPLAY)
        // TO TEST
        // #define BTN_EN1         17
        // #define BTN_EN2         23
      #else
        #define BTN_EN1                       42
        #define BTN_EN2                       43
      #endif

      #define BTN_ENC                         40
      #ifndef SD_DETECT_PIN
        #define SD_DETECT_PIN                 44
      #endif
      //#define KILL_PIN                        49 //Habilitar en la revision final

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        // TO TEST
        // #define LCD_BACKLIGHT_PIN 39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      // TO TEST
      // #define BTN_EN1           56     // Mega/Due:64 - AGCM4:56
      // #define BTN_EN2           72     // Mega/Due:59 - AGCM4:72
      // #define BTN_ENC           55
      // #define SD_DETECT_PIN     42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      // TO TEST
      // #define BTN_EN1           47
      // #define BTN_EN2           43
      // #define BTN_ENC           32
      // #define LCD_SDSS          SDSS
      // #define KILL_PIN          41

    #elif ENABLED(LCD_I2C_VIKI)

      // TO TEST
      // #define BTN_EN1           40   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      // #define BTN_EN2           42
      // #define BTN_ENC           -1

      // #define LCD_SDSS SDSS
      // #define SD_DETECT_PIN     49

    #elif ANY(VIKI2, miniVIKI)

      // TO TEST
      // #define DOGLCD_CS         45
      // #define DOGLCD_A0         44
      // #define LCD_SCREEN_ROT_180

      // #define BEEPER_PIN        33
      // #define STAT_LED_RED_PIN  32
      // #define STAT_LED_BLUE_PIN 35

      // #define BTN_EN1           22
      // #define BTN_EN2            7
      // #define BTN_ENC           39

      // #define SD_DETECT_PIN     -1   // Pin 49 for display SD interface, 72 for easy adapter board
      // #define KILL_PIN          31

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)

      // TO TEST
      // #define DOGLCD_CS         29
      // #define DOGLCD_A0         27

      // #define BEEPER_PIN        23
      // #define LCD_BACKLIGHT_PIN 33

      // #define BTN_EN1           35
      // #define BTN_EN2           37
      // #define BTN_ENC           31

      // #define LCD_SDSS          SDSS
      // #define SD_DETECT_PIN     49
      // #define KILL_PIN          41

    #elif EITHER(MKS_MINI_12864, FYSETC_MINI_12864)

      // TO TEST
      //#define BEEPER_PIN                    37
      //#define BTN_ENC                       35
      //#define SD_DETECT_PIN                 49

      //#ifndef KILL_PIN
      //  #define KILL_PIN         41
      //#endif

      #if ENABLED(MKS_MINI_12864)                 // Added in Marlin 1.1.6

        // TO TEST
        // #define DOGLCD_A0       27
        // #define DOGLCD_CS       25

        // GLCD features
        // Uncomment screen orientation
        // #define LCD_SCREEN_ROT_90
        // #define LCD_SCREEN_ROT_180
        // #define LCD_SCREEN_ROT_270

        // not connected to a pin
        // #define LCD_BACKLIGHT_PIN 57    // backlight LED on A11/D? (Mega/Due:65 - AGCM4:57)

        // #define BTN_EN1         31
        // #define BTN_EN2         33

      #elif ENABLED(FYSETC_MINI_12864)

        // From https://wiki.fysetc.com/Mini12864_Panel/?fbclid=IwAR1FyjuNdVOOy9_xzky3qqo_WeM5h-4gpRnnWhQr_O1Ef3h0AFnFXmCehK8

        // TO TEST
        // #define DOGLCD_A0       16
        // #define DOGLCD_CS       17

        // #define BTN_EN1         33
        // #define BTN_EN2         31

        //#define FORCE_SOFT_SPI                  // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

        // #define LCD_RESET_PIN   23   // Must be high or open for LCD to operate normally.

        #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
          #ifndef RGB_LED_R_PIN
            // TO TEST
            // #define RGB_LED_R_PIN 25
          #endif
          #ifndef RGB_LED_G_PIN
            // TO TEST
            // #define RGB_LED_G_PIN 27
          #endif
          #ifndef RGB_LED_B_PIN
            // TO TEST
            // #define RGB_LED_B_PIN 29
          #endif
        #elif ENABLED(FYSETC_MINI_12864_2_1)
          // TO TEST
          // #define NEOPIXEL_PIN    25
        #endif

      #endif

    #elif ENABLED(MINIPANEL)

      // TO TEST
      // #define BEEPER_PIN        42
      // not connected to a pin
      // #define LCD_BACKLIGHT_PIN 57    // backlight LED on A11/D? (Mega/Due:65 - AGCM4:57)

      // #define DOGLCD_A0         44
      // #define DOGLCD_CS         58    // Mega/Due:66 - AGCM4:58

      // GLCD features
      // Uncomment screen orientation
      // #define LCD_SCREEN_ROT_90
      // #define LCD_SCREEN_ROT_180
      // #define LCD_SCREEN_ROT_270

      // #define BTN_EN1           40
      // #define BTN_EN2           55    // Mega/Due:63 - AGCM4:55
      // #define BTN_ENC           72    // Mega/Due:59 - AGCM4:72

      // #define SD_DETECT_PIN     49
      // #define KILL_PIN          56    // Mega/Due:64 - AGCM4:56

    #elif ENABLED(ZONESTAR_LCD)

      // TO TEST
      // #define ADC_KEYPAD_PIN    12

    #elif ENABLED(AZSMZ_12864)

      // TO TEST

    #else

      // Beeper on AUX-4
      // #define BEEPER_PIN        33

      // Buttons are directly attached to AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        // TO TEST
        // #define SHIFT_OUT       40
        // #define SHIFT_CLK       44
        // #define SHIFT_LD        42
        // #define BTN_EN1         56    // Mega/Due:64 - AGCM4:56
        // #define BTN_EN2         72    // Mega/Due:59 - AGCM4:72
        // #define BTN_ENC         55    // Mega/Due:63 - AGCM4:55
      #elif ENABLED(PANEL_ONE)
        // TO TEST
        // #define BTN_EN1         72    // AUX2 PIN 3 (Mega/Due:59 - AGCM4:72)
        // #define BTN_EN2         55    // AUX2 PIN 4 (Mega/Due:63 - AGCM4:55)
        // #define BTN_ENC         49    // AUX3 PIN 7
      #else
        // TO TEST
        // #define BTN_EN1         37
        // #define BTN_EN2         35
        // #define BTN_ENC         31
      #endif

      #if ENABLED(G3D_PANEL)
        // TO TEST
        // #define SD_DETECT_PIN   49
        // #define KILL_PIN        41
      #endif

    #endif
  #endif // NEWPANEL

#endif // HAS_SPI_LCD

//
// SD Support on the board, its defined on configuration_adv.h
//

// By default the onboard SD is enabled.
// set SDCARD_CONNECTION form 'ONBOARD' to 'LCD' and use an external SD (connected to LCD)
#define HAS_ONBOARD_SD
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              LCD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #undef SDSS
  #define SDSS                                83
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN                       95
#endif
