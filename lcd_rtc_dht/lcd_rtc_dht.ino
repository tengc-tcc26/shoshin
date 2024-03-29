#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <RtcDS3231.h>
#include "DHT.h"

#if 0
#define USING_DS1302
#else
#define USING_DS3231
#endif

#ifdef USING_DS1302
#define DS1302_SCL              12
#define DS1302_SDA              11
#define DS1302_RST              10
#endif

#define DHT_DATA_PIN            9
#define DHTTYPE                 DHT11

#define BTN_0_PIN               8
#define BTN_1_PIN               7

#define INTERVAL_GET_TIME       500
#define INTERVAL_DHT            500
#define INTERVAL_BTN            100

#define LED_1_PIN               3
#define LED_2_PIN               4
#define LED_3_PIN               5
#define LED_4_PIN               6
#define LED_5_PIN               A3
#define LED_6_PIN               A2
#define LED_7_PIN               A1
#define LED_8_PIN               A0

#define LED_INTERVAL_MODE_0     1000
#define LED_INTERVAL_MODE_1     500
#define LED_INTERVAL_MODE_2     250
#define LED_INTERVAL_MODE_3     125
#define LED_INTERVAL_MODE_4     60
#define LED_INTERVAL_MODE_5     30
#define LED_INTERVAL_MODE_6     15
#define LED_INTERVAL_MODE_7     8

typedef enum {
    LED_MODE_0,
    LED_MODE_1,
    LED_MODE_2,
    LED_MODE_3,
    LED_MODE_4,
    LED_MODE_5,
    LED_MODE_6,
    LED_MODE_7,
    LED_MODE_MAX,
} LED_MODE_E;

typedef enum {
    LCD_STATE_MODE_0,
    LCD_STATE_MODE_1,
    LCD_STATE_MODE_2,
    LCD_STATE_MODE_MAX,
} LCD_STATE_E;

typedef struct {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
} rtc_time_st;

typedef struct {
    float temperature;
    float humidity;
} dht_data_st;

typedef struct {
    rtc_time_st lcd_rtc_time;
    dht_data_st lcd_dht_data;
    u8 lcd_state;
    u8 lcd_update;
} lcd_ctl_st;

typedef struct {
    u32 time_lcd;
    u32 time_dht;
    u32 time_btn;
    u32 time_led;
} time_update_st;

typedef struct {
    u8 btn_0_state;
    u8 btn_1_state;
} btn_ctl_st;

typedef struct {
    u8 mode;
    u16 interval;
    u8 is_down;
    u8 val;
} led_ctl_st;

lcd_ctl_st lcd_ctl;
btn_ctl_st btn_ctl;
led_ctl_st led_ctl;
time_update_st time_update;

RtcDateTime rtc_date_time;

DHT dht(DHT_DATA_PIN, DHTTYPE);

LiquidCrystal_PCF8574 lcd(0x27); // set lcd slave addr, 0x27 or 0x3F

#ifdef USING_DS1302
ThreeWire myWire(DS1302_SDA, DS1302_SCL, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);
#elif defined(USING_DS3231)
RtcDS3231<TwoWire> Rtc(Wire);
#endif

const char custom[][8] PROGMEM = {                        // Custom character definitions
    { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 }, // char 1
    { 0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }, // char 2
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x03 }, // char 3
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 4
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C, 0x18 }, // char 5
    { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F }, // char 6
    { 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 7
    { 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }  // char 8
};

