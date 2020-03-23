/*
 * Horizon RTC 1.0.c
 *
 * Created: 06/11/2018 13:18:15
 * Author : mariusz.dudek
 */ 

#include <avr/io.h>
#include "main.h"
int main(void)
{
	Auto_Time_Date_Setup();
	Set_up();
	while (1)
	{
		
			//Lock_Active_Flag = 0;
			Temp_Main = Return_keypad(); 
			if (Keypad_Pressed_Flag == Pressed)				// If any button was pressed
			{
				Keypad_Pressed_Flag = No_Pressed;
				Position ++;								// Position 0 indicates no button was pressed
				Timeout_Counter = 0;							// Reset Timeout Clear flag to zero
				Keycode[Position-1] = Temp_Main;
				Temp_Main = 11;
				if((Position == 1) && (Keycode[0] == 10)) // when LOGO button was pressed 
				{
							if (Current_Lock_State == Admin_Area)
							{
								Key_Press_Flash_LED(3,1);
								Current_Lock_State = Battery_Check;
								Check_Battery(0);
								Clear_Keycode();
							}
							else
							{
								if (Current_Lock_State == Battery_Check)
								{
									Current_Lock_Mode = Previous_Lock_Mode;
									Current_Lock_State = Previous_Lock_State;
									Clear_Keycode();
									Timeout_Counter = 1495;
								} 
								else
								{
									Previous_Lock_Mode = Current_Lock_Mode; //store current lock mode
									Previous_Lock_State = Current_Lock_State; //store current lock state
									Current_Lock_Mode = Admin_Mode;
									Current_Lock_State = Admin_Area;
									Clear_Keycode();
									Key_Press_Flash_LED(4,1);
								}
					} 
				
				}
				else
				{
					if ((Position > 0) && ((Keycode[1] == 10)||(Keycode[2] == 10)||(Keycode[3] == 10)||(Keycode[4] == 10)||(Keycode[5] == 10)||(Keycode[6] == 10)||(Keycode[7] == 10)||(Keycode[8] == 10)||(Keycode[9] == 10)||(Keycode[10] == 10)||(Keycode[11] == 10)||(Keycode[12] == 10)||(Keycode[13] == 10)))
					{
						Flash_Wrong(1);
						Clear_Keycode();
						

					} 
					else
					{
						switch (Current_Lock_Mode)
						{
							case Public_Mode:
							Public_Mode_Fnc();
							break;
							case Private_Mode:
							Private_Mode_Fnc();
							break;
							case Multiuser_Mode:
							Multiuser_Mode_Fnc();
							break;
							case RAS_Mode:
							RAS_Mode_Fnc();
							break;
							case RAS_Single_Use_Mode:
							RAS_Single_Use_Mode_Fnc();
							break;
							case Admin_Mode:
							Admin_Mode_Fnc();
							break;
							case RAS_RTC_Mode :
							RAS_RTC_Mode_Fnc();
							break;
							case RAS_RTC_SS_Mode :
							RAS_RTC_SS_Mode_Fnc();
							break;
							default:
							break;
						}
					}
				}
				switch(Current_Lock_State)
				{
					case RAS_RTC_SS_Date_Code_Entry:
					Max_Allowed_Position = 11;
					break;
					case Set_Date_Mode:
					Max_Allowed_Position = 14;
					break;
					case Set_Lock_Active_And_Unlock_Mode_Start:
					case Set_Lock_Active_And_Unlock_Mode_Stop:
					Max_Allowed_Position = 10;
					break;
					default:
					Max_Allowed_Position = 8;
					break;
				}
// 				if (Current_Lock_State == RAS_RTC_SS_Date_Code_Entry)
// 				{
// 					Max_Allowed_Position = 11;
// 				}
// 				else if (Current_Lock_State == Set_Date_Mode)
// 				{
// 					Max_Allowed_Position = 14;
// 				}
// 				else
// 				{
// 					Max_Allowed_Position = 8;
// 				}
				if(Position == Max_Allowed_Position) 
				{
					Clear_Keycode();
				}
			}//(Keypad_Pressed_Flag == Pressed) // If any button was pressed
			else
			{
				if (Timeout_Counter > Timeout_Constant)
				{
					Timeout_Counter = 0;
					Number_Of_New_Code_Entires = 0;
					// indicate timeout
					if (RTC_Int_Flag == 1) // The RTC Int have been triggered
					{
						Check_Lock_Active();
						RTC_Int_Flag = 0;
					}
					else // flash only when RTC Int is not triggered
					{
						CLEARBIT(LED_PORT,LED_G_1);
						_delay_ms(10);
						CLEARBIT(LED_PORT,LED_G_2);
						SETBIT(LED_PORT,LED_G_1);
						_delay_ms(10);
						CLEARBIT(LED_PORT,LED_G_3);
						SETBIT(LED_PORT,LED_G_2);
						_delay_ms(10);
						CLEARBIT(LED_PORT,LED_G_4);
						SETBIT(LED_PORT,LED_G_3);
						_delay_ms(10);
						SETBIT(LED_PORT,LED_G_4);
					}//else if (RTC_Int_Flag == 1) // The RTC Int have been triggered
					Keycode[0] = 11;
					Keycode[1] = 11;
					Keycode[2] = 11;
					Keycode[3] = 11;
					Keycode[4] = 11;
					Keycode[5] = 11;
					Keycode[6] = 11;
					Keycode[7] = 11;
					if (Position != 0)
					{
						Clear_Keycode();
					}
					if(Current_Lock_Mode == Admin_Mode) 
					{
						Current_Lock_Mode = Previous_Lock_Mode;
						Current_Lock_State = Previous_Lock_State;
					}
					if ((Current_Lock_Mode == RAS_RTC_Mode)&&(Current_Lock_State == RAS_RTC_User_Code_Entry))
					{
						Current_Lock_State = RAS_RTC_Date_Code_Entry;
						Timeout_Constant = 1500;
					}
					if ((Current_Lock_Mode == RAS_RTC_SS_Mode)&&(Current_Lock_State == RAS_RTC_SS_User_Code_Entry))
					{
						Current_Lock_State = RAS_RTC_SS_Date_Code_Entry;
						Timeout_Constant = 1500;
					}
				
					Who_Unlocked = Nobody;
					CLEARBIT(DDRD,3);			// As input
					CLEARBIT(KEY_X1_PORT,KEY_X1);
					CLEARBIT(KEY_X2_PORT,KEY_X2);
					CLEARBIT(KEY_X3_PORT,KEY_X3);
					CLEARBIT(KEY_X4_PORT,KEY_X4);
				// switch off LED if have been enabled
					SETBIT(LED_PORT,LED_R_4);
					SETBIT(LED_PORT,LED_R_3);
					SETBIT(LED_PORT,LED_R_2);
					SETBIT(LED_PORT,LED_R_1);
					SETBIT(LED_PORT,LED_G_4);
					SETBIT(LED_PORT,LED_G_3);
					SETBIT(LED_PORT,LED_G_2);
					SETBIT(LED_PORT,LED_G_1);
				//////////////////////////////////////
					sei();
					sleep_enable();
					sleep_bod_disable();
					sleep_cpu();
					sleep_disable();
					SETBIT(DDRD,3);			// As input
					SETBIT(KEY_X1_PORT,KEY_X1);
					SETBIT(KEY_X2_PORT,KEY_X2);
					SETBIT(KEY_X3_PORT,KEY_X3);
					SETBIT(KEY_X4_PORT,KEY_X4);
	///////////////////////////////////////////////////////////////////////////////////////
	if (RTC_Int_Flag == 1)
	{
		RTC_Int_Flag = 0;
		if (Current_Lock_Mode == Public_Mode) //&& ((Current_Lock_State == Public_Unlock_Code_Entry) || (Previous_Lock_Mode == Public_Unlock_Code_Entry)))
			 {
				uint8_t Unlock_Flag;
				Unlock_Flag = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag);
				if(Unlock_Flag == 1)
				{
					uint8_t time_0;
					time_0 = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0);
					uint8_t time_1;
					time_1 = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1);
					PCF2123_Read_Register(0x03,2);// read the actual time
					SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b01111111; // mask out 7th bit in minutes readout
					if((time_0 == SPI_Data_Buffer[0]) && (time_1 == SPI_Data_Buffer[1]))
					{
						PCF2123_Read_Register(0x01,1);// read Control_2 reg
						uint8_t status = SPI_Data_Buffer[0] & 0b00001000;
						if (status != 0)
						{
							SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b11110111; // clear the alarm flag
							PCF2123_Write_Register(0x01,1);
							SPI_Data_Buffer[0] = 0;
							PCF2123_Read_Register(0x01,1);	// for debugging
							SPI_Data_Buffer[0] = 0;			// for debugging
							for (uint8_t i = 0; i<50;i++)
							{
								Buzzer(5);
								_delay_ms(10);
							}
							Public_Mode_Unlock(0);
						}
					}
				}
			 }
	}
	///////////////////////////////////////////////////////////////////////////////////////
				} //if (Timeout_Counter > Timeout_Constant)
				else
				{
					Timeout_Counter ++;
				}
			}
		
	}//while (1)
	return 0;
}// main
// 
// uint8_t Check_For_Zero_Code(uint8_t Digits_Number)
// {
// 	Temp_1[0]=0, Temp_1[1]=0, Temp_1[2]=0, Temp_1[3]=0, Temp_1[4]=0, Temp_1[5]=0, Temp_1[6]=0, Temp_1[7]=0;
// 	if (memcmp(Keycode,Temp_1,Digits_Number) == 0) // if there is a match with 0000
// 	{
// 		return 1; //when there is a match
// 	} 
// 	else
// 	{
// 		return 0;
// 	}
// }

uint8_t Auto_Time_Date_Setup(void)
{
	New_Motor_Setup();
	SPI_Init();
	//volatile uint8_t SPI_Data[100];
	volatile uint8_t Counter_1 = 0, Counter_2 = 0, Error_Flag = 0, Non_Zero_Flag = 0;
	volatile uint16_t Checksum = 0;
	//volatile uint8_t tempData[7];
	cli();
	SPCR0 = ((1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1));	// SPI enable, master, clk/128
	DDRC = 0xff;
	PORTC = 0xff; //leds off
	SETBIT(DDRA,6); //buzz as output
	SETBIT(RTC_CE_DDR,RTC_CE);			// Set CE pin as output
	CLEARBIT(RTC_CE_PORT,RTC_CE);		// Pull RTC CE line low to disable its rx
	CLEARBIT(LED_PORT,LED_R_1);			//CE Bit for slave
	CLEARBIT(LED_PORT,LED_R_4);			//CE Bit for slave
	//SETBIT(LED_PORT,LED_G_2);
	//SETBIT(LED_PORT,LED_G_3);
	//_delay_ms(100);
	_delay_ms(10);
	do
	{
		SPI_Send(0x10); //Send initiate transfer
		_delay_ms(10);
		Counter_1++;
		SPI_Data_Buffer[0] = SPI_Send(255);			//send to receive data back
		if (SPI_Data_Buffer[0] == 0x11)
		{
			Counter_1 = 4;
		}
	} while (Counter_1 < 3);
	Counter_1 = 0;
	if (SPI_Data_Buffer[0] == 0x11)					//If the slave received it sends the 0x11 confirmation
	{
		_delay_ms(10);
		for (Counter_2 = 0;Counter_2 <= 100;Counter_2++) //send O to receive from slave
		{
			SPI_Data_Buffer[Counter_2-1] = SPI_Send(Counter_2);
			_delay_ms(10);
		}
		Checksum = 0;
		for (Counter_2 = 0;Counter_2 < 99 ;Counter_2++) //Sum up the received data but Checksum
		{
			Checksum = Checksum + (uint16_t)SPI_Data_Buffer[Counter_2];
		}
		
		if (Checksum == 0)
		{
			Non_Zero_Flag = 0;						//All received values has been zero
		}
		else
		{
			Non_Zero_Flag = 1;
		}
		Checksum = Checksum & 0xFF;					//Mask off MSByte and leave one byte only

		if (((uint8_t)Checksum == SPI_Data_Buffer[99]) && (Non_Zero_Flag == 1)) //Valid data has been received
		{
			Error_Flag = 0;
			SPI_Send(0x19);	//Send  transfer successful
			_delay_ms(1);
			SETBIT(LED_PORT,LED_R_1); // switch off the led
			SETBIT(LED_PORT,LED_R_4);
			_delay_ms(100);
			//for (uint16_t e=0;e<0x03FF;e++)
			//{
			//	eeprom_write_byte((uint8_t*)e,0xFF); // erase the eeprom
			//}
			//Time and date will be sent regardless the protocol version
			PCF2123_Write_Register(0x02,7); //Set up the RTC, SPI_Data_Buffer[0] to [6]incl //Receive format ssmmhhddwmmyy 7bytes total BCD encoded
			PCF2123_Read_Register(0x02,7);
			if (SPI_Data_Buffer[98] == 1) //protocol version check
			{
				//tempData[0] = SPI_Data_Buffer[0];
				//tempData[1] = SPI_Data_Buffer[1];
				//tempData[2] = SPI_Data_Buffer[2];
				//tempData[3] = SPI_Data_Buffer[3];
				//tempData[4] = SPI_Data_Buffer[4];
				//tempData[5] = SPI_Data_Buffer[5];
				//tempData[6] = SPI_Data_Buffer[6];
				Usercode_4[0] = SPI_Data_Buffer[7];
				Usercode_4[1] = SPI_Data_Buffer[8];
				Usercode_4[2] = SPI_Data_Buffer[9];
				Usercode_4[3] = SPI_Data_Buffer[10];
				eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
				//
				Usercode_6[0] = SPI_Data_Buffer[11];
				Usercode_6[0] = SPI_Data_Buffer[12];
				Usercode_6[0] = SPI_Data_Buffer[13];
				Usercode_6[0] = SPI_Data_Buffer[14];
				Usercode_6[0] = SPI_Data_Buffer[15];
				Usercode_6[0] = SPI_Data_Buffer[16];
				eeprom_write_block(&Usercode_6,(void*)EEPROM_Usercode_6_0,6);
				//
				Submastercode[0] = SPI_Data_Buffer[17];
				Submastercode[1] = SPI_Data_Buffer[18];
				Submastercode[2] = SPI_Data_Buffer[19];
				Submastercode[3] = SPI_Data_Buffer[20];
				Submastercode[4] = SPI_Data_Buffer[21];
				Submastercode[5] = SPI_Data_Buffer[22];
				eeprom_write_block(&Submastercode,(void*)EEPROM_Submastercode_0,6);
				//
				Techniciancode[0] = SPI_Data_Buffer[23];
				Techniciancode[1] = SPI_Data_Buffer[24];
				Techniciancode[2] = SPI_Data_Buffer[25];
				Techniciancode[3] = SPI_Data_Buffer[26];
				Techniciancode[4] = SPI_Data_Buffer[27];
				Techniciancode[5] = SPI_Data_Buffer[28];
				eeprom_write_block(&Techniciancode,(void*)EEPROM_Techniciancode_0,6);
				//
				Mastercode[0] = SPI_Data_Buffer[29];
				Mastercode[1] = SPI_Data_Buffer[30];
				Mastercode[2] = SPI_Data_Buffer[31];
				Mastercode[3] = SPI_Data_Buffer[32];
				Mastercode[4] = SPI_Data_Buffer[33];
				Mastercode[5] = SPI_Data_Buffer[34];
				Mastercode[6] = SPI_Data_Buffer[35];
				Mastercode[7] = SPI_Data_Buffer[36];
				eeprom_write_block(&Mastercode,(void*)EEPROM_Mastercode_0,8);
				//
				serialcode[0] = SPI_Data_Buffer[37];
				serialcode[1] = SPI_Data_Buffer[38];
				serialcode[2] = SPI_Data_Buffer[39];
				serialcode[3] = SPI_Data_Buffer[40];
				eeprom_write_block(&serialcode,(void*)EEPROM_serialcode_0,4);
				//
				initcode[0] = SPI_Data_Buffer[41];
				initcode[1] = SPI_Data_Buffer[42];
				initcode[2] = SPI_Data_Buffer[43];
				initcode[3] = SPI_Data_Buffer[44];
				eeprom_write_block(&initcode,(void*)EEPROM_initcode_0,4);
				//
				Lock_Active_Enabled_Flag = SPI_Data_Buffer[45];
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
				//
				Lock_Active_Start_Time_Hr = SPI_Data_Buffer[47];
				Lock_Active_Start_Time_Min = SPI_Data_Buffer[46];
				Lock_Active_Stop_Time_Hr = SPI_Data_Buffer[49];
				Lock_Active_Stop_Time_Min = SPI_Data_Buffer[48];
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
				//
				Auto_Timed_Unlock_Flag = SPI_Data_Buffer[50];
				eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag, Auto_Timed_Unlock_Flag);
				//
				eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0,SPI_Data_Buffer[52]);//mins
				eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1,SPI_Data_Buffer[51]);//hrs
				PCF2123_Write_Register(0x09,2); // write auto unlock as an alarm
				//
				if (Auto_Timed_Unlock_Flag == 1)
				{
					PCF2123_Read_Register(0x00,2);
					SPI_Data_Buffer[0] = 0b00000010; // set up an alarm interrupt and generate the pulse
					PCF2123_Write_Register(0x01,1);
					eeprom_write_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1, SPI_Data_Buffer[0]);
					EICRA = (1<<ISC11)|(1<<ISC01);			// The low level of INT1 generates an INT
					EIMSK = (1<<INT1);						// External interrupts enable
					//uint8_t Temp_set_1 = EIMSK;
					//eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,Temp_set_1);
					EIMSK = EIMSK | (1<<INT0); //enable INT0
					eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,EIMSK);
				}
				else
				{
					if (Auto_Timed_Unlock_Flag == 0)
					{
						PCF2123_Read_Register(0x01,1);
						SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b11111101; // Disable up an alarm interrupt and generate the pulse
						PCF2123_Write_Register(0x01,1);
						eeprom_write_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1, SPI_Data_Buffer[0]);
						//eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0); // disable auto unlock
						EICRA = (1<<ISC11)|(1<<ISC01);			// The low level of INT1 generates an INT
						EIMSK = (1<<INT1);						// External interrupts enable
						//uint8_t Temp_set_1 = EIMSK;
						EIMSK = EIMSK & ~(1<<INT0); //disable INT0
						eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,EIMSK);
					}
				}
				//
				Current_Lock_Mode = SPI_Data_Buffer[53];
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
				if (Current_Lock_Mode == Multiuser_Mode)
				{
					Multiuser_Code[0]=Usercode_4[0]; Multiuser_Code[1]=Usercode_4[1]; Multiuser_Code[2]=Usercode_4[2]; Multiuser_Code[3]=Usercode_4[3];
					memcpy(&MultiUsercode[0],Multiuser_Code,4);
					eeprom_write_block(&MultiUsercode[0],(void*)(EEPROM_Multiuser_0),4);
				}
				if ((Current_Lock_Mode == Public_Mode)||(Current_Lock_Mode == RAS_Mode)||(Current_Lock_Mode == RAS_Single_Use_Mode)||(Current_Lock_Mode == RAS_RTC_Mode)||(Current_Lock_Mode == RAS_RTC_SS_Mode))
				{
					Motor_Open_Close_Lock(Open,0);
				}
				//
				Current_Lock_State = SPI_Data_Buffer[54];
				eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
				//
				Lock_Position = SPI_Data_Buffer[55];
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Position,Lock_Position);
				//
				Required_Number_Of_New_Code_Entires = SPI_Data_Buffer[56];
				eeprom_write_byte((uint8_t*)EEPROM_Required_Number_Of_New_Code_Entires,Required_Number_Of_New_Code_Entires);
				//
				Wrong_Code_Attempts_Allowance = SPI_Data_Buffer[57];
				eeprom_write_byte((uint8_t*)EEPROM_Wrong_Code_Attempts_Allowance,Wrong_Code_Attempts_Allowance);
				//
				Lock_Block_Time = SPI_Data_Buffer[58]; // Approx in sec
				eeprom_write_byte((uint8_t*)EEPROM_Set_Lock_Block_Time,Lock_Block_Time);
				//
				Public_Private_Mode_Code_Lenght = SPI_Data_Buffer[59];
				eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
				//
				Audio  = SPI_Data_Buffer[60];
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Audio,Audio);
				//
				eeprom_write_byte((uint8_t*)EEPROM_Battery_Check_During_Motor_Run_Flag,SPI_Data_Buffer[61]); // enable battery check
				//
				eeprom_write_byte((uint8_t*)EEPROM_Fist_Time_Run_1_Flag,SPI_Data_Buffer[0] = 1); // indicate first time run completed
				eeprom_write_byte((uint8_t*)EEPROM_Fist_Time_Run_2_Flag,SPI_Data_Buffer[0] = 1); // indicate first time run completed
				_delay_ms(100);
				for (uint8_t i=0;i<100;i++)
				{
					SPI_Data_Buffer[i] = 0;
				}
				for (uint8_t i =0;i<10;i++)
				{
					Buzzer(8);
					CLEARBIT(LED_PORT,LED_G_2);
					CLEARBIT(LED_PORT,LED_G_3);
					_delay_ms(100);
					Buzzer(8);
					SETBIT(LED_PORT,LED_G_2);
					SETBIT(LED_PORT,LED_G_3);
					_delay_ms(100);
				}
			}
		}
		else
		{
			Error_Flag = 1;
			SPI_Send(0x20); //Wrong CRC
			for (uint8_t i =0;i<10;i++)
			{
				Buzzer(8);
				CLEARBIT(LED_PORT,LED_R_2);
				CLEARBIT(LED_PORT,LED_R_3);
				_delay_ms(100);
				Buzzer(8);
				SETBIT(LED_PORT,LED_R_2);
				SETBIT(LED_PORT,LED_R_3);
				_delay_ms(100);
			}
		}
	}
	else
	{
		Error_Flag = 1;
		SPI_Send(0x21); //No reply	
	}
	// All LED off
	SETBIT(LED_PORT,LED_G_1);
	SETBIT(LED_PORT,LED_G_2);
	SETBIT(LED_PORT,LED_G_3);
	SETBIT(LED_PORT,LED_G_4);
	SETBIT(LED_PORT,LED_R_1);
	SETBIT(LED_PORT,LED_R_2);
	SETBIT(LED_PORT,LED_R_3);
	SETBIT(LED_PORT,LED_R_4);
	_delay_ms(100);
	SPCR0 = ((1<<SPE)|(1<<MSTR)|(1<<SPR0));// SPI enable, master, clk/16
	PCF2123_Read_Register(0x02,7);
	sei();
	return Error_Flag;
}



