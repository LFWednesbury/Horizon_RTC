 /*
 * main.h
 *
 * Created: 06/11/2018 13:21:25
 *  Author: mariusz.dudek
 */ 

 
#ifndef MAIN_H_
#define MAIN_H_
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <stdlib.h>
#include <string.h>
#include <avr/wdt.h>
#include <ctype.h>
#include <stdio.h>
#include <avr/boot.h>
#include <avr/wdt.h>
// Variables
uint8_t Current_Year = 0,  Current_Month = 0,  Current_Day = 0,  Current_Hour = 0, Current_Minute = 0;
uint32_t Received_number = 0;
uint8_t Stop_Hour , Stop_Day , Stop_Month , Stop_Year , Start_Hour, Start_Day, Start_Month, Start_Year, CS , Spare ;

//uint8_t Stop_Hour_Temp, Stop_Day_Temp, Stop_Month_Temp, Stop_Year_Temp, Start_Hour_Temp, Start_Day_Temp, Start_Month_Temp, Start_Year_Temp;
		
uint8_t CS_Calc = 0;
uint8_t key_stroke [4][11] = {
	{0,1,2,3,4,5,6,7,8,9,10},	//0 NORM
	{1,0,9,8,7,6,5,4,3,2,10},	//1 UPSD
	{5,6,1,7,2,8,3,9,4,0,10},	//2 LEFT
	{6,5,0,4,9,3,8,2,7,1,10}};	//3 RIGHT
