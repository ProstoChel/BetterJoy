#include "myevic.h"
#include "screens.h"
#include "display.h"
#include "menus.h"
#include "myrtc.h"
#include "dataflash.h"
#include "atomizer.h"
#include "timers.h"
#include "battery.h"
#include "miscs.h"
#include "events.h"


//=========================================================================

uint8_t		Screen;
uint16_t	ScreenDuration;
uint16_t	ScreenRefreshTimer;

//ScrSaveTimes 3 first in sec
const uint8_t ScrSaveTimes[8] = { 5, 15, 30, 1, 5, 15, 30, 0 }; //, 255 }; //{ 1, 2, 5, 10, 15, 20, 30, 0 };
const uint8_t ScrMainTimes[6] = { 30, 60, 5, 10, 15, 20 };
const uint8_t ScrChargeTimes[4] = { 0, 10, 30, 60 };

uint8_t		EditItemIndex;
uint16_t	EditModeTimer;

uint8_t		ShowWeakBatFlag;
uint8_t		BatAnimLevel;


//=========================================================================
// Change Screen
void SetScreen( int screen, int duration )
{
	Screen = screen;
	ScreenDuration = duration;
	gFlags.refresh_display = 1;
}


//=========================================================================
// Called at a frequency of 10Hz except when firing in TC modes.
// Called at a frequency of 2Hz when firing in TC modes. // it's bullsiht (Dreamject)

