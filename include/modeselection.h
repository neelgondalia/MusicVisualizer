#ifndef _MODESELECTION_
#define _MODESELECTION_

//************************************//
//      NUM LEDS TO TURN ON           //
#define NUM_LEDS   55                //
//************************************//

//************************************//
//              MODES                 //
//  choice 1: Normal                  //
//  choice 2: Night                   //
//************************************//
#define mode 1                        //
//************************************//

//************************************//
// BRIGHTNESS CORRESPONDING TO MODES  //
#define mode1_max_brightness 20       //
#define mode1_med_brightness 10       //
#define mode1_min_brightness 5       //
#define mode2_max_brightness 50       //
//************************************//

//************************************//
//          PATTERN CHOICE            //
//  choice 1: Equalizer               //
//  choice 2: Flashing                //
//  choice 3: Solid                   //
//  choice 4: Multicolor Solid        //
//  choice 5: Timer Solid             //
//************************************//  
#define patternChoice 2
               //
//************************************//

//************************************//
//  Delay between each color change   //           
//        FOR CHOICE 5 ONLY           //
#define delayForTimer 1000           //
int colorSelect = 0;                  //
int fadeDec = 3;                      //
int startingBrightness = 6;          //
//************************************//

//************************************//
// LEVEL OF ANALOG THAT IS CONSIDERED //           
//  HIGH MED OR LOW FOR CHOICE 1 & 2  //
#define MAX_ANALOG 1000               //
#define volumeLevel_high 50              //
#define volumeLevel_medium 28         //
#define volumeLevel_low 0             //
#define PS2_BR_MULTIPLIER 2.5           //
//************************************//

//*****************************************************************************************//
//                            COLORS CORRESPONDING TO EACH PATTERN                         //  
int equalizerColors[3][3] = {{245,176,66},{0,200,0},{130,70,0}};                           //     (WHITE,BLUE,GREEN)
int flashingColor[3][3] = {{100,0,0},{0,100,0},{0,0,100}};                                 //     (RED,BLUE,GREEN)
int solidColor[3] = {130,70,0};                                                             //     (RED)
int multicolorSolid[3][3] = {{250,250,250},{0,100,0},{0,0,100}};                           //     (WHITE,BLUE,GREEN)
int timerSolid[3][3] = {{100,0,0},{100,0,0},{100,0,0}};                                    //     (RED,RED,RED)
//*****************************************************************************************//

#endif