uint8_t Public_Private_Mode_Code_Lenght;
uint8_t Wrong_Code_Attempts_Allowance;
uint8_t Lock_Block_Time;
uint8_t Number_Of_New_Code_Entires = 1;
uint8_t Required_Number_Of_New_Code_Entires;
uint8_t Position;
uint8_t WDT_counter_1 = 0;
uint8_t WDT_counter_2 = 0;
unsigned char UART_Data = 0;
uint8_t SPI_Data_Buffer[100];
uint8_t UART_RX_Flag = 0; // 1- INT has been triggered
uint16_t Timeout_Counter;
uint16_t Timeout_Constant = 1500; //default value
uint8_t  Wrong_Code_Attempts_Clear_Flag = 0;
uint8_t Temp_Main, Temp_0;
uint8_t Temp_1[20];
uint8_t Previous_Lock_State;
uint8_t Previous_Lock_Mode;
uint8_t i;
uint8_t Count_1;
uint8_t Keycode[14] = {11,11,11,11,11,11,11,11,11,11,11,11,11,11};
uint8_t Mastercode[8] = {1,1,3,3,5,5,7,7};
uint8_t Submastercode[6] = {4,4,6,6,8,8};
uint8_t Techniciancode[6] = {3,3,5,5,7,7};
uint8_t Multiuser_Location;
uint8_t Multiuser_Code[4];
uint8_t Who_Unlocked;
uint8_t User_Type;
uint8_t Usercode_4[4] = {2,2,4,4};
uint8_t Usercode_6[6] = {2,2,4,4,6,6};
uint8_t Usercode_RAS_RTC_4[4];
uint8_t Usercode_RAS_RTC_6[6];
uint8_t Max_Allowed_Position;
//uint8_t Communication_Allowed_Flag = 0;
//uint16_t Audit_Trial_Counter = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
#define Max_Allowed_Multiuser_Number			50
uint8_t MultiUsercode[Max_Allowed_Multiuser_Number][4];
uint8_t number_1, number_2, number_3;
uint8_t Wrong_Code_Attempts_Count;
//uint8_t Lock_Block_Time;
uint8_t Public_Mode_Code_Entry_2[8];
uint8_t Public_Mode_Code_Entry_1[8];
uint8_t Temp_Array[8] = {11,11,11,11,11,11,11,11} ; 
uint8_t Multiuser_Location;
uint8_t Multiuser_Code[4];
//uint8_t Auto_Timed_Code_Cancel_Time[2] = {0,0};
//uint8_t Auto_Timed_Code_Cancel_Flag = 0;
uint8_t Auto_Timed_Unlock_Time[2] = {0,0};
uint8_t Auto_Timed_Unlock_Flag = 0;
uint8_t Allow_User_To_Change_Own_Code_Flag = 1; // 0 disallow, 1 allow
uint8_t LED_ON; //used to switch on and off the LED
uint8_t Function_Number;
uint8_t Unsuccessful_Code_Lockout_Flag;
#define UART_Buffer_Length 50
//uint8_t UART_Data_Arrived_Flag = 0; // when 1 it indicates that there is data followed by # char
//uint8_t UART_RX_Buffer[UART_Buffer_Length];
//uint8_t UART_RX_Buffer_Position;
//uint8_t UART_Start_Character_Position;
//uint8_t UART_Start_Character_Flag;
//uint8_t UART_Receive_Started_Flag;
//uint8_t UART_Delimiter_Position;
//uint8_t UART_Delimiter_Character_Flag;
//uint8_t UART_End_Character_Flag;
//uint8_t UART_End_Character_Position;
//uint8_t UART_Received_Command;
//uint8_t UART_Temp_Data_Buffer[UART_Buffer_Length];
//uint8_t UART_Temp_Data_Buffer_Position;
//uint8_t UART_Received_Data[UART_Buffer_Length];
//uint8_t UART_Received_Data_Position;
//uint8_t UART_Received_CRC;
//uint16_t UART_CRC_Calc_Temp_Buffer;
//uint8_t UART_Data_Ready_Flag;
//uint8_t UART_Receiving_Data_Flag;
//uint8_t BLE_VSP_Initialised_Flag = 0; 
//uint8_t UART_Error_code = 0;
uint8_t RTC_Int_Flag = 0; 
uint8_t Lock_Active_Enabled_Flag = 0; // indicates whether this option is enabled
uint8_t Lock_Active_Flag = 1; // actual flag which indicates whether lock is enabled
uint8_t Lock_Active_Start_Time_Hr; // stored as BCD
uint8_t Lock_Active_Start_Time_Min;
uint8_t Lock_Active_Stop_Time_Hr; //stored as BCD
uint8_t Lock_Active_Stop_Time_Min;
uint8_t Lock_Active_Stop_Time_Stop_Day; //stored as BCD
uint8_t Lock_Active_Stop_Time_Stop_Month;
uint8_t Lock_Active_Stop_Time_Stop_Year;
uint8_t Lock_Active_Start_Time_Hr_Temp; // stored as BCD
uint8_t Lock_Active_Start_Time_Min_Temp;
uint8_t Lock_Active_Stop_Time_Hr_Temp; //stored as BCD
uint8_t Lock_Active_Stop_Time_Min_Temp;
uint8_t A_SS, B_SS, C_SS, D_SS, E_SS, F_SS ;
// RAS mode 
int table0[10] = {4,2,9,6,7,4,7,2,9,6};
int table1[10] = {5,8,3,9,8,3,4,3,6,2};
int table2[10] = {7,9,8,3,2,7,6,5,8,7};
int table3[10] = {5,2,5,2,5,5,2,2,9,5};
int table4[10] = {7,5,2,7,4,7,8,8,6,9};
int table5[10] = {2,3,2,5,4,2,3,6,5,7};
int table6[10] = {5,8,5,6,9,5,9,3,8,5};
int table7[10] = {4,5,6,5,3,8,3,9,4,9};
int table8[10] = {9,7,9,6,8,4,6,2,7,2};
int table9[10] = {7,4,8,4,6,7,2,9,4,5};
int columb;
int rand0;
int rand1;
int rand2;
int rand3;
int wrand0;
int wrand1;
int wrand2;
int wrand3;
int val;
char serialcode[4] = {0,1,2,3};
char tempnew[4];
char initcode[4] = {11,11,11,11};
//char Usercode_4[4] = {2,2,4,4};
char nextcode[4] = {11,11,11,11};
void newcode(void);
void fetch_random(char,int);
// Macros
#define BIT(x) 			(1 << (x))
#define SETBITS(x,y) 	((x) |= (y))
#define CLEARBITS(x,y) 	((x) &= (~(y)))
#define SETBIT(x,y) 	SETBITS((x), (BIT((y))))			/* EXAMPLE SETBIT(PORTB,2) sets the 2 bit of PORTB */
#define CLEARBIT(x,y) 	CLEARBITS((x), (BIT((y))))
#define BITSET(x,y) 	((x) & (BIT(y)))
#define BITCLEAR(x,y) 	!BITSET((x), (y))
#define BITSSET(x,y) 	(((x) & (y)) == (y))
#define BITSCLEAR(x,y) 	(((x) & (y)) == 0)
#define BITVAL(x,y) 	(((x)>>(y)) & 1)
#define BITFLIP(x,y)	((x) ^= (y))
// Definitions
 #define LED_PORT	PORTC
 #define LED_R_1	0
 #define LED_G_1	1
 #define LED_R_2	2
 #define LED_G_2	3
 #define LED_R_3	4
 #define LED_G_3	5
 #define LED_R_4	6
 #define LED_G_4	7
 /////////////////////////
 //	buzzer definitions
 #define BUZZ_PORT	PORTA
 #define BUZZ		6

 //	motor definitions
 #define MOT_PORT PORTA
 #define MOT_POS	5
 #define MOT_NEG	4
 //
 #define MOT_1H_PIN	5
 #define MOT_1H_DDR DDRA
 #define MOT_1H_PORT PORTA
 //
 #define MOT_2H_PIN	4
 #define MOT_2H_DDR DDRA
 #define MOT_2H_PORT PORTA
 //
 #define MOT_1L_PIN	1
 #define MOT_1L_DDR DDRE
 #define MOT_1L_PORT PORTE
 //
  #define MOT_2L_PIN	0
  #define MOT_2L_DDR DDRD
  #define MOT_2L_PORT PORTD
  //

