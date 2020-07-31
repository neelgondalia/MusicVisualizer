#ifndef _LIGHTS_
#define _LIGHTS_

struct RGBinfo {
    int red;
    int green;
    int blue;
    int brightness;
    int minAnalogInput;
    int maxAnalogInput;
};

struct modeColorInfo {
    int numColors;
    RGBinfo colorinfo[10];
};

// LED LIGHTING SETUP
#define LED_PIN     6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB


//function prototypes
void initializeLEDColorsStruct();
void setColorToMember(int rgb[], int brightness, int minAnalogInput, int maxAnalogInput, int index);
void colorAndBrightness(int red, int green, int blue, int b);

//led patterns
void patternSelect(int choice);
void equalizerPattern();
void solidPattern();
void flashingPattern();
void multicolorSolidPattern();
void timerBasedSolidPattern(int delayChange);

#endif