const char bigChars[][8] PROGMEM = {
    { 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space
    { 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // !
    { 0x05, 0x05, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // "
    { 0x04, 0xFF, 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01 }, // #
    { 0x08, 0xFF, 0x06, 0x07, 0xFF, 0x05, 0x00, 0x00 }, // $
    { 0x01, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x04 }, // %
    { 0x08, 0x06, 0x02, 0x20, 0x03, 0x07, 0x02, 0x04 }, // &
    { 0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '
    { 0x08, 0x01, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 }, // (
    { 0x01, 0x02, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00 }, // )
    { 0x01, 0x04, 0x04, 0x01, 0x04, 0x01, 0x01, 0x04 }, // *
    { 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01, 0x00, 0x00 }, // +
    { 0x20, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //
    { 0x04, 0x04, 0x04, 0x20, 0x20, 0x20, 0x00, 0x00 }, // -
    { 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // .
    { 0x20, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x20 }, // /
    { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
    { 0x01, 0x02, 0x20, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // 1
    { 0x06, 0x06, 0x02, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // 2
    { 0x01, 0x06, 0x02, 0x04, 0x07, 0x05, 0x00, 0x00 }, // 3
    { 0x03, 0x04, 0xFF, 0x20, 0x20, 0xFF, 0x00, 0x00 }, // 4
    { 0xFF, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 5
    { 0x08, 0x06, 0x06, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 6
    { 0x01, 0x01, 0x02, 0x20, 0x08, 0x20, 0x00, 0x00 }, // 7
    { 0x08, 0x06, 0x02, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 8
    { 0x08, 0x06, 0x02, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 9
    { 0xA5, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // :
    { 0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ;
    { 0x20, 0x04, 0x01, 0x01, 0x01, 0x04, 0x00, 0x00 }, // <
    { 0x04, 0x04, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00 }, // =
    { 0x01, 0x04, 0x20, 0x04, 0x01, 0x01, 0x00, 0x00 }, // >
    { 0x01, 0x06, 0x02, 0x20, 0x07, 0x20, 0x00, 0x00 }, // ?
    { 0x08, 0x06, 0x02, 0x03, 0x04, 0x04, 0x00, 0x00 }, // @
    { 0x08, 0x06, 0x02, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // A
    { 0xFF, 0x06, 0x05, 0xFF, 0x07, 0x02, 0x00, 0x00 }, // B
    { 0x08, 0x01, 0x01, 0x03, 0x04, 0x04, 0x00, 0x00 }, // C
    { 0xFF, 0x01, 0x02, 0xFF, 0x04, 0x05, 0x00, 0x00 }, // D
    { 0xFF, 0x06, 0x06, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // E
    { 0xFF, 0x06, 0x06, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // F
    { 0x08, 0x01, 0x01, 0x03, 0x04, 0x02, 0x00, 0x00 }, // G
    { 0xFF, 0x04, 0xFF, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // H
    { 0x01, 0xFF, 0x01, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // I
    { 0x20, 0x20, 0xFF, 0x04, 0x04, 0x05, 0x00, 0x00 }, // J
    { 0xFF, 0x04, 0x05, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // K
    { 0xFF, 0x20, 0x20, 0xFF, 0x04, 0x04, 0x00, 0x00 }, // L
    { 0x08, 0x03, 0x05, 0x02, 0xFF, 0x20, 0x20, 0xFF }, // M
    { 0xFF, 0x02, 0x20, 0xFF, 0xFF, 0x20, 0x03, 0xFF }, // N
    { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
    { 0x08, 0x06, 0x02, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // P
    { 0x08, 0x01, 0x02, 0x20, 0x03, 0x04, 0xFF, 0x04 }, // Q
    { 0xFF, 0x06, 0x02, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // R
    { 0x08, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // S
    { 0x01, 0xFF, 0x01, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // T
    { 0xFF, 0x20, 0xFF, 0x03, 0x04, 0x05, 0x00, 0x00 }, // U
    { 0x03, 0x20, 0x20, 0x05, 0x20, 0x02, 0x08, 0x20 }, // V
    { 0xFF, 0x20, 0x20, 0xFF, 0x03, 0x08, 0x02, 0x05 }, // W
    { 0x03, 0x04, 0x05, 0x08, 0x20, 0x02, 0x00, 0x00 }, // X
    { 0x03, 0x04, 0x05, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // Y
    { 0x01, 0x06, 0x05, 0x08, 0x07, 0x04, 0x00, 0x00 }, // Z
    { 0xFF, 0x01, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x00 }, // [
    { 0x01, 0x04, 0x20, 0x20, 0x20, 0x20, 0x01, 0x04 }, // Backslash
    { 0x01, 0xFF, 0x04, 0xFF, 0x00, 0x00, 0x00, 0x00 }, // ]
    { 0x08, 0x02, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // ^
    { 0x20, 0x20, 0x20, 0x04, 0x04, 0x04, 0x00, 0x00 }  // _
};

byte col, row, nb = 0, bc = 0;                            // general
byte bb[8];

void lcd_init()
{
    lcd.begin(16, 2);           // 16 * 2 LCD
    //lcd.begin(20, 4);         // 20 * 4 LCD
    lcd.setBacklight(1);        // lcd back lightlight 0-255

    // create 8 custom characters
    for (nb = 0; nb < 8; nb++) {
        for (bc = 0; bc < 8; bc++) {
            bb[bc] = pgm_read_byte(&custom[nb][bc]);
        }

        lcd.createChar(nb + 1, bb);
    }

    lcd.clear();
    writeBigString("RYAN", 0, 0);
}

void led_init()
{
    led_ctl.interval = LED_INTERVAL_MODE_0;

    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
    pinMode(LED_4_PIN, OUTPUT);
    pinMode(LED_5_PIN, OUTPUT);
    pinMode(LED_6_PIN, OUTPUT);
    pinMode(LED_7_PIN, OUTPUT);
    pinMode(LED_8_PIN, OUTPUT);

    digitalWrite(LED_1_PIN, HIGH);
    digitalWrite(LED_2_PIN, HIGH);
    digitalWrite(LED_3_PIN, HIGH);
    digitalWrite(LED_4_PIN, HIGH);
    digitalWrite(LED_5_PIN, HIGH);
    digitalWrite(LED_6_PIN, HIGH);
    digitalWrite(LED_7_PIN, HIGH);
    digitalWrite(LED_8_PIN, HIGH);
}

void rtc_init()
{
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    //Add 6s for compiled done to download into flash
    compiled += 10;

    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) {
#ifdef USING_DS3231

        if (Rtc.LastError() != 0) {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        } else {
#endif
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");
            Rtc.SetDateTime(compiled);
        }
    }

#if defined(USING_DS1302)

    if (Rtc.GetIsWriteProtected()) {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

#endif

    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();

    if (now < compiled) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

#ifdef USING_DS3231
    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
#endif
}

void dht_init()
{
    Serial.println("DHTxx test!");
    dht.begin();
}

void btn_press_init()
{
    pinMode(BTN_0_PIN, INPUT);
    pinMode(BTN_1_PIN, INPUT);
}

void dht_proc()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();

#if 0
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" oC ");
#endif

    lcd_processing_update_dht_data(t, h);
}

void button_press_proc()
{
    unsigned char btn_det;

    btn_det = digitalRead(BTN_0_PIN);

    if (btn_det != btn_ctl.btn_0_state) {
        if (btn_det == LOW) {
            lcd_processing_mode_change();
        }

        btn_ctl.btn_0_state = btn_det;
    }


    btn_det = digitalRead(BTN_1_PIN);

    if (btn_det != btn_ctl.btn_1_state) {
        if (btn_det == LOW) {
            led_change_mode();
        }

        btn_ctl.btn_1_state = btn_det;
    }



#if 0
    Serial.print("Button status ");
    Serial.print(btn_det ? "HIGH" : "LOW");
    Serial.print(" lcd_mode ");
    Serial.println(lcd_ctl.lcd_state);
#endif
}

void led_change_mode()
{
    led_ctl.mode++;

    if (led_ctl.mode >= LED_MODE_MAX) {
        led_ctl.mode = LED_MODE_0;
    }

    switch (led_ctl.mode) {
        case LED_MODE_0:
            led_ctl.interval = LED_INTERVAL_MODE_0;
            break;

        case LED_MODE_1:
            led_ctl.interval = LED_INTERVAL_MODE_1;
            break;

        case LED_MODE_2:
            led_ctl.interval = LED_INTERVAL_MODE_2;
            break;

        case LED_MODE_3:
            led_ctl.interval = LED_INTERVAL_MODE_3;
            break;

        case LED_MODE_4:
            led_ctl.interval = LED_INTERVAL_MODE_4;
            break;

        case LED_MODE_5:
            led_ctl.interval = LED_INTERVAL_MODE_5;
            break;

        case LED_MODE_6:
            led_ctl.interval = LED_INTERVAL_MODE_6;
            break;

        case LED_MODE_7:
            led_ctl.interval = LED_INTERVAL_MODE_7;
            break;

        default:
            led_ctl.interval = LED_INTERVAL_MODE_0;
            break;
    }

#if 0
    Serial.print("LED MODE ");
    Serial.print(led_ctl.mode);
    Serial.print(" LED INTERVAL ");
    Serial.println(led_ctl.interval);
#endif
}

void led_proc()
{
    if (led_ctl.val == 0) {
        led_ctl.val += 1;
    } else {
        if (!led_ctl.is_down) {
            led_ctl.val <<= 1;

            if (led_ctl.val & (0x1 << 7)) {
                led_ctl.is_down = 1;
            }
        } else {
            led_ctl.val >>= 1;

            if (led_ctl.val & 0x1) {
                led_ctl.is_down = 0;
            }
        }
    }

    digitalWrite(LED_1_PIN, led_ctl.val & (0x1 << 0) ? LOW : HIGH);
    digitalWrite(LED_2_PIN, led_ctl.val & (0x1 << 1) ? LOW : HIGH);
    digitalWrite(LED_3_PIN, led_ctl.val & (0x1 << 2) ? LOW : HIGH);
    digitalWrite(LED_4_PIN, led_ctl.val & (0x1 << 3) ? LOW : HIGH);
    digitalWrite(LED_5_PIN, led_ctl.val & (0x1 << 4) ? LOW : HIGH);
    digitalWrite(LED_6_PIN, led_ctl.val & (0x1 << 5) ? LOW : HIGH);
    digitalWrite(LED_7_PIN, led_ctl.val & (0x1 << 6) ? LOW : HIGH);
    digitalWrite(LED_8_PIN, led_ctl.val & (0x1 << 7) ? LOW : HIGH);
#if 0
    Serial.println(led_ctl.is_down);
    Serial.println(led_ctl.val);
#endif
}

void setup()
{
    Serial.begin(115200);

    memset(&time_update, 0, sizeof(time_update_st));
    memset(&lcd_ctl, 0, sizeof(lcd_ctl_st));
    memset(&btn_ctl, 0, sizeof(btn_ctl_st));
    memset(&led_ctl, 0, sizeof(led_ctl_st));

    lcd_init();
    delay(800);

    rtc_init();
    dht_init();
    btn_press_init();

    lcd_ctl.lcd_state = LCD_STATE_MODE_0;

    rtc_get_time(&rtc_date_time);
    lcd_processing_update_time(rtc_date_time);

    lcd_ctl.lcd_update = 1;

    led_init();
}

void rtc_get_time(RtcDateTime *rtc_date_time)
{
    *rtc_date_time = Rtc.GetDateTime();

#if 0
    printDateTime(*rtc_date_time);
    Serial.println();
#endif

    if (!rtc_date_time->IsValid()) {
        Serial.println("RTC lost confidence in the DateTime!");
    }

#ifdef USING_DS3231
#if 0
    RtcTemperature temp = Rtc.GetTemperature();
    temp.Print(Serial);
    // you may also get the temperature as a float and print it
    // Serial.print(temp.AsFloatDegC());
    Serial.println("C");
#endif
#endif
}

void loop()
{
    if (millis() > time_update.time_lcd + INTERVAL_GET_TIME) {
        time_update.time_lcd = millis();

        rtc_get_time(&rtc_date_time);
        lcd_processing_update_time(rtc_date_time);

        if (lcd_ctl.lcd_state == LCD_STATE_MODE_0 || lcd_ctl.lcd_state == LCD_STATE_MODE_2) {
            lcd_ctl.lcd_update = 1;
        }
    }

    if (millis() - time_update.time_dht > INTERVAL_DHT) {
        time_update.time_dht = millis();
        dht_proc();

        if (lcd_ctl.lcd_state == LCD_STATE_MODE_1) {
            lcd_ctl.lcd_update = 1;
        }
    }

    if (millis() - time_update.time_btn > INTERVAL_BTN) {
        time_update.time_btn = millis();
        button_press_proc();
    }

    if (millis() - time_update.time_led > led_ctl.interval) {
        time_update.time_led = millis();
        led_proc();
    }

    lcd_processing_proc();
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.print(datestring);
}


// writeBigChar: writes big character 'ch' to column x, row y; returns number of columns used by 'ch'
int writeBigChar(char ch, byte x, byte y)
{
    if (ch < ' ' || ch > '_') {
        return 0;    // If outside table range, do nothing
    }

    nb = 0;                                                 // character byte counter

    for (bc = 0; bc < 8; bc++) {
        bb[bc] = pgm_read_byte(&bigChars[ch - ' '][bc]);    // read 8 bytes from PROGMEM

        if (bb[bc] != 0) {
            nb++;
        }
    }

    bc = 0;

    for (row = y; row < y + 2; row++) {
        for (col = x; col < x + nb / 2; col++) {
            lcd.setCursor(col, row);                      // move to position
            lcd.write(bb[bc++]);                          // write byte and increment to next
        }

        //    lcd.setCursor(col, row);
        //    lcd.write(' ');                                 // Write ' ' between letters
    }

    return nb / 2 - 1;                                  // returns number of columns used by char
}

// writeBigString: writes out each letter of string
void writeBigString(char *str, byte x, byte y)
{
    char c;

    while ((c = *str++)) {
        x += writeBigChar(c, x, y) + 1;
    }
}


// ********************************************************************************** //
//                                      OPERATION ROUTINES
// ********************************************************************************** //
// FREERAM: Returns the number of bytes currently free in RAM
int freeRam(void)
{
    extern int  __bss_end, *__brkval;
    int free_memory;

    if ((int)__brkval == 0) {
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    } else {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }

    return free_memory;
}

void lcd_processing_update_dht_data(float t, float h)
{
    lcd_ctl.lcd_dht_data.temperature = t;
    lcd_ctl.lcd_dht_data.humidity = h;
}

void lcd_processing_update_time(const RtcDateTime &dt)
{
    lcd_ctl.lcd_rtc_time.year = dt.Year();
    lcd_ctl.lcd_rtc_time.month = dt.Month();
    lcd_ctl.lcd_rtc_time.day = dt.Day();
    lcd_ctl.lcd_rtc_time.hour = dt.Hour();
    lcd_ctl.lcd_rtc_time.minute = dt.Minute();
    lcd_ctl.lcd_rtc_time.second = dt.Second();
}

void lcd_processing_mode_change()
{
    lcd_ctl.lcd_state++;

    if (lcd_ctl.lcd_state >= LCD_STATE_MODE_MAX) {
        lcd_ctl.lcd_state = LCD_STATE_MODE_0;
    }

    lcd_ctl.lcd_update = 1;

    lcd.clear();
}

void lcd_processing_proc()
{
    char temp[16];
    char temp_1[4];

    if (lcd_ctl.lcd_update) {
        lcd_ctl.lcd_update = 0;

        switch (lcd_ctl.lcd_state) {
            case LCD_STATE_MODE_0:

                if (lcd_ctl.lcd_rtc_time.hour > 11) {
                    lcd.setCursor(0, 0);
                    lcd.print("P");
                    lcd.setCursor(0, 1);
                    lcd.print("M");
                } else {
                    lcd.setCursor(0, 0);
                    lcd.print("A");
                    lcd.setCursor(0, 1);
                    lcd.print("M");
                }

                snprintf_P(temp,
                           sizeof(temp),
                           PSTR("%02u:%02u"),
                           lcd_ctl.lcd_rtc_time.hour > 12 ? lcd_ctl.lcd_rtc_time.hour - 12 : lcd_ctl.lcd_rtc_time.hour,
                           lcd_ctl.lcd_rtc_time.minute);
                writeBigString(temp, 1, 0);

                snprintf_P(temp,
                           sizeof(temp),
                           PSTR("%02d"),
                           lcd_ctl.lcd_rtc_time.second);
                lcd.setCursor(14, 1);
                lcd.print(temp);
                break;

            case LCD_STATE_MODE_1:

                //Temperature double to string
                dtostrf(lcd_ctl.lcd_dht_data.temperature, 4, 2, temp_1);
                snprintf_P(temp,
                           sizeof(temp),
                           PSTR("Temp:  %s %cC "),
                           temp_1,
                           (u8)223);
                lcd.setCursor(0, 0);
                lcd.print(temp);


                //RH double to string
                dtostrf(lcd_ctl.lcd_dht_data.humidity, 4, 2, temp_1);
                snprintf_P(temp,
                           sizeof(temp),
                           PSTR("RH  :  %s  %% "),
                           temp_1);
                lcd.setCursor(0, 1);
                lcd.print(temp);
                break;

            case LCD_STATE_MODE_2:
                snprintf_P(temp,
                           sizeof(temp),
                           PSTR("%d/%02d/%02d"),
                           lcd_ctl.lcd_rtc_time.year,
                           lcd_ctl.lcd_rtc_time.month,
                           lcd_ctl.lcd_rtc_time.day);
                lcd.setCursor(0, 0);
                lcd.print(temp);

                snprintf_P(temp,
                           sizeof(temp),
                           PSTR("%02d:%02d:%02d"),
                           lcd_ctl.lcd_rtc_time.hour,
                           lcd_ctl.lcd_rtc_time.minute,
                           lcd_ctl.lcd_rtc_time.second);

                lcd.setCursor(0, 1);
                lcd.print(temp);
                break;

            default:
                break;
        }
    }
}