//	keypad definitions
 #define KEY_X4			7
 #define KEY_X4_PORT	PORTD
 #define KEY_X4_DDR		DDRD
 #define KEY_X4_PIN		PIND
 #define KEY_X3			2
 #define KEY_X3_PORT	PORTE
 #define KEY_X3_DDR		DDRE
 #define KEY_X3_PIN		PINE
 #define KEY_X2			3
 #define KEY_X2_PORT	PORTE
 #define KEY_X2_DDR		DDRE
 #define KEY_X2_PIN		PINE
 #define KEY_X1			1
 #define KEY_X1_PORT	PORTA
 #define KEY_X1_DDR		DDRA
 #define KEY_X1_PIN		PINA
 #define KEY_Y1			6
 #define KEY_Y1_PORT	PORTD
 #define KEY_Y1_DDR		DDRD
 #define KEY_Y1_PIN		PIND
 #define KEY_Y2			5
 #define KEY_Y2_PORT	PORTD
 #define KEY_Y2_DDR		DDRD
 #define KEY_Y2_PIN		PIND
 #define KEY_Y3			4
 #define KEY_Y3_PORT	PORTD
 #define KEY_Y3_DDR		DDRD
 #define KEY_Y3_PIN		PIND
 #define KEYPAD_WAKE	3
 #define KEYPAD_WAKE_PORT	PORTD
 #define KEYPAD_WAKE_DDR	DDRD

 // SPI def
 #define SPI_MOSI_PORT	PORTB
 #define SPI_MOSI_DDR	DDRB
 #define SPI_MOSI		5
 #define SPI_MISO_PORT	PORTB
 #define SPI_MISO_DDR	DDRB
 #define SPI_MISO		6
 #define SPI_SCK_PORT	PORTB
 #define SPI_SCK_DDR	DDRB
 #define SPI_SCK		7
 #define SPI_SS_PORT	PORTB
 #define SPI_SS_DDR	DDRB
 #define SPI_SS			4

 // RTC pin def
 #define RTC_CE_PORT	PORTE
 #define RTC_CE_DDR		DDRE
 #define RTC_CE			0
 #define RTC_INT_PORT	PORTD
 #define RTC_INT_DDR	DDRD
 #define RTC_INT		2
 // EEPROM pin def
 #define EEPROM_CS1_PORT PORTE
 #define EEPROM_CS1_DDR DDRE
 #define EEPROM_CS1		1
 // ADC pin
 #define ADC_PORT	PORTA
 #define ADC_DDR	DDRA
 #define ADC_PIN	0
 //// BLE def
 #define BLE_EN 5
 #define BLE_EN_PORT PORTB
 #define BLE_EN_DDR DDRB
 #define BLE_CS 6
 #define BLE_CS_PORT PORTB
 #define BLE_CS_DDR DDRB
 #define BLE_AUTORUN 4
 #define BLE_AUTORUN_PORT PORTE
 #define BLE_AUTORUN_DDR DDRE
 #define BLE_SPARE 2
 #define BLE_SPARE 
 #define User_4						1
 #define User_6						2
 #define Technician					3
 #define Submaster					4
 #define Master						5
 #define Multiuser					6
 #define Multiuser_erase_one_code	7
 #define Multiuser_erase_all_codes	8
 #define Auto_timed_code_setup		9
 #define Auto_timed_unlock_setup	10
 #define Master_code_change			11
 #define User						12
 #define User_4_code_change			13
 #define User_6_code_change			14
 #define Superuser					15
 #define Nobody						16
 #define Bluetooth_Connection		17