void RAS_Mode_Fnc(void)
{
	if(Position == 1)
	{
		Lock_Active_Flag = Check_Lock_Active();
	}
	
	if (Lock_Active_Flag == 1)
	{
		Key_Press_Flash_LED(Position,1); // FLASH GREEN
	}
	else
	{
		Key_Press_Flash_LED(Position,2); // FLASH ORANGE
	}
	if ((Position == 4) && (Current_Lock_Mode == RAS_Mode) && (Current_Lock_State == RAS_Unlock_Code_Entry))
	{
		
		if (memcmp(Keycode,Usercode_4,4) == 0)			// the 2nd check is for code cant be 0000
		{	// open the lock, beep, flash LEDs
			if (Lock_Active_Flag == 1)
			{
				Who_Unlocked = User_4;
				Private_Mode_Unlock(1);
			}
			else
			{
				Clear_Keycode();
				Who_Unlocked = Nobody;
				Flash_Wrong(5);
			}
		}
		else
		{
			if (memcmp(Keycode,nextcode,4) == 0)
			{
				if (Lock_Active_Flag == 1)
				{
					memcpy(Usercode_4,nextcode,4);
					eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
					newcode();
					Who_Unlocked = User;
					Private_Mode_Unlock(1);
				}
				else
				{
					Clear_Keycode();
					Who_Unlocked = Nobody;
					Flash_Wrong(5);
				}
			}
			else
			{
				if ((memcmp(Keycode,Usercode_4,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0))//  if there is no match with any of the codes
				{//flag the alarm
					Who_Unlocked = Nobody;
					Clear_Keycode();
					Public_Private_Mode_Wrong_Code_Process();
				}
			}
		}
	} //if ((Position == 4) && (Current_Lock_Mode == RAS_Mode))
	if (Who_Unlocked == Nobody) // there was no match with any of the codes before
	{
		if(Position == 8)
		{
			if (memcmp(Keycode,Mastercode,8) == 0)
			{
				Who_Unlocked = Master;
				Private_Mode_Unlock(1);
			}
			else
			{
				Public_Private_Mode_Wrong_Code_Process();
				Who_Unlocked = Nobody;
				Clear_Keycode();
			}
		}
		else if(Position == 6)
		{
			if (memcmp(Keycode,Submastercode,6) == 0)
			{
				Who_Unlocked = Submaster;
				Private_Mode_Unlock(1);
			}
			else
			{
				if(memcmp(Keycode, Techniciancode,6) == 0)
				{
					Who_Unlocked = Technician;
					Private_Mode_Unlock(1);
				}
				else
				{
					if(memcmp(Keycode, Mastercode,6) != 0)
					{
						Public_Private_Mode_Wrong_Code_Process();
						Who_Unlocked = Nobody;
						Clear_Keycode();
					}
				}
			}
			
		} // else if(Position == 6)
	} //if (Who_Unlocked == Nobody) // there was no match with any of the codes before
}


void RAS_Single_Use_Mode_Fnc(void)
{
	if(Position == 1)
	{
		Lock_Active_Flag = Check_Lock_Active();
	}
	
	if (Lock_Active_Flag == 1)
	{
		Key_Press_Flash_LED(Position,1); // FLASH GREEN
	}
	else
	{
		Key_Press_Flash_LED(Position,2); // FLASH ORANGE
	}
	if ((Position == 4) && (Current_Lock_Mode == RAS_Single_Use_Mode) && (Current_Lock_State == RAS_Unlock_Code_Entry))
	{
		
		if ((memcmp(Keycode,Usercode_4,4) == 0) )
		{	// open the lock, beep, flash LEDs
			if (Lock_Active_Flag == 1)
			{
				Who_Unlocked = User_4;
				if (!((Keycode[0] == 1) && (Keycode[1] == 2) && (Keycode[2] == 3) && (Keycode[3] == 4)))
				{
					Temp_1[0] = 11, Temp_1[1] = 11, Temp_1[2] = 11, Temp_1[3] = 11;
					memcpy(Usercode_4,Temp_1,4); // wipe the used code off after two uses
				}
				Private_Mode_Unlock(1);
			}
			else
			{
				Clear_Keycode();
				Who_Unlocked = Nobody;
				Flash_Wrong(5);
			}
		}
		else
		{
			if ((memcmp(Keycode,nextcode,4) == 0) )
			{
				if (Lock_Active_Flag == 1)
				{
					memcpy(Usercode_4,nextcode,4);
					eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
					newcode();
					Who_Unlocked = User;
					Private_Mode_Unlock(1);
				}
				else
				{
					Clear_Keycode();
					Who_Unlocked = Nobody;
					Flash_Wrong(5);
				}
			}
			else
			{
				if ((memcmp(Keycode,Usercode_4,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0))//  if there is no match with any of the codes
				{//flag the alarm
					Who_Unlocked = Nobody;
					Clear_Keycode();
					Public_Private_Mode_Wrong_Code_Process();
				}
			}
		}
	} //if ((Position == 4) && (Current_Lock_Mode == RAS_Mode))
	if (Who_Unlocked == Nobody) // there was no match with any of the codes before
	{
		if(Position == 8)
		{
			if ((memcmp(Keycode,Mastercode,8) == 0))
			{
				Who_Unlocked = Master;
				Private_Mode_Unlock(1);
			}
			else
			{
				Who_Unlocked = Nobody;
				Clear_Keycode();
				Public_Private_Mode_Wrong_Code_Process();
			}
		}
		else if(Position == 6)
		{
			if (memcmp(Keycode,Submastercode,6) == 0)  
			{
				Who_Unlocked = Submaster;
				Private_Mode_Unlock(1);
			}
			else
			{
				if(memcmp(Keycode, Techniciancode,6) == 0)
				{
					Who_Unlocked = Technician;
					Private_Mode_Unlock(1);
				}
				else
				{
					if(memcmp(Keycode, Mastercode,6) != 0)
					{
						Who_Unlocked = Nobody;
						Clear_Keycode();
						Public_Private_Mode_Wrong_Code_Process();
					}
				}
			}
		} // else if(Position == 6)
	} //if (Who_Unlocked == Nobody) // there was no match with any of the codes before
}


void Multiuser_Mode_Fnc(void)
{
	uint8_t mmf_temp[8]={0,0,0,0,0,0,0,0};
	if(Position == 1)
	{
		Lock_Active_Flag = Check_Lock_Active();
	}
	
	if (Lock_Active_Flag == 1)
	{
		Key_Press_Flash_LED(Position,1); // FLASH GREEN
	}
	else
	{
		Key_Press_Flash_LED(Position,2); // FLASH ORANGE
	}
	
	if ((Position == 6) && (Current_Lock_Mode == Multiuser_Mode))
	{
		if(Current_Lock_State == Multiuser_Unlock_Code_Entry) //When the code is typed in to open
		{
			number_1 = Keycode[0];
			number_2 = Keycode[1];
			Multiuser_Location = (10*number_1) + number_2; // combine two numbers to point to the position of the multiuser table
			for (uint8_t m=0;m<4;m++)
			{
				Multiuser_Code[m]=Keycode[m+2]; // shift remaining keycode[2-6] to Multiuser_Code[0-4]
			}
			if (((memcmp((int*)MultiUsercode[Multiuser_Location],Multiuser_Code,4) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0)))
			{
				if (Lock_Active_Flag == 1)
				{
					Clear_Keycode();
					Who_Unlocked = Multiuser_Location;
					Private_Mode_Unlock(1);
					Who_Unlocked = Nobody;
				}
				else
				{
					Flash_Wrong(5);
					Clear_Keycode();
					Who_Unlocked = Nobody;
				}
				
			}//if (memcmp((int*) MultiUsercode[Multiuser_Location],Multiuser_Code,4)!=0)
			else
			{
				if(((memcmp(Keycode,Mastercode,6) != 0) && (memcmp((int*)MultiUsercode[Multiuser_Location],Multiuser_Code,4) != 0)) || (memcmp(Keycode,mmf_temp,6) == 0))
				{
					if (Lock_Active_Flag == 1)
					{
						Clear_Keycode();
						Who_Unlocked = Nobody;
						Public_Private_Mode_Wrong_Code_Process();
					}
					else
					{
						Clear_Keycode();
						Who_Unlocked = Nobody;
						Flash_Wrong(5);
					}
				}
			}
		}
	} //if((Position == 6) && (Current_Lock_Mode == Multiuser_Mode))
	else if ((Position == 8) && (Current_Lock_Mode == Multiuser_Mode))
	{
		if ((memcmp(Keycode,Mastercode,8) == 0) && (memcmp(Keycode,mmf_temp,8) != 0))
		{
			Who_Unlocked = Master;
			Private_Mode_Unlock(1);
		}
		else
		{
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Public_Private_Mode_Wrong_Code_Process();
		}
	}//else if ((Position == 8) && (Current_Lock_Mode == Multiuser_Mode))
}
void Private_Mode_Fnc(void)
{
	uint8_t pmf_temp[8]= {0,0,0,0,0,0,0,0};
	if(Position == 1)
	{
		Lock_Active_Flag = Check_Lock_Active();
	}
	
	if (Lock_Active_Flag == 1)
	{
		Key_Press_Flash_LED(Position,1); // FLASH GREEN
	}
	else
	{
		Key_Press_Flash_LED(Position,2); // FLASH ORANGE
	}
	if ((Position == Public_Private_Mode_Code_Lenght) && (Current_Lock_State == Private_Unlock_Code_Entry))
	{
 			Lock_Active_Flag = Check_Lock_Active();
			if (Public_Private_Mode_Code_Lenght == 4)
			{
					if ((memcmp(Keycode,Usercode_4,4) == 0) && (memcmp(Keycode,pmf_temp,4) != 0))
					{	// open the lock, beep, flash LEDs
							if (Lock_Active_Flag == 1)
							{
									Who_Unlocked = User_4;
									Private_Mode_Unlock(1);
							}
							else
							{
									Clear_Keycode();
									Who_Unlocked = Nobody;
									Flash_Wrong(5);
							}
					}
					else
					{
							if (((memcmp(Keycode,Usercode_4,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0)) || (memcmp(Keycode,pmf_temp,4) == 0))//  if there is no match with any of the codes
							{//flag the alarm
									if (Lock_Active_Flag == 1)
									{
										Clear_Keycode();
										Who_Unlocked = Nobody;
										Public_Private_Mode_Wrong_Code_Process();
									}
									else
									{
										Clear_Keycode();
										Who_Unlocked = Nobody;
										Flash_Wrong(5);
									}
							}
					}
			}
			if (Public_Private_Mode_Code_Lenght == 6)
			{
					
							if ((memcmp(Keycode,Submastercode,6) == 0)  && (memcmp(Keycode,pmf_temp,6) != 0))
							{
									Who_Unlocked = Submaster;
									Private_Mode_Unlock(1);
							}
							else
							{
									if ((memcmp(Keycode,Usercode_6,6) == 0) && (memcmp(Keycode,pmf_temp,6) != 0))
									{
											if (Lock_Active_Flag == 1)
											{
													Who_Unlocked = User_4;
													Private_Mode_Unlock(1);
											}
											else
											{
												Flash_Wrong(5);
												Clear_Keycode();
												Who_Unlocked = Nobody;
											}
											
									}
									else
									{
											if (((memcmp(Keycode,Usercode_6,6) != 0) && (memcmp(Keycode,Mastercode,6) != 0) && (memcmp(Keycode,Submastercode,6) != 0) && (memcmp(Keycode,Techniciancode,6) != 0)) || (memcmp(Keycode,pmf_temp,6) == 0))//  if there is no match with any of the codes
											{//flag the alarm
													Who_Unlocked = Nobody;
													if (Lock_Active_Flag == 1)
													{
														Clear_Keycode();
														Who_Unlocked = Nobody;
														Public_Private_Mode_Wrong_Code_Process();
													}
													else
													{
														Clear_Keycode();
														Who_Unlocked = Nobody;
														Flash_Wrong(5);
													}
											}
									}
							
					}// if (Public_Private_Mode_Code_Lenght == 6)
			}
	}// if ((Position == Public_Private_Mode_Code_Lenght) && (Current_Lock_State == Private_Unlock_Code_Entry))
	if (Who_Unlocked == Nobody) // there was no match with any of the codes before
	{
			if(Position == 8)
			{
					if( (memcmp(Keycode,Mastercode,8) == 0) && (memcmp(Keycode,pmf_temp,8) != 0))
					{
						Who_Unlocked = Master;
						Private_Mode_Unlock(1);
					}
					else
					{
						Clear_Keycode();
						Who_Unlocked = Nobody;
						Public_Private_Mode_Wrong_Code_Process();
					}
					
			}
			else if(Position == 6)
			{
					if ((memcmp(Keycode,Submastercode,6) == 0)  && (memcmp(Keycode,pmf_temp,6) != 0))
					{
						Who_Unlocked = Submaster;
						Private_Mode_Unlock(1);
					}
					else
					{
							if(memcmp(Keycode, Mastercode,6) != 0)
							{
								Clear_Keycode();
								Who_Unlocked = Nobody;
								Public_Private_Mode_Wrong_Code_Process();
							}
					}
			} // else if(Position == 6)
	}
}

void Public_Mode_Fnc(void)
{	
	uint8_t pmf_temp[8] = {0,0,0,0,0,0,0,0};
	if(Position == 1)
	{
		Lock_Active_Flag = Check_Lock_Active();
	}
	
	if (Lock_Active_Flag == 1)
	{
		Key_Press_Flash_LED(Position,1); // FLASH GREEN
	} 
	else
	{
		Key_Press_Flash_LED(Position,2); // FLASH ORANGE
	}
	if (Position == Public_Private_Mode_Code_Lenght) 
	{
		switch (Current_Lock_State)
		{
			case Public_Unlock_Code_Entry:
				if (Public_Private_Mode_Code_Lenght == 4) 
				{
						//if ((memcmp(Keycode,Usercode_4,4) == 0) && ((Keycode[0]!=0)&&(Keycode[1]!=0)&&(Keycode[2]!=0)&&(Keycode[3]!=0)))
						if ((memcmp(Keycode,Usercode_4,4) == 0) && (memcmp(Keycode,pmf_temp,4) != 0))
						{	// open the lock, beep, flash LEDs
								if (Lock_Active_Flag == 1)
								{
									Who_Unlocked = User_4;
									Public_Mode_Unlock(1);
								}
								else
								{
									Clear_Keycode();
									Who_Unlocked = Nobody;
									Flash_Wrong(3);
								}
						}
						else
						{
								if (((memcmp(Keycode,Usercode_4,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0)) || ((Keycode[0]==0)&&(Keycode[1]==0)&&(Keycode[2]==0)&&(Keycode[3]==0)))//  if there is no match with any of the codes
								{//flag the alarm
										Who_Unlocked = Nobody;

										Clear_Keycode();
										if (Lock_Active_Flag == 1)
										{
												Public_Private_Mode_Wrong_Code_Process();
										}
										else
										{
												Clear_Keycode();
												Who_Unlocked = Nobody;
												Flash_Wrong(3);
										}
								}
						}
				}
				if (Public_Private_Mode_Code_Lenght == 6)
				{
						if ((memcmp(Keycode,Submastercode,6) == 0)  && (memcmp(Keycode,pmf_temp,6) != 0))
						{
							Who_Unlocked = Submaster;
							Private_Mode_Unlock(1);
						} 
						else
						{
							if  ((memcmp(Keycode,Techniciancode,6) == 0)  && (memcmp(Keycode,pmf_temp,6) != 0))
							{
								Who_Unlocked = Technician;
								Private_Mode_Unlock(1);
							} 
							else
							{
									if  ((memcmp(Keycode,Usercode_6,6) == 0)  && (memcmp(Keycode,pmf_temp,6) != 0))
									{
											if (Lock_Active_Flag == 1)
											{
												Who_Unlocked = User_4;
												Public_Mode_Unlock(1);
											}
											else
											{
													Clear_Keycode();
													Who_Unlocked = Nobody;
													Flash_Wrong(3);
											}
									}
									else
									{
											if (((memcmp(Keycode,Usercode_6,6) != 0) && (memcmp(Keycode,Mastercode,6) != 0) && (memcmp(Keycode,Submastercode,6) != 0) && (memcmp(Keycode,Techniciancode,6) != 0)) || (memcmp(Keycode,pmf_temp,6) == 0))//  if there is no match with any of the codes
											{//flag the alarm
												Who_Unlocked = Nobody;
													if (Lock_Active_Flag == 1)
													{
														Clear_Keycode();
														Who_Unlocked = Nobody;
														Public_Private_Mode_Wrong_Code_Process();
													}
													else
													{
														Clear_Keycode();
														Who_Unlocked = Nobody;
														Flash_Wrong(3);
													}
											}
									}
							}
					}
				}// if (Public_Private_Mode_Code_Lenght == 6)
// 					if (Who_Unlocked == Nobody) // there was no match with any of the codes before
// 					{
// 						if(Position == 8) 
// 							{
// 									if ((memcmp(Keycode,Mastercode,8) == 0))
// 									{
// 											Who_Unlocked = Master;
// 											Public_Mode_Unlock(1);
// 											Keycode[0] = 0;
// 											Keycode[1] = 0;
// 											Keycode[2] = 0;
// 											Keycode[3] = 0;
// 											memcpy(Usercode_4,Keycode,4);
// 											eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
// 									}
// 									else
// 									{
// 											Clear_Keycode();
// 											Who_Unlocked = Nobody;
// 											Public_Private_Mode_Wrong_Code_Process();
// 									}
// 							}
// 							else if(Position == 6)
// 							{
// 									if (memcmp(Keycode,Submastercode,6) == 0)
// 									{
// 											Who_Unlocked = Submaster;
// 											Public_Mode_Unlock(1);
// 									}
// 									else
// 									{
// 											if(memcmp(Keycode, Techniciancode,6) == 0)
// 											{
// 													Who_Unlocked = Technician;
// 													Private_Mode_Unlock(1);
// 											}
// 											else
// 											{
// 													if(memcmp(Keycode, Mastercode,6) != 0)
// 													{
// 														Clear_Keycode();
// 														Who_Unlocked = Nobody;
// 														Public_Private_Mode_Wrong_Code_Process();
// 													}
// 											}
// 							
// 									}
// 							} // else if(Position == 6)
// 					}
					break;
				case Public_Lock_Code_Entry:
					if (Lock_Active_Flag == 1)
					{
						if (Public_Private_Mode_Code_Lenght == 4)
						{
							if(  (memcmp(Keycode,Mastercode,4) == 0) || (memcmp(Keycode,Submastercode,4) == 0) || (memcmp(Keycode,Techniciancode,4) == 0) || (memcmp(Keycode,pmf_temp,6) == 0))
							{ // if there is a match between new code and the old one or a sub-user code
								Clear_Keycode();
								Flash_Wrong(3);
								memcpy(Keycode,Temp_Array,4); // Wipe it clean
							}
							else // There was no match with any of th codes
							{
								if((memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0))
								{
									if(Required_Number_Of_New_Code_Entires == 1)
									{
										memcpy(Usercode_4,Keycode,4);
										eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
										Flash_Green_LED(1);
										Clear_Keycode();
										Motor_Open_Close_Lock(Lock,1);
										Current_Lock_State = Public_Unlock_Code_Entry;
										eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
										memcpy(Public_Mode_Code_Entry_1,Temp_Array,4); // Wipe it clean
										memcpy(Public_Mode_Code_Entry_2,Temp_Array,4); // Wipe it clean
										memcpy(Keycode,Temp_Array,4); // Wipe it clean
									}
									if(Required_Number_Of_New_Code_Entires == 2)
									{
										switch (Number_Of_New_Code_Entires)
										{
											case 0:
												memcpy(Public_Mode_Code_Entry_1,Keycode,4);
												Number_Of_New_Code_Entires = 1;
												Clear_Keycode();
												Flash_Green_LED(2);
												break;
											case 1:
												memcpy(Public_Mode_Code_Entry_2,Keycode,4);
												Number_Of_New_Code_Entires = 0;
												if (memcmp(Public_Mode_Code_Entry_1,Public_Mode_Code_Entry_2,4) == 0) // If both codes are the same
												{
													memcpy(Usercode_4,Public_Mode_Code_Entry_2,4);
													eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
													Flash_Green_LED(3);
													Clear_Keycode();
													memcpy(Public_Mode_Code_Entry_1,Temp_Array,4); // Wipe it clean
													memcpy(Public_Mode_Code_Entry_2,Temp_Array,4); // Wipe it clean
													memcpy(Keycode,Temp_Array,4); // Wipe it clean
													Motor_Open_Close_Lock(Lock,1);
													Current_Lock_State = Public_Unlock_Code_Entry;
													eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
												}
												else
												{
													Flash_Wrong(0);
													Clear_Keycode();
													memcpy(Public_Mode_Code_Entry_1,Temp_Array,4); // Wipe it clean
													memcpy(Public_Mode_Code_Entry_2,Temp_Array,4); // Wipe it clean
													memcpy(Keycode,Temp_Array,4); // Wipe it clean
												}
												break;
											default:
												break;
										}//switch (Number_Of_New_Code_Entires)
									}//if(Required_Number_Of_New_Code_Entires == 2)
								}//if((memcmp(Keycode,Usercode_4,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0))
							}//else if((memcmp(Keycode,Usercode_4,4) == 0) || (memcmp(Keycode,Mastercode,4) == 0) || (memcmp(Keycode,Submastercode,4) == 0) || (memcmp(Keycode,Techniciancode,4) == 0))
						}// if (Public_Private_Mode_Code_Lenght == 4)
						if (Public_Private_Mode_Code_Lenght == 6)
						{
							if((memcmp(Keycode,Mastercode,6) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0)  || (memcmp(Keycode,pmf_temp,6) == 0))
							{ // if there is a match between new code and the old one or a sub-user code
								Flash_Wrong(5);
								//Position = 0;
								Clear_Keycode();
								memcpy(Keycode,Temp_Array,6); // Wipe it clean
							}
							else // There was no match with any of th codes
							{
								if((memcmp(Keycode,Mastercode,6) != 0) && (memcmp(Keycode,Submastercode,6) != 0) && (memcmp(Keycode,Techniciancode,6) != 0))
								{
									if(Required_Number_Of_New_Code_Entires == 1)
									{
										memcpy(Usercode_6,Keycode,6);
										eeprom_write_block(&Usercode_6,(void*)EEPROM_Usercode_6_0,6);
										Flash_Green_LED(1);
										Motor_Open_Close_Lock(Lock,1);
										Current_Lock_State = Public_Unlock_Code_Entry;
										eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
										Flash_Green_LED(1);
										Clear_Keycode();
										memcpy(Public_Mode_Code_Entry_1,Temp_Array,6); // Wipe it clean
										memcpy(Public_Mode_Code_Entry_2,Temp_Array,6); // Wipe it clean
										memcpy(Keycode,Temp_Array,6); // Wipe it clean
									}
									if(Required_Number_Of_New_Code_Entires == 2)
									{
										switch (Number_Of_New_Code_Entires)
										{
											case 0:
												memcpy(Public_Mode_Code_Entry_1,Keycode,6);
												Number_Of_New_Code_Entires = 1;
												//Position = 0;
												Clear_Keycode();
												Flash_Green_LED(1);
												break;
											case 1:
												memcpy(Public_Mode_Code_Entry_2,Keycode,6);
												Number_Of_New_Code_Entires = 0;
												if (memcmp(Public_Mode_Code_Entry_1,Public_Mode_Code_Entry_2,6) == 0) // If both codes are the same
												{
													memcpy(Usercode_6,Public_Mode_Code_Entry_2,6);
													eeprom_write_block(&Usercode_6,(void*)EEPROM_Usercode_6_0,6);
													Flash_Green_LED(2);
													Clear_Keycode();
													memcpy(Public_Mode_Code_Entry_1,Temp_Array,6); // Wipe it clean
													memcpy(Public_Mode_Code_Entry_2,Temp_Array,6); // Wipe it clean
													memcpy(Keycode,Temp_Array,6); // Wipe it clean
													Motor_Open_Close_Lock(Lock,1);
													Current_Lock_State = Public_Unlock_Code_Entry;
													eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
												}
												else
												{
													Flash_Wrong(0);
													memcpy(Public_Mode_Code_Entry_1,Temp_Array,6); // Wipe it clean
													memcpy(Public_Mode_Code_Entry_2,Temp_Array,6); // Wipe it clean
													memcpy(Keycode,Temp_Array,6); // Wipe it clean
												}
												break;
											default:
												break;
										}//switch (Number_Of_New_Code_Entires)
									}//if(Required_Number_Of_New_Code_Entires == 2)
								}//if((memcmp(Keycode,Usercode_6,6) != 0) && (memcmp(Keycode,Mastercode,6) != 0) && (memcmp(Keycode,Submastercode,6) != 0) && (memcmp(Keycode,Techniciancode,6) != 0))
							}//else if((memcmp(Keycode,Usercode_6,6) == 0) || (memcmp(Keycode,Mastercode,6) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0))
						}// if (Public_Private_Mode_Code_Lenght == 6)
					} 
					else
					{
						Flash_Wrong(5);
						Clear_Keycode();
					}
					break;
				default:
					break;
			}// switch (Current_Lock_State)
		} //if (Position == Public_Private_Mode_Code_Lenght)
	if ((Who_Unlocked == Nobody) && (Current_Lock_State == Public_Unlock_Code_Entry))// there was no match with any of the codes before
	{
		if(Position == 8) 
			{
					//if (memcmp(Keycode,Mastercode,8) == 0)
					if (memcmp(Keycode,Mastercode,8) == 0)
					{
							Who_Unlocked = Master;
							Public_Mode_Unlock(1);
					}
					else
					{
							Who_Unlocked = Master;
							Clear_Keycode();
							Public_Private_Mode_Wrong_Code_Process();
					}
			}
			else if(Position == 6)
			{
					//if ((memcmp(Keycode,Submastercode,6) == 0) && ((Keycode[0]!=0) && (Keycode[1]!=0) && (Keycode[2]!=0) && (Keycode[3]!=0) && (Keycode[4]!=0) && (Keycode[5]!=0)))
					 if(memcmp(Keycode,Submastercode,6) == 0) 
					{
							Who_Unlocked = Submaster;
							Private_Mode_Unlock(1);
					}
					else
					{
							//if((memcmp(Keycode, Techniciancode,6) == 0) && ((Keycode[0]!=0) && (Keycode[1]!=0) && (Keycode[2]!=0) && (Keycode[3]!=0) && (Keycode[4]!=0) && (Keycode[5]!=0)))
							 if(memcmp(Keycode, Techniciancode,6) == 0) 
							{
									Who_Unlocked = Technician;
									Private_Mode_Unlock(1);
							}
							else
							{
									if(memcmp(Keycode, Mastercode,6) != 0)
									{
										Who_Unlocked = Nobody;
										Clear_Keycode();
										Public_Private_Mode_Wrong_Code_Process();
									}
							}
					}
			} // else if(Position == 6)
		}
}
void Admin_Mode_Fnc(void)
{
	
	switch (Current_Lock_State)
	{
	case Admin_Area:
			if (Who_Unlocked == Nobody)
			{
				Key_Press_Flash_LED(Position,1);
				switch (Position)
				{
					case 4:
						if((memcmp(Keycode,Usercode_4,4) == 0) && (Public_Private_Mode_Code_Lenght == 4) && (Previous_Lock_Mode != Public_Mode))
						{
							Who_Unlocked = User;
							Position = 0;
							Flash_Green_LED(2);
						}
						if(Previous_Lock_Mode == Public_Mode)
						{
							if ((memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0))//  if there is no match with any of the codes
							{
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								SETBIT(LED_PORT,LED_G_4);
								SETBIT(LED_PORT,LED_G_3);
								SETBIT(LED_PORT,LED_G_2);
								SETBIT(LED_PORT,LED_G_1);
								SETBIT(LED_PORT,LED_R_4);
								SETBIT(LED_PORT,LED_R_3);
								SETBIT(LED_PORT,LED_R_2);
								SETBIT(LED_PORT,LED_R_1);
								//Flash_Wrong(2);
								Public_Private_Mode_Wrong_Code_Process();
							}
						}
						else
						{
							if ((memcmp(Keycode,Usercode_4,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0) && (memcmp(Keycode,Techniciancode,4) != 0))//  if there is no match with any of the codes
							{
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								SETBIT(LED_PORT,LED_G_4);
								SETBIT(LED_PORT,LED_G_3);
								SETBIT(LED_PORT,LED_G_2);
								SETBIT(LED_PORT,LED_G_1);
								SETBIT(LED_PORT,LED_R_4);
								SETBIT(LED_PORT,LED_R_3);
								SETBIT(LED_PORT,LED_R_2);
								SETBIT(LED_PORT,LED_R_1);
								//Flash_Wrong(2);
								Public_Private_Mode_Wrong_Code_Process();
							}
						}
						
						break;
					case 6:
						if((memcmp(Keycode,Usercode_6,6) == 0) && (Public_Private_Mode_Code_Lenght == 6) && (Previous_Lock_Mode != Public_Mode))
						{
							Who_Unlocked = User;
							Position = 0;
							Flash_Green_LED(2);
						}
						if((memcmp(Keycode,Techniciancode,6) == 0) && (Previous_Lock_Mode == Public_Mode))
						{
							Who_Unlocked = Technician;
							Clear_Keycode();
							Flash_Green_LED(2);
						}
						if((memcmp(Keycode,Submastercode,6) == 0))
						{
							Who_Unlocked = Submaster;
							Position = 0;
							Flash_Green_LED(2);
						}
						if(Previous_Lock_Mode == Public_Mode)
						{
							if ((memcmp(Keycode,Usercode_6,6) != 0) && (memcmp(Keycode,Mastercode,6) != 0) && (memcmp(Keycode,Submastercode,6) != 0) && ((memcmp(Keycode,Techniciancode,6) != 0) && (Current_Lock_Mode == Public_Mode)))//  if there is no match with any of the codes
							{//flag the alarm
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								SETBIT(LED_PORT,LED_G_4);
								SETBIT(LED_PORT,LED_G_3);
								SETBIT(LED_PORT,LED_G_2);
								SETBIT(LED_PORT,LED_G_1);
								SETBIT(LED_PORT,LED_R_4);
								SETBIT(LED_PORT,LED_R_3);
								SETBIT(LED_PORT,LED_R_2);
								SETBIT(LED_PORT,LED_R_1);
								Public_Private_Mode_Wrong_Code_Process();
							}
						}
						else
						{
							if ((memcmp(Keycode,Mastercode,6) != 0) && (memcmp(Keycode,Submastercode,6) != 0) && ((memcmp(Keycode,Techniciancode,6) != 0) && (Current_Lock_Mode == Public_Mode)))//  if there is no match with any of the codes
							{//flag the alarm
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								SETBIT(LED_PORT,LED_G_4);
								SETBIT(LED_PORT,LED_G_3);
								SETBIT(LED_PORT,LED_G_2);
								SETBIT(LED_PORT,LED_G_1);
								SETBIT(LED_PORT,LED_R_4);
								SETBIT(LED_PORT,LED_R_3);
								SETBIT(LED_PORT,LED_R_2);
								SETBIT(LED_PORT,LED_R_1);
								Public_Private_Mode_Wrong_Code_Process();
							}
						}
						break;
					case 8:
						if ((memcmp(Keycode,Mastercode,8) == 0))
						{
							Who_Unlocked = Master;
							Position = 0;
							Flash_Green_LED(2);
						}
						else
						{
							Who_Unlocked = Nobody;
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							SETBIT(LED_PORT,LED_G_4);
							SETBIT(LED_PORT,LED_G_3);
							SETBIT(LED_PORT,LED_G_2);
							SETBIT(LED_PORT,LED_G_1);
							SETBIT(LED_PORT,LED_R_4);
							SETBIT(LED_PORT,LED_R_3);
							SETBIT(LED_PORT,LED_R_2);
							SETBIT(LED_PORT,LED_R_1);
							Clear_Keycode();
							Public_Private_Mode_Wrong_Code_Process();
						
						}
						break;
					default:
						break;
				}//switch (Position)
			} //if (Who_Unlocked == Nobody)
			else //There was a match with one of the codes
			{
				Logo_Press_Function();
			}
		//}//else if((Position == 1) && (Keycode[0] == 10))
		break;
	case Change_Master_Code:
		Logo_Press_Function_Execute_01();
		break;
	case Change_User_Code:
		Logo_Press_Function_Execute_02();
		break;
	case Set_Change_Submaster_Code:
		Logo_Press_Function_Execute_03();
		break;
	case Add_Change_User_Access_Code:
		Logo_Press_Function_Execute_05();
		break;
	case Delete_User_Acceess_Code:
		Logo_Press_Function_Execute_06();
		break;
	case Set_Change_Technician_Code:
		Logo_Press_Function_Execute_12();
		break;
	case Set_Timer_Auto_Unlock_ON_And_Set_Time:
		Logo_Press_Function_Execute_16();
		break;
	case Set_Lock_Position:
		Logo_Press_Function_Execute_50();
		break;
	case RAS_Serial_Code_Entry:
		Logo_Press_Function_Execute_60();
		break;
	case RAS_Single_Use_Serial_Code_Entry:
		Logo_Press_Function_Execute_61();
		break;
	case Reset_Lock_To_Default:
		Logo_Press_Function_Execute_66();
		break;
	case Set_Time_Mode:
		Logo_Press_Function_Execute_90();
		break;
	case Set_Date_Mode:
		Logo_Press_Function_Execute_91();
		break;
	case Set_Lock_Active_And_Unlock_Mode_Start:
	case Set_Lock_Active_And_Unlock_Mode_Stop:
		Logo_Press_Function_Execute_96();
		break;
	case Set_Lock_Active_Mode_Start:
	case Set_Lock_Active_Mode_Stop:
		Logo_Press_Function_Execute_95();
		break;
	case RAS_RTC_Serial_Entry:
		RAS_RTC_Serial_Number_Entry();
		break;
	case RAS_RTC_SS_Serial_Entry:
		RAS_RTC_SS_Serial_Number_Entry();
		break;
	default:
		break;
	} //switch (Current_Lock_State)
}

void RAS_RTC_User_Code_Enter(void)
{
	Key_Press_Flash_LED(Position,1);
	if ((Public_Private_Mode_Code_Lenght == 4) && (Position == 4))
	{
		switch(Number_Of_New_Code_Entires)
		{
			case 0:
				if ((memcmp(Keycode,Mastercode,4) == 0) || (memcmp(Keycode,Submastercode,4) == 0) || (memcmp(Keycode,Techniciancode,4) == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0)))
				{
				// new code is the same like one of the existing codes
					Clear_Keycode();
					Number_Of_New_Code_Entires = 0;
					Flash_Wrong(1);
				}
				else
				{ // if the new code is different than the other codes
					memcpy(Public_Mode_Code_Entry_1,Keycode,4);
					Number_Of_New_Code_Entires = 1;
					Clear_Keycode();
					Flash_Green_LED(1);
				}
				break;
			case 1: //Second code type in
				memcpy(Public_Mode_Code_Entry_2,Keycode,4);
				if (memcmp(Public_Mode_Code_Entry_2,Public_Mode_Code_Entry_1,4) == 0)// if codes are the same
				{
					memcpy(Usercode_RAS_RTC_4,Keycode,4);
					eeprom_write_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0,4);
					Current_Lock_State = RAS_RTC_Unlock_Code_Entry;
					eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
					Clear_Keycode();
					Who_Unlocked = Nobody;
					Number_Of_New_Code_Entires = 0;
					Lock_Active_Enabled_Flag = 0;
					eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
					Motor_Open_Close_Lock(Open,0);
					Flash_Green_LED(3);
					Motor_Open_Close_Lock(Lock,0);
					Timeout_Constant = 1500;
				}//if (Public_Mode_Code_Entry_1==Public_Mode_Code_Entry_2)// if codes are the same
				else
				{ // different codes
					Flash_Wrong(2);
					Clear_Keycode();
					Number_Of_New_Code_Entires = 0;
				}
				break;
			default:
				break;
		}//switch(Number_Of_New_Code_Entires)
	}	
}

uint8_t Check_Lock_Active()
{	
	uint8_t Current_Hr = 0;
	uint8_t Current_Min = 0;
	uint8_t Active_Check = 0;
	cli();
	if (Lock_Active_Enabled_Flag == 1)
	{
		PCF2123_Read_Register(3,2); // Reads minutes and hrs
		Current_Min = SPI_Data_Buffer[0] & 0b01111111; // 7th bit of min can have 1 or 0 value so mask it off
		Current_Hr = SPI_Data_Buffer[1];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (Lock_Active_Stop_Time_Hr > Lock_Active_Start_Time_Hr) // eg: active  from 8:00-16:15
		{
			if(Current_Hr > Lock_Active_Start_Time_Hr) // current hrs is bigger than set up
			{
						if (Current_Hr < Lock_Active_Stop_Time_Hr)
						{
								Active_Check = 1;
						}
						else if (Current_Hr == Lock_Active_Stop_Time_Hr)
						{
								if (Current_Min < Lock_Active_Stop_Time_Min)
								{
									Active_Check = 1;
								}
								else
								{
									Active_Check = 0;
								}
						}
						else
						{
							Active_Check = 0;
						}
			}  //if ((SPI_Data_Buffer[1] >= Lock_Active_Start_Time_Hr)) // check hrs
			else if(Current_Hr == Lock_Active_Start_Time_Hr) // current hrs is the same as set up
			{
						if (Current_Min >= Lock_Active_Start_Time_Min)
						{
									if (Current_Hr < Lock_Active_Stop_Time_Hr)
									{
										Active_Check = 1;
									}
									else if (Current_Hr == Lock_Active_Stop_Time_Hr)
									{
												if (Current_Min < Lock_Active_Stop_Time_Min)
												{
													Active_Check = 1;
												}
												else
												{
													Active_Check = 0;
												}
									}
						}
						else
						{
							Active_Check = 0;
						}
			} //else if(SPI_Data_Buffer[1] == Lock_Active_Start_Time_Hr) // current hrs is the same as set up
			else // current time is smaller than start time
			{
				Active_Check = 0;
			}
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		else if (Lock_Active_Stop_Time_Hr < Lock_Active_Start_Time_Hr) // eg: 8:00-2:00
		{
			if (Current_Hr == Lock_Active_Start_Time_Hr) //
			{
				//if ((SPI_Data_Buffer[0]) >= Lock_Active_Start_Time_Min)
				if (Current_Min >= Lock_Active_Start_Time_Min)
				{
					Active_Check = 1;
				}
				else
				{
					Active_Check = 0;
				}
			}
			else if (Current_Hr > Lock_Active_Start_Time_Hr)
			{
				Active_Check = 1;
			}
			else // Current hr < Start Hr
			{
				if (Current_Hr < Lock_Active_Stop_Time_Hr) //
				{
					Active_Check = 1;
				}
				else if (Current_Hr == Lock_Active_Stop_Time_Hr)
				{
					if (Current_Min < Lock_Active_Stop_Time_Min)
					{
						Active_Check = 1;
					}
					else
					{
						Active_Check = 0;
					}
				}
				else 
				{
					Active_Check = 0;
				}
			}
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		else if (Lock_Active_Stop_Time_Hr == Lock_Active_Start_Time_Hr) // eg: 8:00-8:45
		{
				if(Lock_Active_Start_Time_Min != Lock_Active_Stop_Time_Min)
				{
						if(Current_Hr < Lock_Active_Start_Time_Hr)
						{
							Active_Check = 0;
						}
						else if (Current_Hr > Lock_Active_Start_Time_Hr)
						{
							Active_Check = 0;
						}
						else
						{
								if((Current_Min >= Lock_Active_Start_Time_Min) && (Current_Min < Lock_Active_Stop_Time_Min))
								{
									Active_Check = 1;
								}
								else
								{
									Active_Check = 0;
								}
						}
				}//if(Lock_Active_Start_Time_Min != Lock_Active_Stop_Time_Min)
				else // if start and stop Hr and Min is exactly the same
				{
					Active_Check = 1;
				}
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	} //if (Lock_Active_Enabled_Flag == 1)
	else
	{
		Active_Check = 1;
	}
		
	if (Active_Check == 1)
	{
		return 1;
	} 
	else
	{
		return 0;
	}
	sei();
}
uint16_t  Check_Battery(uint8_t Motor_Run_Flag) // 0 no motor run; 1 motor run
{
	uint16_t ADC_Result;
	ADMUX = 0b01011110; // Vcc as Vref and 1.1 as input
	ADCSRA = (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); // Enable ADC and set the speed 
	ADCSRA |= (1<<ADSC); 
	if (Motor_Run_Flag == 1)
	{
		Motor_CCW();
		_delay_ms(1);
	}
	CLEARBIT(LED_PORT,LED_G_4);
	CLEARBIT(LED_PORT,LED_G_3);
	CLEARBIT(LED_PORT,LED_G_2);
	CLEARBIT(LED_PORT,LED_G_1);
	CLEARBIT(LED_PORT,LED_R_4);
	CLEARBIT(LED_PORT,LED_R_3);
	CLEARBIT(LED_PORT,LED_R_2);
	CLEARBIT(LED_PORT,LED_R_1);
	while (!(ADCSRA & 0x10));
	ADC_Result = ADC;
	ADCSRA |= (1<<ADSC);
	while (!(ADCSRA & 0x10));
	ADC_Result = ADC;
	if (Motor_Run_Flag == 1)
	{
		Motor_OFF();
	}
	SETBIT(LED_PORT,LED_R_4);
	SETBIT(LED_PORT,LED_R_3);
	SETBIT(LED_PORT,LED_R_2);
	SETBIT(LED_PORT,LED_R_1);
	SETBIT(LED_PORT,LED_G_4);
	SETBIT(LED_PORT,LED_G_3);
	SETBIT(LED_PORT,LED_G_2);
	SETBIT(LED_PORT,LED_G_1);
	if (ADC_Result <= 386)
	{
		CLEARBIT(LED_PORT,LED_G_4);
		CLEARBIT(LED_PORT,LED_G_3);
		CLEARBIT(LED_PORT,LED_G_2);
		CLEARBIT(LED_PORT,LED_G_1);
	}
	else
	{
		if (ADC_Result <= 400)
		{
			CLEARBIT(LED_PORT,LED_G_3);
			CLEARBIT(LED_PORT,LED_G_2);
			CLEARBIT(LED_PORT,LED_G_1);
		}
		else
		{
			if (ADC_Result <= 415)
			{
				CLEARBIT(LED_PORT,LED_G_2);
				CLEARBIT(LED_PORT,LED_G_1);
			}
			else
			{
				if (ADC_Result <= 431)
				{
					CLEARBIT(LED_PORT,LED_R_2);
					CLEARBIT(LED_PORT,LED_R_1);
					CLEARBIT(LED_PORT,LED_G_2);
					CLEARBIT(LED_PORT,LED_G_1);
				}
				else
				{
					if (ADC_Result <= 447)
					{
						CLEARBIT(LED_PORT,LED_R_1);
						CLEARBIT(LED_PORT,LED_G_1);
					}
					else
					{
						CLEARBIT(LED_PORT,LED_R_1);
					}
				}
			}
		}
	}
	if (Motor_Run_Flag == 1)
	{
		Motor_CW();
		_delay_ms(1);
	}
	return	ADC_Result;
}

void New_Motor_Setup(void)
{
	//Set MOTOR PINS as outputs
	SETBIT(MOT_1H_DDR,MOT_1H_PIN); //Set as output
	SETBIT(MOT_2H_DDR,MOT_2H_PIN); //Set as output
	SETBIT(MOT_1L_DDR,MOT_1L_PIN); //Set as output
	SETBIT(MOT_2L_DDR,MOT_2L_PIN); //Set as output
	//Set Motor Off
	SETBIT(MOT_1H_PORT,MOT_1H_PIN); 
	SETBIT(MOT_2H_PORT,MOT_2H_PIN); 
	CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
	CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
}


void Set_up(void)
{
	uint8_t Temp_set_1;
	uint8_t Temp_set_2;
	uint8_t Temp_Set_3;
	_delay_ms(50);
	LED_PORT = 0b11111111; //drive LED high to switch them off
	DDRA = 0b11110000;
	PORTC = 0b11111111; // switch off LEDS
	DDRC = 0b11111111;   // LED port as output
	DDRD = 0b00000000;
	DDRE = 0b00000001;
	New_Motor_Setup();
	/// Keypad set up
	CLEARBIT(KEY_Y1_DDR,KEY_Y1); // As inputs
	CLEARBIT(KEY_Y2_DDR,KEY_Y2);
	CLEARBIT(KEY_Y3_DDR,KEY_Y3);
	// Pull up
	SETBIT(KEY_Y1_PORT,KEY_Y1);
	SETBIT(KEY_Y2_PORT,KEY_Y2);
	SETBIT(KEY_Y3_PORT,KEY_Y3);
	// As outputs
	SETBIT(KEY_X1_DDR,KEY_X1);
	SETBIT(KEY_X2_DDR,KEY_X2);
	SETBIT(KEY_X3_DDR,KEY_X3);
	SETBIT(KEY_X4_DDR,KEY_X4);
	// Pull high
	SETBIT(KEY_X1_PORT,KEY_X1);
	SETBIT(KEY_X2_PORT,KEY_X2);
	SETBIT(KEY_X3_PORT,KEY_X3);
	SETBIT(KEY_X4_PORT,KEY_X4);
	// Set up Keypad Wake INT pin
	SETBIT(DDRD,3);						// As input
	SETBIT(PORTD,3);					// Pull up
	// Set CS1 pin eeprom 
	SETBIT(EEPROM_CS1_DDR,EEPROM_CS1);	// CS1 pin as output
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);	// CS1 high to deactivate eeprom
	// Configure CE INT pin for RTC
	CLEARBIT(RTC_INT_DDR, RTC_INT);		// Pin is set as input
	SETBIT(RTC_INT_PORT,RTC_INT);		// Enable pull-up resistor
	// Configure ADC pin
	CLEARBIT(ADC_DDR, ADC_PIN);			//Pin is set as input
	Timeout_Constant = 1500; 
	Motor_OFF();
	SPI_Init();
	//////////////////////////////////////////
	PCF2123_Read_Register(0x00,16);
	PCF2123_Read_Register(0x02,1);
	if((SPI_Data_Buffer[0] && 0x80)!=0)
	{
		SPI_Data_Buffer[0]=0x58; // software reset
		PCF2123_Write_Register(0x00,1); // software reset RTC
		PCF2123_Read_Register(0x00,12);
	}
// First, check whether it's first time run
	Temp_set_1 = eeprom_read_byte((uint8_t*)EEPROM_Fist_Time_Run_1_Flag);	// First flag
	Temp_set_2 = eeprom_read_byte((uint8_t*)EEPROM_Fist_Time_Run_2_Flag);	// Second flag
	//Temp_set_1 = 0;
	if ((Temp_set_1 == 1) && (Temp_set_2 == 1)) // it's not the first fun
	{
		Lock_Position = eeprom_read_byte((uint8_t*)EEPROM_Lock_Position);
		Current_Lock_Mode = eeprom_read_byte((uint8_t*)EEPROM_Lock_Mode);
		Public_Private_Mode_Code_Lenght = eeprom_read_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght);
		Current_Lock_State = eeprom_read_byte((uint8_t*)EEPROM_Lock_State);
		Wrong_Code_Attempts_Allowance = eeprom_read_byte((uint8_t*)EEPROM_Wrong_Code_Attempts_Allowance);
		Lock_Block_Time = 10;			// Approx in sec
		Number_Of_New_Code_Entires = 0;
		Required_Number_Of_New_Code_Entires = eeprom_read_byte((uint8_t*)EEPROM_Required_Number_Of_New_Code_Entires);
		Audio = eeprom_read_byte((uint8_t*)EEPROM_Lock_Audio);			//buzzer ON
		Position = 0;
		Unsuccessful_Code_Lockout_Flag = eeprom_read_byte((uint8_t*)EEPROM_Unsuccessful_Code_Lockout_Flag); //wrong code lockout enabled
		LED_ON = 0;
		Max_Allowed_Position = 8;
		Who_Unlocked = Nobody;
		//Auto_Timed_Unlock_Flag = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag);
		eeprom_read_block(&Auto_Timed_Unlock_Time,(void*)EEPROM_Auto_Timed_Unlock_Time_0 ,2);
		eeprom_read_block(&nextcode,(void*)EEPROM_nextcode_0 ,4);
		eeprom_read_block(&initcode,(void*)EEPROM_initcode_0 ,4);
		eeprom_read_block(&serialcode,(void*)EEPROM_serialcode_0 ,4);
		eeprom_read_block(&Usercode_4,(void*)EEPROM_Usercode_4_0 ,4);
		eeprom_read_block(&Submastercode,(void*)EEPROM_Submastercode_0 ,6);
		eeprom_read_block(&Techniciancode,(void*)EEPROM_Techniciancode_0 ,6);
		eeprom_read_block(&Usercode_6,(void*)EEPROM_Usercode_6_0 ,6);
		eeprom_read_block(&Mastercode,(void*)EEPROM_Mastercode_0,8);
		for(Temp_Set_3=0;Temp_Set_3<Max_Allowed_Multiuser_Number; Temp_Set_3 ++)
		{
			eeprom_read_block(&MultiUsercode[Temp_Set_3],(void*)(EEPROM_Multiuser_0 + (Temp_Set_3*4)),4);
		}
		Lock_Active_Enabled_Flag = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Flag);
		Lock_Active_Start_Time_Hr = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr);
		Lock_Active_Start_Time_Min = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min);
		Lock_Active_Stop_Time_Hr = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr);
		Lock_Active_Stop_Time_Min = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min);
		Stop_Hour = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Hour);
		Stop_Day = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Day);
		Stop_Month = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Month);
		Stop_Year = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Year);
		eeprom_read_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0 ,4);
		EICRA = (1<<ISC11)|(1<<ISC01);			// The low level of INT1 generates an INT 
		Temp_set_1 = eeprom_read_byte((uint8_t*)EEPROM_Interrupts_Container);
		EIMSK = Temp_set_1;
		EIFR = EIFR & 0b0000010;
		SPI_Data_Buffer[0] = eeprom_read_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1);
		PCF2123_Write_Register(1,1);
		SPI_Data_Buffer[0] = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0);
		SPI_Data_Buffer[1] = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1);
		PCF2123_Write_Register(0x09,2);		// send from the SPI_Data_Buffer[0] SPI_Data_Buffer[1] to an alarm register
		//PCF2123_Read_Register(0x00,12);
		SPI_Data_Buffer[0] = 0;
		SPI_Data_Buffer[1] = 0;
	} 
	else // it's the first time run
	{
		CLEARBIT(LED_PORT,LED_R_2); CLEARBIT(LED_PORT,LED_R_3); _delay_ms(250); SETBIT(LED_PORT,LED_R_2); SETBIT(LED_PORT,LED_R_3); _delay_ms(250);
		for (uint16_t e=0;e<0x03FF;e++)
		{
			eeprom_write_byte((uint8_t*)e,0xFF);
		}
		Lock_Position = Normal;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Position,Lock_Position);
		Current_Lock_Mode = Public_Mode;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
		Public_Private_Mode_Code_Lenght = 4;
		eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
		Current_Lock_State = Public_Lock_Code_Entry;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
		Wrong_Code_Attempts_Allowance = 5;
		eeprom_write_byte((uint8_t*)EEPROM_Wrong_Code_Attempts_Allowance,Wrong_Code_Attempts_Allowance);
		Lock_Block_Time = 10; // Approx in sec
		Number_Of_New_Code_Entires = 0;
		Required_Number_Of_New_Code_Entires = 2;
		eeprom_write_byte((uint8_t*)EEPROM_Required_Number_Of_New_Code_Entires,Required_Number_Of_New_Code_Entires);
		Audio = Audio_ON;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Audio,Audio);							//buzzer ON
		Position = 0;
		Unsuccessful_Code_Lockout_Flag = Lockout_ON; //wrong code lockout enabled
		eeprom_write_byte((uint8_t*)EEPROM_Unsuccessful_Code_Lockout_Flag,Unsuccessful_Code_Lockout_Flag);
		LED_ON = 0;
		Max_Allowed_Position = 8;
		Who_Unlocked = Nobody;
		eeprom_write_byte((uint8_t*)EEPROM_Battery_Check_During_Motor_Run_Flag,1); // enable battery check
		serialcode[0] = 0;
		serialcode[1] = 0;
		serialcode[2] = 0;
		serialcode[3] = 0;
		eeprom_write_block(&serialcode,(void*)EEPROM_serialcode_0,4);
		initcode[0] = 0;
		initcode[1] = 0;
		initcode[2] = 0;
		initcode[3] = 0;
		eeprom_write_block(&initcode,(void*)EEPROM_initcode_0,4);
		Usercode_4[0] = 2;
		Usercode_4[1] = 2;
		Usercode_4[2] = 4;
		Usercode_4[3] = 4;
		eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
		nextcode[0] = 0;
		nextcode[1] = 0;
		nextcode[2] = 0;
		nextcode[3] = 0;
		eeprom_write_block(&nextcode,(void*)EEPROM_nextcode_0,4);
		Submastercode[0] = 4;
		Submastercode[1] = 4;
		Submastercode[2] = 6;
		Submastercode[3] = 6;
		Submastercode[4] = 8;
		Submastercode[5] = 8;
		eeprom_write_block(&Submastercode,(void*)EEPROM_Submastercode_0,6);
		Techniciancode[0] = 3;
		Techniciancode[1] = 3;
		Techniciancode[2] = 5;
		Techniciancode[3] = 5;
		Techniciancode[4] = 7;
		Techniciancode[5] = 7;
		eeprom_write_block(&Techniciancode,(void*)EEPROM_Techniciancode_0,6);
		Usercode_6[0] = 2;
		Usercode_6[0] = 2;
		Usercode_6[0] = 4;
		Usercode_6[0] = 4;
		Usercode_6[0] = 6;
		Usercode_6[0] = 6;
		eeprom_write_block(&Usercode_6,(void*)EEPROM_Usercode_6_0,6);
		Mastercode[0] = 1;
		Mastercode[1] = 1;
		Mastercode[2] = 3;
		Mastercode[3] = 3;
		Mastercode[4] = 5;
		Mastercode[5] = 5;
		Mastercode[6] = 7;
		Mastercode[7] = 7;
		eeprom_write_block(&Mastercode,(void*)EEPROM_Mastercode_0,8);
		Multiuser_Code[0] = 22;//fill multiuser table with no user - 0
		Multiuser_Code[1] = 22;
		Multiuser_Code[2] = 22;
		Multiuser_Code[3] = 22;
		for(Temp_Set_3=0;Temp_Set_3<Max_Allowed_Multiuser_Number; Temp_Set_3 ++)
			{
				memcpy((int*)MultiUsercode[Temp_Set_3],Multiuser_Code,4);
				eeprom_write_block(&MultiUsercode[Temp_Set_3],(void*)(EEPROM_Multiuser_0 + (Temp_Set_3*4)),4);
			}
		CLEARBIT(LED_PORT,LED_R_2); CLEARBIT(LED_PORT,LED_R_3);	_delay_ms(250);	SETBIT(LED_PORT,LED_R_2); SETBIT(LED_PORT,LED_R_3); _delay_ms(250); CLEARBIT(LED_PORT,LED_R_2); CLEARBIT(LED_PORT,LED_R_3); _delay_ms(250); SETBIT(LED_PORT,LED_R_2); SETBIT(LED_PORT,LED_R_3); _delay_ms(250);
		Lock_Active_Enabled_Flag = 0;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
		Lock_Active_Start_Time_Hr = 0x00;
		Lock_Active_Start_Time_Min = 0x00;
		Lock_Active_Stop_Time_Hr = 0x00;
		Lock_Active_Stop_Time_Min = 0x00;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
		SPI_Data_Buffer[0] = 0;
		SPI_Data_Buffer[1] = 0;
		eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0,SPI_Data_Buffer[0]);//mins
		eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1,SPI_Data_Buffer[1]);//hrs
		PCF2123_Write_Register(0x09,2); 
		Auto_Timed_Unlock_Flag = 0;
		eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag, Auto_Timed_Unlock_Flag);
		SPI_Data_Buffer[0] = 0;
		PCF2123_Write_Register(0x01,1); 
		EICRA = (1<<ISC11)|(1<<ISC01);			// The low level of INT1 generates an INT 
		EIMSK = (1<<INT1);						// External interrupts enable
		Temp_set_1 = EIMSK;
		eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,Temp_set_1);
		SPI_Data_Buffer[0] = 0;
		CLEARBIT(LED_PORT,LED_R_2); CLEARBIT(LED_PORT,LED_R_3); _delay_ms(250);	SETBIT(LED_PORT,LED_R_2); SETBIT(LED_PORT,LED_R_3); _delay_ms(250); CLEARBIT(LED_PORT,LED_R_2); CLEARBIT(LED_PORT,LED_R_3); _delay_ms(250); SETBIT(LED_PORT,LED_R_2); SETBIT(LED_PORT,LED_R_3); _delay_ms(250);	CLEARBIT(LED_PORT,LED_R_2); CLEARBIT(LED_PORT,LED_R_3);	_delay_ms(250);	SETBIT(LED_PORT,LED_R_2); SETBIT(LED_PORT,LED_R_3); _delay_ms(250);
		Initial_Test_Routine();
		SPI_Data_Buffer[0] = 1;
		eeprom_write_byte((uint8_t*)EEPROM_Fist_Time_Run_1_Flag,SPI_Data_Buffer[0] = 1); // indicate first time run completed
		eeprom_write_byte((uint8_t*)EEPROM_Fist_Time_Run_2_Flag,SPI_Data_Buffer[0] = 1); // indicate first time run completed
		
	}
	Clear_Keycode();
	//PCF2123_Read_Register(0x00,16);//uncomment for debugging
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);		// Set sleep mode
	Temp_set_1 = EIFR;
	if ((Temp_set_1 & 0b00000001)!= 0)
	{
		EIFR = 0b00000001;
	}
	PCF2123_Read_Register(0x00,12);
	sei();
}