__myevic__ void DrawScreen()
{
	static uint8_t	TenthOfSecs = 0;
	static uint16_t	CurrentFD = 0;
        static uint8_t scrSaveOnce = 1;
        static uint8_t StealthPuffs = 0; //dfStealthPuffsCnt

	if ( ( !PE0 || AutoPuffTimer ) && Screen == 2 && FireDuration && FireDuration != CurrentFD )
	{
		CurrentFD = FireDuration;
		//ScreenDuration = ISMODETC(dfMode) ? 1 : 2;
                ScreenDuration = dfFireScrDuration; //2;
		TenthOfSecs = 0;
		gFlags.refresh_display = 1;
	}
	else if ( ScreenRefreshTimer && !--ScreenRefreshTimer )
	{
		if ( Screen != 2 ) 
                    gFlags.refresh_display = 1;
	}

	if ( gFlags.refresh_display )
	{
		gFlags.refresh_display = 1;
		ClearScreenBuffer();

		switch ( Screen )
		{
			case  0: // Black
                                if ( dfStatus.off ) StealthPuffs = 0;
                                
				break;

			case  1: // Main view
			//case  3: // Main view (?)
			//case  4: // (unused?)

                                if ( dfStealthOn == 1 && LastEvent == 15 ) //&& FireDuration < 2
                                {
                                        StealthPuffs = dfStealthPuffsCnt;       
                                }
                        
				ShowMainView(); 
				break;

			case  2: // Firing

				if ( dfStealthOn != 2 && !gFlags.MainContrast )
				{
                                        DisplaySetContrast( dfContrast );
                                        gFlags.MainContrast = 1;
                                }
                                else if ( dfStealthOn == 2 && gFlags.MainContrast )
                                {
                                        DisplaySetContrast( dfContrast2 );
                                        gFlags.MainContrast = 0;
                                }
                                                                   
                                if ( ( dfStealthOn != 1 || StealthPuffs ) || ShowWeakBatFlag ) 
                                {
                                        ShowMainView();
                                }

				break;

			case  5: // charge screen
                            
                                if ( gFlags.MainContrast )
                                {
                                    DisplaySetContrast( dfContrast2 );
                                    gFlags.MainContrast = 0;
                                }
 
				ShowBatCharging();
				break;

			case 20: // No Atomizer Found
				ShowNoAtoFound();
				break;

			case 21: // Atomizer Short
				ShowAtoShort();
				break;
                                
                        case 70: // Atomizer Short Current
				ShowAtoShortCurrent();
				break;
                                
                        case 71: // Atomizer Short Bad
				ShowAtoShortBad();
				break;
                                
			case 22: // Atomizer Low
				ShowAtoLow();
				break;

			case 23: // 10s Protection
				Show10sProtec();
				break;

			case 24: // Battery Low
				ShowBatLow();
				break;

			case 25: // Battery Low Lock
				ShowBatLowLock();
				break;

			case 28: // Key Lock
				ShowKeyLock();
				break;

			case 29: // Device too hot
				ShowDevTooHot();
				break;

			case 31: // Key UnLock
				ShowKeyUnLock();
				break;

			//case 37: // Board Temp
			//	ShowBoardTemp();
			//	break;

			case 40: // Stealth ON/OFF
				ShowStealthMode();
				break;

			case 50: // FW Version
				ShowVersion();
				break;

			case 51: // New Coil
				ShowNewCoil();
				break;

			case 54: // Battery Voltage
				ShowBattVolts();
				break;

			case 55: // Imbalanced Batteries
				ShowImbBatts();
				break;

			case 56: // Check Battery
				ShowCheckBattery();
				break;

			case 57: // Check USB Adapter
				ShowCheckUSB();
				break;

			case 58: // Charge Error
				ShowChargeError();
				break;

			case 60: // Screen Saver
                            
                                if ( dfStatus.off )
                                {
                                    DisplaySetContrast( dfContrast ); //main contrast in off state fire clock 
                                    gFlags.MainContrast = 1;
                                }
                                else
                                {
                                    if ( gFlags.MainContrast )
                                    {
                                        DisplaySetContrast( dfContrast2 );
                                        gFlags.MainContrast = 0;
                                    }
                                }
				ShowScreenSaver();
				break;
                                
			case 61: //goodbye
                                DisplaySetContrast( dfContrast2 );
                                gFlags.MainContrast = 0;
                                ShowGoodBye();
				break;
                                
			//case 100:
			//	ShowInfos();
			//	break;

			case 101:
				ShowContrast();
				break;

			case 102:
				ShowMenus();
				break;

			case 103:
				ShowRTCSpeed();
				break;

			case 104:
				ShowRTCAdjust();
				break;

			case 105:
				ShowSetTime();
				break;

			case 106:
				ShowSetDate();
				break;

			case 107:
				ShowPowerCurve();
				break;
                                
                        case EVENT_SET_JOULES: //scr = event
				ShowSetJoules();
				break;        
                                                               
			default:
				break;
		}
                
		DisplayRefresh();
	}



//if ( gFlags.debug & 1 )
        
// scr edges x,y       
//        
// 0,0       64,0
//
//
//
// 0,108 ... 64,108
        
//DrawValue( 0, 108, Screen, 0, 0x01, 0 );
//DrawValue( 20, 108, LastEvent, 0, 0x01, 0 );
//DrawValueRight( 38, 108, gFlags.inverse, 0, 0x01, 0 );
//DrawValueRight( 64, 108, FireDuration, 0, 0x01, 0 );
        
//DrawValue( 0, 0, StealthPuffs, 0, 0x01, 0 );
//DrawValue( 64, 0, FireDuration ? : 0, 0, 0x01, 0 );
//DrawValueRight( 64, 0, LastEvent, 0, 0x01, 0 ); 
//NextPreheatTimer UserInputs AutoPuffTimer
//UserInputs LastInputs TargetVolts AtoRez
//FireClicksEvent  FireClickCount CurrentFD ScreenDuration PreheatDelay SleepTimer KeyUpTimer
//dfResistance AtoRez dfTempAlgo StealthPuffs
        
//DisplayRefresh();


	if ( ( Screen == 1 || Screen == 60 ) && ( ScreenDuration <= 4 ) )
	{
		if ( !gFlags.fading  )
		{
			FadeOutTimer = 300;
			gFlags.fading = 1;
		}
	}
	else if ( gFlags.fading )
	{
		FadeOutTimer = 0;
		//DisplaySetContrast( dfContrast );
                gFlags.MainContrast = 0; //need for interrupt fading
		gFlags.fading = 0;
	}

	if (( gFlags.firing ) && ISMODETC(dfMode))
		TenthOfSecs += 1;
	else
		TenthOfSecs += 1;

	if ( TenthOfSecs < 10 )
		return;

	TenthOfSecs = 0;
        //                              every second now:

	if (  100 * ScreenDuration < EditModeTimer )
		ScreenDuration = EditModeTimer / 100 + 1;

	if ( ScreenDuration && --ScreenDuration )
		return;

	switch ( Screen ) //                EXIT from screens 
	{
		case   0: // Black
			if ( dfStatus.off )
                        {
				SleepTimer = 0;
                        }
			else
			{
				if (( !gFlags.firing )
				&&	( dfStealthOn != 1 )
				&&	( SleepTimer > 0 )
				&&	( dfScreenSaver > 0 )
				&&	( GetScreenProtection() > 0 )
                                &&	( scrSaveOnce ) )
				{
					Screen = 60;
					ScreenDuration = GetScreenProtection();
					gFlags.refresh_display = 1;
                                        scrSaveOnce = 0;
				}
			}
			break;

		case   2: // Firing
                        if ( dfStealthOn == 1 && StealthPuffs && FireDuration > 1 ) //CurrentFD
                        {
                            --StealthPuffs;
                        }
                        //no brake
		case  28: // Key Lock
		case  40: // Stealth ON/OFF
                       
                        if ( dfStealthOn != 1 )
                        {
                                StealthPuffs = 0;
                        }
                        
			if ( dfStealthOn == 1 && !StealthPuffs )
			{
				Screen = 0;
				SleepTimer = dfDimOffTimeout * 100; //18000;
				gFlags.refresh_display = 1;
			}
			else
			{
				MainView();
			}
			break;

		case   5: // charge screen
                        if ( ScrChargeTimes[dfScrChargeTime] )
                        {
                            gFlags.screen_on = 0;
                        }
                    
                    	break;
                        
		case  22: // Atomizer Low
		case  24: // Battery Low
		case  25: // Battery Low Lock
		//case  50: // FW Version
			break;

		case  23: // 10s Protection
			if ( !dfStatus.off )
			{
				// Switch box off after 10s if no response
				// have been given to a long fire.
				if ( !PE0 && gFlags.user_idle )
				{
					Event = 17; //on-off mod
				}
				else
				{
					Screen = 0;
					SleepTimer = dfDimOffTimeout * 100; //18000;
					gFlags.refresh_display = 1;
                                        scrSaveOnce = 1;
				}
			}
			break;

		case  20: // No Atomizer Found
		case  21: // Atomizer Short
		case  29: // Device too hot
		case  31: // Key UnLock
		case  51: // New Coil
		case  55: // Imbalanced Batteries
		case  56: // Check Battery
		case  57: // Check USB Adapter
		case  58: // Charge Error
                case  70: // Atomizer Short current error
                case  71: // Atomizer Short contacts check
			MainView();
			break;

		case 101: // Contrast Menu
		case 102: // Menus
		case 103: // RTC Speed
		case 104: // Adjust Clock
		case 105: // Set Time
		case 106: // Set Date
		case 107: // Power Curve
                case EVENT_SET_JOULES:    
			EditModeTimer = 0;
			gFlags.edit_capture_evt = 0;
			gFlags.edit_value = 0;
			//LEDOff();
			UpdateDataFlash();
                        //DisplaySetContrast( dfContrast );
                        //scrSaveOnce = 1;
			//NOBREAK
		case   1: // Main view
                case  50: // FW Version
		case  37: // Board Temp
		case  54: // Battery Voltage
		case 100: // Infos page
                        scrSaveOnce = 1;
                        //NOBREAK
                case  60: // Screen Saver
			if ( gFlags.battery_charging )
			{
				ChargeView();

				if ( dfStealthOn == 1 || !gFlags.screen_on )
				{
					ScreenDuration = 0;
				}
			}
			else
			{
				Screen = 0;
				SleepTimer = dfDimOffTimeout * 100; //18000;
				gFlags.refresh_display = 1;
			}
                         
			break;
                        
                case  61: // goodbye
			Screen = 0;
			SleepTimer = 0;
                        gFlags.refresh_display = 1;
                        break;
                        
		default:
			break;
	}

	return;
}