#define Normal						0
#define	Upside_down					1	
#define Left						2
#define Right						3
uint8_t Lock_Position;

#define	Private_Mode				80
#define	Public_Mode					81
#define	Multiuser_Mode				82
#define	RAS_Mode					83
#define	RAS_Single_Use_Mode			84
#define RAS_RTC_Mode				85
#define RAS_RTC_SS_Mode				86
#define Admin_Mode					87
uint8_t Current_Lock_Mode;

#define No_Function									0
#define Change_Master_Code							1	
#define	Set_Change_Submaster_Code					3
#define	Delete_Submaster_Code						7//4 
#define Set_Change_Technician_Code					10//12
#define Delete_Technician_Code  					11//13 
#define	Change_User_Code							2// Works in private mode
#define Add_Change_User_Access_Code					61//5  Works in multiuser mode
#define Delete_User_Acceess_Code					62//6 Works in multiuser mode
#define Delete_All_User_Access_Codes				63//30 Works in multiuser mode
#define	Set_Single_User_Private_Mode_4				9//7 
#define	Set_Single_User_Private_Mode_6				6//8
#define	Set_Multiuser_Private_Mode					60//9 
#define	Set_Public_Mode_4  							8//10 
#define	Set_Public_Mode_6							5//11	
#define	Set_Audio_ON								12//14 
#define	Set_Audio_OFF								13//15	
#define	Set_Unsuccessful_Code_Lockout_ON			50//18 
#define	Set_Unsuccessful_Code_Lockout_OFF			51//19 
#define Enable_Battery_Check_During_Motor_Run		33
#define Disable_Battery_Check_During_Motor_Run		34
#define Allow_User_To_Change_Own_Code				52//28 
#define	Disallow_User_To_Change_Own_Code			53//29 
#define	Set_Double_User_Code_Entry_Mode				54//16  
#define Set_Single_User_Code_Entry_Mode				55//17 
#define	Set_Timer_Auto_Unlock_ON_And_Set_Time		16//20 
#define	Show_Timer_Auto_Unlock_Time					21
#define Set_Timer_Auto_Unlock_OFF					17//21	
#define Set_Lock_Active_And_Auto_Unlock				28//
#define Disable_Lock_Active_And_Auto_Unlock			29//
#define Set_Lock_Position							88//50
#define Set_RAS_Mode								20//60	
#define	Set_Single_Use_RAS_Mode						35	
#define	Reset_Lock_To_Default						66
#define Set_Time									40
#define Set_Date									41
#define Show_Time									42
#define Show_Date									43
#define Set_Lock_Active								45	
#define Diasble_Lock_Active							46
#define Show_Lock_Active_Start_Time					47
#define Show_Lock_Active_Stop_Time					48

