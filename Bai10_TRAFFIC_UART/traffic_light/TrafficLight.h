/* 
 * File:   TrafficLight.h
 * Author: Dell
 *
 * Created on February 11, 2022, 8:17 PM
 */

#ifndef TRAFFICLIGHT_H
#define	TRAFFICLIGHT_H

// MAIN MENU
#define     MENU                0
#define     DISPLAY             1
#define     NORMAL              2
#define     RUSH                3
#define     SLOW                4
#define     SETTING             5
#define     NORMAL_SETTING      6
#define     RUSH_SETTING        7
#define     P1_NORMAL_SETTING   8
#define     P2_NORMAL_SETTING   9
#define     P1_RUSH_SETTING     10
#define     P2_RUSH_SETTING     11

#define     NUM_OF_PAGES        2
#define     DIS_NUM_OF_PAGES    3

//setting light time
#define     CHANGE_GREEN        0
#define     CHANGE_YELLOW       1

//display
#define     INIT_SYSTEM         255
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
#define     WAIT                4

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



#endif	/* TRAFFICLIGHT_H */