//=========================================================================

__myevic__ uint16_t GetScreenProtection()
{
    if ( dfScreenProt < 3 ) 
    {
        return ( ScrSaveTimes[dfScreenProt] ); //in sec
    }
    else
    {
        return ( 60 * ScrSaveTimes[dfScreenProt] );  //in min
    }
}

__myevic__ uint16_t GetMainScreenDuration()
{
	return dfDimTimeout ? : ScrMainTimes[dfScrMainTime];
}


//=========================================================================

__myevic__ int convert_string1( uint8_t *strbuf, const char *s )
{
	int i;
	char c;

	i = 0;
	while (( c = *s++ ) && ( i < 20 ))
	{
		if (( c >= '0' ) && ( c <= '9' ))
		{
			strbuf[i++] = c - '0' + 0x0B;
		}
		else if (( c >= 'A' ) && ( c <= 'Z' ))
		{
			strbuf[i++] = c - 'A' + 0x9C;
		}
		else if (( c >= 'a' ) && ( c <= 'z' ))
		{
			strbuf[i++] = c - 'a' + 0x82;
		}
		else if ( c == '/' )
		{
			strbuf[i++] = c - '/' + 0xD6;
		}
		else                    
		{
			strbuf[i++] = 0xBC;
		}
	}
	strbuf[i] = 0;
	return i;
}


//=========================================================================

__myevic__ void ChargeView()
{
	Screen = 5;
	gFlags.refresh_display = 1;
	ScreenDuration = ScrChargeTimes[dfScrChargeTime]; //5;
        
        //if ( dfStealthOn != 1 ) 
        //    gFlags.screen_on = 1;
}


//=========================================================================

/*
__myevic__ void ShowInfos()
{
	uint8_t strbuf[20];

	// TODO : infos page
	convert_string1( strbuf, "Ferox" );
	DrawStringCentered( strbuf, 71 );
        convert_string1( strbuf, "MicroSur" );
        DrawStringCentered( strbuf, 82 );
	convert_string1( strbuf, "were" );
	DrawStringCentered( strbuf, 92 );
	convert_string1( strbuf, "here" );
	DrawStringCentered( strbuf, 102 );

	return;
}
*/

//=========================================================================

