#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM        255
#define     ENTER_PASSWORD     1
#define     CHECK_PASSWORD     2
#define     UNLOCK_DOOR        3
#define     WRONG_PASSWORD     4

#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [5][4]= {
  {1,2,3,4},
  {2,7,8,9},
  {3,3,3,3},
  {4,8,6,8},
  {5,'A','B','C'},
};
unsigned char arrayPassword[4];
unsigned char statusPassword = INIT_SYSTEM;
unsigned char indexOfNumber = 0;
unsigned char timeDelay = 0;

void App_PasswordDoor();
unsigned char CheckPassword();
unsigned char isButtonNumber();
unsigned char numberValue;
//unsigned char isButtonEnter();
void UnlockDoor();
void LockDoor();

// Den giao thong
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
unsigned char isButtonNumber();
unsigned char isButtonEnter();
unsigned char isButtonBack();

//app MENU
#define     NUM_OF_PAGES        2
#define     DIS_NUM_OF_PAGES    3
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

#define     CHANGE_GREEN        0
#define     CHANGE_YELLOW       1
unsigned char timeState = CHANGE_GREEN;
unsigned int blinking = 0;

#define     INIT_SYSTEM         255
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
#define     WAIT                4

unsigned char statusOfLight = INIT_SYSTEM;

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


unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
unsigned char tp;
unsigned char k;

void TrafficLight();
void MainMenu();

void NormalTimeSetting1();
void NormalTimeSetting2();
void RushTimeSetting1();
void RushTimeSetting2();

void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0;
	init_system();
        delay_ms(1000);
	while (1)
	{
            while (!flag_timer3);
            flag_timer3 = 0;
            scan_key_matrix_with_uart(); // 8 button
//            scan_key_matrix();
            k = (k + 1)% 20;
            DisplayDataReceive();
            UartDataReceiveProcess();
//            TrafficLight();
            MainMenu();

//            DisplayLcdScreenOld(); //Output process 14ms vs 10ms with no timer
            DisplayLcdScreen();
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
    PORTD_OUT = 0x00;
}

void init_system(void)
{
    init_output();
    lcd_init();
    init_key_matrix_with_uart_i2c();
    init_interrupt();
    init_i2c();
    init_uart();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
    
    //g?i hàm này ?? dùng các icon
    init_user_defined_char();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}


}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void App_PasswordDoor()
{
    switch (statusPassword)
    {
        case INIT_SYSTEM:
            LcdPrintStringS(0,0,"PRESS # FOR PASS");
            LcdPrintStringS(1,0,"                ");
            LockDoor();
            if (isButtonEnter())
            {
                indexOfNumber = 0;
                timeDelay = 0;
                statusPassword = ENTER_PASSWORD;
            }
            break;
        case ENTER_PASSWORD:
            LcdPrintStringS(0,0,"ENTER PASSWORD  ");
            timeDelay ++;
            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if (indexOfNumber >= 4)
                statusPassword = CHECK_PASSWORD;
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case CHECK_PASSWORD:
            timeDelay = 0;
            if (CheckPassword())
                statusPassword = UNLOCK_DOOR;
            else
                statusPassword = WRONG_PASSWORD;
            break;
        case UNLOCK_DOOR:
            timeDelay ++;
            LcdPrintStringS(0,0,"OPENING DOOR    ");
            UnlockDoor();
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            break;
        case WRONG_PASSWORD:
            timeDelay ++;
            LcdPrintStringS(0,0,"PASSWORD WRONG  ");
            if (timeDelay >= 40)
                statusPassword = INIT_SYSTEM;
            break;
        default:
            break;

    }
}
unsigned char CheckPassword()
{
    unsigned char i,j;
    unsigned result = 1;
    for (i=0;i<5;i++)
    {
        result = 1;
        for (j=0;j<4;j++)
        {
            if (arrayPassword[j] != arrayMapOfPassword[i][j])
                result = 0;
        }
        if (result == 1)
            return (i+1);
    }

}
unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 10)
        {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    return 0;
}

//unsigned char isButtonEnter()
//{
//    if (key_code[14] == 10)
//        return 1;
//    else
//        return 0;
//}
void UnlockDoor()
{
    OpenOutput(0);
}
void LockDoor()
{
    CloseOutput(0);
}

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
            LcdPrintStringS(0,0,"PHASE1_GREEN:   ");
            LcdPrintNumS(0,14,timeOfLight);
            LcdPrintCharS(1,0, LONG_LEFT_ARROW);

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
            LcdPrintStringS(0,0,"PHASE1_YELLOW:  ");
            LcdPrintNumS(0,14,timeOfLight);
            LcdPrintCharS(1,0, LONG_LEFT_ARROW);

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
            LcdPrintStringS(0,0,"PHASE2_GREEN:   ");
            LcdPrintNumS(0,14,timeOfLight);
            LcdPrintCharS(1,0, LONG_LEFT_ARROW);

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
            LcdPrintStringS(0,0,"PHASE2_YELLOW:  ");
            LcdPrintNumS(0,14,timeOfLight);
            LcdPrintCharS(1,0, LONG_LEFT_ARROW);

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
                LcdPrintLineS(0, "   SLOWLY RUN!  ");
                LcdPrintCharS(1,0, LONG_LEFT_ARROW);
            }
            else
            {
                Phase1_YellowOff();
                Phase2_YellowOff();
                LcdPrintLineS(0, "              ");
                LcdPrintCharS(1,0, LONG_LEFT_ARROW);
            }
            break;
        default:
            statusOfLight = PHASE1_GREEN;
            break;
    }
}

void UartDataReceiveProcess()
{
    if(flagOfDataReceiveComplete == 1)
    {
        flagOfDataReceiveComplete = 0;
        if (dataReceive[0] == 0)
        {
            timeOfGreenPhase1 = dataReceive[1];
            timeOfYellowPhase1 = dataReceive[2];
            timeOfGreenPhase2 = dataReceive[3];
            timeOfYellowPhase2 = dataReceive[4];
        }
    }
    LcdPrintNumS(0,15,statusReceive+4);
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
            
            TrafficLight();
            
            if (isButtonBack()) {
                option = DISPLAY;
                reset_package();
            }
            break;
            
        case SLOW:
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