//////////////////////////////////////////////////////////////////////////
#define RAS_RTC_Serial_Entry						36// set RAS Stop Mode
#define RAS_RTC_SS_Serial_Entry						37// set RAS Start Stop Mode
#define RAS_RTC_Date_Code_Entry						160
#define RAS_RTC_User_Code_Entry						161
#define RAS_RTC_Unlock_Code_Entry					162
//////////////////////////////////////////////////////////////////////////
#define RAS_RTC_SS_Date_Code_Entry					163
#define RAS_RTC_SS_User_Code_Entry					164
#define RAS_RTC_SS_Unlock_Code_Entry				165
//////////////////////////////////////////////////////////////////////////
#define Fuction_Executed_Successfuly				200
#define Lock_Open									201
#define Lock_Lock									202
#define Wrong_Code									203
#define Code_Changed_Successfully					204
#define Repeated_Code_Typed							205
#define New_Code_First_Typein						206
#define New_Code_Second_Typein_Disagreement			207
#define New_Code_Accepted							208
#define Timeout_Data_Cleared						209
#define Lock_Startup								210
#define User_Not_Allowed_To_Execute_This_Function	211
#define Too_Many_Wrong_Codes_Lock_Blocked			212
#define Ext_EEPROM_Erase_Sucessful					213
#define BLE_Address_Mismatch						214
#define Lock_Bounded_Buccessfuly					215
#define Communication_Allowed_Flag_Set				216
#define	Private_Unlock_Code_Entry					101
#define	Private_Change_Code_Entry					102
#define	Public_Unlock_Code_Entry					103
#define	Public_Lock_Code_Entry						104
#define	Multiuser_Unlock_Code_Entry					105
#define	Multiuser_Lock_Code_Entry					106
#define	RAS_Unlock_Code_Entry						107
#define	Auto_Timed_Unlock							108 //when the mastercode is entered
#define	Auto_Timed_Code_Cancel						109
#define	Admin_Area									110//when the logo button is pressed
#define	Battery_Check								111
#define	Transmit_Enable								112
#define	Admin_Code_Change							113
#define	RAS_Serial_Code_Entry						114

#define	Set_Time_Mode								115
#define	Set_Date_Mode								116
#define	Set_Lock_Active_Mode_Start					117
#define	Set_Lock_Active_Mode_Stop					118
#define Show_Lock_Active_Start_Time_Mode			119
#define Show_Lock_Active_Stop_Time_Mode				120
#define Lock_Deactivated							121
#define RAS_Single_Use_Serial_Code_Entry			122
#define	Set_Lock_Active_And_Unlock_Mode_Start		123
#define	Set_Lock_Active_And_Unlock_Mode_Stop		124
 uint8_t Current_Lock_State;
 
 #define	Audio_OFF	0
 #define	Audio_ON	1
uint8_t Audio;

#define Code_Accepted	1
#define Code_Rejected	2
#define Code_Processing	3
uint8_t Code_Status;

#define No_Pressed		0
#define Pressed			1
uint8_t Keypad_Pressed_Flag; 

#define Lockout_OFF		0
#define	Lockout_ON		1
// Motor open/lock 
#define Open			1
#define Lock			0
//// UART def
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
//#define UART_Buffer_Length 50
#define Ext_EEPROM_Bufer_Length 88 // this number / 8 gives number of entries