__myevic__ void ShowContrast()
{
	int pc, nd, x, t;
        uint8_t dfc;

        dfc = ContrastNum ? dfContrast2 : dfContrast;   

        DrawValueRight( 63, 6, ContrastNum+1, 0, 0x0B, 1 );
	DrawString( String_Contrast, 4, 6 );
	DrawHLine( 0, 16, 63, 1 );

	pc = ( dfc / 85 + 1 );
//	nd = ( pc < 100 ? pc < 10 ? 1 : 2 : 3 );
//	x = ( 64 - ( 6 * nd + 9 )) / 2;
	DrawValue( 30, 20, pc, 0, 0x0B, 0 );
//	DrawImage( x + 6 * nd, 20, 0xC2 );

	DrawFillRect( 0, 32, 63, 44, 1 );
	DrawFillRect( 1, 33, 62, 43, 0 );
	if ( dfc )
	{
		DrawFillRect( 2, 34, 2 + ( ( 59 * dfc ) / 255 ), 42, 1 );
	}

	DrawStringCentered( String_LongFire, 53 );
	DrawStringCentered( String_Exit, 64 );
        
        t = dfStatus.nologo;
	dfStatus.nologo = 0;
        DrawLOGO( 0, 80 );
        dfStatus.nologo = t;
}


//=========================================================================

__myevic__ void ShowMenus()
{
	DrawMenu();
}


//=========================================================================

__myevic__ void ShowRTCSpeed()
{
	unsigned int cs;
	S_RTC_TIME_DATA_T rtd;

	DrawString( String_ClkSpeed, 4, 6 );
	DrawHLine( 0, 16, 63, 1 );

	GetRTC( &rtd );
	//DrawTimeSmall( 10, 25, &rtd, 0x1F );
        DrawTime( 3, 25, &rtd, 0x1F );

	if ( gFlags.has_x32 )
	{
		DrawString( String_X32, 11, 48 );
		DrawString( String_On, 37, 48 );
	}
	else
	{
		cs = RTCGetClockSpeed();
		DrawValue( 12, 48, cs, 0, 0x1F, 5 );
	}
}


//=========================================================================

__myevic__ int IsClockOnScreen()
{
        return (  ( ( Screen == 1 ) && ( ( dfAPT == 4 ) || ( dfAPT3 == 4 ) ) )
	//return (  ((( Screen == 1 ) || ( Screen == 2 )) && ( ( dfAPT == 8 ) || ( dfAPT3 == 8 ) ) )
			|| (( Screen == 1 ) && ( dfStatus.clock ))
			|| (( Screen == 60 ) && ( dfScreenSaver == SSAVER_CLOCK ))
			||  ( Screen == 103 )
			||  ( Screen == 104 )
			);
}


//=========================================================================
//----- (000067C8) --------------------------------------------------------
__myevic__ void ShowBattery()
{
        int i;	
        int x0;
        int y0;
        int y;
        
        if ( BLINKITEM(6) ) return;
            if ( dfBattLine == 1 )
		{
		for ( int i = 0 ; i < NumBatteries ; ++i )
		{
//BatteryTenthAll[i]
			uint16_t bv = gFlags.firing ? RTBVolts[i] : BattVolts[i];                    
			DrawValue(  51, 87 + 14 * ( i), BatteryVoltsToPercent( bv ), 0, 0x1F, 2 );


//			DrawValue(  2 + 24 * ( i), 117, BatteryVoltsToPercent( BattVolts[i] ), 0, 0x1F, 2 );
///			DrawValue(  2 + 21 * ( i), 116, BattVolts[i], 2, 0x1F, 3 );
	//		DrawImage( 23 + 32 * ( i & 1 ), 100 + ( i >> 1 ) * 12, 0x7D );
		}
                }
                        
        if ( dfBattLine == 2 )
		{
			uint16_t bv = gFlags.firing ? RTBattVolts : BatteryVoltage;
			DrawValueRight(	20, 120-3, bv, 2, 0x1F, 0 );
			DrawImage( 21, 120-3, 0x7D );
		}


	if ( gFlags.battery_10pc && !gFlags.battery_charging )
	{
		if ( gFlags.draw_battery )
		{

/*			if ( dfBattLine == 0 )
			{
				DrawImage( 8, 116, 0xC4 ); //big empty
			}
                        else if ( dfBattLine == 3 )
                        {
                                DrawImage( 1, 116, 0xE2 ); //2 small empty
                                DrawImage( 33, 116, 0xE2 );    
                        }*/
		}
	}
	else if ( gFlags.draw_battery_charging && gFlags.battery_charging )
	{
		if ( dfBattLine == 2 || dfBattLine == 1 )
		{
//			DrawImage( 31, 120, 0xD4 ); //charging icon
		}

		else if ( dfBattLine == 0 )
		{
			DrawImage( 8, 116, 0xC5 ); //big charging
		}
                else if ( dfBattLine == 3 )
		{
			DrawImage( 1, 116, 0xE3 ); //2 small charging
                        DrawImage( 33, 116, 0xE3 );
		}
	}
	else
	{
                if ( dfBattLine == 0 )
		{
			DrawImage( 8, 116, 0xC4 );
			if ( gFlags.batteries_ooe && gFlags.draw_battery )
			{
				//DrawString( String_BALANCE_s, 10, 117 );
                                DrawImage( 23, 117, 0xC6 );
                                //DrawHLine ( 9, 117, 50, 1 );
			}
			else if ( BatteryTenth )
			{
				DrawFillRectLines( 10, 121, (4 * BatteryTenth + 9), 125, 1 );
			}
		}
/*		else if ( dfBattLine == 3 )
		{
                        DrawImage( 1, 116, 0xE2 ); //2 small empty
                        DrawImage( 33, 116, 0xE2 ); 
			if ( gFlags.batteries_ooe && gFlags.draw_battery )
			{
				//DrawString( String_BAL_s, 37, 117 );
                                DrawImage( 40, 119, 0xC6 );
                                //DrawHLine ( 34, 117, 59, 1 ); //fill space after drawstring
			}
			else 
			{
                            for ( i = 0 ; i < NumBatteries ; ++i )
                            {
                                if ( i == 0 || i == 1 )
                                {
                                    y0 = 120;
                                    y = 122;
                                    if ( i == 0 ) x0 = 2;
                                    else x0 = 34;
                                }
                                else
                                {
                                    y0 = 124;
                                    y = 126;
                                    if ( i == 2 ) x0 = 2;
                                    else x0 = 34;   
                                }
                
                                if ( BatteryTenthAll[i] )
                                {
                                    if ( NumBatteries == 2 )
                                        DrawFillRectLines( x0+1, y0+1, (25 * BatteryTenthAll[i] / 10 + x0), 125, 1 );               
                                    else
                                        DrawFillRect( x0, y0, (25 * BatteryTenthAll[i] / 10 + x0+2), y, 1 );               
                                }
                            }				
			}
		}         */       
	}
}


