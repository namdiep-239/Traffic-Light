
#include "simu_clock.h"

#include "..\i2c\i2c.h"
#include "..\lcd\lcd.h"
#include "..\button_matrix\button.h"
#include "..\traffic_light\TrafficLight.h"
//#include "main.h"

#define     ADDRESS_FIRST_PROGRAM   0x20

unsigned char clockState =  INIT_CLOCK;

unsigned char second = 0,minute = 0,hour = 0;
unsigned char temp_hour = 0;
unsigned char temp_minute = 0;
unsigned char temp_second = 0;

unsigned char late_hour = 0;
unsigned char late_minute = 0;
unsigned char late_second = 0;

void SetupTimeForRealTime();
void SetupForFirstProgram(void);
void DisplayRealTime();

void DisplayTempTime(unsigned char hour, unsigned char minute, unsigned char second);
void smolClock();
void smolLateClock();
unsigned char isButtonIncrease1();
unsigned char isButtonDecrease1();
unsigned char isButtonMode1();

unsigned int blink = 0;


//CLOCK SECTION



void SetupTimeForRealTime()
{
    second = 50;
    minute = 59;
    hour = 23;
}

void SetupForFirstProgram(void)
{
   
        SetupTimeForRealTime();

}

void clock_run() {
    second = (second + 1) % 60;
    if (second == 0) {
        minute = (minute + 1) % 60;
        if (minute == 0) {
            hour = (hour + 1) % 24;
        }
    }
}
void DisplayRealTime()
{

    if(hour < 10)
    {
        LcdPrintStringS(0,4,"0");
        LcdPrintNumS(0,5,hour);
    }
    else
        LcdPrintNumS(0,4,hour);
    
    LcdPrintStringS(0,6,":");
    if(minute < 10)
    {
        LcdPrintStringS(0,7,"0");
        LcdPrintNumS(0,8,minute);
    }
    else
        LcdPrintNumS(0,7,minute);

    LcdPrintStringS(0,9,":");
    if(second < 10)
    {
        LcdPrintStringS(0,10,"0");
        LcdPrintNumS(0,11,second);
    }
    else
        LcdPrintNumS(0,10,second);
    
}

void DisplayTempTime(unsigned char hour, unsigned char minute, unsigned char second)
{
//    second = Read_DS1307(ADDRESS_SECOND);
//    minute = Read_DS1307(ADDRESS_MINUTE);
//    hour = Read_DS1307(ADDRESS_HOUR);
//    date = Read_DS1307(ADDRESS_DATE);
//    month = Read_DS1307(ADDRESS_MONTH);

    if(hour < 10)
    {
        LcdPrintStringS(0,4,"0");
        LcdPrintNumS(0,5,hour);
    }
    else
        LcdPrintNumS(0,4,hour);
    
    LcdPrintStringS(0,6,":");
    if(minute < 10)
    {
        LcdPrintStringS(0,7,"0");
        LcdPrintNumS(0,8,minute);
    }
    else
        LcdPrintNumS(0,7,minute);

    LcdPrintStringS(0,9,":");
    if(second < 10)
    {
        LcdPrintStringS(0,10,"0");
        LcdPrintNumS(0,11,second);
    }
    else
        LcdPrintNumS(0,10,second);
    
}