uint8_t Return_keypad()
{
	uint8_t temp, temp1=0;
	temp = Read_keypad();
	if (temp!=0)
	{
		temp1 = (key_stroke[Lock_Position][(temp-1)]); // values
		Keypad_Pressed_Flag = 1;
	}
	else
	{
		Keypad_Pressed_Flag = 0;
	}
	return temp1;
}
uint8_t Read_keypad()
{
	uint8_t temp_press = 0;
	double time = 20;
	CLEARBIT(KEY_X1_PORT,KEY_X1);
	SETBIT(KEY_X2_PORT,KEY_X2);
	SETBIT(KEY_X3_PORT,KEY_X3);
	SETBIT(KEY_X4_PORT,KEY_X4);
	// scan first row
	if ((!(KEY_Y1_PIN & (1<<KEY_Y1)))|(!(KEY_Y2_PIN & (1<<KEY_Y2)))) // first check whether any button on this  X line was checked, if no return 0
	{
		if (!(KEY_Y1_PIN & (1<<KEY_Y1)))
		{
			temp_press = 2;
			Buzzer(8);
			while (!(KEY_Y1_PIN & (1<<KEY_Y1)))
			_delay_ms(time);
		}
		else if (!(KEY_Y2_PIN & (1<<KEY_Y2)))
		{
			temp_press = 11;
			Buzzer(8);
			while (!(KEY_Y2_PIN & (1<<KEY_Y2)))
			_delay_ms(time);
		}
	}
	// scan second row
	SETBIT(KEY_X1_PORT,KEY_X1);
	CLEARBIT(KEY_X2_PORT,KEY_X2);
	_delay_ms(1);
	if ((!(KEY_Y1_PIN & (1<<KEY_Y1)))|(!(KEY_Y2_PIN & (1<<KEY_Y2)))|(!(KEY_Y3_PIN & (1<<KEY_Y3)))) // first check whether any button on this  X line was checked, if no return 0
	{
		if (!(KEY_Y1_PIN & (1<<KEY_Y1)))
		{
			temp_press = 4;
			Buzzer(8);
			while (!(KEY_Y1_PIN & (1<<KEY_Y1)))
			_delay_ms(time);
		}
		else if (!(KEY_Y2_PIN & (1<<KEY_Y2)))
		{
			temp_press = 5;
			Buzzer(8);
			while (!(KEY_Y2_PIN & (1<<KEY_Y2)))
			_delay_ms(time);
		}
		else if (!(KEY_Y3_PIN & (1<<KEY_Y3)))
		{
			temp_press = 3;
			Buzzer(8);
			while (!(KEY_Y3_PIN & (1<<KEY_Y3)))
			_delay_ms(time);
		}
	}
	// scan third row
	SETBIT(KEY_X2_PORT,KEY_X2);
	CLEARBIT(KEY_X3_PORT,KEY_X3);
	_delay_ms(1);
	if ((!(KEY_Y1_PIN & (1<<KEY_Y1)))|(!(KEY_Y2_PIN & (1<<KEY_Y2)))|(!(KEY_Y3_PIN & (1<<KEY_Y3)))) // first check whether any button on this  X line was checked, if no return 0
	{
		if (!(KEY_Y1_PIN & (1<<KEY_Y1)))
		{
			temp_press = 8;
			Buzzer(8);
			while (!(KEY_Y1_PIN & (1<<KEY_Y1)))
			_delay_ms(time);
		}
		else if (!(KEY_Y2_PIN & (1<<KEY_Y2)))
		{
			temp_press = 6;
			Buzzer(8);
			while (!(KEY_Y2_PIN & (1<<KEY_Y2)))
			_delay_ms(time);
		}
		else if (!(KEY_Y3_PIN & (1<<KEY_Y3)))
		{
			temp_press = 7;
			Buzzer(8);
			while (!(KEY_Y3_PIN & (1<<KEY_Y3)))
			_delay_ms(time);
		}
	}
	// scan fourth row
	SETBIT(KEY_X3_PORT,KEY_X3);
	CLEARBIT(KEY_X4_PORT,KEY_X4);
	_delay_ms(1);
	if ((!(KEY_Y1_PIN & (1<<KEY_Y1)))|(!(KEY_Y2_PIN & (1<<KEY_Y2)))|(!(KEY_Y3_PIN & (1<<KEY_Y3)))) // first check whether any button on this  X line was checked, if no return 0
	{
		if (!(KEY_Y1_PIN & (1<<KEY_Y1)))
		{
			temp_press = 10;
			Buzzer(8);
			while (!(KEY_Y1_PIN & (1<<KEY_Y1)))
			_delay_ms(time);
		}
		else if (!(KEY_Y2_PIN & (1<<KEY_Y2)))
		{
			temp_press = 1;
			Buzzer(8);
			while (!(KEY_Y2_PIN & (1<<KEY_Y2)))
			_delay_ms(time);
		}
		else if (!(KEY_Y3_PIN & (1<<KEY_Y3)))
		{
			temp_press = 9;
			Buzzer(8);
			while (!(KEY_Y3_PIN & (1<<KEY_Y3)))
			_delay_ms(time);
		}
		SETBIT(KEY_X4_PORT,KEY_X4);
	}
	return temp_press;
}
void Buzzer(uint8_t Cycles_b)
{
	uint8_t i_bu;
	if (Audio == Audio_ON)
	{
		for (i_bu=0;i_bu<Cycles_b;i_bu++)
		{
			SETBIT(BUZZ_PORT,BUZZ);
			_delay_us(170);
			CLEARBIT(BUZZ_PORT,BUZZ);
			_delay_us(170);
		}
	}
}