//=========================================================================
//----- (00006764) --------------------------------------------------------
__myevic__ void ShowBatCharging()
{
	if ( ( dfStealthOn == 1 && ScreenDuration == 0 ) || !gFlags.screen_on )
	{
		return;
	}
               
/*
	switch ( dfScreenSaver )
	{
		case SSAVER_CLOCK:
		case SSAVER_LOGO:
			ShowScreenSaver();
			break;

		default:
			break;
	}
*/
  
	if ( dfBattLine == 0 )
	{
		DrawImage( 8, 116, 0xC4 );
	}
	else
	{
		DrawValueRight(	18, 120, BatteryPercent, 0, 0x1F, 0 );
		DrawImage( 19, 120, 0xC2 );
		DrawImage( 30, 116, 0xE2 );
	}
        
	if ( BatteryTenth != 10 )
	{
		if ( BatAnimLevel )
		{
			if ( dfBattLine == 0 )
			{
				DrawFillRectLines( 10, 121, (4 * BatAnimLevel + 9), 125, 1 );
			}
                        else
			{
				DrawFillRectLines( 32, 121, (25 * BatAnimLevel / 10 + 31), 125, 1 );
			}
		}
	}
	else if ( gFlags.draw_battery_charging )
	{
		if ( dfBattLine == 0 )
		{
			DrawFillRectLines( 10, 121, 49, 125, 1 );
		}
                else
		{
			DrawFillRectLines( 32, 121, 56, 125, 1 );
		}
	}

/*
	if (( dfScreenSaver == SSAVER_CLOCK ) || ( dfScreenSaver == SSAVER_LOGO ))
	{
		DrawValue(  1, 104, BatteryVoltage, 2, 0x1F, 3 );
		DrawImage( 27, 104, 0x7D );
	}
	else
	{
*/
		for ( int i = 0 ; i < NumBatteries ; ++i )
		{
			DrawValue(  1, 104 - i * 14, BattVolts[NumBatteries - i - 1], 2, 0x1F, 3 );
			DrawImage( 23, 104 - i * 14, 0x7D );
		}
                
                if ( NumBatteries > 1 )
                {
                DrawString( String_USB, 6, 0 );
                DrawValue(  31, 9, USBVolts, 2, 0x1F, 3 );
                DrawImage( 52, 9, 0x7D );
        
                DrawString( String_Charge, 6, 20 );
                DrawValue(  31, 34, ChargeCurrent / 10, 2, 0x0B, 3 );
                DrawImage( 52, 34, 0x68 );
                } else
                {
                    DrawString( String_Charge, 6, 0 );    
                }
        
	//}
        int t;
        if ( ISSINFJ200 )
        {
                t = dfIsCelsius ? AkkuTemp : CelsiusToF( AkkuTemp );
                DrawValueRight( 52, 90, t, 0, 0x0B, 0 );
                DrawImage( 54, 90, dfIsCelsius ? 0xC9 : 0xC8 );
        }
                        
	t = dfIsCelsius ? BoardTemp : CelsiusToF( BoardTemp );

	DrawValueRight( 52, 104, t, 0, 0x0B, 0 );
	DrawImage( 54, 104, dfIsCelsius ? 0xC9 : 0xC8 );
}