// Internal EEPROM Allocation Table
#define EEPROM_Fist_Time_Run_1_Flag				0x01	// first EEPROM position
#define EEPROM_Fist_Time_Run_2_Flag				0x02	// last EEPROM position
#define EEPROM_Log_Head_Pos_LSB					0x05
#define EEPROM_Log_Head_Pos_MSB					0x06
#define EEPROM_Log_Tail_Pos_LSB					0x17
#define EEPROM_Log_Tail_Pos_MSB					0x18
#define EEPROM_Lock_Position					0x07
#define EEPROM_Lock_Mode						0x08
#define EEPROM_Lock_State						0x09
#define EEPROM_Public_Private_Mode_Code_Lenght	0x0A
#define EEPROM_Lock_Audio						0x0B
#define EEPROM_Lock_Unsuccessful_Code_Lockout	0x0C
#define EEPROM_Wrong_Code_Attempts_Allowance	0x0D
#define EEPROM_Auto_Timed_Code_Cancel_Flag		0x0E
#define EEPROM_Auto_Timed_Code_Cancel_Time_0	0x0F
#define EEPROM_Auto_Timed_Code_Cancel_Time_1	0x10
#define EEPROM_Auto_Timed_Unlock_Flag			0x11
#define EEPROM_Auto_Timed_Unlock_Time_0			0x12
#define EEPROM_Auto_Timed_Unlock_Time_1			0x13
#define EEPROM_Allow_User_To_Change_Own_Code_Flag	0x14
#define EEPROM_Required_Number_Of_New_Code_Entires	0x15
#define EEPROM_Unsuccessful_Code_Lockout_Flag	0x16
#define EEPROM_Mastercode_0						0x1E
#define EEPROM_Submastercode_0					0x26
#define EEPROM_Techniciancode_0					0x2C
#define EEPROM_Usercode_4_0						0x32
#define EEPROM_Usercode_6_0						0x36
#define EEPROM_serialcode_0						0x3C
#define EEPROM_initcode_0						0x40
#define EEPROM_usercode_0						0x44
#define EEPROM_nextcode_0						0x48
#define EEPROM_Multiuser_0						0x4C	// start multiuser table 50 users * 5 bits
// Reserved for EEPROM_Multiuser including 0x113 Address
#define EEPROM_Set_Stop_Minute					0x114
#define EEPROM_Set_Stop_Hour					0x115
#define EEPROM_Set_Stop_Day						0x116
#define EEPROM_Set_Stop_Month					0x117
#define EEPROM_Set_Stop_Year					0x118

#define EEPROM_Lock_Active_Flag					0x119
#define EEPROM_Battery_Check_During_Motor_Run_Flag 0x11A
#define EEPROM_Lock_Active_Start_Time_Hr		0x11B
#define EEPROM_Lock_Active_Start_Time_Min		0x11C
#define EEPROM_Lock_Active_Stop_Time_Hr			0x11D
#define EEPROM_Lock_Active_Stop_Time_Min		0x11E
#define EEPROM_Lock_Deactivated_Prevoius_State	0x11F
#define EEPROM_Interrupts_Container				0x120
#define EEPROM_PCF_Control_Reg_Container_0		0x121
#define EEPROM_PCF_Control_Reg_Container_1		0x122
#define EEPROM_Usercode_RAS_RTC_4_0				0x123 // 123-126
#define EEPROM_Set_Start_Hour					0x127
#define EEPROM_Set_Start_Day					0x128
#define EEPROM_Set_Start_Month					0x129
#define EEPROM_Set_Start_Year					0x12A
#define EEPROM_Set_Lock_Block_Time				0x12B
#define DEBUG_MODE 0 // 1 when in DEBUG mode, change to 0 when in RUN mode

 // Function prototypes