void Buzzer_Beep(uint8_t count_bb)
{
	uint8_t i_bb;
	for (i_bb=0;i_bb<count_bb;i_bb++)
	{
		Flash_Green_LED(0);
		Buzzer(20);
		Flash_Green_LED(0);
		_delay_ms(250);
	}
}

uint16_t ADC_Convert(uint8_t channel)
{
	uint16_t result;
	channel = (channel & 0b00000111)|0b10000000 ;								//Set 1.1 internal ref as ref and a channel number;
	ADMUX = channel;
	ADCSRA  =   (1 << ADEN)|(1 << ADPS2)|(0 << ADPS1)|(1 << ADPS0); //Turn on ADC and set the speed of conversion
	ADCSRA |= (1 << ADSC);											//Start dummy conversion
	while(!(ADCSRA & 0x10));										//Dummy step
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & 0x10));
	result = ADC;													//Take reading
	ADCSRA &= ~(1 << ADEN);											// switch the ADC off
	return result;
}

void Motor_Open_Close_Lock (uint8_t direction, uint8_t Do_Battery_Check) // OPEN/LOCK ,1 for the battery check
{
	uint8_t  i,j, Temp, counter = 0;
	uint16_t readings[10];
	float sum = 0.0;
	if (direction == Open)
	{
		Motor_CCW();
	} 
	else
	{
		Motor_CW();
	}
	_delay_ms(100);		// delay to allow for inrush motor start
	Temp = eeprom_read_byte((uint8_t*)EEPROM_Battery_Check_During_Motor_Run_Flag);	// First flag
	if ((Do_Battery_Check == 1) && (Temp == 1))
	{
		Check_Battery(0); // check but without engaging the motor - it's already running
	}
	for (i=0;i<10;i++)
	{
		readings[i] = 0;  // fill the array with 0
	}
	do 
	{	
		for(i=0;i<10;i++) 
			{
				for (j=9;j>0;j--)
					{
						readings[j] = readings[j-1];		//shift the readings one position up
					}
				readings[0]= ADC_Convert(0);
				_delay_ms(1);
			}
		for (j=0;j<10;j++)
			{
				sum += (float)readings[j];		// sum all the readings up
			}
		sum = sum / (float)10;				//calculate the mean value
		counter ++;
		_delay_ms(10);
	} 
	while ((sum < (float)50) && (counter < 50));
	//Motor_OFF();
	if (direction == Open)
	{
		Motor_CCW_Slow();
	}
	else
	{
		Motor_CW_Slow();
	}
	Motor_OFF();
	

	Timeout_Counter = 1000;
	if (Do_Battery_Check == 1)
	{
		// switch off LED if have been enabled
		SETBIT(LED_PORT,LED_R_4);
		SETBIT(LED_PORT,LED_R_3);
		SETBIT(LED_PORT,LED_R_2);
		SETBIT(LED_PORT,LED_R_1);
		SETBIT(LED_PORT,LED_G_4);
		SETBIT(LED_PORT,LED_G_3);
		SETBIT(LED_PORT,LED_G_2);
		SETBIT(LED_PORT,LED_G_1);
	}
}

void Motor_CW (void)
{
// 	SETBIT(MOT_PORT,MOT_POS);
// 	_delay_ms(1);
// 	CLEARBIT(MOT_PORT,MOT_NEG);
	SETBIT(MOT_1H_PORT,MOT_1H_PIN);
	CLEARBIT(MOT_2H_PORT,MOT_2H_PIN);
	_delay_ms(10);
	SETBIT(MOT_1L_PORT,MOT_1L_PIN);
	CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
	_delay_ms(10);
}

 void Motor_CW_Slow (void)
 {
	for( uint8_t z =0;z<25;z++)
	{
		SETBIT(MOT_1H_PORT,MOT_1H_PIN);
		CLEARBIT(MOT_2H_PORT,MOT_2H_PIN);
		_delay_ms(1);
		SETBIT(MOT_1L_PORT,MOT_1L_PIN);
		CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
		_delay_ms(1);
		/// motor off below
		SETBIT(MOT_1H_PORT,MOT_1H_PIN);
		SETBIT(MOT_2H_PORT,MOT_2H_PIN);
		_delay_ms(1);
		CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
		CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
		_delay_ms(1);
	}
	for( uint8_t z =0;z<25;z++)
	{
		SETBIT(MOT_1H_PORT,MOT_1H_PIN);
		CLEARBIT(MOT_2H_PORT,MOT_2H_PIN);
		_delay_ms(1);
		SETBIT(MOT_1L_PORT,MOT_1L_PIN);
		CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
		_delay_ms(1);
		/// motor off below
		SETBIT(MOT_1H_PORT,MOT_1H_PIN);
		SETBIT(MOT_2H_PORT,MOT_2H_PIN);
		_delay_ms(1);
		CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
		CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
		_delay_ms(2);
	}
 }

  void Motor_CCW_Slow (void)
  {
	  for( uint8_t z = 0; z<25; z++)
	  {
		  CLEARBIT(MOT_1H_PORT,MOT_1H_PIN);
		  SETBIT(MOT_2H_PORT,MOT_2H_PIN);
		  _delay_ms(1);
		  CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
		  SETBIT(MOT_2L_PORT,MOT_2L_PIN);
		  _delay_ms(1);
		  /// motor off below
		  SETBIT(MOT_1H_PORT,MOT_1H_PIN);
		  SETBIT(MOT_2H_PORT,MOT_2H_PIN);
		  _delay_ms(1);
		  CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
		  CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
		  _delay_ms(1);
	  }
	  for( uint8_t z = 0; z<25; z++)
	  {
		  CLEARBIT(MOT_1H_PORT,MOT_1H_PIN);
		  SETBIT(MOT_2H_PORT,MOT_2H_PIN);
		  _delay_ms(1);
		  CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
		  SETBIT(MOT_2L_PORT,MOT_2L_PIN);
		  _delay_ms(1);
		  /// motor off below
		  SETBIT(MOT_1H_PORT,MOT_1H_PIN);
		  SETBIT(MOT_2H_PORT,MOT_2H_PIN);
		  _delay_ms(1);
		  CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
		  CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
		  _delay_ms(2);
	  }
  }