//=========================================================================
//----- (0000683C) --------------------------------------------------------
__myevic__ void ShowBattVolts()
{
	if ( NumBatteries > 1 )
	{
		DrawStringCentered( String_Batteries, 32 );
		for ( int i = 0 ; i < NumBatteries ; ++i )
		{
			DrawValue(  6, 44+20*i, BattVolts[i], 2, 0x29, 3 );
			DrawImage( 46, 44+20*i, 0xB8 );
		}
	}
	else
	{
		DrawStringCentered( String_Battery, 88 );
		DrawValue( 6, 102, BatteryVoltage, 2, 0x29, 3 );
		DrawImage( 46, 102, 0xB8 );
	}
}


//=========================================================================
//----- (00006874) --------------------------------------------------------
/*
__myevic__ void ShowBoardTemp()
{
	DrawStringCentered( String_Temp, 88 );
	DrawValue( 16, 102, BoardTemp, 0, 0x48, 2 );
}
*/


//=========================================================================
//----- (00007684) --------------------------------------------------------
__myevic__ void ShowVersion()
{
	uint8_t buf[12];

	DrawStringCentered( String_myevic, 24 );

	DrawStringCentered( String_Build, 44 );
	Value2Str( buf, __BUILD1, 0, 0x1F, 0 );
	DrawStringCentered( buf, 58 );

        
        uint8_t strbuf[20];
        convert_string1( strbuf, BoxName );
        DrawStringCentered( strbuf, 118 );

}


//=========================================================================
//----- (000068D4) --------------------------------------------------------
__myevic__ void ShowNewCoil()
{
	int rez;

	DrawStringCentered( String_NewCoil, 50 );
	DrawValue( 16, 62, dfResistance, 2, 0x0B, 3 );
	DrawImage( 40, 62, 0xC0 );
	DrawStringCentered( String_Right, 74 );
	DrawStringCentered( String_SameCoil, 90 );

	if ( ISMODETC( dfMode ) )
	{
		if ( dfMode == 0 )
		{
			rez = dfRezNI;
		}
		else if ( dfMode == 1 )
		{
			rez = dfRezTI;
		}
		else if ( dfMode == 2 )
		{
			rez = dfRezSS;
		}
		else if ( dfMode == 3 )
		{
			rez = dfRezTCR;
		}
		DrawValue( 16, 102, rez, 2, 0x0B, 3 );
	}

	DrawImage( 40, 102, 0xC0 );
	DrawStringCentered( String_Left, 114 );
}


//=========================================================================
//----- (000076AC) --------------------------------------------------------
__myevic__ void ShowStealthMode()
{
	DrawStringCentered( String_Stealth, 88 );
        //DrawStringCentered( dfStealthOn ? String_ON : String_OFF, 102 );
        if ( !dfStealthOn )
            DrawStringCentered( String_Off, 102 );
        else if ( dfStealthOn == 1 )
            DrawStringCentered( String_On, 102 );
        else
            DrawStringCentered( String_Contrast, 102 );
}


//=========================================================================
//----- (000076DC) --------------------------------------------------------
__myevic__ void ShowDevTooHot()
{
	DrawStringCentered( String_Device, 88 );
	DrawStringCentered( String_TooHot, 102 );
        
/*
        int t;
        if ( ISSINFJ200 )
        {
                t = dfIsCelsius ? AkkuTemp : CelsiusToF( AkkuTemp );
                DrawValueRight( 52, 0, t, 0, 0x0B, 0 );
                DrawImage( 54, 0, dfIsCelsius ? 0xC9 : 0xC8 );
        }
                        
	t = dfIsCelsius ? BoardTemp : CelsiusToF( BoardTemp );

	DrawValueRight( 52, 14, t, 0, 0x0B, 0 );
	DrawImage( 54, 14, dfIsCelsius ? 0xC9 : 0xC8 );
*/
}


//=========================================================================
//----- (00007718) --------------------------------------------------------
__myevic__ void ShowAtoLow()
{
	DrawStringCentered( String_Atomizer, 88 );
	DrawStringCentered( String_Low, 102 );
}


//=========================================================================
//----- (00007734) --------------------------------------------------------
__myevic__ void ShowAtoShort()
{
	DrawStringCentered( String_Atomizer, 88 );
	DrawStringCentered( String_Short, 102 );
}
__myevic__ void ShowAtoShortCurrent()
{
    //from read ato current 70
	DrawStringCentered( String_Atomizer, 88 );
	DrawStringCentered( String_Error, 102 );
}
__myevic__ void ShowAtoShortBad()
{
    //bad contacts 71
        DrawStringCentered( String_Check, 88 );
	DrawStringCentered( String_Atomizer, 102 );
}


//=========================================================================
//----- (00007750) --------------------------------------------------------
__myevic__ void ShowBatLow()
{
	DrawStringCentered( String_Battery, 88 );
	DrawStringCentered( String_Low, 102 );
}


//=========================================================================
//----- (0000776C) --------------------------------------------------------
__myevic__ void ShowBatLowLock()
{
	DrawStringCentered( String_Battery, 82 );
	DrawStringCentered( String_Low, 92 );
	DrawStringCentered( String_Lock, 102 );
}


//=========================================================================
//----- (00007794) --------------------------------------------------------
__myevic__ void ShowKeyLock()
{
	DrawStringCentered( String_Key, 88 );
	DrawStringCentered( String_Lock, 102 );
}


