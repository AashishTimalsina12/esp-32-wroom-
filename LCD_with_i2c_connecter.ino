#include <LiquidCrystal_I2C.h>

int totalColumns = 16;
int totalRows = 2;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
 
LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);

void setup(){
lcd.init(); 
lcd.backlight(); // use to turn on and turn off LCD back light
}

void loop()
{
lcd.setCursor(0, 0);
lcd.print("Microcontrollerslab");
lcd.setCursor(0,1);
lcd.print("I2C LCD tutorial");
delay(8000);
lcd.clear(); 
lcd.setCursor(0, 0);
lcd.print("WELCOME");
delay(8000);
lcd.setCursor(0,1);
lcd.print("AASHISH TIMALSINA");
delay(8000);
lcd.clear(); 
}