void Motor_CCW (void)
{
// 	SETBIT(MOT_PORT,MOT_NEG);
// 	_delay_ms(1);
// 	CLEARBIT(MOT_PORT,MOT_POS);
	CLEARBIT(MOT_1H_PORT,MOT_1H_PIN);
	SETBIT(MOT_2H_PORT,MOT_2H_PIN);
	_delay_ms(10);
	CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
	SETBIT(MOT_2L_PORT,MOT_2L_PIN);
	_delay_ms(10);
}
void Motor_OFF (void)
{
// 	CLEARBIT(MOT_PORT,MOT_POS);
// 	CLEARBIT(MOT_PORT,MOT_NEG);
// 	_delay_ms(20);
	SETBIT(MOT_1H_PORT,MOT_1H_PIN);
	SETBIT(MOT_2H_PORT,MOT_2H_PIN);
	_delay_ms(10);
	CLEARBIT(MOT_1L_PORT,MOT_1L_PIN);
	CLEARBIT(MOT_2L_PORT,MOT_2L_PIN);
	_delay_ms(10);
}
void Flash_Green_LED(uint8_t Count_f) // flashes the green leds; toggle if 0 passed into
{
	uint8_t i_f;
	if (Count_f == 0)
	{
		if (LED_ON == 0)
		{
			CLEARBIT(LED_PORT,LED_G_4);
			CLEARBIT(LED_PORT,LED_G_3);
			CLEARBIT(LED_PORT,LED_G_2);
			CLEARBIT(LED_PORT,LED_G_1);
			LED_ON = 1;
		}
		else
		{
			SETBIT(LED_PORT,LED_G_4);
			SETBIT(LED_PORT,LED_G_3);
			SETBIT(LED_PORT,LED_G_2);
			SETBIT(LED_PORT,LED_G_1);
			LED_ON = 0;
		}
	}
	else
	{
		for (i_f=0;i_f<Count_f;i_f++)
		{
			CLEARBIT(LED_PORT,LED_G_4);
			CLEARBIT(LED_PORT,LED_G_3);
			CLEARBIT(LED_PORT,LED_G_2);
			CLEARBIT(LED_PORT,LED_G_1);
			_delay_ms(100);
			SETBIT(LED_PORT,LED_G_4);
			SETBIT(LED_PORT,LED_G_3);
			SETBIT(LED_PORT,LED_G_2);
			SETBIT(LED_PORT,LED_G_1);
			_delay_ms(100);
		}
			
	}
	
}
void Flash_Wrong(uint8_t Count) // If Count != 0 then flash all red LED on once for 500ms. Count != 0 flashes all red for number of times;
{
	uint8_t i_f;
	if (Count != 0)
	{
		_delay_ms(400);
		for (i_f=0;i_f<Count;i_f++)
		{
			CLEARBIT(LED_PORT,LED_R_4);
			CLEARBIT(LED_PORT,LED_R_3);
			CLEARBIT(LED_PORT,LED_R_2);
			CLEARBIT(LED_PORT,LED_R_1);
			_delay_ms(250);
			SETBIT(LED_PORT,LED_R_4);
			SETBIT(LED_PORT,LED_R_3);
			SETBIT(LED_PORT,LED_R_2);
			SETBIT(LED_PORT,LED_R_1);
			_delay_ms(500);
		}
	}
	else
	{
		CLEARBIT(LED_PORT,LED_R_4);
		CLEARBIT(LED_PORT,LED_R_3);
		CLEARBIT(LED_PORT,LED_R_2);
		CLEARBIT(LED_PORT,LED_R_1);
		_delay_ms(500);
		SETBIT(LED_PORT,LED_R_4);
		SETBIT(LED_PORT,LED_R_3);
		SETBIT(LED_PORT,LED_R_2);
		SETBIT(LED_PORT,LED_R_1);
	}
}
void Key_Press_Flash_LED(uint8_t Position_k, uint8_t Mode)
{
	
	switch(Mode)
	{
		case 1: // green LEDS
			switch(Position_k)
			{
				case 0:
					break;
				case 4:
				case 8:
				case 12:
				case 16:
					CLEARBIT(LED_PORT,LED_G_4);
				case 3:
				case 7:
				case 11:
				case 15:
					CLEARBIT(LED_PORT,LED_G_3);
				case 2:
				case 6:
				case 10:
				case 14:
					CLEARBIT(LED_PORT,LED_G_2);
				case 1:
				case 5:
				case 9:
				case 13:
					CLEARBIT(LED_PORT,LED_G_1);
				default:
					_delay_ms(50);
					SETBIT(LED_PORT,LED_G_4);
					SETBIT(LED_PORT,LED_G_3);
					SETBIT(LED_PORT,LED_G_2);
					SETBIT(LED_PORT,LED_G_1);
					break;
			}//switch(Position_k)
			break;
		case 2:
			switch(Position_k)
			{
				case 0:
					break;
				case 4:
				case 8:
				case 12:
				case 16:
					CLEARBIT(LED_PORT,LED_G_4);
					CLEARBIT(LED_PORT,LED_R_4);
				case 3:
				case 7:
				case 11:
				case 15:
					CLEARBIT(LED_PORT,LED_G_3);
					CLEARBIT(LED_PORT,LED_R_3);
				case 2:
				case 6:
				case 10:
				case 14:
					CLEARBIT(LED_PORT,LED_G_2);
					CLEARBIT(LED_PORT,LED_R_2);
				case 1:
				case 5:
				case 9:
				case 13:
					CLEARBIT(LED_PORT,LED_G_1);
					CLEARBIT(LED_PORT,LED_R_1);
				default:
					_delay_ms(50);
					SETBIT(LED_PORT,LED_G_4);
					SETBIT(LED_PORT,LED_G_3);
					SETBIT(LED_PORT,LED_G_2);
					SETBIT(LED_PORT,LED_G_1);
					SETBIT(LED_PORT,LED_R_4);
					SETBIT(LED_PORT,LED_R_3);
					SETBIT(LED_PORT,LED_R_2);
					SETBIT(LED_PORT,LED_R_1);
					break;
			}//switch(Position_k)
			break;
		default:
			break;
	}//switch(mode)
	
}
void Public_Mode_Unlock(uint8_t Do_Battery_Check) // 1 do check, 0 not
{//
	//Flash_Green_LED(0);
	Motor_Open_Close_Lock(Open,Do_Battery_Check);
	//Flash_Green_LED(0);
	Clear_Keycode();
	Wrong_Code_Attempts_Count = 0;
	memcpy(Keycode,Temp_Array,4); // Wipe it clean
	Current_Lock_State = Public_Lock_Code_Entry;
	eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
	Who_Unlocked = Nobody;
}
void Public_Private_Mode_Wrong_Code_Process(void)
{
	
	if(Unsuccessful_Code_Lockout_Flag == Lockout_ON)
	{	
		Wrong_Code_Attempts_Count ++;
		if (Wrong_Code_Attempts_Count >= Wrong_Code_Attempts_Allowance) //oops, too many wrong codes
		{
			for (i=0;i<=Lock_Block_Time;i++) // loop for number of Lock_Block_Time seconds
			{
				CLEARBIT(LED_PORT,LED_R_4);
				CLEARBIT(LED_PORT,LED_R_3);
				CLEARBIT(LED_PORT,LED_R_2);
				CLEARBIT(LED_PORT,LED_R_1);
				_delay_ms(25);
				SETBIT(LED_PORT,LED_R_4);
				SETBIT(LED_PORT,LED_R_3);
				SETBIT(LED_PORT,LED_R_2);
				SETBIT(LED_PORT,LED_R_1);
				_delay_ms(975);
			}
			for(i=0;i<=3;i++)
			{
				CLEARBIT(LED_PORT,LED_G_4);
				CLEARBIT(LED_PORT,LED_G_3);
				CLEARBIT(LED_PORT,LED_G_2);
				CLEARBIT(LED_PORT,LED_G_1);
				_delay_ms(100);
				SETBIT(LED_PORT,LED_G_4);
				SETBIT(LED_PORT,LED_G_3);
				SETBIT(LED_PORT,LED_G_2);
				SETBIT(LED_PORT,LED_G_1);
				_delay_ms(100);
				Wrong_Code_Attempts_Count = 0;
			}
		} 
		Flash_Unsuccessful_Code_Attempts(Wrong_Code_Attempts_Allowance-Wrong_Code_Attempts_Count);
		
	} //if(Unsuccessful_Code_Lockout_Flag == Lockout_ON)
	else
	{
		Wrong_Code_Attempts_Count = 0;
		Flash_Unsuccessful_Code_Attempts(4);
	}
	
	
}
void Private_Mode_Unlock(uint8_t Do_Battery_Check)
{
	//Flash_Green_LED(0);
	Motor_Open_Close_Lock(Open,1);
	_delay_ms(100);
	Motor_Open_Close_Lock(Lock,0);
	//Flash_Green_LED(0);
	Clear_Keycode();
	Wrong_Code_Attempts_Count = 0;
	Who_Unlocked = Nobody;
}
void Flash_Unsuccessful_Code_Attempts(uint8_t count_f)
{
	switch(count_f)
	{
		case 4:
			CLEARBIT(LED_PORT,LED_R_4);
		case 3:
			CLEARBIT(LED_PORT,LED_R_3);
		case 2:
			CLEARBIT(LED_PORT,LED_R_2);
		case 1:
			CLEARBIT(LED_PORT,LED_R_1);
			break;
		default:
			break;
	}
	_delay_ms(1000)	;
	SETBIT(LED_PORT,LED_R_4);
	SETBIT(LED_PORT,LED_R_3);
	SETBIT(LED_PORT,LED_R_2);
	SETBIT(LED_PORT,LED_R_1);
}
void Logo_Press_Function(void)
{
	Key_Press_Flash_LED(Position,1);
	if (Position == 2) // when the two digits of the function have been typed in
	{
		Function_Number=(Keycode[0]*10)+Keycode[1];
		Clear_Keycode();
				if ((Function_Number == Change_Master_Code)||(Function_Number == Set_Change_Submaster_Code)||(Function_Number == Delete_Submaster_Code)||(Function_Number == Set_Lock_Position)||(Function_Number == Enable_Battery_Check_During_Motor_Run)||(Function_Number == Set_Timer_Auto_Unlock_ON_And_Set_Time)
				||(Function_Number == Set_Change_Technician_Code)||(Function_Number == Delete_Technician_Code)||(Function_Number == Set_Single_User_Private_Mode_4)||(Function_Number == RAS_RTC_Serial_Entry)||(Function_Number == Disable_Battery_Check_During_Motor_Run)
				||(Function_Number == Set_Single_User_Private_Mode_6)||(Function_Number == Set_Multiuser_Private_Mode)||(Function_Number == Set_Public_Mode_4)||(Function_Number == Add_Change_User_Access_Code)||(Function_Number == Reset_Lock_To_Default)||(Function_Number == Show_Timer_Auto_Unlock_Time)
				||(Function_Number == Set_Public_Mode_6)||(Function_Number == Set_Audio_ON)||(Function_Number == Set_Audio_OFF)||(Function_Number == Set_Unsuccessful_Code_Lockout_ON)||(Function_Number == Set_Double_User_Code_Entry_Mode)||(Function_Number == Set_Single_User_Code_Entry_Mode)||(Function_Number == Set_RAS_Mode)
				||(Function_Number == Set_Single_Use_RAS_Mode)||(Function_Number == Set_Time)||(Function_Number == Set_Date)||(Function_Number == Show_Time)||(Function_Number == Show_Date)||(Function_Number == Delete_User_Acceess_Code)||(Function_Number == RAS_RTC_SS_Serial_Entry)||(Function_Number == Set_Lock_Active_And_Auto_Unlock)||(Function_Number == Disable_Lock_Active_And_Auto_Unlock)
				||(Function_Number == Set_Lock_Active)||(Function_Number == Diasble_Lock_Active)||(Function_Number == Show_Lock_Active_Start_Time)||(Function_Number == Set_Unsuccessful_Code_Lockout_ON)||(Function_Number == Set_Unsuccessful_Code_Lockout_OFF)||(Function_Number == Delete_All_User_Access_Codes)
				||(Function_Number == Show_Lock_Active_Stop_Time)||(Function_Number == Diasble_Lock_Active)||(Function_Number == Show_Lock_Active_Start_Time)||(Function_Number == Change_User_Code)||(Function_Number == Allow_User_To_Change_Own_Code)||(Function_Number == Disallow_User_To_Change_Own_Code))
				{
					switch(Function_Number)
					{
						
						case Reset_Lock_To_Default: //66
							if (Who_Unlocked == Master)
							{
								Current_Lock_State = Reset_Lock_To_Default;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Enable_Battery_Check_During_Motor_Run: 
							if (Who_Unlocked == Master)
							{
								eeprom_write_byte((uint8_t*)EEPROM_Battery_Check_During_Motor_Run_Flag,1);
								Flash_Green_LED(3);
							}
							else
							{
								Flash_Wrong(4);
							}
							Current_Lock_Mode = Previous_Lock_Mode;
							Current_Lock_State = Previous_Lock_State;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							break;
						case Disable_Battery_Check_During_Motor_Run: 
							if (Who_Unlocked == Master)
							{
								eeprom_write_byte((uint8_t*)EEPROM_Battery_Check_During_Motor_Run_Flag,0);
								Flash_Green_LED(3);
							}
							else
							{
								Flash_Wrong(4);
							}
							Current_Lock_Mode = Previous_Lock_Mode;
							Current_Lock_State = Previous_Lock_State;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							break;
						case RAS_RTC_Serial_Entry: 
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Current_Lock_State = RAS_RTC_Serial_Entry;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case RAS_RTC_SS_Serial_Entry: 
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Current_Lock_State = RAS_RTC_SS_Serial_Entry;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
							
						case Change_Master_Code: //01
							if (Who_Unlocked == Master)
							{
								Current_Lock_State = Change_Master_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Change_User_Code: //02
							if (((Who_Unlocked == Master) || (Who_Unlocked == Submaster)  || (Who_Unlocked == User)) && (Previous_Lock_Mode == Private_Mode))
							{
								Current_Lock_State = Change_User_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Change_Submaster_Code: //03
							if ((Who_Unlocked == Master)||(Who_Unlocked == Submaster))
							{
								Current_Lock_State = Set_Change_Submaster_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Delete_Submaster_Code: //04
							if (Who_Unlocked == Master)
							{
								uint8_t i_l4;
								for (i_l4=0;i_l4<6;i_l4++)
								{
									Submastercode[i_l4] = 11;
								}
								eeprom_write_block(&Submastercode,(void*)EEPROM_Submastercode_0,6);
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Add_Change_User_Access_Code: //05
							if (((Who_Unlocked == Master)||(Who_Unlocked == Submaster))&&(Previous_Lock_Mode == Multiuser_Mode))
							{
								Current_Lock_State = Add_Change_User_Access_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Delete_User_Acceess_Code: //06
							if (((Who_Unlocked == Master)||(Who_Unlocked == Submaster))&&(Previous_Lock_Mode == Multiuser_Mode))
							{
								Current_Lock_State = Delete_User_Acceess_Code;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Single_User_Private_Mode_4: //07
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Public_Private_Mode_Code_Lenght = 4;
								Current_Lock_State = Change_User_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Single_User_Private_Mode_6://08
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Public_Private_Mode_Code_Lenght = 6;
								Current_Lock_State = Change_User_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Multiuser_Private_Mode://09
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Current_Lock_Mode = Multiuser_Mode;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
								Current_Lock_State = Multiuser_Unlock_Code_Entry;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
								//set the first user pin to 002244 by default
								Multiuser_Code[0]=2; Multiuser_Code[1]=2; Multiuser_Code[2]=4; Multiuser_Code[3]=4;
								memcpy(&MultiUsercode[0],Multiuser_Code,4);
								eeprom_write_block(&MultiUsercode[Multiuser_Location],(void*)(EEPROM_Multiuser_0),4);
								Motor_Open_Close_Lock(Lock,0);
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Public_Mode_4://10
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Current_Lock_Mode = Public_Mode;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
								Public_Private_Mode_Code_Lenght = 4;
								eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
								Current_Lock_State = Public_Lock_Code_Entry;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
								Who_Unlocked = Nobody;
								Motor_Open_Close_Lock(Open,0);
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Public_Mode_6://11
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Current_Lock_Mode = Public_Mode;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
								Public_Private_Mode_Code_Lenght = 6;
								eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
								Current_Lock_State = Public_Lock_Code_Entry;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
								Who_Unlocked = Nobody;
								Motor_Open_Close_Lock(Open,0);
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Change_Technician_Code:
							if ((Who_Unlocked == Master)||(Who_Unlocked == Technician))
							{
								Current_Lock_State = Set_Change_Technician_Code;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Delete_Technician_Code://13
							//if ((Who_Unlocked == Master) && ((Previous_Lock_Mode == Public_Mode)||(Previous_Lock_Mode == RAS_RTC_Mode)))
							if (Who_Unlocked == Master)
							{
								Techniciancode[0] = 11;
								Techniciancode[1] = 11;
								Techniciancode[2] = 11;
								Techniciancode[3] = 11;
								Techniciancode[4] = 11;
								Techniciancode[5] = 11;
								eeprom_write_block(&Techniciancode,(void*)EEPROM_Techniciancode_0,6);
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Audio_ON://14
							if (Who_Unlocked == Master)
							{
								Audio = Audio_ON;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Audio,Audio);
								Who_Unlocked = Nobody;
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Audio_OFF://15
							if (Who_Unlocked == Master)
							{
								Audio = Audio_OFF;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Audio,Audio);
								Who_Unlocked = Nobody;
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Double_User_Code_Entry_Mode://
							if ((Who_Unlocked == Master) && (Previous_Lock_Mode == Public_Mode))
							{
								Required_Number_Of_New_Code_Entires = 2;
								eeprom_write_byte((uint8_t*)EEPROM_Required_Number_Of_New_Code_Entires,Required_Number_Of_New_Code_Entires);
								Who_Unlocked = Nobody;
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Flash_Green_LED(2);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Single_User_Code_Entry_Mode://17
							if ((Who_Unlocked == Master) && (Previous_Lock_Mode == Public_Mode))
							{
								Required_Number_Of_New_Code_Entires = 1;
								eeprom_write_byte((uint8_t*)EEPROM_Required_Number_Of_New_Code_Entires,Required_Number_Of_New_Code_Entires);
								Who_Unlocked = Nobody;
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Unsuccessful_Code_Lockout_ON://18
							if (Who_Unlocked == Master)
							{
								Unsuccessful_Code_Lockout_Flag = Lockout_ON;
								eeprom_write_byte((uint8_t*)EEPROM_Unsuccessful_Code_Lockout_Flag,Unsuccessful_Code_Lockout_Flag);
								Who_Unlocked = Nobody;
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Unsuccessful_Code_Lockout_OFF://19
							if (Who_Unlocked == Master)
							{
								Unsuccessful_Code_Lockout_Flag = Lockout_OFF;
								eeprom_write_byte((uint8_t*)EEPROM_Unsuccessful_Code_Lockout_Flag,Unsuccessful_Code_Lockout_Flag);
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Timer_Auto_Unlock_ON_And_Set_Time://16
							if ((Who_Unlocked == Master) && (Previous_Lock_Mode == Public_Mode))
							{
								Current_Lock_State = Set_Timer_Auto_Unlock_ON_And_Set_Time;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Set_Timer_Auto_Unlock_OFF://
							if ((Who_Unlocked == Master) && (Previous_Lock_Mode == Public_Mode))
							{
								Auto_Timed_Unlock_Flag = 0;
								eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,Auto_Timed_Unlock_Flag);
								Auto_Timed_Unlock_Time[0] = 0;
								Auto_Timed_Unlock_Time[1] = 0;
								eeprom_write_block(&Auto_Timed_Unlock_Time,(void*)EEPROM_Auto_Timed_Unlock_Time_0,2);
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;

						case Allow_User_To_Change_Own_Code: //28
							if ((Who_Unlocked == Master) && ((Previous_Lock_Mode == Private_Mode) || (Previous_Lock_Mode == Multiuser_Mode)))
							{
								Allow_User_To_Change_Own_Code_Flag  = 1;
								eeprom_write_byte((uint8_t*)EEPROM_Allow_User_To_Change_Own_Code_Flag,Allow_User_To_Change_Own_Code_Flag);
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							Current_Lock_Mode = Previous_Lock_Mode;
							Current_Lock_State = Previous_Lock_State;
							break;

						case Disallow_User_To_Change_Own_Code: //29
							if ((Who_Unlocked == Master) && ((Previous_Lock_Mode == Private_Mode) || (Previous_Lock_Mode == Multiuser_Mode)))
							{
								Allow_User_To_Change_Own_Code_Flag  = 0;
								eeprom_write_byte((uint8_t*)EEPROM_Allow_User_To_Change_Own_Code_Flag,Allow_User_To_Change_Own_Code_Flag);
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							Current_Lock_Mode = Previous_Lock_Mode;
							Current_Lock_State = Previous_Lock_State;
							break;

						case Delete_All_User_Access_Codes:
							if (((Who_Unlocked == Master) || (Who_Unlocked == Submaster)) && (Previous_Lock_Mode == Multiuser_Mode))
							{
								Multiuser_Code[0] = 11;
								Multiuser_Code[1] = 11;
								Multiuser_Code[2] = 11;
								Multiuser_Code[3] = 11;
								for(Count_1=0;Count_1<Max_Allowed_Multiuser_Number;Count_1++)
								{
									memcpy((int*)MultiUsercode[Count_1],Multiuser_Code,4);
									eeprom_write_block(&MultiUsercode[Count_1],(void*)(EEPROM_Multiuser_0 + (Count_1*4)),4);
								}
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Green_LED(3);
							}
							else
							{
								Who_Unlocked = Nobody;
								Flash_Wrong(4);
							}
							Current_Lock_Mode = Previous_Lock_Mode;
							Current_Lock_State = Previous_Lock_State;
							break;

						case Set_Lock_Position:
							if (Who_Unlocked == Master)
								{
									Clear_Keycode();
									Current_Lock_State = Set_Lock_Position;
									Flash_Green_LED(3);
								}
								else
								{
									Current_Lock_Mode = Previous_Lock_Mode;
									Current_Lock_State = Previous_Lock_State;
									Who_Unlocked = Nobody;
									Clear_Keycode();
									Flash_Wrong(4);
								}
								break;

						case Set_RAS_Mode: //60
							if (Who_Unlocked == Master)
							{
								Erase_All_Time_Related_Funcions();
								Current_Lock_State = RAS_Serial_Code_Entry;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						
						case Set_Single_Use_RAS_Mode: //61
							if (Who_Unlocked == Master)
							{
								Current_Lock_State = RAS_Single_Use_Serial_Code_Entry;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;

						case Set_Time: //90
							if (Who_Unlocked == Master)
							{
								Clear_Keycode();
								Current_Lock_State = Set_Time_Mode;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;

						case Set_Date: //91
							if (Who_Unlocked == Master)
							{
								Clear_Keycode();
								Current_Lock_State = Set_Date_Mode;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							break;
						case Show_Time: //92
						if (Who_Unlocked == Master)
						{
							cli();
							PCF2123_Read_Register(3,2);
							SPI_Data_Buffer[0] = SPI_Data_Buffer[0] &0b01111111;
							Temp_1[1] = (SPI_Data_Buffer[1] & 0b00001111);			//extract hrs units
							Temp_1[0] = ((SPI_Data_Buffer[1] >> 4) & 0b0000111);	//extract hrs tens									// read mins and hrs
							Temp_1[3] = (SPI_Data_Buffer[0] & 0b00001111);			//extract minutes units
							Temp_1[2] = ((SPI_Data_Buffer[0] >> 4) & 0b0000111);	//extract minutes tens
							_delay_ms(1000)	;
							for(uint8_t sh_counter1=0; sh_counter1<4;sh_counter1++)
							{
								if (Temp_1[sh_counter1]>0)
								{
									for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
									{
										Buzzer(8);
										switch(sh_counter1)
										{
											case 0: // hrs dec
											CLEARBIT(LED_PORT,LED_G_1);
											CLEARBIT(LED_PORT,LED_R_1);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_G_1);
											SETBIT(LED_PORT,LED_R_1);
											_delay_ms(500);
											break;
											case 1:
											CLEARBIT(LED_PORT,LED_G_2);
											CLEARBIT(LED_PORT,LED_R_2);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_G_2);
											SETBIT(LED_PORT,LED_R_2);
											_delay_ms(500);
											break;
											case 2:
											CLEARBIT(LED_PORT,LED_G_3);
											CLEARBIT(LED_PORT,LED_R_3);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_G_3);
											SETBIT(LED_PORT,LED_R_3);
											_delay_ms(500);
											break;
											case 3:
											CLEARBIT(LED_PORT,LED_G_4);
											CLEARBIT(LED_PORT,LED_R_4);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_G_4);
											SETBIT(LED_PORT,LED_R_4);
											_delay_ms(500);
											break;
											default:
											break;
										}//switch(sh_counter1)
									}//for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
								} //if (Temp_1[sh_counter1]<0)
								else
								{
									switch(sh_counter1)
									{
										case 0: // hrs dec
										CLEARBIT(LED_PORT,LED_R_1);
										_delay_ms(250);
										SETBIT(LED_PORT,LED_R_1);
										_delay_ms(500);
										break;
										case 1:
										CLEARBIT(LED_PORT,LED_R_2);
										_delay_ms(250);
										SETBIT(LED_PORT,LED_R_2);
										_delay_ms(500);
										break;
										case 2:
										CLEARBIT(LED_PORT,LED_R_3);
										_delay_ms(250);
										SETBIT(LED_PORT,LED_R_3);
										_delay_ms(500);
										break;
										case 3:
										CLEARBIT(LED_PORT,LED_R_4);
										_delay_ms(250);
										SETBIT(LED_PORT,LED_R_4);
										_delay_ms(500);
										break;
										default:
										break;
									}//switch(sh_counter1)
								}// else if(Temp_1[sh_counter1]>0)

								_delay_ms(1000);
							}	//for(uint8_t sh_counter1 = 0; sh_counter1 <4;sh_counter1 ++)
							
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							
						}
						else
						{
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							Flash_Wrong(4);
						}
						Timeout_Counter = 1490;
						sei();
						break;
						case Show_Date: //43
							if ((Who_Unlocked == Master) || (Who_Unlocked == Submaster) || (Who_Unlocked == Technician))
							{
								cli();
								
								PCF2123_Read_Register(5,4);
								Temp_1[1] = (SPI_Data_Buffer[0] & 0b00001111);			//extract Stop_Days units
								Temp_1[0] = ((SPI_Data_Buffer[0] >> 4) & 0b0000111);	//extract Stop_Days tens
								Temp_1[6] = (SPI_Data_Buffer[1] & 0b00001111);			//extract weekStop_Days units
								//Temp_1[6] = ((SPI_Data_Buffer[1] >> 4) & 0b0000111);	//extract weekStop_Days tens
								Temp_1[3] = (SPI_Data_Buffer[2] & 0b00001111);			//extract Stop_Months units
								Temp_1[2] = ((SPI_Data_Buffer[2] >> 4) & 0b0000111);	//extract Stop_Months tens
								Temp_1[5] = (SPI_Data_Buffer[3] & 0b00001111);			//extract Stop_Years units
								Temp_1[4] = ((SPI_Data_Buffer[3] >> 4) & 0b0000111);	//extract Stop_Years tens
								_delay_ms(1000)	;
								for(uint8_t sh_counter1=0; sh_counter1<7;sh_counter1++)
								{
									if (Temp_1[sh_counter1]>0)
									{
										for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
										{
											Buzzer(8);
											switch(sh_counter1)
											{
												case 0: // hrs dec
												case 4:
													CLEARBIT(LED_PORT,LED_G_1);
													CLEARBIT(LED_PORT,LED_R_1);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_1);
													SETBIT(LED_PORT,LED_R_1);
													_delay_ms(500);
													break;
												case 1:
												case 5:
													CLEARBIT(LED_PORT,LED_G_2);
													CLEARBIT(LED_PORT,LED_R_2);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_2);
													SETBIT(LED_PORT,LED_R_2);
													_delay_ms(500);
													break;
												case 2:
												case 6:
													CLEARBIT(LED_PORT,LED_G_3);
													CLEARBIT(LED_PORT,LED_R_3);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_3);
													SETBIT(LED_PORT,LED_R_3);
													_delay_ms(500);
													break;
												case 3:
													CLEARBIT(LED_PORT,LED_G_4);
													CLEARBIT(LED_PORT,LED_R_4);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_4);
													SETBIT(LED_PORT,LED_R_4);
													_delay_ms(500);
													break;
												default:
													break;
											}//switch(sh_counter1)
										}//for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
									} //if (Temp_1[sh_counter1]<0)
									else
									{
										switch(sh_counter1)
										{
											case 0: // hrs dec
											case 4:
												CLEARBIT(LED_PORT,LED_R_1);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_1);
												_delay_ms(500);
												break;
											case 1:
											case 5:
												CLEARBIT(LED_PORT,LED_R_2);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_2);
												_delay_ms(500);
												break;
											case 2:
											case 6:
												CLEARBIT(LED_PORT,LED_R_3);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_3);
												_delay_ms(500);
												break;
											case 3:
											case 7:
												CLEARBIT(LED_PORT,LED_R_4);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_4);
												_delay_ms(500);
												break;
											default:
												break;
										}//switch(sh_counter1)
									}// else if(Temp_1[sh_counter1]>0)

									_delay_ms(1000);
								}	//for(uint8_t sh_counter1 = 0; sh_counter1 <4;sh_counter1 ++)
								
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								//Flash_Green_LED(3);
								
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							Timeout_Counter = 1490;
							sei();
							break;

						case Show_Timer_Auto_Unlock_Time: //21
							if ((Who_Unlocked == Master) || (Who_Unlocked == Submaster) || (Who_Unlocked == Technician))
							{
								cli();
								PCF2123_Read_Register(0x09,2);
								
								Temp_1[1] = (SPI_Data_Buffer[1] & 0b00001111);			//extract hrs units
								Temp_1[0] = ((SPI_Data_Buffer[1] >> 4) & 0b0000111);	//extract hrs tens									// read mins and hrs
								Temp_1[3] = (SPI_Data_Buffer[0] & 0b00001111);			//extract minutes units
								Temp_1[2] = ((SPI_Data_Buffer[0] >> 4) & 0b0000111);	//extract minutes tens
								PCF2123_Read_Register(0x01,1); //check control_1 register
								if ((SPI_Data_Buffer[0] & 0b00000010) != 0)
								{
									if ((EIMSK & 0b0000001) == 1) //if int0 is enabled
									{
										_delay_ms(1000);
										for(uint8_t sh_counter1=0; sh_counter1<4;sh_counter1++)
										{
											if (Temp_1[sh_counter1]>0)
											{
												for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
												{
													Buzzer(8);
													switch(sh_counter1)
													{
														case 0: // hrs dec
															CLEARBIT(LED_PORT,LED_G_1);
															CLEARBIT(LED_PORT,LED_R_1);
															_delay_ms(250);
															SETBIT(LED_PORT,LED_G_1);
															SETBIT(LED_PORT,LED_R_1);
															_delay_ms(500);
															break;
														case 1:
															CLEARBIT(LED_PORT,LED_G_2);
															CLEARBIT(LED_PORT,LED_R_2);
															_delay_ms(250);
															SETBIT(LED_PORT,LED_G_2);
															SETBIT(LED_PORT,LED_R_2);
															_delay_ms(500);
															break;
														case 2:
															CLEARBIT(LED_PORT,LED_G_3);
															CLEARBIT(LED_PORT,LED_R_3);
															_delay_ms(250);
															SETBIT(LED_PORT,LED_G_3);
															SETBIT(LED_PORT,LED_R_3);
															_delay_ms(500);
															break;
														case 3:
															CLEARBIT(LED_PORT,LED_G_4);
															CLEARBIT(LED_PORT,LED_R_4);
															_delay_ms(250);
															SETBIT(LED_PORT,LED_G_4);
															SETBIT(LED_PORT,LED_R_4);
															_delay_ms(500);
															break;
														default:
															break;
													}//switch(sh_counter1)
												}//for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
											} //if (Temp_1[sh_counter1]<0)
											else
											{
												switch(sh_counter1)
												{
													case 0: // hrs dec
														CLEARBIT(LED_PORT,LED_R_1);
														_delay_ms(250);
														SETBIT(LED_PORT,LED_R_1);
														_delay_ms(500);
														break;
													case 1:
														CLEARBIT(LED_PORT,LED_R_2);
														_delay_ms(250);
														SETBIT(LED_PORT,LED_R_2);
														_delay_ms(500);
														break;
													case 2:
														CLEARBIT(LED_PORT,LED_R_3);
														_delay_ms(250);
														SETBIT(LED_PORT,LED_R_3);
														_delay_ms(500);
														break;
													case 3:
														CLEARBIT(LED_PORT,LED_R_4);
														_delay_ms(250);
														SETBIT(LED_PORT,LED_R_4);
														_delay_ms(500);
														break;
													default:
														break;
												}//switch(sh_counter1)
											}// else if(Temp_1[sh_counter1]>0)

											_delay_ms(1000);
										}	//for(uint8_t sh_counter1 = 0; sh_counter1 <4;sh_counter1 ++)
										Current_Lock_State = Previous_Lock_State;
										Current_Lock_Mode = Previous_Lock_Mode;
										Who_Unlocked = Nobody;
										Clear_Keycode();
										
										//Flash_Green_LED(3);
									}
									else
									{
										Current_Lock_Mode = Previous_Lock_Mode;
										Current_Lock_State = Previous_Lock_State;
										Who_Unlocked = Nobody;
										Clear_Keycode();
										Flash_Wrong(4);
									}
								}
								else
								{
									Current_Lock_Mode = Previous_Lock_Mode;
									Current_Lock_State = Previous_Lock_State;
									Who_Unlocked = Nobody;
									Clear_Keycode();
									Flash_Wrong(4);
								}
								
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(4);
							}
							Timeout_Counter = 1490;
							sei();
							break;
						case Show_Lock_Active_Start_Time: //
						cli();
						Lock_Active_Enabled_Flag = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Flag);
						if ((Who_Unlocked == Master) && (Lock_Active_Enabled_Flag == 1))
						{
							
							Lock_Active_Start_Time_Hr = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr);
							Lock_Active_Start_Time_Min = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min);
							Temp_1[1] = (Lock_Active_Start_Time_Hr & 0b00001111);			//extract hrs units
							Temp_1[0] = ((Lock_Active_Start_Time_Hr >> 4) & 0b0000111);		//extract hrs tens									// read mins and hrs
							Temp_1[3] = (Lock_Active_Start_Time_Min & 0b00001111);			//extract minutes units
							Temp_1[2] = ((Lock_Active_Start_Time_Min >> 4) & 0b0000111);	//extract minutes tens
							_delay_ms(1000);
							for(uint8_t sh_counter1=0; sh_counter1<4;sh_counter1++)
							{
								if (Temp_1[sh_counter1]>0)
								{
									Buzzer(8);
									for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
									{
										Buzzer(8);
										switch(sh_counter1)
										{
											case 0: // hrs dec
												CLEARBIT(LED_PORT,LED_G_1);
												CLEARBIT(LED_PORT,LED_R_1);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_G_1);
												SETBIT(LED_PORT,LED_R_1);
												_delay_ms(500);
												break;
											case 1:
												CLEARBIT(LED_PORT,LED_G_2);
												CLEARBIT(LED_PORT,LED_R_2);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_G_2);
												SETBIT(LED_PORT,LED_R_2);
												_delay_ms(500);
												break;
											case 2:
												CLEARBIT(LED_PORT,LED_G_3);
												CLEARBIT(LED_PORT,LED_R_3);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_G_3);
												SETBIT(LED_PORT,LED_R_3);
												_delay_ms(500);
												break;
											case 3:
												CLEARBIT(LED_PORT,LED_G_4);
												CLEARBIT(LED_PORT,LED_R_4);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_G_4);
												SETBIT(LED_PORT,LED_R_4);
												_delay_ms(500);
												break;
											default:
												break;
										}//switch(sh_counter1)
									}//for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
								} //if (Temp_1[sh_counter1]<0)
								else
								{
									switch(sh_counter1)
									{
										case 0: // hrs dec
											CLEARBIT(LED_PORT,LED_R_1);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_R_1);
											_delay_ms(500);
											break;
										case 1:
											CLEARBIT(LED_PORT,LED_R_2);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_R_2);
											_delay_ms(500);
											break;
										case 2:
											CLEARBIT(LED_PORT,LED_R_3);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_R_3);
											_delay_ms(500);
											break;
										case 3:
											CLEARBIT(LED_PORT,LED_R_4);
											_delay_ms(250);
											SETBIT(LED_PORT,LED_R_4);
											_delay_ms(500);
											break;
										default:
											break;
									}//switch(sh_counter1)
								}// else if(Temp_1[sh_counter1]>0)
								_delay_ms(1000);
							}	//for(uint8_t sh_counter1 = 0; sh_counter1 <4;sh_counter1 ++)
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							
							//Flash_Green_LED(3);
							
						}
						else
						{
							Current_Lock_Mode = Previous_Lock_Mode;
							Current_Lock_State = Previous_Lock_State;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							_delay_ms(500);
							Flash_Wrong(4);
						}
						Timeout_Counter = 1490;
						sei();
						break;
						
						case Show_Lock_Active_Stop_Time:
							cli();
							Lock_Active_Enabled_Flag = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Flag);
							if ((Who_Unlocked == Master) && (Lock_Active_Enabled_Flag == 1))
							{
								Lock_Active_Stop_Time_Hr = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr);
								Lock_Active_Stop_Time_Min = eeprom_read_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min);
								Temp_1[1] = (Lock_Active_Stop_Time_Hr & 0b00001111);		//extract hrs units
								Temp_1[0] = ((Lock_Active_Stop_Time_Hr >> 4) & 0b0000111);	//extract hrs tens									// read mins and hrs
								Temp_1[3] = (Lock_Active_Stop_Time_Min & 0b00001111);		//extract minutes units
								Temp_1[2] = ((Lock_Active_Stop_Time_Min >> 4) & 0b0000111);	//extract minutes tens
								_delay_ms(1000)	;
								for(uint8_t sh_counter1=0; sh_counter1<4;sh_counter1++)
								{
									if (Temp_1[sh_counter1]>0)
									{
										for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
										{
											Buzzer(8);
											switch(sh_counter1)
											{
												case 0: // hrs dec
													CLEARBIT(LED_PORT,LED_G_1);
													CLEARBIT(LED_PORT,LED_R_1);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_1);
													SETBIT(LED_PORT,LED_R_1);
													_delay_ms(500);
													break;
												case 1:
													CLEARBIT(LED_PORT,LED_G_2);
													CLEARBIT(LED_PORT,LED_R_2);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_2);
													SETBIT(LED_PORT,LED_R_2);
													_delay_ms(500);
													break;
												case 2:
													CLEARBIT(LED_PORT,LED_G_3);
													CLEARBIT(LED_PORT,LED_R_3);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_3);
													SETBIT(LED_PORT,LED_R_3);
													_delay_ms(500);
													break;
												case 3:
													CLEARBIT(LED_PORT,LED_G_4);
													CLEARBIT(LED_PORT,LED_R_4);
													_delay_ms(250);
													SETBIT(LED_PORT,LED_G_4);
													SETBIT(LED_PORT,LED_R_4);
													_delay_ms(500);
													break;
												default:
													break;
											}//switch(sh_counter1)
										}//for (uint8_t sh_counter2 = 0; sh_counter2<Temp_1[sh_counter1];sh_counter2 ++)
									} //if (Temp_1[sh_counter1]<0)
									else
									{
										switch(sh_counter1)
										{
											case 0: // hrs dec
												CLEARBIT(LED_PORT,LED_R_1);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_1);
												_delay_ms(500);
												break;
											case 1:
												CLEARBIT(LED_PORT,LED_R_2);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_2);
												_delay_ms(500);
												break;
											case 2:
												CLEARBIT(LED_PORT,LED_R_3);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_3);
												_delay_ms(500);
												break;
											case 3:
												CLEARBIT(LED_PORT,LED_R_4);
												_delay_ms(250);
												SETBIT(LED_PORT,LED_R_4);
												_delay_ms(500);
												break;
											default:
												break;
										}//switch(sh_counter1)
									}// else if(Temp_1[sh_counter1]>0)
									_delay_ms(1000);
								}	//for(uint8_t sh_counter1 = 0; sh_counter1 <4;sh_counter1 ++)
								
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								//Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								_delay_ms(500);
								Flash_Wrong(4);
							}
							Timeout_Counter = 1490;
							sei();
							break;
						case Set_Lock_Active_And_Auto_Unlock: //95
						if ((Who_Unlocked == Master) && ((Previous_Lock_Mode == Private_Mode) || (Previous_Lock_Mode == Public_Mode) || (Previous_Lock_Mode == Multiuser_Mode)))
						{
							Clear_Keycode();
							Current_Lock_State = Set_Lock_Active_And_Unlock_Mode_Start;
							//Current_Lock_Mode = Previous_Lock_Mode;
							Flash_Green_LED(3);
						}
						else
						{
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Clear_Keycode();
							Who_Unlocked = Nobody;
							Flash_Wrong(4);
						}
						break;
						case Set_Lock_Active: //95
							if ((Who_Unlocked == Master) && ((Previous_Lock_Mode == Private_Mode) || (Previous_Lock_Mode == Public_Mode) || (Previous_Lock_Mode == Multiuser_Mode)))
							{
								Clear_Keycode();
								Current_Lock_State = Set_Lock_Active_Mode_Start;
								//Current_Lock_Mode = Previous_Lock_Mode;
								Flash_Green_LED(3);
							}
							else
							{
								Current_Lock_State = Previous_Lock_State;
								Current_Lock_Mode = Previous_Lock_Mode;
								Clear_Keycode();
								Who_Unlocked = Nobody;
								Flash_Wrong(4);
							}
							break;
						case Diasble_Lock_Active: //96
							Lock_Active_Enabled_Flag = eeprom_read_byte((uint8_t*)EEPROM_Lock_Deactivated_Prevoius_State);
							if ((Who_Unlocked == Master) && ((Previous_Lock_Mode == Private_Mode) || (Previous_Lock_Mode == Public_Mode) || (Previous_Lock_Mode == Multiuser_Mode)))		// if master and lock is deactivated
							{
								Erase_All_Time_Related_Funcions();
								Lock_Active_Enabled_Flag = 0;
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
								Lock_Active_Start_Time_Hr = 0;	// stored as BCD
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
								Lock_Active_Start_Time_Min = 0; // stored as BCD
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
								Lock_Active_Stop_Time_Hr = 0;	// stored as BCD
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
								Lock_Active_Stop_Time_Min = 0;	// stored as BCD
								eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
								//bellow is for removing auto open and lock the active with auto-open function
								PCF2123_Read_Register(0x01,1);
								SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b11111101; // Disable up an alarm interrupt and generate the pulse
								PCF2123_Write_Register(0x01,1);
								eeprom_write_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1, SPI_Data_Buffer[0]);
								eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0); // disable auto unlock
								EIMSK = EIMSK & ~(1<<INT0); //disable INT0
								eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,EIMSK);
								Flash_Green_LED(3);
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Who_Unlocked = Nobody;
							}
							else
							{
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Clear_Keycode();
								Who_Unlocked = Nobody;
								Flash_Wrong(4);
							}
							break;
						default:
								
								Current_Lock_Mode = Previous_Lock_Mode;
								Current_Lock_State = Previous_Lock_State;
								Who_Unlocked = Nobody;
								Clear_Keycode();
								Flash_Wrong(6);
								break;
						}//switch(Function_Number)
					}
					else
					{
						Current_Lock_Mode = Previous_Lock_Mode;
						Current_Lock_State = Previous_Lock_State;
						Who_Unlocked = Nobody;
						Clear_Keycode();
						Flash_Wrong(6);
					}
			}//if (Position == 3) // when the two digit fnt have been typed in
}