//=========================================================================
//----- (000077B0) --------------------------------------------------------
__myevic__ void ShowKeyUnLock()
{
	DrawStringCentered( String_Key, 88 );
	DrawStringCentered( String_UnLock, 102 );
}


//=========================================================================
//----- (000077CC) --------------------------------------------------------
__myevic__ void ShowNoAtoFound()
{
	DrawStringCentered( String_No, 82 );
	DrawStringCentered( String_Atomizer, 92 );
	DrawStringCentered( String_Found, 102 );
}


//=========================================================================
//----- (000077F4) --------------------------------------------------------
__myevic__ void Show10sProtec()
{
	ShowFireDuration( 0 );
	DrawStringCentered( String_LongFire, 88 );
	DrawStringCentered( String_Protection, 102 );
}


//=========================================================================
//----- (00007810) --------------------------------------------------------
__myevic__ void ShowWeakBat()
{
	DrawStringCentered( String_Weak, 110 );
	DrawStringCentered( String_Battery, 119 );
}


//=========================================================================
__myevic__ void ShowRTCAdjust()
{
	S_RTC_TIME_DATA_T rtd;

	DrawString( String_ClkAdjust, 4, 6 );
	DrawHLine( 0, 16, 63, 1 );

	GetRTC( &rtd );
	DrawTime( 3, 40, &rtd, 0x1F );
}


//=========================================================================
__myevic__ void ShowGoodBye()
{
    //DrawStringCentered( String_Off, 63 );
    DrawStringCentered( ScreenDuration > 1 ? String_On : String_Off, 63 );
    gFlags.refresh_display = 1;
}

//=========================================================================
__myevic__ void ShowScreenSaver()
{
        if ( dfStatus.off ) //&& !dfStatus.offmodclock )
        {
                DrawDigitClock( 82, 0 );
                DrawClock( 0 );
                gFlags.refresh_display = 1;
                return;
        }

	switch ( dfScreenSaver )
	{
		case SSAVER_CLOCK:
                        
                        DrawDigitClock( 82, 0 );
                        DrawClock( 0 );           
			break;

		case SSAVER_3D:
			anim3d( 1 );
			break;

		case SSAVER_LOGO:
		{
			int h = GetLogoHeight();
			if ( h )
			{
				DrawLOGO( 0, 32 - ( h - 48 )/2 );
			}
			break;
		}

		case SSAVER_QIX:
			qix( 1 );
			break;

		case SSAVER_SNOW:
			Snow( 1 );
			break;

		case SSAVER_SPLASH:
			//DrawImage( 0, 0, 0xFF );
                        ShowSplash();
			break;

		default:
			break;
        }
}


//=========================================================================
__myevic__ void AnimateScreenSaver()
{
/*
        if ( dfStatus.off )
        {
                DrawDigitClock( 82, 0 );
                DrawClock( 0 );
                gFlags.refresh_display = 1;
                return;
        }
*/
            
	switch ( dfScreenSaver )
	{
		case SSAVER_3D:
			anim3d( 0 );
			break;

		case SSAVER_QIX:
			qix( 0 );
			break;

		case SSAVER_SNOW:
			Snow( 0 );
			break;

		default:
			break;
	}
}

//=========================================================================
__myevic__ void ShowSetJoules()
{
        uint32_t vv, t;
        
	DrawString( String_mlkJ, 4, 6 );
	DrawHLine( 0, 16, 63, 1 );
        
        DrawValueRight( 45, 24, dfVVRatio, 0, 0x1F, 0 );

        vv = ( MilliJoules / 10 ) / 3600;
        if ( vv > 9999 ) vv = 9999;                        
        DrawImage( 2, 75, 0xDE ); //energy
        DrawValueRight( 52, 73, vv, 2, 0x1F, 0 );
        DrawImageRight( 62, 73, 0x67 ); //wh
   
   	vv = dfVVRatio * ( MilliJoules / 1000 ) / 1000;
	vv /= 10;
	if ( vv > 9999 ) vv = 9999;
        DrawImage( 2, 43, 0xF9 ); //ml
        DrawValueRight( 52, 41, vv, 2, 0x1F, 0 );
        DrawImageRight( 61, 43, 0xCD ); //flask

        // Elasped seconds since last VV reset
        t = RTCGetEpoch( 0 );
        t -= RTCReadRegister( RTCSPARE_VV_BASE );
        vv = vv * 86400 / ( t ? : 1 );
        DrawImage( 2, 59, 0xF3 ); //mld
        DrawValueRight( 52, 57, vv, 2, 0x1F, 0 );
        DrawImageRight( 61, 59, 0xCD ); //flask

        //DrawStringCentered( String_Fire, 53 );
	//DrawStringCentered( String_Exit, 64 );
}