void Set_up(void);
uint8_t SPI_Send(uint8_t data);
void New_Motor_Setup(void);
void UART0_Transmit (unsigned char data);
unsigned char UART0_Recieve (void);
uint8_t Return_keypad(void);
uint8_t Read_keypad(void);
void Buzzer(uint8_t Cycles_b);
void Buzzer_Beep(uint8_t count_bb);
uint8_t Check_Entered_Code(uint8_t Typed_Code);
uint8_t Process_Entered_Code(uint8_t Code_Check_Status);
void Clear(void);
void Key_Press_Flash_LED_Wrong_Code(uint8_t Wrong_Code_Attempts_Count);
uint16_t ADC_Convert(uint8_t channel);
void Motor_Open_Close_Lock (uint8_t direction, uint8_t Do_Battery_Check);
void Motor_CW (void);
void Motor_CW_Slow (void);
void Motor_CCW (void);
void Motor_CCW_Slow (void);
void Motor_OFF (void);
void Key_Press_Flash_LED(uint8_t Position_k, uint8_t Mode);
void Flash_Green_LED(uint8_t Count_f);
void Flash_Wrong(uint8_t Count);
void Public_Mode_Unlock(uint8_t Do_Battery_Check); // 1 do check, 0 not
void Private_Mode_Unlock(uint8_t Do_Battery_Check); // 1 do check, 0 not
void Public_Private_Mode_Wrong_Code_Process(void);
void Flash_Unsuccessful_Code_Attempts(uint8_t count_f);
void Logo_Press_Function(void);
void Put_To_Sleep(void);
void Watchdog_Enable(void);
uint8_t Compare_Code_Against_Multiuser(void);//Returns 0 when there IS A_SS MATCH, Returns 1 when there is NO MATCH
void Logo_Press_Function_Execute_01(void); // Change master code
void Logo_Press_Function_Execute_02(void); // Change user code
void Logo_Press_Function_Execute_03(void); // Set/Change submaster code
void Logo_Press_Function_Execute_04(void); // Delete submaster code
void Logo_Press_Function_Execute_05(void); // Add/Change user access code
void Logo_Press_Function_Execute_06(void); // Delete user access code
void Logo_Press_Function_Execute_12(void); // Set/change technician code
void Logo_Press_Function_Execute_13(void); // Delete technician code
void Logo_Press_Function_Execute_16(void); // 
void Logo_Press_Function_Execute_20(void); // Set timer auto unlock ON and set the time
void Logo_Press_Function_Execute_24(void); // Set auto timed code cancel and set the time
void Logo_Press_Function_Execute_50(void); // Set Lock Position
void Logo_Press_Function_Execute_60(void); // Set serial number in RAS mode and set RAS mode
void Logo_Press_Function_Execute_61(void);
void Logo_Press_Function_Execute_66(void); //Reset Lock to Default Settings
void Logo_Press_Function_Execute_90(void); // Set Time
void Logo_Press_Function_Execute_91(void); // Set Date
void Logo_Press_Function_Execute_95(void); // Set time of active
void Logo_Press_Function_Execute_96(void); // Set time of active and unlock
void PCF2123_Read_Register(uint8_t reg, uint8_t bytes);
void PCF2123_Write_Register(uint8_t reg, uint8_t byte_count);
void EEPROM_Read_Register(uint16_t EE_address, uint8_t bytes);
void EEPROM_Write_Byte(uint16_t Data);
uint8_t EEPROM_Read_Status_Register(void);
void EEPROM_Write(uint16_t Address, uint8_t Data);
void SPI_Init(void);
void USART_Putdata(char* StringPtr, uint8_t byte_count);
void UART_RX(void);
void USART_Put_String(char* StringPtr);
uint16_t Check_Battery(uint8_t Motor_Run_Flag); // 0 no motor run; 1 motor run
void Clear_Keycode(void);
void Write_Time_to_RTC(void);
uint8_t Calculate_Sum (uint8_t *ptr,uint8_t start, uint8_t stop) ;
void Flash_Time(uint8_t *ptr);
uint8_t Check_Lock_Active(void);
void Admin_Mode_Fnc(void);
void Public_Mode_Fnc(void);
void Private_Mode_Fnc(void);
void RAS_RTC_Mode_Fnc(void);
void Multiuser_Mode_Fnc(void);
// RAS RTC functions
void RAS_Mode_Fnc(void);
void RAS_Single_Use_Mode_Fnc(void);
void RAS_RTC_Serial_Number_Entry(void); // 
void RAS_RTC_User_Code_Enter(void);
void RAS_RTC_Check_Code(void);
void RAS_RTC_Date_Code_Enter(void); // Set RAS RTC mode
uint8_t RAS_RTC_Check_Lock_Active(void);
// RAS RTC SS functions
void RAS_RTC_SS_Mode_Fnc(void);
void RAS_RTC_SS_Serial_Number_Entry(void); //
void RAS_RTC_SS_Date_Code_Enter(void);
uint8_t RAS_RTC_SS_Check_Lock_Active(void);
void RAS_RTC_SS_User_Code_Enter(void);
void RAS_RTC_SS_Check_Code(void);
//
uint8_t BCD_to_Dec (uint8_t number_to_convert);
void Erase_All_Time_Related_Funcions(void);
void Initial_Test_Routine(void);
uint8_t Auto_Time_Date_Setup(void);
//uint8_t Check_For_Zero_Code(uint8_t Digits_Number);
#endif /* MAIN_H_ */
//////////////////////////////////////////////////////////////////////////