void Logo_Press_Function_Execute_01(void)// Change master code
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 8)
	{
	 	switch(Number_Of_New_Code_Entires)
	 	{
	 		case 0:
	 			if ((Who_Unlocked != Master) || (memcmp(Keycode,Usercode_4,4) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0) || (Compare_Code_Against_Multiuser() == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0) && (Keycode[4]==0) && (Keycode[5]==0) && (Keycode[6]==0) && (Keycode[7]==0)))//  if there is no match with any of the codes
	 			{// new code is the same like one of the existing codes
					Clear_Keycode();
					Current_Lock_Mode = Previous_Lock_Mode;
					Current_Lock_State = Previous_Lock_State;
					Who_Unlocked = Nobody;
	 				Number_Of_New_Code_Entires = 0;
	 				Flash_Wrong(2);
	 			}
	 			else
	 			{ // if the new code is different than the other codes
	 				if(Who_Unlocked == Master)
					{
						memcpy(Public_Mode_Code_Entry_1,Keycode,8);
	 					Number_Of_New_Code_Entires = 1;
						Clear_Keycode();
	 					Flash_Green_LED(3);
					}
	 			}
	 			break;
			case 1: //Second code type in
				memcpy(Public_Mode_Code_Entry_2,Keycode,8);
				if (memcmp(Public_Mode_Code_Entry_2,Public_Mode_Code_Entry_1,8) == 0)// if codes are the same
				{
					memcpy(Mastercode,Public_Mode_Code_Entry_2,8);
					eeprom_write_block(&Mastercode,(void*)EEPROM_Mastercode_0,8);
					Clear_Keycode();
					Current_Lock_Mode = Previous_Lock_Mode;
					Current_Lock_State = Previous_Lock_State;
					Who_Unlocked = Nobody;
					Clear_Keycode();
					Number_Of_New_Code_Entires = 0;
					Flash_Green_LED(3);
				}//if (Public_Mode_Code_Entry_1==Public_Mode_Code_Entry_2)// if codes are the same
				else
				{ // different codes
					Flash_Wrong(2);
					Clear_Keycode();
					Current_Lock_Mode = Previous_Lock_Mode;
					Current_Lock_State = Previous_Lock_State;
					Who_Unlocked = Nobody;
					Number_Of_New_Code_Entires = 0;
				}
				break;
			default:
				break;
			}//switch(Number_Of_New_Code_Entires)
		}
	}
void Logo_Press_Function_Execute_02(void)// Change user code
{
	Key_Press_Flash_LED(Position,1);
	if((Position == 4) && (Current_Lock_Mode == Admin_Mode) && (Public_Private_Mode_Code_Lenght == 4)) 
	{
		if ((memcmp(Keycode,Mastercode,4) == 0) || (memcmp(Keycode,Submastercode,4) == 0) || (memcmp(Keycode,Techniciancode,4) == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0)))
		{	
			Current_Lock_State = Previous_Lock_State;
			Current_Lock_Mode = Previous_Lock_Mode;
			Public_Private_Mode_Code_Lenght = eeprom_read_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght);
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Wrong(2);
		}
		else
		{ // if the new code is different than the other codes
			memcpy(Usercode_4,Keycode,4);
			eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
			Current_Lock_State = Private_Unlock_Code_Entry;
			Current_Lock_Mode = Private_Mode;
			eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
			eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Green_LED(3);
			Motor_Open_Close_Lock(Lock,0);
		}
	} //if((Position == 4) && (Current_Lock_Mode == Set_Single_User_Private_Mode_4))
	if((Position == 6) && (Current_Lock_Mode == Admin_Mode) && (Public_Private_Mode_Code_Lenght == 6))
	{
		if ((memcmp(Keycode,Mastercode,6) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0)&& (Keycode[4]==0) && (Keycode[5]==0)))
		{
			Current_Lock_State = Previous_Lock_State;
			Current_Lock_Mode = Previous_Lock_Mode;
			Public_Private_Mode_Code_Lenght = eeprom_read_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght);
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Wrong(2);
		}
		else
		{ // if the new code is different than the other codes
			memcpy(Usercode_6,Keycode,6);
			eeprom_write_block(&Usercode_6,(void*)EEPROM_Usercode_6_0,6);
			Current_Lock_State = Private_Unlock_Code_Entry;
			Current_Lock_Mode = Private_Mode;
			eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
			eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Green_LED(3);
			Motor_Open_Close_Lock(Lock,0);
		}
	} //if((Position == 6) && (Current_Lock_Mode == Set_Single_User_Private_Mode_6))
}
void Logo_Press_Function_Execute_03(void)// Set/Change submaster code
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 6)
	{
		switch(Who_Unlocked)
		{
			case Master:
				if ((memcmp(Keycode,Usercode_4,4) == 0) || (memcmp(Keycode,Usercode_6,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0)|| ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0) && (Keycode[4]==0) && (Keycode[5]==0)))
				{
					Current_Lock_State = Previous_Lock_State;
					Current_Lock_Mode = Previous_Lock_Mode;
					Who_Unlocked = Nobody;
					Clear_Keycode();
					Flash_Wrong(2);
				}
				else
				{ // if the new code is different than the other codes
					memcpy(Submastercode,Keycode,6);
					eeprom_write_block(&Submastercode,(void*)EEPROM_Submastercode_0,6);
					Current_Lock_State = Previous_Lock_State;
					Current_Lock_Mode = Previous_Lock_Mode;
					Who_Unlocked = Nobody;
					Clear_Keycode();
					Flash_Green_LED(3);
				}
				break;
////////////////////////////////////////////////////////////////
			case Submaster:
				switch(Number_Of_New_Code_Entires)
				{
					case 0:
						if ((memcmp(Keycode,Usercode_4,4) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Mastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0) || (Compare_Code_Against_Multiuser() == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0) && (Keycode[4]==0) && (Keycode[5]==0)))//  if there is no match with any of the codes
						{// new code is the same like one of the existing codes
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Clear_Keycode();
							Number_Of_New_Code_Entires = 0;
							Flash_Green_LED(3);
						}
						else
						{ // if the new code is different than the other codes
								memcpy(Public_Mode_Code_Entry_1,Keycode,6);
								Number_Of_New_Code_Entires = 1;
								Clear_Keycode();
								Flash_Wrong(2);
							}
						break;
					case 1: //Second code type in
						memcpy(Public_Mode_Code_Entry_2,Keycode,6);
						if (memcmp(Public_Mode_Code_Entry_2,Public_Mode_Code_Entry_1,6) == 0)// if codes are the same
						{
							memcpy(Submastercode,Public_Mode_Code_Entry_2,6);
							eeprom_write_block(&Submastercode,(void*)EEPROM_Submastercode_0,6);
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Who_Unlocked = Nobody;
							Clear_Keycode();
							Number_Of_New_Code_Entires = 0;
							Flash_Green_LED(3);
						}//if (Public_Mode_Code_Entry_1==Public_Mode_Code_Entry_2)// if codes are the same
						else
						{
							Who_Unlocked = Nobody;
							Current_Lock_State = Previous_Lock_State;
							Current_Lock_Mode = Previous_Lock_Mode;
							Clear_Keycode();
							Number_Of_New_Code_Entires = 0;
							Flash_Wrong(2);
						}
						break;
					default:
						break;
				}//switch(Number_Of_New_Code_Entires)
		}
	} //if(Positi
}

void Logo_Press_Function_Execute_05(void)// Add/Change user access code
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 2)
	{
		number_1 = Keycode[0];
		number_2 = Keycode[1];
		Multiuser_Location = (10*number_1) + number_2; // combine two numbers to point to position of multiuser table
		if (Multiuser_Location >= 50) //MULTIUSER location from 00 - 49
		{
				Current_Lock_State = Previous_Lock_State;
				Current_Lock_Mode = Previous_Lock_Mode;
				Who_Unlocked = Nobody;
				Clear_Keycode();
				Flash_Wrong(2);
		}
	}
	if(Position == 6)
	{
		if ((memcmp(Keycode,Mastercode,6) == 0) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0) || (Compare_Code_Against_Multiuser() == 0)|| ((Keycode[2]==0) && (Keycode[3]==0) && (Keycode[4]==0) && (Keycode[5]==0)))
		{
			Current_Lock_State = Previous_Lock_State;
			Current_Lock_Mode = Previous_Lock_Mode;
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Wrong(2);
		}
		else
		{ // if the new code is different than the other codes
			number_1 = Keycode[0];
			number_2 = Keycode[1];
			Multiuser_Location = (10*number_1) + number_2; // combine two numbers to point to position of multiuser table
			if (Multiuser_Location < 50)
			{
				for (uint8_t m=0;m<4;m++)
				{
					Multiuser_Code[m]=Keycode[m+2]; // shift remaining keycode[2-6] to Multiuser_Code[0-4]
				}
				memcpy(&MultiUsercode[Multiuser_Location],Multiuser_Code,4);
				eeprom_write_block(&MultiUsercode[Multiuser_Location],(void*)(EEPROM_Multiuser_0 + (Multiuser_Location*4)),4);
				Current_Lock_State = Previous_Lock_State;
				Current_Lock_Mode = Previous_Lock_Mode;
				Who_Unlocked = Nobody;
				Clear_Keycode();
				Flash_Green_LED(3);
			}
			else
			{
				Current_Lock_State = Previous_Lock_State;
				Current_Lock_Mode = Previous_Lock_Mode;
				Who_Unlocked = Nobody;
				Clear_Keycode();
				Flash_Wrong(2);
			}
			
		}
	} //if(Positi
}
void Logo_Press_Function_Execute_06(void) // Delete user access code
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 2)
	{
		number_1 = Keycode[0];
		number_2 = Keycode[1];
		Multiuser_Location = (10*number_1) + number_2; // combine two numbers to point to position of multiuser table
		Multiuser_Code[0] = 11;
		Multiuser_Code[1] = 11;
		Multiuser_Code[2] = 11;
		Multiuser_Code[3] = 11;
		memcpy(&MultiUsercode[Multiuser_Location],Multiuser_Code,4);
		eeprom_write_block(&MultiUsercode[Multiuser_Location],(void*)(EEPROM_Multiuser_0 + (Multiuser_Location*4)),4);
		Current_Lock_State = Previous_Lock_State;
		Current_Lock_Mode = Previous_Lock_Mode;
		Who_Unlocked = Nobody;
		Clear_Keycode();
		Flash_Green_LED(3);
	}
}
void Logo_Press_Function_Execute_12(void) // Set/change technician code
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 6)
	{
		if (((memcmp(Keycode,Usercode_4,4) == 0) && Public_Private_Mode_Code_Lenght == 4) ||((memcmp(Keycode,Usercode_6,6) == 0) && Public_Private_Mode_Code_Lenght == 6) || (memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Mastercode,6) == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0) && (Keycode[4]==0) && (Keycode[5]==0)))
		{
			Current_Lock_State = Previous_Lock_State;
			Current_Lock_Mode = Previous_Lock_Mode;
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Wrong(2);
		}
		else
		{ // if the new code is different than the other codes
			memcpy(Techniciancode,Keycode,6);
			eeprom_write_block(&Techniciancode,(void*)EEPROM_Techniciancode_0,6);
			Current_Lock_State = Previous_Lock_State;
			Current_Lock_Mode = Previous_Lock_Mode;
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Green_LED(3);
		}
	} //if(Position == 4)
}

void Logo_Press_Function_Execute_20(void) // Set timer auto unlock on and set the time
{
	if(Position == 4)
	{
		Auto_Timed_Unlock_Time[0] = (Keycode[0]<<4)|(Keycode[1]); //convert to BCD hrs
		Auto_Timed_Unlock_Time[1] = (Keycode[2]<<4)|(Keycode[3]); //convert to BCD mins
		eeprom_write_block(&Auto_Timed_Unlock_Time,(void*)EEPROM_Auto_Timed_Unlock_Time_0,2);
		Auto_Timed_Unlock_Flag = 1;
		eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,Auto_Timed_Unlock_Flag);
		Current_Lock_State = Previous_Lock_State;
		Who_Unlocked = Nobody;
		Clear_Keycode();
		Flash_Green_LED(3);
	}
	
}
/*
void Logo_Press_Function_Execute_24(void) // Set auto timed code cancel and set the time
{
	if(Position == 4)
	{
		Auto_Timed_Code_Cancel_Time[0]= (Keycode[0]<<4)|(Keycode[1]); //convert to BCD hrs
		Auto_Timed_Code_Cancel_Time[1]= (Keycode[2]<<4)|(Keycode[3]); //convert to BCD mins
		eeprom_write_block(&Auto_Timed_Code_Cancel_Time,(void*)EEPROM_Auto_Timed_Code_Cancel_Time_0,2);
		Auto_Timed_Code_Cancel_Flag = 1;
		eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Code_Cancel_Flag,Auto_Timed_Code_Cancel_Flag);
		Current_Lock_State = Previous_Lock_State;
		Current_Lock_Mode = Previous_Lock_Mode;
		Clear_Keycode();
		Flash_Green_LED(3);
	}
}
*/
void Logo_Press_Function_Execute_50(void) // Set Lock Position
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 1)
	{
		if((Keycode[0] == 0)||(Keycode[0] == 1)||(Keycode[0] == 2)||(Keycode[0] == 3))
		{
			Lock_Position = Keycode[0];
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Position,Lock_Position);
			Current_Lock_State = Previous_Lock_State;
			Current_Lock_Mode = Previous_Lock_Mode;
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Flash_Green_LED(3);
		}
		else
		{
			Who_Unlocked = Nobody;
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Flash_Wrong(3);
		}
	}
}

void Logo_Press_Function_Execute_60(void) // Enable RAS Mode
{
	Key_Press_Flash_LED(Position,1); // FLASH GREEN
	
	if(Position == 4)
	{
		memcpy(serialcode, Keycode,4);
		eeprom_write_block(&serialcode,(void*)EEPROM_serialcode_0,4);
		Usercode_4[0] = 1;
		Usercode_4[1] = 2;
		Usercode_4[2] = 3;
		Usercode_4[3] = 4;
		memcpy(initcode,Usercode_4,4);
		eeprom_write_block(&initcode,(void*)EEPROM_initcode_0,4);
		for (uint8_t count_60=0; count_60 <=32;count_60++)
		{
			newcode();
			memcpy(Usercode_4,nextcode,4);
		}
		newcode();
		Usercode_4[0] = 1;
		Usercode_4[1] = 2;
		Usercode_4[2] = 3;
		Usercode_4[3] = 4;
		eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
		Current_Lock_State = RAS_Unlock_Code_Entry;
		Current_Lock_Mode = RAS_Mode;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
		Erase_All_Time_Related_Funcions();
		Who_Unlocked = Nobody;
		Clear_Keycode();
		Flash_Green_LED(3);
		Motor_Open_Close_Lock(Lock,0);
	}
}

void Logo_Press_Function_Execute_61(void) // Enable RAS Mode
{
	Key_Press_Flash_LED(Position,1); // FLASH GREEN
	
	if(Position == 4)
	{
		memcpy(serialcode, Keycode,4);
		eeprom_write_block(&serialcode,(void*)EEPROM_serialcode_0,4);
		Usercode_4[0] = 1;
		Usercode_4[1] = 2;
		Usercode_4[2] = 3;
		Usercode_4[3] = 4;
		memcpy(initcode,Usercode_4,4);
		eeprom_write_block(&initcode,(void*)EEPROM_initcode_0,4);
		for (uint8_t count_61=0; count_61 <=32;count_61++)
		{
			newcode();
			memcpy(Usercode_4,nextcode,4);
		}
		newcode();
		Usercode_4[0] = 1;
		Usercode_4[1] = 2;
		Usercode_4[2] = 3;
		Usercode_4[3] = 4;
		eeprom_write_block(&Usercode_4,(void*)EEPROM_Usercode_4_0,4);
		Current_Lock_State = RAS_Unlock_Code_Entry;
		Current_Lock_Mode = RAS_Single_Use_Mode;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
		Erase_All_Time_Related_Funcions();
		Who_Unlocked = Nobody;
		Clear_Keycode();
		Flash_Green_LED(3);
		Motor_Open_Close_Lock(Lock,0);
	}
}

void Logo_Press_Function_Execute_66(void) //Reset Lock to Default Settings
{
	Key_Press_Flash_LED(Position,2);
	if(Position == 8)
	{
		if ((Keycode[0]==9) && (Keycode[1]==8) && (Keycode[2]==7) && (Keycode[3]== 6) && (Keycode[4]== 0) && (Keycode[5]==1 ) && (Keycode[6]==2) && (Keycode[7]==3))
		{ 
			Flash_Wrong(20);
			eeprom_write_byte((uint8_t*)EEPROM_Fist_Time_Run_1_Flag,0); // indicate first time run hasn't been done
			eeprom_write_byte((uint8_t*)EEPROM_Fist_Time_Run_2_Flag,0); // indicate first time run hasn't been done
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Flash_Green_LED(3);
		}
		else
		{
			Who_Unlocked = Nobody;
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Flash_Wrong(3);
		}
	}
}

void Logo_Press_Function_Execute_16(void)
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 4) // flash orange to indicate 1st time entry
	{
		if (((Keycode[0]>=0) && (Keycode[0]<3)) && ((Keycode[2]>=0) && (Keycode[2]<6))) // check whether mins and hrs are within limits
		{ // are within limits
			Flash_Green_LED(3);
		}
		else
		{ // not within limits
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Temp_1[0] = 0;
			Temp_1[1] = 0;
			Temp_1[2] = 0;
			Temp_1[3] = 0;
			Temp_1[4] = 0;
			SPI_Data_Buffer[0] = 0;
			SPI_Data_Buffer[1] = 0;
			SPI_Data_Buffer[2] = 0;
			SPI_Data_Buffer[3] = 0;
			SPI_Data_Buffer[4] = 0;
			Flash_Wrong(3);
		}
	}
	else if(Position == 8) //Keycode[0] is a hr dec, Keycode[1] is a hr units, Keycode[2] is a min dec, Keycode[3] is a min units
	{
		SPI_Data_Buffer[0] = (((Keycode[2])<<4)|(Keycode[3]));	// Min
		SPI_Data_Buffer[1] = (((Keycode[0])<<4)|(Keycode[1]));	// Hrs
		SPI_Data_Buffer[2] = (((Keycode[6])<<4)|(Keycode[7]));	// Min
		SPI_Data_Buffer[3] = (((Keycode[4])<<4)|(Keycode[5]));	// Hrs
		if ((SPI_Data_Buffer[0] == SPI_Data_Buffer[2]) && (SPI_Data_Buffer[1] == SPI_Data_Buffer[3]))
		{ // check whether second entered time matches the first one
			//SPI_Data_Buffer[0] = 0;				//sets seconds to 0 and re-sets the OS flag (bit 7) Set seconds to zero
			// disable lock active time
			eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0);
			Lock_Active_Enabled_Flag = 0; // Enable Lock Inactive
			// 
			PCF2123_Write_Register(0x09,2);		// send from the SPI_Data_Buffer[0] SPI_Data_Buffer[1] 
			eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0,SPI_Data_Buffer[0]);//mins
			eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1,SPI_Data_Buffer[1]);//hrs
			eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,1); // enable auto unlock
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,0); //disable time dependent lock active feature
			Lock_Active_Start_Time_Hr = 0x00;
			Lock_Active_Start_Time_Min = 0x00;
			Lock_Active_Stop_Time_Hr = 0x00;
			Lock_Active_Stop_Time_Min = 0x00;
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
			PCF2123_Read_Register(0x00,2);
			SPI_Data_Buffer[0] = 0b00000010; // set up an alarm interrupt and generate the pulse
			PCF2123_Write_Register(0x01,1);
			eeprom_write_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1, SPI_Data_Buffer[0]);
			//eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
			EIMSK = EIMSK | (1<<INT0); //enable INT0
			eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,EIMSK);
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Flash_Green_LED(3);
		}
		else
		{ // if the codes are different flash an error
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Flash_Wrong(2);
		}
		Who_Unlocked = Nobody;
		Clear_Keycode();
		SPI_Data_Buffer[0] = 0;
		SPI_Data_Buffer[1] = 0;
		SPI_Data_Buffer[2] = 0;
		SPI_Data_Buffer[3] = 0;
		SPI_Data_Buffer[4] = 0;
		PCF2123_Read_Register(0x09,2);
		SPI_Data_Buffer[0] = 0;
		SPI_Data_Buffer[1] = 0;
		SPI_Data_Buffer[2] = 0;
		SPI_Data_Buffer[3] = 0;
		SPI_Data_Buffer[4] = 0;
		PCF2123_Read_Register(0x00,2);
	}
}

void Logo_Press_Function_Execute_90(void) // Set Time
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 4) // flash orange to indicate 1st time entry
	{
		if (((Keycode[0]>=0) && (Keycode[0]<3)) && ((Keycode[2]>=0) && (Keycode[2]<6))) // check whether mins and hrs are within limits
		{ // are within limits
			Flash_Green_LED(3);
		} 
		else
		{ // not within limits
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Temp_1[0] = 0;
			Temp_1[1] = 0;
			Temp_1[2] = 0;
			Temp_1[3] = 0;
			Temp_1[4] = 0;
			SPI_Data_Buffer[0] = 0;
			SPI_Data_Buffer[1] = 0;
			SPI_Data_Buffer[2] = 0;
			SPI_Data_Buffer[3] = 0;
			SPI_Data_Buffer[4] = 0;
			Flash_Wrong(3);
		}
	}
	else if(Position == 8) //Keycode[0] is a hr dec, Keycode[1] is a hr units, Keycode[2] is a min dec, Keycode[3] is a min units
	{
		SPI_Data_Buffer[1] = (((Keycode[2])<<4)|(Keycode[3]));	// Min
		SPI_Data_Buffer[2] = (((Keycode[0])<<4)|(Keycode[1]));	// Hrs
		SPI_Data_Buffer[3] = (((Keycode[6])<<4)|(Keycode[7]));	// Min
		SPI_Data_Buffer[4] = (((Keycode[4])<<4)|(Keycode[5]));	// Hrs
		if ((SPI_Data_Buffer[1] == SPI_Data_Buffer[3]) && (SPI_Data_Buffer[2] == SPI_Data_Buffer[4]))
		{ // check whether second entered time matches the first one
			SPI_Data_Buffer[0] = 0;				//sets seconds to 0 and re-sets the OS flag (bit 7) Set seconds to zero
			PCF2123_Write_Register(2,3);		// send from the SPI_Data_Buffer[0] SPI_Data_Buffer[1] SPI_Data_Buffer[2]
			PCF2123_Read_Register(0x00,12); //for debugging
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Flash_Green_LED(3);
		}
		else
		{ // if the codes are different flash an error 
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Flash_Wrong(2);
		}
		Who_Unlocked = Nobody;
		Clear_Keycode();
		SPI_Data_Buffer[0] = 0;
		SPI_Data_Buffer[1] = 0;
		SPI_Data_Buffer[2] = 0;
		SPI_Data_Buffer[3] = 0;
		SPI_Data_Buffer[4] = 0;
	}
}