void smolClock() {
    
    switch (clockState) {
        case INIT_CLOCK:
                LcdPrintLineS(1,"1.UP 2.DN 3.MODE");
                DisplayRealTime();
                if (isButtonMode()) {
                    clockState = GET_TIME;
                }
            break;
        case GET_TIME:
                temp_second = second;
                temp_minute = minute;
                temp_hour = hour;
                clockState = CHANGE_HOUR;
            break;
            case CHANGE_HOUR:
                
                DisplayTempTime(temp_hour, temp_minute, temp_second);
                if (key_code[0] == 0 && key_code[1] == 0) {
                    if (blink < 10) {
                         LcdPrintStringS(0,4,"  ");
                    }
                }
                if (isButtonIncrease()) {
                    temp_hour = (temp_hour + 1) % 24;
                }
                if (isButtonDecrease()) {
                    temp_hour--;
                    if (temp_hour > 23) temp_hour = 23;
                }
                if (isButtonMode()) {
                    clockState = CHANGE_MINUTE;
                }
            break;
            case CHANGE_MINUTE:
               
                DisplayTempTime(temp_hour, temp_minute, temp_second);
                if (key_code[0] == 0 && key_code[1] == 0) {
                   if (blink < 10) {
                         LcdPrintStringS(0,7,"  ");
                    }
                }
                if (isButtonIncrease()) {
                    temp_minute = (temp_minute + 1) % 60;
                }
                if (isButtonDecrease()) {
                    temp_minute--;
                    if (temp_minute > 59) temp_minute = 59;
                }
                if (isButtonMode()) {
                    clockState = CHANGE_SECOND;
                }
            break;
            case CHANGE_SECOND:
                
                DisplayTempTime(temp_hour, temp_minute, temp_second);
                if (key_code[0] == 0 && key_code[1] == 0) {
                   if (blink < 10) {
                         LcdPrintStringS(0,10,"  ");
                    }
                }
                if (isButtonIncrease()) {
                    temp_second = (temp_second + 1) % 60;
                }
                if (isButtonDecrease()) {
                    temp_second--;
                    if (temp_second > 59) temp_second = 59;
                }
                if (isButtonMode()) {
                    clockState = SET_TIME;
                }
            break;
            case SET_TIME:
                second = temp_second;
                minute = temp_minute;
                hour = temp_hour;
                clockState = INIT_CLOCK;
            break;
        default:
            clockState = INIT_CLOCK;
            break;
            
    }
    blink = (blink + 1) % 20;
}

void smolLateClock() {
    switch (clockState) {
        case INIT_CLOCK:
            LcdPrintLineS(1,"1.UP 2.DN 3.MODE");
            DisplayTempTime(late_hour, late_minute, late_second);
            if (isButtonMode()) {
                clockState = CHANGE_HOUR;
            }
        break;

        case CHANGE_HOUR:
            
            DisplayTempTime(late_hour, late_minute, late_second);
            if (key_code[0] == 0 && key_code[1] == 0) {
               if (blink < 10) {
                     LcdPrintStringS(0,4,"  ");
                }
            }
            if (isButtonIncrease()) {
                late_hour = (late_hour + 1) % 24;
                
            }
            if (isButtonDecrease()) {
                late_hour--;
                if (late_hour > 23) late_hour = 23;
                
            }
            if (isButtonMode()) {
                clockState = CHANGE_MINUTE;
            }
        break;
        case CHANGE_MINUTE:
            
            DisplayTempTime(late_hour, late_minute, late_second);
            if (key_code[0] == 0 && key_code[1] == 0) {
                if (blink < 10) {
                     LcdPrintStringS(0,7,"  ");
                }
            }
            if (isButtonIncrease()) {
                late_minute = (late_minute + 1) % 60;
                
            }
            if (isButtonDecrease()) {
                late_minute--;
                if (late_minute > 59) late_minute = 59;
                
            }
            if (isButtonMode()) {
                clockState = CHANGE_SECOND;
            }
        break;
        case CHANGE_SECOND:
            
            DisplayTempTime(late_hour, late_minute, late_second);
            if (key_code[0] == 0 && key_code[1] == 0) {
                 if (blink < 10) {
                     LcdPrintStringS(0,10,"  ");
                }
            }
            if (isButtonIncrease()) {
                late_second = (late_second + 1) % 60;
                
            }
            if (isButtonDecrease()) {
                late_second--;
                if (late_second > 59) late_second = 59;
                
            }
            if (isButtonMode()) {
                clockState = INIT_CLOCK;
            }
        break;
        default:
            clockState = INIT_CLOCK;
        break;
            
    }
    blink = (blink + 1) % 20;
}

unsigned char isButtonIncrease1() {
    if (key_code[0] == 1 || (key_code[0] > 20 && key_code[0] % 5 == 0)) return 1;
    else return 0;
}

unsigned char isButtonDecrease1() {
    if (key_code[1] == 1  || (key_code[1] > 20 && key_code[1] % 5 == 0)) return 1;
    else return 0;
}

unsigned char isButtonMode1() {
    if (key_code[2] == 1) return 1;
    else return 0;
}