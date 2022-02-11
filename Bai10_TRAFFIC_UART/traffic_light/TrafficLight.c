#include "TrafficLight.h"

#include "..\i2c\i2c.h"
#include "..\lcd\lcd.h"
#include "..\button_matrix\button.h"
#include "..\clock\simu_clock.h"        //use this for simulator
#include <stdlib.h>


unsigned char option = MENU;
//variable use for main menu
unsigned char menu_page = 0;
unsigned char menu[NUM_OF_PAGES] = {
    DISPLAY,
    SETTING
};

//variable use for display
unsigned char display_page = 0;
unsigned char display_menu[DIS_NUM_OF_PAGES] = {
    NORMAL,
    RUSH,
    SLOW
};


//variable use for setting menu
unsigned char setting_menu_page = 0;
unsigned char setting_menu[NUM_OF_PAGES] = {
    NORMAL_SETTING,
    RUSH_SETTING
};

//variable use for setting
unsigned char setting_page = 0;
unsigned char normal_setting_menu[NUM_OF_PAGES] = {
    P1_NORMAL_SETTING,
    P2_NORMAL_SETTING
};
unsigned char rush_setting_menu[NUM_OF_PAGES] = {
    P1_RUSH_SETTING,
    P2_RUSH_SETTING
};
unsigned char timeState = CHANGE_GREEN;
unsigned int blinking = 0;

//variable use for display
unsigned char statusOfLight = INIT_SYSTEM;

//Period of lights
unsigned char timeOfGreenPhase1 = 0;
unsigned char timeOfYellowPhase1 = 0;
unsigned char timeOfGreenPhase2 = 0;
unsigned char timeOfYellowPhase2 = 0;

// Normal mode
unsigned char normalOfGreenPhase1 = 25;
unsigned char normalOfYellowPhase1 = 3;
unsigned char normalOfGreenPhase2 = 20;
unsigned char normalOfYellowPhase2 = 3;

// Rush mode
unsigned char rushOfGreenPhase1 = 15;
unsigned char rushOfYellowPhase1 = 3;
unsigned char rushOfGreenPhase2 = 10;
unsigned char rushOfYellowPhase2 = 5;

//Mode signal
unsigned char MODE = 'X';

//time
unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;



//function prototype

//Status of light Phase
void Phase1_GreenOn();
void Phase1_GreenOff();
void Phase1_YellowOn();
void Phase1_YellowOff();
void Phase1_RedOn();
void Phase1_RedOff();

void Phase2_GreenOn();
void Phase2_GreenOff();
void Phase2_YellowOn();
void Phase2_YellowOff();
void Phase2_RedOn();
void Phase2_RedOff();

//thanh dieu huong
unsigned char isButtonNext();
unsigned char isButtonEnter();
unsigned char isButtonBack();

//Period setting functions
void NormalTimeSetting1();
void NormalTimeSetting2();
void RushTimeSetting1();
void RushTimeSetting2();
unsigned char isButtonIncrease();
unsigned char isButtonDecrease();
unsigned char isButtonMode();

//reset
void reset_package();

//Main funtions
void TrafficLight();
void MainMenu();

// Function implementation
void Phase1_GreenOn()
{
    OpenOutput(0);
}
void Phase1_GreenOff()
{
    CloseOutput(0);
}

void Phase1_YellowOn()
{
    OpenOutput(4);
}
void Phase1_YellowOff()
{
    CloseOutput(4);
}

void Phase1_RedOn()
{
    OpenOutput(6);
}
void Phase1_RedOff()
{
    CloseOutput(6);
}

void Phase2_GreenOn()
{
    OpenOutput(1);
}
void Phase2_GreenOff()
{
    CloseOutput(1);
}

void Phase2_YellowOn()
{
    OpenOutput(5);
}
void Phase2_YellowOff()
{
    CloseOutput(5);
}

void Phase2_RedOn()
{
    OpenOutput(7);
}
void Phase2_RedOff()
{
    CloseOutput(7);
}