void Logo_Press_Function_Execute_91(void) // Set Date
{
	Key_Press_Flash_LED(Position,1);
	if (Position == 7)
	{
		if (((Keycode[0]>=0) && (Keycode[0]<4)) && ((Keycode[2]>=0) && (Keycode[2]<2))) // check whether Stop_Days, Stop_Months and yr are within limits
		{ // are within limits
			SPI_Data_Buffer[0] = (((Keycode[0])<<4)|(Keycode[1]));	// Stop_Day
			SPI_Data_Buffer[2] = (((Keycode[2])<<4)|(Keycode[3]));	// Stop_Month
			SPI_Data_Buffer[1] = Keycode[6];						// wkdStop_Day
			SPI_Data_Buffer[3] = (((Keycode[4])<<4)|(Keycode[5]));	//yr
			Flash_Green_LED(1);
		}
		else
		{ // not within limits
			
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			SPI_Data_Buffer[0] = 0;
			SPI_Data_Buffer[1] = 0;
			SPI_Data_Buffer[2] = 0;
			SPI_Data_Buffer[3] = 0;
			SPI_Data_Buffer[4] = 0;
			SPI_Data_Buffer[5] = 0;
			SPI_Data_Buffer[6] = 0;
			SPI_Data_Buffer[7] = 0;
			Flash_Wrong(2);
		}
		Timeout_Counter = 0;
	}
	else
	{
		if(Position == 14) //Keycode[0] is a Stop_Day dec, Keycode[1] is a Stop_Day units, Keycode[2] is a Stop_Month dec, Keycode[4] is a Stop_Month units, Keycode[5] is a yr dec, Keycode[5] is a yr units,Keycode[5] is a wkStop_Day
		{
			SPI_Data_Buffer[4] = (((Keycode[7])<<4)|(Keycode[8]));	// Stop_Day
			SPI_Data_Buffer[6] = (((Keycode[9])<<4)|(Keycode[10]));	// Stop_Month
			SPI_Data_Buffer[5] = Keycode[13];								// wkdStop_Day
			SPI_Data_Buffer[7] = (((Keycode[11])<<4)|(Keycode[12]));	//yr
			if ((SPI_Data_Buffer[0] == SPI_Data_Buffer[4]) && (SPI_Data_Buffer[1] == SPI_Data_Buffer[5]) && (SPI_Data_Buffer[2] == SPI_Data_Buffer[6]) && (SPI_Data_Buffer[3] == SPI_Data_Buffer[7]))
			{
				Who_Unlocked = Nobody;
				Current_Lock_Mode = Previous_Lock_Mode;
				Current_Lock_State = Previous_Lock_State;
				PCF2123_Write_Register(5,4);
				Flash_Green_LED(3);
			} 
			else
			{
				Who_Unlocked = Nobody;
				Current_Lock_Mode = Previous_Lock_Mode;
				Current_Lock_State = Previous_Lock_State;
				Flash_Wrong(2);
			}
		
			Clear_Keycode();
			SPI_Data_Buffer[0] = 0;
			SPI_Data_Buffer[1] = 0;
			SPI_Data_Buffer[2] = 0;
			SPI_Data_Buffer[3] = 0;
			SPI_Data_Buffer[4] = 0;
			SPI_Data_Buffer[5] = 0;
			SPI_Data_Buffer[6] = 0;
			SPI_Data_Buffer[7] = 0;
		}
	}
}

void Logo_Press_Function_Execute_95(void) // Set lock inactive Start Time
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 4) // flash orange to indicate 1st time entry
	{
		if (((Keycode[0]>=0) && (Keycode[0]<3)) && ((Keycode[2]>=0) && (Keycode[2]<6))) // check whether mins and hrs are within limits
		{ // are within limits
			Flash_Green_LED(3);
		}
		else
		{ // not within limits
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Temp_1[0] = 0;
			Temp_1[1] = 0;
			Temp_1[2] = 0;
			Temp_1[3] = 0;
			Temp_1[4] = 0;
			Flash_Wrong(3);
		}
	}
	else if(Position == 8) //Keycode[0] is a hr dec, Keycode[1] is a hr units, Keycode[2] is a min dec, Keycode[3] is a min units
	{
		Temp_1[0] = (((Keycode[2])<<4)|(Keycode[3]));	// Min
		Temp_1[1] = (((Keycode[0])<<4)|(Keycode[1]));	// Hrs
		Temp_1[2] = (((Keycode[6])<<4)|(Keycode[7]));	// Min
		Temp_1[3] = (((Keycode[4])<<4)|(Keycode[5]));	// Hrs
		if ((Temp_1[0] == Temp_1[2]) && (Temp_1[1] == Temp_1[3]))
		{ // check whether the second entered time matches the first one
			if (Current_Lock_State == Set_Lock_Active_Mode_Start)
			{
				Lock_Active_Start_Time_Hr_Temp = Temp_1[1];		// store  hrs temporarily 
				Lock_Active_Start_Time_Min_Temp = Temp_1[0];	// store mins temporarily
				Flash_Green_LED(3);
				Current_Lock_State = Set_Lock_Active_Mode_Stop;
			} 
			else if(Current_Lock_State == Set_Lock_Active_Mode_Stop)
			{
				Lock_Active_Enabled_Flag = 1; // Enable Lock Inactive
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
				Lock_Active_Start_Time_Hr = Lock_Active_Start_Time_Hr_Temp;	// set hrs
				Lock_Active_Start_Time_Min = Lock_Active_Start_Time_Min_Temp;	// set mins
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
				Lock_Active_Stop_Time_Hr = Temp_1[1];	// set hrs
				Lock_Active_Stop_Time_Min = Temp_1[0];	// set mins
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
				eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0); 
				Who_Unlocked = Nobody;
				Clear_Keycode();
				Current_Lock_Mode = Previous_Lock_Mode;
				Current_Lock_State = Previous_Lock_State;
				Flash_Green_LED(3);
			}
		}
		else
		{ // if the codes are different flash an error
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Lock_Active_Enabled_Flag = 0; // Don't enable lock active checks
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Flash_Wrong(2);
		}
		Temp_1[0] = 0;
		Temp_1[1] = 0;
		Temp_1[2] = 0;
		Temp_1[3] = 0;
	}
}

void Logo_Press_Function_Execute_96(void) // Set lock inactive Start Time
{
	cli();
	Key_Press_Flash_LED(Position,1);
	if(Position == 5) // flash orange to indicate 1st time entry
	{
		if (((Keycode[0]>=0) && (Keycode[0]<3)) && ((Keycode[2]>=0) && (Keycode[2]<6)) && ((Keycode[4]>=0) && (Keycode[4]<3))) // check whether mins and hrs are within limits
		{ // are within limits
			Flash_Green_LED(3);
		}
		else
		{ // not within limits
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Clear_Keycode();
			Who_Unlocked = Nobody;
			Temp_1[0] = 0;
			Temp_1[1] = 0;
			Temp_1[2] = 0;
			Temp_1[3] = 0;
			Temp_1[4] = 0;
			Flash_Wrong(3);
		}
	}
	else if(Position == 10) //Keycode[0] is a hr dec, Keycode[1] is a hr units, Keycode[2] is a min dec, Keycode[3] is a min units
	{
		Temp_1[0] = (((Keycode[2])<<4)|(Keycode[3]));	// Min
		Temp_1[1] = (((Keycode[0])<<4)|(Keycode[1]));	// Hrs
		Temp_1[2] = (((Keycode[7])<<4)|(Keycode[8]));	// Min
		Temp_1[3] = (((Keycode[5])<<4)|(Keycode[6]));	// Hrs

		if ((Temp_1[0] == Temp_1[2]) && (Temp_1[1] == Temp_1[3]) && (Keycode[4] == Keycode[9]))
		{ // check whether the second entered time matches the first one
			if (Current_Lock_State == Set_Lock_Active_And_Unlock_Mode_Start)
			{
				Lock_Active_Start_Time_Hr_Temp = Temp_1[1];		// store  hrs temporarily
				Lock_Active_Start_Time_Min_Temp = Temp_1[0];	// store mins temporarily
				Flash_Green_LED(3);
				Current_Lock_State = Set_Lock_Active_And_Unlock_Mode_Stop;
			}
			else if(Current_Lock_State == Set_Lock_Active_And_Unlock_Mode_Stop)
			{
				Lock_Active_Enabled_Flag = 1; // Enable Lock Inactive
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
				Lock_Active_Start_Time_Hr = Lock_Active_Start_Time_Hr_Temp;	// set hrs
				Lock_Active_Start_Time_Min = Lock_Active_Start_Time_Min_Temp;	// set mins
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
				Lock_Active_Stop_Time_Hr = Temp_1[1];	// set hrs
				Lock_Active_Stop_Time_Min = Temp_1[0];	// set mins
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
				eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
				switch(Keycode[4])
					{
						case 0: // no auto open
							SPI_Data_Buffer[0] = 0;
							SPI_Data_Buffer[1] = 0;
							break;
						case 1: // auto open on start active
							SPI_Data_Buffer[0] = Lock_Active_Start_Time_Min;
							SPI_Data_Buffer[1] = Lock_Active_Start_Time_Hr;

							break;
						case 2: // auto active on stop auto-active
							SPI_Data_Buffer[0] = Lock_Active_Stop_Time_Min;
							SPI_Data_Buffer[1] = Lock_Active_Stop_Time_Hr;
							break;
						default:
							break;
					}
					eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0,SPI_Data_Buffer[0]);//mins
					eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1,SPI_Data_Buffer[1]);//hrs
					PCF2123_Write_Register(0x09,2);		// send from the SPI_Data_Buffer[0] SPI_Data_Buffer[1]
					PCF2123_Read_Register(0x09,2);
					PCF2123_Read_Register(0x01,1);
					switch(Keycode[4])
					{
						case 0: // no auto open
							SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b11111101; // Disable up an alarm interrupt and generate the pulse
							EIMSK = EIMSK & ~(1<<INT0); //disable INT0
							eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0); // disable auto unlock
							break;
						case 1:
						case 2:
							SPI_Data_Buffer[0] = SPI_Data_Buffer[0] | 0b00000010; // set up an alarm interrupt and generate the pulse
							EIMSK = EIMSK | (1<<INT0); //enable INT0
							eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,1); // enable auto unlock
							break;
					}
					PCF2123_Write_Register(0x01,1);
					eeprom_write_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1, SPI_Data_Buffer[0]);
					eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,EIMSK);
				Who_Unlocked = Nobody;
				Clear_Keycode();
				Current_Lock_Mode = Previous_Lock_Mode;
				Current_Lock_State = Previous_Lock_State;
				Flash_Green_LED(3);
			}
		}
		else
		{ // if the codes are different flash an error
			Who_Unlocked = Nobody;
			Clear_Keycode();
			Lock_Active_Enabled_Flag = 0; // Don't enable lock active checks
			eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
			Current_Lock_Mode = Previous_Lock_Mode;
			Current_Lock_State = Previous_Lock_State;
			Flash_Wrong(2);
		}
		Temp_1[0] = 0;
		Temp_1[1] = 0;
		Temp_1[2] = 0;
		Temp_1[3] = 0;
	}
	sei();
}

void RAS_RTC_Serial_Number_Entry(void)
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 4)
	{
		memcpy(serialcode, Keycode,4);
		eeprom_write_block(&serialcode,(void*)EEPROM_serialcode_0,4);
		Current_Lock_State = RAS_RTC_Date_Code_Entry;
		Current_Lock_Mode = RAS_RTC_Mode;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
		Public_Private_Mode_Code_Lenght = 4;
		eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
		Lock_Active_Enabled_Flag = 0; // Don't enable lock active checks
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
		Erase_All_Time_Related_Funcions();
		Who_Unlocked = Nobody;
		Clear_Keycode();
		Motor_Open_Close_Lock(Open,0);
		Timeout_Counter=1000;
		Flash_Green_LED(3);
	}
}

void RAS_RTC_SS_Serial_Number_Entry(void)
{
	Key_Press_Flash_LED(Position,1);
	if(Position == 4)
	{
		memcpy(serialcode, Keycode,4);
		eeprom_write_block(&serialcode,(void*)EEPROM_serialcode_0,4);
		Current_Lock_State = RAS_RTC_SS_Date_Code_Entry;
		Current_Lock_Mode = RAS_RTC_SS_Mode;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Mode,Current_Lock_Mode);
		Public_Private_Mode_Code_Lenght = 4;
		eeprom_write_byte((uint8_t*)EEPROM_Public_Private_Mode_Code_Lenght,Public_Private_Mode_Code_Lenght);
		Lock_Active_Enabled_Flag = 0; // Don't enable lock active checks
		eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
		eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0);
		Erase_All_Time_Related_Funcions();
		Who_Unlocked = Nobody;
		Clear_Keycode();
		Motor_Open_Close_Lock(Open,0);
		Timeout_Counter=1000;
		Flash_Green_LED(3);
	}
}
uint8_t BCD_to_Dec (uint8_t number_to_convert)
{
	uint8_t Result;
	Result = (((number_to_convert & 0xF0 )>>4 ) * 10) + (number_to_convert & 0x0F);
	return Result;
}
uint8_t RAS_RTC_Check_Lock_Active(void)
{
	PCF2123_Read_Register(3,6);								// Reads minutes and hrs
		Current_Minute = SPI_Data_Buffer[0] & 0b01111111;	// 7th bit of min can have 1 or 0 value so mask it off
		Current_Hour = SPI_Data_Buffer[1] & 0b00111111;
		Current_Day = SPI_Data_Buffer[2] & 0b00111111;
		Current_Month = SPI_Data_Buffer[4] & 0b00011111;
		Current_Year = SPI_Data_Buffer[5];
		// convert to decimal
		Current_Minute = BCD_to_Dec(Current_Minute);
		Current_Hour = BCD_to_Dec(Current_Hour);
		Current_Day = BCD_to_Dec(Current_Day);
		Current_Month = BCD_to_Dec(Current_Month);
		Current_Year = BCD_to_Dec(Current_Year & 0x0F);
		Stop_Hour = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Hour);
		Stop_Day = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Day);
		Stop_Month = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Month);
		Stop_Year = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Year);
		if(Current_Year > Stop_Year) // complex case because of 2019/2020 Stop_Year overflow
		{
			if ((Current_Year == 9) && (Stop_Year == 0)) //2019/2020 Stop_Year overflow
			{
				return 1;
			} 
			else
			{
				return 0;
			}
		}
		else if(Current_Year == Stop_Year)
		{
			if (Current_Month > Stop_Month)
			{
				return 0;
			}
			else if (Current_Month == Stop_Month)
			{
					if (Current_Day > Stop_Day)
					{
						return 0;
					}
					else if (Current_Day == Stop_Day)
					{
						if (Current_Hour >= Stop_Hour)
						{
							return 0;
						}
						else
						{
							return 1;
						}
					}
					else
					{
						return 1;
					}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
}


void RAS_RTC_SS_Mode_Fnc(void)
{
	if (Current_Lock_Mode == RAS_RTC_SS_Mode)
	{
		switch (Current_Lock_State)
		{
			case RAS_RTC_SS_Date_Code_Entry:
				RAS_RTC_SS_Date_Code_Enter();
				break;
			case RAS_RTC_SS_User_Code_Entry:
				RAS_RTC_SS_User_Code_Enter();
				break;
			case RAS_RTC_SS_Unlock_Code_Entry:
				RAS_RTC_SS_Check_Code();
				break;
			default:
				break;
		}
	}
}

void RAS_RTC_SS_User_Code_Enter(void)
{
	Key_Press_Flash_LED(Position,1);
	if ((Public_Private_Mode_Code_Lenght == 4) && (Position == 4))
	{
		switch(Number_Of_New_Code_Entires)
		{
			case 0:
				if ((memcmp(Keycode,Mastercode,4) == 0) || (memcmp(Keycode,Submastercode,4) == 0) || (memcmp(Keycode,Techniciancode,4) == 0) || ((Keycode[0]==0) && (Keycode[1]==0) && (Keycode[2]==0) && (Keycode[3]==0)))
				{
					// new code is the same like one of the existing codes
					Clear_Keycode();
					Number_Of_New_Code_Entires = 0;
					Flash_Wrong(1);
				}
				else
				{ // if the new code is different than the other codes
					memcpy(Public_Mode_Code_Entry_1,Keycode,4);
					Number_Of_New_Code_Entires = 1;
					Clear_Keycode();
					Flash_Green_LED(1);
				}
				break;
			case 1: //Second code type in
				memcpy(Public_Mode_Code_Entry_2,Keycode,4);
				if (memcmp(Public_Mode_Code_Entry_2,Public_Mode_Code_Entry_1,4) == 0)// if codes are the same
				{
					memcpy(Usercode_RAS_RTC_4,Keycode,4);
					eeprom_write_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0,4);
					Current_Lock_State = RAS_RTC_SS_Unlock_Code_Entry;
					eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
					Clear_Keycode();
					Who_Unlocked = Nobody;
					Number_Of_New_Code_Entires = 0;
					Lock_Active_Enabled_Flag = 0;
					eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
					eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0);
					Motor_Open_Close_Lock(Open,0);
					Flash_Green_LED(3);
					Motor_Open_Close_Lock(Lock,0);
				}//if (Public_Mode_Code_Entry_1==Public_Mode_Code_Entry_2)// if codes are the same
				else
				{ // different codes
					Flash_Wrong(2);
					Clear_Keycode();
					Number_Of_New_Code_Entires = 0;
				}
				break;
			default:
				break;
		}//switch(Number_Of_New_Code_Entires)
	}
}


void RAS_RTC_Check_Code(void)
{
	Lock_Active_Flag = RAS_RTC_Check_Lock_Active(); //Returns 1 when current time is smaller than set time
	if (Lock_Active_Flag == 1)
	{
		Key_Press_Flash_LED(Position,1);
		if ((Public_Private_Mode_Code_Lenght == 4) && (Position == 4))
		{
			eeprom_read_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0 ,4);
			if ((memcmp(Keycode,Usercode_RAS_RTC_4,4) == 0) && ((Keycode[0]!=0)&&(Keycode[1]!=0)&&(Keycode[2]!=0)&&(Keycode[3]!=0)))
			{	// open the lock, beep, flash LEDs
				Who_Unlocked = User_4;
				Private_Mode_Unlock(1);
			}
			else
			{
				if (((memcmp(Keycode,Techniciancode,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0)) || ((Keycode[0]==0)&&(Keycode[1]==0)&&(Keycode[2]==0)&&(Keycode[3]==0)))//  if there is no match with any of the codes
				{//flag the alarm
					Who_Unlocked = Nobody;
					Clear_Keycode();
					Public_Private_Mode_Wrong_Code_Process();
				}
			}
		}
		if ((Who_Unlocked == Nobody) && (Position >=6))// there was no match with any of the codes before
		{
			if(Position == 8)
			{
				if ((memcmp(Keycode,Mastercode,8) == 0) && ((Keycode[0]!=0)&&(Keycode[1]!=0)&&(Keycode[2]!=0)&&(Keycode[3]!=0)&&(Keycode[4]!=0)&&(Keycode[5]!=0)&&(Keycode[6]!=0)&&(Keycode[7]!=0)))
				{
					Who_Unlocked = Master;
					Private_Mode_Unlock(1);
				}
				else
				{
					Clear_Keycode();
					Who_Unlocked = Nobody;
					Public_Private_Mode_Wrong_Code_Process();
				}
			}
			else if(Position == 6)
			{
				if (((memcmp(Keycode,Submastercode,6) == 0) || (memcmp(Keycode,Techniciancode,6) == 0)) && ((Keycode[0]!=0)&&(Keycode[1]!=0)&&(Keycode[2]!=0)&&(Keycode[3]!=0)&&(Keycode[4]!=0)&&(Keycode[5]!=0)))
				{
					Who_Unlocked = Submaster;
					Private_Mode_Unlock(1);
				}
				else
				{
					if((memcmp(Keycode, Mastercode,6) != 0) || ((Keycode[0]==0)&&(Keycode[1]==0)&&(Keycode[2]==0)&&(Keycode[3]==0)&&(Keycode[4]==0)&&(Keycode[5]==0)))
					{
						Clear_Keycode();
						Who_Unlocked = Nobody;
						Public_Private_Mode_Wrong_Code_Process();
					}
				}
			} // else if(Position == 6)
		} //if (Who_Unlocked == Nobody) // there was no match with any of the codes before
	} // (Lock_Active_Flag == 1)
	else
	{
		Key_Press_Flash_LED(Position,2);
		Usercode_RAS_RTC_4[0]=0;
		Usercode_RAS_RTC_4[1]=0;
		Usercode_RAS_RTC_4[2]=0;
		Usercode_RAS_RTC_4[3]=0;
		eeprom_write_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0,4);
		Who_Unlocked = Nobody;
		Current_Lock_State = RAS_RTC_Date_Code_Entry;
		eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
		RAS_RTC_Date_Code_Enter();
	}
}


uint8_t RAS_RTC_SS_Check_Lock_Active(void)
{
	uint8_t Check_Flag = 0;
	PCF2123_Read_Register(3,6);							// Reads minutes and hrs
	Current_Minute = SPI_Data_Buffer[0] & 0b01111111;	// 7th bit of min can have 1 or 0 value so mask it off
	Current_Hour = SPI_Data_Buffer[1] & 0b00111111;
	Current_Day = SPI_Data_Buffer[2] & 0b00111111;
	Current_Month = SPI_Data_Buffer[4] & 0b00011111;
	Current_Year = SPI_Data_Buffer[5];
	// convert to decimal
	Current_Minute = BCD_to_Dec(Current_Minute);
	Current_Hour = BCD_to_Dec(Current_Hour);
	Current_Day = BCD_to_Dec(Current_Day);
	Current_Month = BCD_to_Dec(Current_Month);
	Current_Year = BCD_to_Dec(Current_Year & 0x0F);
	Stop_Hour = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Hour);
	Stop_Day = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Day);
	Stop_Month = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Month);
	//Stop_Year = eeprom_read_byte((uint8_t*)EEPROM_Set_Stop_Year);
	Start_Hour = eeprom_read_byte((uint8_t*)EEPROM_Set_Start_Hour);
	Start_Day = eeprom_read_byte((uint8_t*)EEPROM_Set_Start_Day);
	Start_Month = eeprom_read_byte((uint8_t*)EEPROM_Set_Start_Month);
	//Start_Year = eeprom_read_byte((uint8_t*)EEPROM_Set_Start_Year);
	// 
	if (Stop_Month > Start_Month)
	{
		Check_Flag = 1;
	}
	else if (Stop_Month == Start_Month)
	{
		if (Stop_Day > Start_Day)
		{
			Check_Flag = 1;
		} 
		else if (Stop_Day == Start_Day)
		{
				if (Stop_Hour > Start_Hour)
				{
					Check_Flag = 1;
				}
				else
				{
					Check_Flag = 0;
				}
		}
		else //if (Stop_Day > Start_Day)
		{
			Check_Flag = 0;
		}
	}
	else // consider months overflow 
	{
		Check_Flag = 1; // stop date could be next year
	}
	//// 
	if(Check_Flag == 1)
	{
			if (Current_Month > Start_Month)
			{
				Check_Flag = 1;
			}
			else if (Current_Month == Start_Month)
			{
				if (Current_Day > Start_Day)
				{
					Check_Flag = 1;
				}
				else if (Current_Day == Start_Day)
				{
					if (Current_Hour >= Start_Hour)
					{
						Check_Flag = 1;
					}
					else
					{
						Check_Flag = 0;
					}
				}
				else
				{
					Check_Flag = 0;
				}
			}
			else
			{
				Check_Flag = 1; //it's allowed to account for year overflow
			}
	}
	
	if (Check_Flag == 1)
	{
			if (Current_Month > Stop_Month)
			{
				Check_Flag = 0;
			}
			else if (Current_Month == Stop_Month)
			{
				if (Current_Day > Stop_Day)
				{
					Check_Flag = 0;
				}
				else if (Current_Day == Stop_Day)
				{
					if (Current_Hour >= Stop_Hour)
					{
						Check_Flag = 0;
					}
					else
					{
						Check_Flag = 1;
					}
				}
				else
				{
					Check_Flag = 1;
				}
			}
			else
			{
				Check_Flag = 1;
			}
	} 
	else
	{
		Check_Flag = 0;
	}
	return Check_Flag;
}

