#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

#define DS1302_SCL              10
#define DS1302_SDA              9
#define DS1302_RST              8

ThreeWire myWire(DS1302_SDA, DS1302_SCL, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);

LiquidCrystal_PCF8574 lcd(0x27); // set lcd slave addr, 0x27 or 0x3F

//以下是大字型的設定
//數字的字型設定
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

//文字和符號的設定
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
//int bb[8];  // 若編譯出現錯誤，請用這行
byte bb[8];

void lcd_init()
{
    lcd.begin(16, 2);  //16x2的LCD
    //lcd.begin(20, 4); //20x4的LCD
    lcd.setBacklight(1); // lcd back lightlight 0-255

    //建立大字型
    for (nb = 0; nb < 8; nb++ ) {                 // create 8 custom characters
        for (bc = 0; bc < 8; bc++) bb[bc] = pgm_read_byte( &custom[nb][bc] );
        lcd.createChar ( nb + 1, bb );
    }

    lcd.clear(); //清除畫面

    //writeBigString(要顯示的字串,第幾個字,行：第1行是0，第2行是1)
    //請注意，因為大字型2行才能顯示1個字，所以行1，就是指正常的1~2行，
    //16x2的LCD只能顯示1行大字型
    //如果用的是20x4的LCD，第二行大字型，就是行2

    writeBigString("JMKAER", 0, 0); //測試顯示大字型
}

void rtc_init()
{
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    //__DATE__，__TIME__，是程式碼編譯時的日期和時間
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    //判斷DS1302是否接好
    if (!Rtc.IsDateTimeValid()) {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected()) {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    //判斷DS1302上紀綠的時間和編譯時的時間，哪個比較新
    //如果編譯時間比較新，就進行設定，把DS1302上的時間改成新的時間
    //now：DS1302上紀綠的時間，compiled：編譯時的時間
    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        //編譯時間比較新，把DS1302上的時間改成編譯的時間
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

void setup ()
{
    Serial.begin(9600);

    lcd_init();
    delay(800);

    rtc_init();
}

void loop ()
{
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    //判斷DS1302是否正常，如果不正常，一般是線沒接好，或是電池沒電了
    if (!now.IsValid()) {
        Serial.println("RTC lost confidence in the DateTime!");
    }

    lcd.clear(); //清除畫面

    //如果12點以後，就顯示PM，否則顯示AM
    if(now.Hour()>11) {
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

    //組合要顯示在LCD上的時間
    char datestring[10];

    snprintf_P(datestring,
               10,
               PSTR("%02u:%02u"),
               now.Hour(),
               now.Minute());

    writeBigString(datestring, 3, 0);

    delay(10000); // 10秒更新一次
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

//顯示完整年月日時間的副程式
void printDateTime(const RtcDateTime& dt)
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
               dt.Second() );
    Serial.print(datestring);
}


//以下是處理大字型的副程式
// writeBigChar: writes big character 'ch' to column x, row y; returns number of columns used by 'ch'
int writeBigChar(char ch, byte x, byte y)
{
    if (ch < ' ' || ch > '_') return 0;               // If outside table range, do nothing
    nb = 0;                                           // character byte counter
    for (bc = 0; bc < 8; bc++) {
        bb[bc] = pgm_read_byte( &bigChars[ch - ' '][bc] ); // read 8 bytes from PROGMEM
        if (bb[bc] != 0) nb++;
    }

    bc = 0;
    for (row = y; row < y + 2; row++) {
        for (col = x; col < x + nb / 2; col++ ) {
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
    while ((c = *str++))
        x += writeBigChar(c, x, y) + 1;
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