unsigned char isButtonNext()
{
    if (key_code[6] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonEnter()
{
    if (key_code[5] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonBack()
{
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonIncrease() {
    if (key_code[6] == 1 || (key_code[6] > 20 && key_code[6] % 5 == 0)) return 1;
    else return 0;
}

unsigned char isButtonDecrease() {
    if (key_code[4] == 1  || (key_code[4] > 20 && key_code[4] % 5 == 0)) return 1;
    else return 0;
}

unsigned char isButtonMode() {
    if (key_code[5] == 1) return 1;
    else return 0;
}



void NormalTimeSetting1() {
    switch (timeState) {
        case CHANGE_GREEN:
            LcdPrintStringS(0,0,"PHASE1 GREEN :   ");
            LcdPrintNumS(0,14,normalOfGreenPhase1);
            LcdPrintStringS(1,0,"PHASE1 YELLOW:   ");
            LcdPrintNumS(1,14,normalOfYellowPhase1);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(0,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                normalOfGreenPhase1 = (normalOfGreenPhase1 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                normalOfGreenPhase1--;
                if (normalOfGreenPhase1 < 0) normalOfGreenPhase1 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_YELLOW;
            }
            break;
        
        case CHANGE_YELLOW:
            LcdPrintStringS(0,0,"PHASE1 GREEN :  ");
            LcdPrintNumS(0,14,normalOfGreenPhase1);
            LcdPrintStringS(1,0,"PHASE1 YELLOW:   ");
            LcdPrintNumS(1,14,normalOfYellowPhase1);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(1,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                normalOfYellowPhase1 = (normalOfYellowPhase1 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                normalOfYellowPhase1--;
                if (normalOfYellowPhase1 < 0) normalOfYellowPhase1 = 0;
                
            }
            if (isButtonMode()) {
                option = NORMAL_SETTING;
                timeState = CHANGE_GREEN;
                reset_package();
            }
            break;
    }
    
    blinking = (blinking + 1) % 20;
}

void NormalTimeSetting2() {
    switch (timeState) {
        case CHANGE_GREEN:
            LcdPrintStringS(0,0,"PHASE2 GREEN :   ");
            LcdPrintNumS(0,14,normalOfGreenPhase2);
            LcdPrintStringS(1,0,"PHASE2 YELLOW:   ");
            LcdPrintNumS(1,14,normalOfYellowPhase2);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(0,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                normalOfGreenPhase2 = (normalOfGreenPhase2 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                normalOfGreenPhase2--;
                if (normalOfGreenPhase2 < 0) normalOfGreenPhase2 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_YELLOW;
            }
            break;
        
        case CHANGE_YELLOW:
            LcdPrintStringS(0,0,"PHASE2 GREEN :   ");
            LcdPrintNumS(0,14,normalOfGreenPhase2);
            LcdPrintStringS(1,0,"PHASE2 YELLOW:   ");
            LcdPrintNumS(1,14,normalOfYellowPhase2);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(1,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                normalOfYellowPhase2 = (normalOfYellowPhase2 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                normalOfYellowPhase2--;
                if (normalOfYellowPhase2 < 0) normalOfYellowPhase2 = 0;
                
            }
            if (isButtonMode()) {
                option = NORMAL_SETTING;
                timeState = CHANGE_GREEN;
                reset_package();
            }
            break;
    }
    
    blinking = (blinking + 1) % 20;
}

void RushTimeSetting1() {
    switch (timeState) {
        case CHANGE_GREEN:
            LcdPrintStringS(0,0,"PHASE1 GREEN :   ");
            LcdPrintNumS(0,14,rushOfGreenPhase1);
            LcdPrintStringS(1,0,"PHASE1 YELLOW:   ");
            LcdPrintNumS(1,14,rushOfYellowPhase1);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(0,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                rushOfGreenPhase1 = (rushOfGreenPhase1 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                rushOfGreenPhase1--;
                if (rushOfGreenPhase1 < 0) rushOfGreenPhase1 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_YELLOW;
            }
            break;
        
        case CHANGE_YELLOW:
            LcdPrintStringS(0,0,"PHASE1 GREEN :  ");
            LcdPrintNumS(0,14,rushOfGreenPhase1);
            LcdPrintStringS(1,0,"PHASE1 YELLOW:   ");
            LcdPrintNumS(1,14,rushOfYellowPhase1);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(1,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                rushOfYellowPhase1 = (rushOfYellowPhase1 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                rushOfYellowPhase1--;
                if (rushOfYellowPhase1 < 0) rushOfYellowPhase1 = 0;
                
            }
            if (isButtonMode()) {
                option = RUSH_SETTING;
                timeState = CHANGE_GREEN;
                reset_package();
            }
            break;
    }
    
    blinking = (blinking + 1) % 20;
}

void RushTimeSetting2() {
    switch (timeState) {
        case CHANGE_GREEN:
            LcdPrintStringS(0,0,"PHASE1 GREEN :   ");
            LcdPrintNumS(0,14,rushOfGreenPhase2);
            LcdPrintStringS(1,0,"PHASE1 YELLOW:   ");
            LcdPrintNumS(1,14,rushOfYellowPhase2);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(0,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                rushOfGreenPhase2 = (rushOfGreenPhase2 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                rushOfGreenPhase2--;
                if (rushOfGreenPhase2 < 0) rushOfGreenPhase2 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_YELLOW;
            }
            break;
        
        case CHANGE_YELLOW:
            LcdPrintStringS(0,0,"PHASE1 GREEN :  ");
            LcdPrintNumS(0,14,rushOfGreenPhase2);
            LcdPrintStringS(1,0,"PHASE1 YELLOW:   ");
            LcdPrintNumS(1,14,rushOfYellowPhase2);
            
            if (key_code[4] == 0 && key_code[6] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(1,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                rushOfYellowPhase2 = (rushOfYellowPhase2 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                rushOfYellowPhase2--;
                if (rushOfYellowPhase2 < 0) rushOfYellowPhase2 = 0;
                
            }
            if (isButtonMode()) {
                option = RUSH_SETTING;
                timeState = CHANGE_GREEN;
                reset_package();
            }
            break;
    }
    
    blinking = (blinking + 1) % 20;
}

void reset_package() {
    LcdClearS();
    statusOfLight = 0;
    timeOfLight = 0;
    cntOfLight = 0;
//    timeDelay = 0;
//    indexOfNumber = 0;
//    indexOfID = 0;
}

void TrafficLight()
{
    cntOfLight = (cntOfLight + 1)%20;
    if (cntOfLight == 0)
        timeOfLight --;
    switch (statusOfLight)
    {
        case PHASE1_GREEN:
            Phase1_GreenOn();
            Phase1_RedOff();
            Phase2_RedOn();
            LcdPrintStringS(1,0,"PHASE1_GREEN:   ");
            LcdPrintNumS(1,14,timeOfLight);
            LcdPrintCharS(0,0, LONG_LEFT_ARROW);
            DisplayRealTime();
            LcdPrintCharS(0,15, MODE);
            
            if (timeOfLight == 0)
            {
                statusOfLight = PHASE1_YELLOW;
                Phase1_GreenOff();
                timeOfLight = timeOfYellowPhase1;
            }
            break;
        case PHASE1_YELLOW:
            Phase1_YellowOn();
            Phase2_RedOn();
            LcdPrintStringS(1,0,"PHASE1_YELLOW:  ");
            LcdPrintNumS(1,14,timeOfLight);
            LcdPrintCharS(0,0, LONG_LEFT_ARROW);
            DisplayRealTime();
            LcdPrintCharS(0,15, MODE);
            
            if (timeOfLight == 0)
            {
                statusOfLight = PHASE2_GREEN;
                Phase1_YellowOff();
                timeOfLight = timeOfGreenPhase2;
            }
            break;
        case PHASE2_GREEN:
            Phase2_GreenOn();
            Phase2_RedOff();
            Phase1_RedOn();
            LcdPrintStringS(1,0,"PHASE2_GREEN:   ");
            LcdPrintNumS(1,14,timeOfLight);
            LcdPrintCharS(0,0, LONG_LEFT_ARROW);
            DisplayRealTime();
            LcdPrintCharS(0,15, MODE);

            if (timeOfLight == 0)
            {
                statusOfLight = PHASE2_YELLOW;
                Phase2_GreenOff();
                timeOfLight = timeOfYellowPhase2;
            }
            break;
        case PHASE2_YELLOW:
            Phase2_YellowOn();
            Phase1_RedOn();
            LcdPrintStringS(1,0,"PHASE2_YELLOW:  ");
            LcdPrintNumS(1,14,timeOfLight);
            LcdPrintCharS(0,0, LONG_LEFT_ARROW);
            DisplayRealTime();
            LcdPrintCharS(0,15, MODE);

            if (timeOfLight == 0)
            {
                statusOfLight = PHASE1_GREEN;
                Phase2_YellowOff();
                timeOfLight = timeOfGreenPhase1;
            }
            break;
        case WAIT:
            Phase1_GreenOff();
            Phase2_GreenOff();
            Phase1_RedOff();
            Phase2_RedOff();
            if (cntOfLight<=12)
            {
                Phase1_YellowOn();
                Phase2_YellowOn();
                LcdPrintLineS(1, "   SLOWLY RUN!  ");
                LcdPrintCharS(0,0, LONG_LEFT_ARROW);
                DisplayRealTime();
                LcdPrintCharS(0,15, MODE);
            }
            else
            {
                Phase1_YellowOff();
                Phase2_YellowOff();
                LcdPrintLineS(1, "              ");
                LcdPrintCharS(0,0, LONG_LEFT_ARROW);
                DisplayRealTime();
                LcdPrintCharS(0,15, MODE);
            }
            break;
        default:
            statusOfLight = PHASE1_GREEN;
            break;
    }
}

void MainMenu() {
    switch(option){
        case MENU:
            switch(menu_page){
                case 0:
                    LcdPrintLineS(0, "1.DISPLAY LIGHTS");
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 1:
                    LcdPrintLineS(0, "   2.SETTING    ");
                    LcdPrintCharS(1,0,LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    break;
                default:
                    menu_page = 0;
                    break;
            }
            
            if (isButtonNext()){
                menu_page++;
                if (menu_page >= NUM_OF_PAGES)
                    menu_page = NUM_OF_PAGES - 1;
                reset_package();
            }
            if (isButtonBack()){
                if (menu_page <= 0){
                    menu_page = 0;
                }
                else {
                    menu_page--;
                    reset_package();
                }
            }
            if (isButtonEnter()) {
                option = menu[menu_page];
                reset_package();
            }
            
            break;
        
        case DISPLAY:
            switch(display_page){
                case 0:
                    
                    LcdPrintLineS(0, " 1.NORMAL MODE  ");
                    LcdPrintCharS(1,0, LONG_LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 1:
                    LcdPrintLineS(0, "  2.RUSH MODE   ");
                    LcdPrintCharS(1,0,LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 2:
                    LcdPrintLineS(0, "  3.SLOW MODE   ");
                    LcdPrintCharS(1,0,LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    break;
                default:
                    display_page = 0;
                    break;
            }
            if (isButtonNext()){
                display_page++;
                if (display_page >= DIS_NUM_OF_PAGES)
                    display_page = DIS_NUM_OF_PAGES - 1;
                reset_package();
            }
            if (isButtonBack()){
                if (display_page <= 0){
                    display_page = 0;
                    option = MENU;
                    reset_package();
                }
                else {
                    display_page--;
                    reset_package();
                }
            }
            if (isButtonEnter()) {
                option = display_menu[display_page];
                reset_package();
            }
            
            break;

            
        case NORMAL:
            //update time mode
            timeOfGreenPhase1 = normalOfGreenPhase1;
            timeOfYellowPhase1 = normalOfYellowPhase1;
            timeOfGreenPhase2 = normalOfGreenPhase2;
            timeOfYellowPhase2 = normalOfYellowPhase2;
            //update MODE
            MODE = 'N';
            
            TrafficLight();
            
            if (isButtonBack()) {
                option = DISPLAY;
                reset_package();
            }
            break;
            
        case RUSH:
            //update time mode
            timeOfGreenPhase1 = rushOfGreenPhase1;
            timeOfYellowPhase1 = rushOfYellowPhase1;
            timeOfGreenPhase2 = rushOfGreenPhase2;
            timeOfYellowPhase2 = rushOfYellowPhase2;
            //update MODE
            MODE = 'R';
            
            TrafficLight();
            
            if (isButtonBack()) {
                option = DISPLAY;
                reset_package();
            }
            break;
            
        case SLOW:
            //update MODE
            MODE = 'S';
            //update state
            statusOfLight = WAIT;
            
            TrafficLight();
            if (isButtonBack()) {
                option = DISPLAY;
                reset_package();
                statusOfLight = 0;
            }
            break;
            
        case SETTING:
            switch(setting_menu_page){
                case 0:
                    LcdPrintLineS(0, "1.NORMAL SETTING ");
                    LcdPrintCharS(1,0, LONG_LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 1:
                    LcdPrintLineS(0, " 2.RUSH SETTING  ");
                    LcdPrintCharS(1,0,LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    break;
                default:
                    setting_menu_page = 0;
                    break;
            }
            if (isButtonNext()){
                setting_menu_page++;
                if (setting_menu_page >= NUM_OF_PAGES)
                    setting_menu_page = NUM_OF_PAGES - 1;
                reset_package();
            }
            if (isButtonBack()){
                if (setting_menu_page <= 0){
                    setting_menu_page = 0;
                    option = MENU;
                    reset_package();
                }
                else {
                    setting_menu_page--;
                    reset_package();
                }
            }
            if (isButtonEnter()) {
                option = setting_menu[setting_menu_page];
                reset_package();
            }
            
            break;
            
        case NORMAL_SETTING:
            switch(setting_page){
                case 0:
                    
                    LcdPrintLineS(0, "   1.PHASE 1    ");
                    LcdPrintCharS(1,0, LONG_LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 1:
                    LcdPrintLineS(0, "   2.PHASE 2    ");
                    LcdPrintCharS(1,0,LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    break;
                default:
                    setting_page = 0;
                    break;
            }
            if (isButtonNext()){
                setting_page++;
                if (setting_page >= NUM_OF_PAGES)
                    setting_page = NUM_OF_PAGES - 1;
                reset_package();
            }
            if (isButtonBack()){
                if (setting_page <= 0){
                    setting_page = 0;
                    option = SETTING;
                    reset_package();
                }
                else {
                    setting_page--;
                    reset_package();
                }
            }
            if (isButtonEnter()) {
                option = normal_setting_menu[setting_page];
                reset_package();
            }
            
            break;
            
        case P1_NORMAL_SETTING:
            NormalTimeSetting1();
            break;
        case P2_NORMAL_SETTING:
            NormalTimeSetting2();
            break;
            
        case RUSH_SETTING:
            switch(setting_page){
                case 0:
                    LcdPrintLineS(0, "   1.PHASE 1    ");
                    LcdPrintCharS(1,0, LONG_LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 1:
                    LcdPrintLineS(0, "   2.PHASE 2    ");
                    LcdPrintCharS(1,0,LEFT_ARROW);
                    LcdPrintCharS(1,7,CENTER_NODE);
                    break;
                default:
                    setting_page = 0;
                    break;
            }
            if (isButtonNext()){
                setting_page++;
                if (setting_page >= NUM_OF_PAGES)
                    setting_page = NUM_OF_PAGES - 1;
                reset_package();
            }
            if (isButtonBack()){
                if (setting_page <= 0){
                    setting_page = 0;
                    option = SETTING;
                    reset_package();
                }
                else {
                    setting_page--;
                    reset_package();
                }
            }
            if (isButtonEnter()) {
                option = rush_setting_menu[setting_page];
                reset_package();
            }
            
            break;
            
        case P1_RUSH_SETTING:
            RushTimeSetting1();
            break;
        case P2_RUSH_SETTING:
            RushTimeSetting2();
            break;
    }
        
}