void RAS_RTC_SS_Check_Code(void)
{
		Lock_Active_Flag = RAS_RTC_SS_Check_Lock_Active(); //Returns 1 when current time is smaller than set time
		if (Lock_Active_Flag == 1)
		{
				Key_Press_Flash_LED(Position,1);
				if ((Public_Private_Mode_Code_Lenght == 4) && (Position == 4))
				{
						eeprom_read_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0 ,4);
						if ((memcmp(Keycode,Usercode_RAS_RTC_4,4) == 0) && ((Keycode[0]!=0)&&(Keycode[1]!=0)&&(Keycode[2]!=0)&&(Keycode[3]!=0)&&(Keycode[4]!=0)&&(Keycode[5]!=0)))
						{	// open the lock, beep, flash LEDs
							Who_Unlocked = User_4;
							Private_Mode_Unlock(1);
						}
						else
						{
								if (((memcmp(Keycode,Techniciancode,4) != 0) && (memcmp(Keycode,Mastercode,4) != 0) && (memcmp(Keycode,Submastercode,4) != 0)) || ((Keycode[0]==0)&&(Keycode[1]==0)&&(Keycode[2]!=0)&&(Keycode[3]==0)))//  if there is no match with any of the codes
								{//flag the alarm
									Who_Unlocked = Nobody;
									Clear_Keycode();
									Public_Private_Mode_Wrong_Code_Process();
								}
						}
				}
				if ((Who_Unlocked == Nobody) && (Position >=6))// there was no match with any of the codes before
				{
						if(Position == 8)
						{
								if ((memcmp(Keycode,Mastercode,8) == 0))
								{
									Who_Unlocked = Master;
									Private_Mode_Unlock(1);
								}
								else
								{
									Clear_Keycode();
									Who_Unlocked = Nobody;
									Public_Private_Mode_Wrong_Code_Process();
								}
						}
						else if(Position == 6)
						{
								if (((memcmp(Keycode,Submastercode,6) == 0)||(memcmp(Keycode,Techniciancode,6) == 0)) && ((Keycode[0]!=0)&&(Keycode[1]!=0)&&(Keycode[2]!=0)&&(Keycode[3]!=0)&&(Keycode[4]!=0)&&(Keycode[5]!=0)))
								{
									Who_Unlocked = Submaster;
									Private_Mode_Unlock(1);
								}
								else
								{
										if(memcmp(Keycode, Mastercode,6) != 0)
										{
											Clear_Keycode();
											Who_Unlocked = Nobody;
											Public_Private_Mode_Wrong_Code_Process();
										}
								}
						} // else if(Position == 6)
				} //if (Who_Unlocked == Nobody) // there was no match with any of the codes before
		} // (Lock_Active_Flag == 1)
		else
		{
			Key_Press_Flash_LED(Position,2);
			Usercode_RAS_RTC_4[0]=0;
			Usercode_RAS_RTC_4[1]=0;
			Usercode_RAS_RTC_4[2]=0;
			Usercode_RAS_RTC_4[3]=0;
			eeprom_write_block(&Usercode_RAS_RTC_4,(void*)EEPROM_Usercode_RAS_RTC_4_0,4);
			Who_Unlocked = Nobody;
			Current_Lock_State = RAS_RTC_SS_Date_Code_Entry;
			eeprom_write_byte((uint8_t*)EEPROM_Lock_State,Current_Lock_State);
			RAS_RTC_SS_Date_Code_Enter();
		}
}	

	void RAS_RTC_Mode_Fnc(void)
	{
		if (Current_Lock_Mode == RAS_RTC_Mode)
		{
			switch (Current_Lock_State)
			{
				case RAS_RTC_Date_Code_Entry:
					RAS_RTC_Date_Code_Enter();
					break;
				case RAS_RTC_User_Code_Entry:
					RAS_RTC_User_Code_Enter();
					break;
				case RAS_RTC_Unlock_Code_Entry:
					RAS_RTC_Check_Code();
				default:
					break;
			}
		}
	}

	void RAS_RTC_Date_Code_Enter(void)
	{
	//cli();
	//uint8_t A_SS ,B_SS ,C_SS,D_SS;
		Key_Press_Flash_LED(Position,2);
		if(Position == 6)
		{
			if (memcmp(Keycode,Submastercode,6) == 0)
			{
				Who_Unlocked = Submaster;
				Motor_Open_Close_Lock(Open,0);
				Clear_Keycode();
				Who_Unlocked = Nobody;
			}
		} // else if(Position == 6)
		else if(Position == 8)
		{
			if (memcmp(Keycode,Mastercode,8) == 0)
			{
				Who_Unlocked = Master;
				Motor_Open_Close_Lock(Open,0);
				Clear_Keycode();
				Who_Unlocked = Nobody;
			}
			else
			{
				eeprom_read_block(&serialcode,(void*)EEPROM_serialcode_0 ,4);
				if ((Keycode[0] - serialcode[2]) < 0)
				{
					Keycode[0] = (uint8_t)(10 + (int16_t)(Keycode[0] - serialcode[2]));
				}
				else
				{
					Keycode[0] = Keycode[0] - serialcode[2];
				}
				if ((Keycode[1] - serialcode[0]) < 0)
				{
					Keycode[1] = (uint8_t)(10 + (int16_t)(Keycode[1] - serialcode[0]));
				}
				else
				{
					Keycode[1] = Keycode[1] - serialcode[0];
				}
				if ((Keycode[3] - serialcode[3]) < 0)
				{
					Keycode[3] = (uint8_t)(10 + (int16_t)(Keycode[3] - serialcode[3]));
				}
				else
				{
					Keycode[3] = Keycode[3] - serialcode[3];
				}
				if ((Keycode[5] - serialcode[1]) < 0)
				{
					Keycode[5] = (uint8_t)(10 + (int16_t)(Keycode[5] - serialcode[1]));
				}
				else
				{
					Keycode[5] = Keycode[5] - serialcode[1];
				}
				CS_Calc = Keycode[0] + Keycode[1] + Keycode[2] + Keycode[3] + Keycode[4] + Keycode[5];
				CS = Keycode[6] * 10 + Keycode[7];
				if (CS_Calc == CS) // Checksum is Valid
				{
					Received_number = (((uint32_t)Keycode[0] * 100000) + ((uint32_t)Keycode[1] * 10000) + ((uint32_t)Keycode[2] * 1000) + ((uint32_t)Keycode[3] * 100) + ((uint32_t)Keycode[4] * 10) + ((uint32_t)Keycode[5])) ;
					Stop_Hour = (uint8_t)(Received_number & (uint32_t)0b00011111);
					Stop_Year = (uint8_t)((Received_number >> 5) & (uint32_t)0b0001111);
					Stop_Month = (uint8_t)((Received_number >>9) & (uint32_t)0b00001111);
					Stop_Day = (uint8_t)((Received_number >> 13) & (uint32_t)0b00011111);
					A_SS = Stop_Hour & 6;
					C_SS = Stop_Month & 6;
					D_SS = Stop_Day & 6;
					B_SS = Stop_Year  & 6;
					// Combine the number and shifted 1,2nd bit

					Stop_Hour = (Stop_Hour & 0b11111001) | D_SS;
					Stop_Year = (Stop_Year & 0b11111001) | C_SS;
					Stop_Month = (Stop_Month & 0b11111001) | B_SS;
					Stop_Day = (Stop_Day & 0b11111001) | A_SS;
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Hour,Stop_Hour);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Day,Stop_Day);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Month,Stop_Month);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Year,Stop_Year);
					if (RAS_RTC_Check_Lock_Active() == 1)	// if entered time is higher than the current one
					{
						Current_Lock_State = RAS_RTC_User_Code_Entry;
						Flash_Green_LED(3);
						///Motor_Open_Close_Lock(Open,0);
						Timeout_Constant = 3000;
					}
					else
					{
						Public_Private_Mode_Wrong_Code_Process();
					}
				}
				else
				{
					Public_Private_Mode_Wrong_Code_Process();
				}
				Who_Unlocked = Nobody;
				Clear_Keycode();
			}
		}
		//sei();
	}

	
	void RAS_RTC_SS_Date_Code_Enter(void)
	{
		cli();
		uint8_t Wrong_Code_Flag = 0;
		//uint8_t A_SS ,B_SS ,C_SS,D_SS ,E_SS,F_SS ;
		//uint8_t Stop_Hour_Temp, Stop_Day_Temp, Stop_Month_Temp, Stop_Year_Temp, Start_Hour_Temp, Start_Day_Temp, Start_Month_Temp, Start_Year_Temp;
		Key_Press_Flash_LED(Position,2);
		if(Position == 8)
		{
			if (memcmp(Keycode,Mastercode,8) == 0)
			{
				Who_Unlocked = Master;
				Motor_Open_Close_Lock(Open,0);
				Clear_Keycode();
				Who_Unlocked = Nobody;
			}
		}
		else if(Position == 6)
		{
			if (memcmp(Keycode,Submastercode,6) == 0)
			{
				Who_Unlocked = Submaster;
				Motor_Open_Close_Lock(Open,0);
				Clear_Keycode();
				Who_Unlocked = Nobody;
			}
		} // else if(Position == 6)
		else if(Position == 11)
		{
			eeprom_read_block(&serialcode,(void*)EEPROM_serialcode_0 ,4);
			if ((Keycode[0] - serialcode[2]) < 0)
			{
				Keycode[0] = (uint8_t)(10 + (int16_t)(Keycode[0] - serialcode[2]));
			}
			else
			{
				Keycode[0] = Keycode[0] - serialcode[2];
			}
			if ((Keycode[1] - serialcode[0]) < 0)
			{
				Keycode[1] = (uint8_t)(10 + (int16_t)(Keycode[1] - serialcode[0]));
			}
			else
			{
				Keycode[1] = Keycode[1] - serialcode[0];
			}
			if ((Keycode[3] - serialcode[3]) < 0)
			{
				Keycode[3] = (uint8_t)(10 + (int16_t)(Keycode[3] - serialcode[3]));
			}
			else
			{
				Keycode[3] = Keycode[3] - serialcode[3];
			}
			if ((Keycode[5] - serialcode[1]) < 0)
			{
				Keycode[5] = (uint8_t)(10 + (int16_t)(Keycode[5] - serialcode[1]));
			}
			else
			{
				Keycode[5] = Keycode[5] - serialcode[1];
			}
			///
			if ((Keycode[6] - serialcode[0]) < 0)
			{
				Keycode[6] = (uint8_t)(10 + (int16_t)(Keycode[6] - serialcode[0]));
			}
			else
			{
				Keycode[6] = Keycode[6] - serialcode[0];
			}
			///
			if ((Keycode[7] - serialcode[2]) < 0)
			{
				Keycode[7] = (uint8_t)(10 + (int16_t)(Keycode[7] - serialcode[2]));
			}
			else
			{
				Keycode[7] = Keycode[7] - serialcode[2];
			}
			///
			if ((Keycode[8] - serialcode[3]) < 0)
			{
				Keycode[8] = (uint8_t)(10 + (int16_t)(Keycode[8] - serialcode[3]));
			}
			else
			{
				Keycode[8] = Keycode[8] - serialcode[3];
			}
			CS_Calc = Keycode[0] + Keycode[1] + Keycode[2] + Keycode[3] + Keycode[4] + Keycode[5]+ Keycode[6] + Keycode[7] + Keycode[8];
			CS = Keycode[9] * 10 + Keycode[10];
			
			if (CS_Calc == CS) // Checksum is Valid
			{
				Received_number = (((uint32_t)Keycode[0] * 100000000) + ((uint32_t)Keycode[1] * 10000000) + ((uint32_t)Keycode[2] * 1000000) + ((uint32_t)Keycode[3] * 100000) + ((uint32_t)Keycode[4] * 10000) + ((uint32_t)Keycode[5]*1000)+ ((uint32_t)Keycode[6] * 100) + ((uint32_t)Keycode[7] * 10) + ((uint32_t)Keycode[8])) ;
				Stop_Hour = (uint8_t)((Received_number >> 14) & (uint32_t)0b00011111);
				Stop_Month = (uint8_t)((Received_number >> 19) & (uint32_t)0b00001111);
				Stop_Day = (uint8_t)((Received_number >> 23) & (uint32_t)0b00011111);
				Start_Hour = (uint8_t)((Received_number) & (uint32_t)0b00011111);
				Start_Month = (uint8_t)((Received_number >> 5) & (uint32_t)0b00001111);
				Start_Day = (uint8_t)((Received_number >> 9) & (uint32_t)0b00011111);
				// Extract 1st and 2nd bit from the number
				A_SS = Stop_Hour & 0b00000110;
				B_SS = Stop_Month & 0b00000110;
				C_SS = Stop_Day & 0b00000110;
				D_SS = Start_Hour & 0b00000110;
				E_SS = Start_Month & 0b00000110;
				F_SS = Start_Day & 0b00000110;
				// Combine the number and shifted 1,2nd bit
				Stop_Hour = (Stop_Hour & 0b11111001) | F_SS;
				Stop_Month = (Stop_Month & 0b11111001) | E_SS;
				Stop_Day = (Stop_Day & 0b11111001) | D_SS;
				Start_Hour = (Start_Hour & 0b11111001) | C_SS;
				Start_Month = (Start_Month & 0b11111001) | B_SS;
				Start_Day = (Start_Day & 0b11111001) | A_SS;
				if ((Stop_Hour <=23) && (Start_Hour <=23) && (Stop_Day <= 31) && (Start_Day <= 31) && (Start_Month<=12) && (Stop_Month<=12))
				{
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Hour, Stop_Hour);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Day, Stop_Day);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Month, Stop_Month);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Start_Hour, Start_Hour);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Start_Day, Start_Day);
					eeprom_write_byte((uint8_t*)EEPROM_Set_Start_Month, Start_Month);
					if (RAS_RTC_SS_Check_Lock_Active() == 1)	// if entered time is higher than the current one
					{
						Current_Lock_State = RAS_RTC_SS_User_Code_Entry;
						Flash_Green_LED(3);
						//Motor_Open_Close_Lock(Open,0);
						Timeout_Constant = 3000;
					}
					else
					{
						Wrong_Code_Flag = 1;
					}
				} 
				else
				{
					Wrong_Code_Flag = 1;
				}
			
			}
			else
			{
				Wrong_Code_Flag = 1;
			}
			Who_Unlocked = Nobody;
			Clear_Keycode();
		}
		if (Wrong_Code_Flag == 1)
		{
			Wrong_Code_Flag = 0;
			Public_Private_Mode_Wrong_Code_Process();
			Stop_Hour = 0;
			Stop_Month = 0;
			Stop_Day = 0;
			Start_Hour = 0;
			Start_Month = 0;
			Start_Day = 0;
			eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Hour, Stop_Hour);
			eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Day, Stop_Day);
			eeprom_write_byte((uint8_t*)EEPROM_Set_Stop_Month, Start_Month);
			eeprom_write_byte((uint8_t*)EEPROM_Set_Start_Hour, Start_Hour);
			eeprom_write_byte((uint8_t*)EEPROM_Set_Start_Day, Start_Day);
			eeprom_write_byte((uint8_t*)EEPROM_Set_Start_Month, Start_Month);
		}
		sei();
	}

uint8_t Compare_Code_Against_Multiuser(void)//Returns 0 when there IS a MATCH, Returns 1 when there is NO MATCH
{
	uint8_t m;
	number_1 = Keycode[0];
	number_2 = Keycode[1];
	Multiuser_Location = (10*number_1) + number_2; // combine two numbers to point to position of multiuser table
	for (m=0;m<4;m++)
	{
		Multiuser_Code[m]=Keycode[m+2]; // shift remaining keycode[2-6] to Multiuser_Code[0-4]
	}
	if ((memcmp(Keycode,&Multiuser_Code[Multiuser_Location],4) == 0))
	{
		return 0; // when there is a match
	}
	else
	{
		return 1; // there is no match between codes
	}
}
void Put_To_Sleep(void)
{
	EICRA = 0x08;				// The low level of INT1 generates an INT
// 	uint8_t temp_sleep; 
// 	eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,temp_sleep);
// 	EIMSK = temp_sleep;			// External interrupt enable
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
}
void Watchdog_Enable(void) // It was checked that at the battery voltage of 2.9V wdt runs 18% slower and 17% slower at 3.2V
{
	cli();
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0b01010110; //1s, wdt enable
	sei();
}

ISR(WDT_vect)
{
	cli();
	//UART_RX_Buffer_Position = 0;
	Clear_Keycode();

	if (WDT_counter_1 >= 51) //  it takes 51 WDT cycles to make up 60.18sec
	{
		WDT_counter_1 = 0;
		Wrong_Code_Attempts_Count = 0; // after a minute it clears Wrong Code Attempts Counter
		WDT_counter_2 ++; // counts minutes up
	} 
	else
	{
		WDT_counter_1 ++;
	}
	sei();
}
void PCF2123_Read_Register(uint8_t reg, uint8_t bytes)
{
	uint8_t  m;
	SETBIT(RTC_CE_PORT,RTC_CE);					// CE line high to activate PCF2123
	SPI_Send(reg|0x90);							// Send the reg address with the write mask off
	
	for (m=0;m<bytes;m++)
	{
		SPI_Data_Buffer[m] = 0;					// clear buffer before receiving
		SPI_Data_Buffer[m] = SPI_Send(0x00);	//send dummy data to receive
	}
	CLEARBIT(RTC_CE_PORT,RTC_CE);				// CE line low to de-activate PCF2123
}
void EEPROM_Read_Register(uint16_t EE_address, uint8_t bytes)
{
	uint8_t m;
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);			// CE line high to activate PCF2123
	SPI_Send(0x03);							// Send the READ opcode
	SPI_Send((uint8_t)((EE_address>>8) & 0xff));	
	SPI_Send((uint8_t)(EE_address & 0xff));
	for (m=0;m<bytes;m++)
	{
		SPI_Data_Buffer[m] = SPI_Send(0x00);		//send dummy data to receive
	}
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);	// CE line low to de-activate PCF2123
}
uint8_t EEPROM_Read_Status_Register(void)
{
	uint8_t E_temp;
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);			// CE line high to activate PCF2123
	SPI_Send(0x05);							// Send the READ opcode
	E_temp = SPI_Send(0x00);		//send dummy data to receive
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);	// CE line low to de-activate PCF2123
	return E_temp;
}
void PCF2123_Write_Register(uint8_t reg, uint8_t bytes)
{
	uint8_t i;
	SETBIT(RTC_CE_PORT,RTC_CE); // CE line high to activate PCF2123
	SPI_Send((reg & ~0x80)|0x10);
	for (i=0;i<bytes;i++)
	{
		SPI_Send(SPI_Data_Buffer[i]);
	}
	CLEARBIT(RTC_CE_PORT,RTC_CE); // CE line low to deactivate PCF2123
}
/*
void EEPROM_Write_Byte(uint16_t Data)
{
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	SPI_Send(0x06);						// WREN latch
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	SPI_Send(0x02);						// Write Data Opcode
	SPI_Send(UART_Received_Data[0]);	// MSB Address
	SPI_Send(UART_Received_Data[1]);	// LSB Address
	SPI_Send(UART_Received_Data[2]);	// Data to be written
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	UART_Received_Data[2] = 0;			// Reset the data buffer
	do
	{
		CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
		SPI_Send(0x05);
		Temp_0 = (SPI_Send(0xff) & 0b00000001); // mask the RDY bit (0- is ready, 1- not ready)
		SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	} 
	while (Temp_0 != 0);
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	SPI_Send(0x04);						// WRDI latch
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
}
void EEPROM_Write(uint16_t Address, uint8_t Data)
{
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	SPI_Send(0x06);								// WREN latch
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	SPI_Send(0x02);								// Write Data Opcode
	SPI_Send((uint8_t)((Address>>8) & 0xff));	// MSB Address
	SPI_Send((uint8_t)(Address & 0xff));		// LSB Address
	SPI_Send(Data);								// Data to be written
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	do
	{
		CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
		SPI_Send(0x05);
		Temp_0 = (SPI_Send(0xff) & 0b00000001); // mask the RDY bit (0- is ready, 1- not ready)
		SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	} while (Temp_0 != 0);
	CLEARBIT(EEPROM_CS1_PORT,EEPROM_CS1);
	SPI_Send(0x04);								// WRDI latch
	SETBIT(EEPROM_CS1_PORT,EEPROM_CS1);
}*/
void SPI_Init(void)
{	
	SETBIT(SPI_MOSI_DDR,SPI_MOSI);		// set MOSI and SCK as outputs
	SETBIT(SPI_SCK_DDR,SPI_SCK);
	SETBIT(SPI_SS_DDR,SPI_SS);			// Set SS pin as output
	SETBIT(RTC_CE_DDR,RTC_CE);			// Set CE pin as output
	CLEARBIT(RTC_CE_PORT,RTC_CE);		// Pull RTC CE line low
	SPCR0 = ((1<<SPE)|(1<<MSTR)|(1<<SPCR0));// SPI enable, master, clk/16
}
uint8_t SPI_Send(uint8_t data)
{
	SPDR0 = data;
	while(!(SPSR0 & (1<<SPIF))); //Wait for transmission complete
	return (SPDR0);
}
ISR(INT1_vect)				//Wake up routine for the lock 
{
// 	CLEARBIT(LED_PORT,LED_R_1);
// 	CLEARBIT(LED_PORT,LED_G_1);
// 	_delay_ms(1);
// 	SETBIT(LED_PORT,LED_R_1);
// 	SETBIT(LED_PORT,LED_G_1);
}

ISR(INT0_vect)				//Wake up routine for lock from the RTC
{
	
	cli();
	 
// 	 if (Current_Lock_Mode == Public_Mode) //&& ((Current_Lock_State == Public_Unlock_Code_Entry) || (Previous_Lock_Mode == Public_Unlock_Code_Entry)))
// 	 {
// 		if(eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag) == 1)
// 		{
// 			Temp_1[0] = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_0);
// 			Temp_1[1] = eeprom_read_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Time_1);
// 			PCF2123_Read_Register(0x03,2);// read the actual time
// 			if((Temp_1[0]==SPI_Data_Buffer[0]) && (Temp_1[1]==SPI_Data_Buffer[1]))
// 			{
// 				PCF2123_Read_Register(0x01,1);// read Control_2 reg
// 				uint8_t status = SPI_Data_Buffer[0] & 0b00001000;
// 				if (status != 0)
// 				{
// 					SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b11110111; // clear the alarm flag
// 					PCF2123_Write_Register(0x01,1);
// 					SPI_Data_Buffer[0] = 0;
// 					PCF2123_Read_Register(0x01,1);	// for debugging
// 					SPI_Data_Buffer[0] = 0;			// for debugging
// 					Public_Mode_Unlock(0);
// 				}
// 			}
// 		}
// 	 }
	 
	
	RTC_Int_Flag = 1; //Set the flag to check the timeouts
// 	CLEARBIT(LED_PORT,LED_R_1);
// 	_delay_ms(50); 
// 	SETBIT(LED_PORT,LED_R_1);
	sei();
}

uint8_t Calculate_Sum (uint8_t *ptr,uint8_t start, uint8_t stop) // calculates sum from - to location
{
	uint8_t j;
	uint16_t sum = 0;
	for (j = 0; j<stop; j++)
	{
		sum = sum + ptr[j];
	}
	sum = sum % 256;
	return (uint8_t)sum;
}

/// RAS Mode
void newcode()// For rental mode
{
	//fetch_random();
	columb = ((Usercode_4[0]+Usercode_4[1]+Usercode_4[2]+Usercode_4[3])%10); //column location for tables
	fetch_random(initcode[0],columb);
	wrand0 = val-2;
	fetch_random(serialcode[0],wrand0);
	rand0 = val;
	tempnew[0]=(serialcode[0]+Usercode_4[0]+rand0);// Calculate the next codes using formula number is columb randomiser
	tempnew[0]=(tempnew[0]%10);
	fetch_random(initcode[1],tempnew[0]);
	wrand1 = val-2;
	fetch_random(serialcode[1],wrand1);
	rand1 = val;
	tempnew[1]=(serialcode[1]+Usercode_4[1]+rand1);
	tempnew[1]=(tempnew[1]%10);
	fetch_random(initcode[2],tempnew[1]);
	wrand2 = val-2;
	fetch_random(serialcode[2],wrand2);
	rand2=val;
	tempnew[2]=(serialcode[2]+Usercode_4[2]+rand2);
	tempnew[2]=(tempnew[2]%10);
	fetch_random(initcode[3],tempnew[2]);
	wrand3=val-2;
	fetch_random(serialcode[3],wrand3);
	rand3=val;
	tempnew[3]=(serialcode[3]+Usercode_4[3]+rand3);
	tempnew[3]=(tempnew[3]%10);
	memcpy(nextcode,tempnew,4);
	eeprom_write_block(&nextcode,(void*)EEPROM_nextcode_0,4);
}

void fetch_random(char tab,int row)
{
	switch (tab)
	{
		case 0:
			val=table0[row];
			break;
		case 1:
			val=table1[row];
			break;
		case 2:
			val=table2[row];
			break;
		case 3:
			val=table3[row];
			break;
		case 4:
			val=table4[row];
			break;
		case 5:
			val=table5[row];
			break;
		case 6:
			val=table6[row];
			break;
		case 7:
			val=table7[row];
			break;
		case 8:
			val=table8[row];
			break;
		case 9:
			val=table9[row];
			break;
		default:
			break;
	}
}

void Clear_Keycode(void)
{
	for (uint8_t count_CK=0;count_CK<14;count_CK++)
	{
		Keycode[count_CK]=11;
	}
	Position = 0;
}

void Erase_All_Time_Related_Funcions(void)
{
	Lock_Active_Enabled_Flag = 0;
	eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Flag,Lock_Active_Enabled_Flag);
	Lock_Active_Start_Time_Hr = 0;	// stored as BCD
	eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Hr,Lock_Active_Start_Time_Hr);
	Lock_Active_Start_Time_Min = 0; // stored as BCD
	eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Start_Time_Min,Lock_Active_Start_Time_Min);
	Lock_Active_Stop_Time_Hr = 0;	// stored as BCD
	eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Hr,Lock_Active_Stop_Time_Hr);
	Lock_Active_Stop_Time_Min = 0;	// stored as BCD
	eeprom_write_byte((uint8_t*)EEPROM_Lock_Active_Stop_Time_Min,Lock_Active_Stop_Time_Min);
	//bellow is for removing auto open and lock the active with auto-open function
	PCF2123_Read_Register(0x01,1);
	SPI_Data_Buffer[0] = SPI_Data_Buffer[0] & 0b11111101; // Disable up an alarm interrupt and generate the pulse
	PCF2123_Write_Register(0x01,1);
	eeprom_write_byte((uint8_t*)EEPROM_PCF_Control_Reg_Container_1, SPI_Data_Buffer[0]);
	eeprom_write_byte((uint8_t*)EEPROM_Auto_Timed_Unlock_Flag,0); // disable auto unlock
	EIMSK = EIMSK & ~(1<<INT0); //disable INT0
	eeprom_write_byte((uint8_t*)EEPROM_Interrupts_Container,EIMSK);
}
/*
ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	TCNT1 = 64974;   // for 1 sec at 16 MHz
	if (Communication_Allowed_Flag == 1)
	{
		TCNT1 = 62974;   // slow flash
		LED_PORT ^= 0xAA;
	} 
	else
	{
		TCNT1 = 64974;   // fast flash
		LED_PORT ^= 0x0A;
	}
}
*/
void Initial_Test_Routine(void)// test routine performed once after first startup
{
	uint8_t itr_counter,itr_counter1 = 0;
	for (itr_counter = 0; itr_counter < 9; itr_counter++)
	{
		Buzzer(8);
		if (itr_counter1 == 0)
		{	
			CLEARBIT(LED_PORT,LED_G_4);
			CLEARBIT(LED_PORT,LED_G_3);
			CLEARBIT(LED_PORT,LED_G_2);
			CLEARBIT(LED_PORT,LED_G_1);
			SETBIT(LED_PORT,LED_R_4);
			SETBIT(LED_PORT,LED_R_3);
			SETBIT(LED_PORT,LED_R_2);
			SETBIT(LED_PORT,LED_R_1);
			Motor_CCW();
			itr_counter1 = 1;
			_delay_ms(100);
			Motor_OFF();
		} 
		else
		{
			SETBIT(LED_PORT,LED_G_4);
			SETBIT(LED_PORT,LED_G_3);
			SETBIT(LED_PORT,LED_G_2);
			SETBIT(LED_PORT,LED_G_1);
			CLEARBIT(LED_PORT,LED_R_4);
			CLEARBIT(LED_PORT,LED_R_3);
			CLEARBIT(LED_PORT,LED_R_2);
			CLEARBIT(LED_PORT,LED_R_1);
			Motor_CW();
			itr_counter1 = 0;
			_delay_ms(100);
			Motor_OFF();
		}
		_delay_ms(1000);
	}
	SETBIT(LED_PORT,LED_R_4);
	SETBIT(LED_PORT,LED_R_3);
	SETBIT(LED_PORT,LED_R_2);
	SETBIT(LED_PORT,LED_R_1);
	SETBIT(LED_PORT,LED_G_4);
	SETBIT(LED_PORT,LED_G_3);
	SETBIT(LED_PORT,LED_G_2);
	SETBIT(LED_PORT,LED_G_1);
	Motor_OFF();
}