//=========================================================================
__myevic__ void ShowSetTime()
{
	DrawString( String_Time, 4, 6 );
	DrawHLine( 0, 16, 63, 1 );

	DrawTime( 3, 46, &SetTimeRTD, 0x1F & ~( 1 << ( EditItemIndex << 1 ) ) );
	DrawDate( 4, 64, &SetTimeRTD, 0x1F );
        
        DrawStringCentered( String_LongFire, 103 );
	DrawStringCentered( String_Save, 114 );
}


//=========================================================================
__myevic__ void ShowSetDate()
{
	const uint8_t cols[4][3] =
	{
		{ 0x1E, 0x1B, 0x0F },
		{ 0x1E, 0x0F, 0x1B },
		{ 0x1E, 0x1B, 0x0F },
		{ 0x0F, 0x1B, 0x1E }
	};

	int f = dfStatus.dfmt1 | ( dfStatus.dfmt2 << 1 );
	int col = cols[f][EditItemIndex];

	S_RTC_TIME_DATA_T rtd;

	GetRTC( &rtd );

	DrawString( String_Date, 4, 6 );
	DrawHLine( 0, 16, 63, 1 );

	DrawTime( 3, 46, &rtd, 0x1F );
	DrawDate( 4, 64, &SetTimeRTD, col );
        
        DrawStringCentered( String_LongFire, 103 );
	DrawStringCentered( String_Save, 114 );
}


//=========================================================================
__myevic__ int IsMenuScreen()
{
	return (( Screen >= 101 ) && ( Screen <= 107 ));
}


//=========================================================================
__myevic__ void ShowCheckBattery()
{
  DrawStringCentered( String_Check, 88 );
  DrawStringCentered( String_Battery, 102 );
}


//=========================================================================
__myevic__ void ShowCheckUSB()
{
	DrawStringCentered( String_Check, 80 );
	DrawStringCentered( String_USB, 92 );
	DrawStringCentered( String_Adapter, 102 );
}


//=========================================================================
__myevic__ void ShowChargeError()
{
	DrawStringCentered( String_Charge, 88 );
	DrawStringCentered( String_Error, 102 );
}


//=========================================================================
__myevic__ void ShowImbBatts()
{
	DrawStringCentered( String_Imbalanced, 88 );
	DrawStringCentered( String_Batteries, 102 );
}


//=========================================================================
__myevic__ void ShowPowerCurve()
{
	DrawHLine( 10, 127,  60, 1 );
	DrawHLine( 10,  26,  60, 1 );
	DrawVLine( 10,  27, 126, 1 );
	DrawVLine( 60,  27, 126, 1 );

	int t = EditItemIndex; // * 5;
	int j = -1;

	for ( int i = 0; i < PWR_CURVE_PTS; ++i )
	{
		int t1, t2;

		t1 = dfPwrCurve[i].time;

		if ( ( i > 0 ) && ( t1 == 0 ) )
			break;

		if ( i < PWR_CURVE_PTS - 1 )
		{
			t2 = dfPwrCurve[i+1].time;

			if ( t2 == 0 ) t2 = 250;
		}
		else
		{
			t2 = 250;
		}

		DrawVLine(	10 + dfPwrCurve[i].power / 4,
					27 + 2 * t1, // / 5,
					27 + 2 * t2, // / 5,
					1 );

		if (( t2 > t ) && ( j < 0 ))
		{
			j = i;

			if ( t == t1 )
			{
				DrawFillRect(	10,
								27 + 2 * t1, // / 5,
								10 + dfPwrCurve[i].power / 4,
								28 + 2 * t1, // / 5,
								1 );
			}
		}
	}

	if ( !gFlags.edit_value || gFlags.draw_edited_item )
	{
		DrawImage( 6, 23 + EditItemIndex *2, 0xD4 ); //2
	}

	DrawImage( 12, 3, 0xAF );
	DrawValueRight( 44, 3, t, 1, 0x0B, 0 );
	DrawImage( 46, 3, 0x94 );

	DrawImage( 12, 13, 0xAB );
	DrawValueRight( 44, 13, dfPwrCurve[j].power, 0, 0x0B, 0 );
	DrawImage( 46, 13, 0xC2 );
}


//=========================================================================
/*
__myevic__ int SplashExists()
{
	int i, h, l;
	const image_t *img = Images[0xFF-1];

	h = img->height;
	l = img->width * h / 8;
	
	if ( img->width != 64 ) return 0;

	for ( i = 0 ; i < l ; ++i )
		if ( img->bitmap[i] ) break;

	return ( ( l && i < l ) ? 1 : 0 );
}
*/


__myevic__ void ShowSplash()
{
//         		  off	on	box
//dfStatus2.splash0         0	1	0
//dfStatus2.splash1         0	0	1

	//if ( gFlags.splash )
	//{
    
	DrawImage( 0, 0, 0xFF );
                
        if ( !dfStatus2.splash0 && dfStatus2.splash1 )
        {
                uint8_t strbuf[20];
                convert_string1( strbuf, BoxName );
                DrawStringCentered( strbuf, 115 );
        }
        
	//}
	//else
	//{
	//	MainView();
	//}
}
