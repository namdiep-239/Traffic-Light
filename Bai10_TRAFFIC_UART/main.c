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
#define     MENU                0
#define     DISPLAY             1
#define     SETTING             2
#define     P1_SETTING          3
#define     P2_SETTING          4

unsigned char option = MENU;
//variable use for main menu
unsigned char menu_page = 0;
unsigned char menu[NUM_OF_PAGES] = {
    DISPLAY,
    SETTING
};

//variable use for setting
unsigned char setting_page = 0;
unsigned char setting_menu[NUM_OF_PAGES] = {
    P1_SETTING,
    P2_SETTING
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
unsigned char timeOfGreenPhase1 = 25;
unsigned char timeOfYellowPhase1 = 3;
unsigned char timeOfGreenPhase2 = 20;
unsigned char timeOfYellowPhase2 = 3;
unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
unsigned char tp;
unsigned char k;

void TrafficLight();
void MainMenu();
void TimeSetting1();
void TimeSetting2();
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
            LcdPrintStringS(1,0,"PHASE1_GREEN:   ");
            LcdPrintNumS(1,14,timeOfLight);

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
            }
            else
            {
                Phase1_YellowOff();
                Phase2_YellowOff();
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

void reset_smol_package() {
    LcdClearS();
//    timeDelay = 0;
//    indexOfNumber = 0;
//    indexOfID = 0;
}

unsigned char isButtonIncrease() {
    if (key_code[0] == 1 || (key_code[0] > 20 && key_code[0] % 5 == 0)) return 1;
    else return 0;
}

unsigned char isButtonDecrease() {
    if (key_code[1] == 1  || (key_code[1] > 20 && key_code[1] % 5 == 0)) return 1;
    else return 0;
}

unsigned char isButtonMode() {
    if (key_code[2] == 1) return 1;
    else return 0;
}



void TimeSetting1() {
    switch (timeState) {
        case CHANGE_GREEN:
            LcdPrintStringS(0,0,"GREEN PHASE1:   ");
            LcdPrintNumS(0,14,timeOfGreenPhase1);
            LcdPrintStringS(1,0,"YELLOW PHASE1:   ");
            LcdPrintNumS(1,14,timeOfYellowPhase1);
            
            if (key_code[0] == 0 && key_code[1] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(0,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                timeOfGreenPhase1 = (timeOfGreenPhase1 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                timeOfGreenPhase1--;
                if (timeOfGreenPhase1 < 0) timeOfGreenPhase1 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_YELLOW;
            }
            break;
        
        case CHANGE_YELLOW:
            LcdPrintStringS(0,0,"GREEN PHASE1:   ");
            LcdPrintNumS(0,14,timeOfGreenPhase1);
            LcdPrintStringS(1,0,"YELLOW PHASE1:   ");
            LcdPrintNumS(1,14,timeOfYellowPhase1);
            
            if (key_code[0] == 0 && key_code[1] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(1,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                timeOfYellowPhase1 = (timeOfYellowPhase1 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                timeOfYellowPhase1--;
                if (timeOfYellowPhase1 < 0) timeOfYellowPhase1 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_GREEN;
            }
            break;
    }
    
    blinking = (blinking + 1) % 20;
}

void TimeSetting2() {
    switch (timeState) {
        case CHANGE_GREEN:
            LcdPrintStringS(0,0,"GREEN PHASE2:   ");
            LcdPrintNumS(0,14,timeOfGreenPhase2);
            LcdPrintStringS(1,0,"YELLOW PHASE2:   ");
            LcdPrintNumS(1,14,timeOfYellowPhase2);
            
            if (key_code[0] == 0 && key_code[1] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(0,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                timeOfGreenPhase2 = (timeOfGreenPhase2 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                timeOfGreenPhase2--;
                if (timeOfGreenPhase2 < 0) timeOfGreenPhase2 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_YELLOW;
            }
            break;
        
        case CHANGE_YELLOW:
            LcdPrintStringS(0,0,"GREEN PHASE1:   ");
            LcdPrintNumS(0,14,timeOfGreenPhase2);
            LcdPrintStringS(1,0,"YELLOW PHASE1:   ");
            LcdPrintNumS(1,14,timeOfYellowPhase2);
            
            if (key_code[0] == 0 && key_code[1] == 0) {
               if (blinking < 10) {
                     LcdPrintStringS(1,14,"  ");
                }
            }
            if (isButtonIncrease()) {
                timeOfYellowPhase2 = (timeOfYellowPhase2 + 1) % 100;
                
            }
            if (isButtonDecrease()) {
                timeOfYellowPhase2--;
                if (timeOfYellowPhase2 < 0) timeOfYellowPhase2 = 0;
                
            }
            if (isButtonMode()) {
                timeState = CHANGE_GREEN;
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
                    LcdPrintLineS(0, "2.SETTING");
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
                reset_smol_package();
            }
            if (isButtonBack()){
                if (menu_page <= 0){
                    menu_page = 0;
                }
                else {
                    menu_page--;
                    reset_smol_package();
                }
            }
            if (isButtonEnter()) {
                option = menu[menu_page];
                reset_smol_package();
            }
            
            break;
        
        case DISPLAY:
            TrafficLight();
            if (isButtonBack()) {
                option = MENU;
                reset_smol_package();
            }
            break;
            
        case SETTING:
            switch(setting_page){
                case 0:
                    LcdPrintLineS(0, "1.PHASE 1");
                    LcdPrintCharS(1,7,CENTER_NODE);
                    LcdPrintCharS(1,14,RIGHT_ARROW);
                    break;
                case 1:
                    LcdPrintLineS(0, "2.PHASE 2");
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
                reset_smol_package();
            }
            if (isButtonBack()){
                if (setting_page <= 0){
                    setting_page = 0;
                    option = MENU;
                    reset_smol_package();
                }
                else {
                    setting_page--;
                    reset_smol_package();
                }
            }
            if (isButtonEnter()) {
                option = setting_menu[setting_page];
                reset_smol_package();
            }
            
            break;
            
        case P1_SETTING:
            TimeSetting1();
            if (isButtonBack()) {
                option = SETTING;
                reset_smol_package();
            }
            break;
        case P2_SETTING:
            TimeSetting2();
            if (isButtonBack()) {
                option = SETTING;
                reset_smol_package();
            }
            break;
    }
        
}

