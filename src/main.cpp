#include <FastLED.h>
#include <Arduino.h>
#include "lights.h"
#include "modeselection.h"

CRGB leds[NUM_LEDS];
int audio = A4;

long pre_react = 0; //RAW ANALOG SIGNAL 
long react = 0; //PROCESSED ANALOG SIGNAL

//DECAY LEDS
int decay = 0;
int decay_check = 0;

modeColorInfo lighting;

void setup()
{
  delay( 3000 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(10);

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();

  initializeLEDColorsStruct();

  // SERIAL AND INPUT SETUP
  Serial.begin(115200); 
  pinMode(audio, INPUT);
  Serial.println("Music Visualizer ON...");
}

void loop()
{
  int audio_input = analogRead(audio); 

  if (audio_input > 0)
  {
    audio_input /= 2;
    
    //Serial.println(audio_input);

    pre_react = audio_input;  //ACCOUNTING FOR TURNING OFF THE FIRST 4 LEDS OFF
    react = pre_react;
  }

  patternSelect(patternChoice);

  //remove LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
}

void colorAndBrightness(int red, int green, int blue, int b)
{
  FastLED.setBrightness(b);

  //FIRST 4 LEDS ALWAYS OFF
  for (int i = 0; i < NUM_LEDS; i++)
  {
     if (i < react)
       leds[i] = CRGB(red, green, blue);
     else
       leds[i] = CRGB(0,0,0);
  }
  FastLED.show();  
}

void patternSelect(int choice)
{
  if(choice == 1)
    equalizerPattern();
  else if (choice == 2)
    flashingPattern();
  else if (choice == 3)
    solidPattern();
  else if (choice == 4)
    multicolorSolidPattern();
  else if (choice == 5)
    timerBasedSolidPattern(delayForTimer);
}


//led patterns
void equalizerPattern()
{
  if (lighting.numColors <= 0)
    return;

  for (int i = 0; i < lighting.numColors; i++)
  {
    if (pre_react >= lighting.colorinfo[i].minAnalogInput && pre_react <= lighting.colorinfo[i].maxAnalogInput)
    {
      colorAndBrightness(lighting.colorinfo[i].red,lighting.colorinfo[i].green,lighting.colorinfo[i].blue, pre_react*1.5);
    }
  }
}

void flashingPattern()
{
  if (lighting.numColors <= 0)
    return;
  
  react = NUM_LEDS;
  
  for (int i = 0; i < lighting.numColors; i++)
  {
    if (pre_react >= lighting.colorinfo[i].minAnalogInput && pre_react <= lighting.colorinfo[i].maxAnalogInput)
    {
      colorAndBrightness(lighting.colorinfo[i].red,lighting.colorinfo[i].green,lighting.colorinfo[i].blue,pre_react*PS2_BR_MULTIPLIER);
    }
  }
}

void timerBasedSolidPattern(int delayChange)
{
  if (lighting.numColors <= 0)
    return;
        
  if (colorSelect == lighting.numColors)
    colorSelect = 0;

  FastLED.setBrightness(startingBrightness);
  for (int j = 4; j < NUM_LEDS; j++) 
    leds[j] = CRGB(lighting.colorinfo[colorSelect].red,lighting.colorinfo[colorSelect].green,lighting.colorinfo[colorSelect].blue);
  FastLED.show();
  
  startingBrightness += fadeDec;
  if (startingBrightness <= 6 || startingBrightness >= lighting.colorinfo[colorSelect].brightness) {
    fadeDec = -fadeDec;
  }
  delay(delayChange);
  colorSelect++;
}

void multicolorSolidPattern()
{
  FastLED.setBrightness(lighting.colorinfo[0].brightness);
  int ledspercolor = ((NUM_LEDS-4) / lighting.numColors) - 2;

  //FIRST 4 LEDS ALWAYS OFF
  for (int i = 4; i < NUM_LEDS; i++)
  {
    if (i < ledspercolor + 4)
      leds[i] = CRGB(lighting.colorinfo[2].red, lighting.colorinfo[2].green, lighting.colorinfo[2].blue);
    else if (i < (2*ledspercolor) + 4)
      leds[i] = CRGB(lighting.colorinfo[1].red, lighting.colorinfo[1].green, lighting.colorinfo[1].blue);
    else
      leds[i] = CRGB(lighting.colorinfo[0].red, lighting.colorinfo[0].green, lighting.colorinfo[0].blue);
  }
  FastLED.show();  
}

void solidPattern()
{
  if (lighting.numColors <= 0)
    return;
  
  react = NUM_LEDS;
  colorAndBrightness(lighting.colorinfo[0].red,lighting.colorinfo[0].green,lighting.colorinfo[0].blue,lighting.colorinfo[0].brightness);
}


void initializeLEDColorsStruct()
{
    //brightness levels
    int brightnessmax = 0;
    int brightnessmed = 0; 
    int brightnessmin = 0;
    
    if (mode == 1)
    {
      brightnessmax = mode1_max_brightness;
      brightnessmed = mode1_med_brightness;
      brightnessmin = mode1_min_brightness;
    }
    else if (mode == 2)
    {
      brightnessmax = mode2_max_brightness;
      brightnessmed = mode2_max_brightness;
      brightnessmin = mode2_max_brightness;
    }
    
    if (patternChoice == 1)
    {
      lighting.numColors = 3;
      setColorToMember(equalizerColors[0],brightnessmax,volumeLevel_high,MAX_ANALOG,0);
      setColorToMember(equalizerColors[1],brightnessmed,volumeLevel_medium,volumeLevel_high,1);
      setColorToMember(equalizerColors[2],brightnessmed,volumeLevel_low,volumeLevel_medium,2);
    }
    else if (patternChoice == 2)
    {
      lighting.numColors = 3;
      setColorToMember(flashingColor[0],brightnessmax,volumeLevel_high,MAX_ANALOG,0);
      setColorToMember(flashingColor[1],brightnessmed,volumeLevel_medium,volumeLevel_high,1);
      setColorToMember(flashingColor[2],brightnessmin,volumeLevel_low,volumeLevel_medium,2);
    }
    else if (patternChoice == 3)
    {
      lighting.numColors = 1;
      setColorToMember(solidColor,brightnessmed,volumeLevel_low,MAX_ANALOG,0);
    }
    else if (patternChoice == 4)
    {
      lighting.numColors = 3;
      setColorToMember(multicolorSolid[0],brightnessmed,volumeLevel_high,MAX_ANALOG,0);
      setColorToMember(multicolorSolid[1],brightnessmed,volumeLevel_medium,volumeLevel_high,1);
      setColorToMember(multicolorSolid[2],brightnessmed,volumeLevel_low,volumeLevel_medium,2);
    }
    else if (patternChoice == 5)
    {
      lighting.numColors = 3;
      setColorToMember(timerSolid[0],brightnessmed,volumeLevel_high,MAX_ANALOG,0);
      setColorToMember(timerSolid[1],brightnessmed,volumeLevel_medium,volumeLevel_high,1);
      setColorToMember(timerSolid[2],brightnessmed,volumeLevel_low,volumeLevel_medium,2);
    }
    else
    {
      lighting.numColors = 0;
    }
}

void setColorToMember(int rgb[], int brightness, int minAnalogInput, int maxAnalogInput, int index)
{
  if (index >= lighting.numColors)
    return;

  lighting.colorinfo[index].red = rgb[0]; 
  lighting.colorinfo[index].blue = rgb[1]; 
  lighting.colorinfo[index].green = rgb[2]; 
  lighting.colorinfo[index].brightness = brightness;
  lighting.colorinfo[index].minAnalogInput = minAnalogInput;
  lighting.colorinfo[index].maxAnalogInput = maxAnalogInput;
}