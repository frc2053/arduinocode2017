#include "FastLED.h"

#define UNDERGLOW_DATA_PIN 3
#define UNDERGLOW_NUM_LEDS 69
CRGB underglow[UNDERGLOW_NUM_LEDS];
int underglow_pattern = 6;
int underglow_clock = 0;

#define CYCLONE_DATA_PIN 4
#define CYCLONE_NUM_LEDS 34
#define CYCLONE_LED_SPEED 3
CRGB cyclone[CYCLONE_NUM_LEDS];
int cyclone_pattern = 6;
int cyclone_clock = 0;

#define HOPPER_DATA_PIN 5
#define HOPPER_NUM_LEDS 80
CRGB hopper[HOPPER_NUM_LEDS];
int hopper_pattern;
int hopper_clock = 0;

/*
 * underglow patterns
 *  u0x : off
 *  u1x : red
 *  u2x : blue
 *  u3x : pulsing orange
 *  u4x : chasing orange
 *  u5x : rainbow
 *  u6x : chasing rainbow
 *  u7x : spiral orange scheme
 *  
 * cyclone patterns
 *  c0x : off
 *  c1x : orange
 *  c2x : chasing orange
 *  c3x : chasing white
 *  c4x : chasing "screamin" yellow
 *  c5x : rainbow
 *  c6x : chasing rainbow
 *  
 * hopper patterns
 *  h0x : off
 *  h1x : 
 *  h2x :
 *  h3x :
 *  h4x : 
 *  h5x : rainbow
 *  h6x : chasing rainbow
 */

int pattern_id = -1;
int prev_pattern_id;

void setup()
{
  FastLED.addLeds<NEOPIXEL, UNDERGLOW_DATA_PIN>(underglow, UNDERGLOW_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, CYCLONE_DATA_PIN>(cyclone, CYCLONE_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, HOPPER_DATA_PIN>(hopper, HOPPER_NUM_LEDS);

  Serial.begin(9600);
  Serial.setTimeout(1000);
}

void loop()
{
  prev_pattern_id = pattern_id;
  while (Serial.available() > 0)
  {
    pattern_id = Serial.parseInt();
  }

  
  if (pattern_id >= 0 && pattern_id < 10)
    underglow_pattern = pattern_id;
  else if (pattern_id >= 10 && pattern_id < 20)
    cyclone_pattern = pattern_id - 10;
  else if (pattern_id >= 20 && pattern_id < 30)
    hopper_pattern = pattern_id - 20;
  

  if (prev_pattern_id != pattern_id)
    setupPatterns(pattern_id);
  runPatterns();

  FastLED.show();
  delay(50);
}

void runPatterns() 
{
  //underglow
  switch (underglow_pattern)
  {
    case 0: //off
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
      {
        underglow[i] = CRGB::Black;
      } break;
      
    case 1: //red
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
      {
        underglow[i].setRGB(127, 0, 0);
      } break;
      
     case 2: //blue
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
      {
        underglow[i].setRGB(0, 0, 127);
      } break;
      
     case 3: // pulse orange
      underglow_clock++;
      if (underglow_clock >= 40)
        underglow_clock = 0;
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
      {
        if (underglow_clock < 20)
          underglow[i].setHSV(24, 255, (10 * underglow_clock) + 30);
        else if (underglow_clock >= 20)
          underglow[i].setHSV(24, 255, 200 - (10 * (underglow_clock - 20)));
      } break;

      case 4: //chase orange
        underglow_clock++;
        if (underglow_clock >= UNDERGLOW_NUM_LEDS - 1)
          underglow_clock = 1;
          
        underglow[underglow_clock].setHSV(24, 255, 230);
        underglow[underglow_clock-1].setHSV(24, 255, 127);
        break;

      case 5: //rainbow
        if (underglow_clock >= 255)
          underglow_clock = 0;
        underglow_clock += 2;
        for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
        {
          underglow[i].setHue(underglow_clock);
        } break;

      case 6: // chasing rainbow
        underglow_clock += 2;
        if (underglow_clock >= UNDERGLOW_NUM_LEDS)
          underglow_clock = 0;
        for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
        {
          if ((i + underglow_clock) < UNDERGLOW_NUM_LEDS)
            underglow[i + underglow_clock].setHue((i * floor(255/UNDERGLOW_NUM_LEDS)) + 1);

          else if ((i + underglow_clock) > UNDERGLOW_NUM_LEDS)
            underglow[(i + underglow_clock - UNDERGLOW_NUM_LEDS) - 1].setHue(i * floor(255/UNDERGLOW_NUM_LEDS));
        }
        break;

      case 7: //orange chase scheme
        underglow_clock++;
        if (underglow_clock > UNDERGLOW_NUM_LEDS * 3)
          underglow_clock = 0;
        if (underglow_clock < UNDERGLOW_NUM_LEDS)
          underglow[underglow_clock] = 0xFF3300;
        else if (underglow_clock >= UNDERGLOW_NUM_LEDS && underglow_clock < UNDERGLOW_NUM_LEDS * 2)
          underglow[underglow_clock - UNDERGLOW_NUM_LEDS] = 0xFF6600;
        else if (underglow_clock >= UNDERGLOW_NUM_LEDS * 2)
          underglow[underglow_clock - UNDERGLOW_NUM_LEDS * 2] = 0xFF9900;
      
      
  }

  //cyclone
  switch (cyclone_pattern)
  {
    case 0: // off
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i] = CRGB::Black;
      }break;
      
    case 1: //solid orange
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i].setHSV(24, 255, 180);
      } break;

    case 2: // chase orange
      cyclone_clock += CYCLONE_LED_SPEED;
      if (cyclone_clock > CYCLONE_NUM_LEDS)
         cyclone_clock = 0;
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i].setHSV(24, 255, 130);
      }
      cyclone[cyclone_clock].setHSV(24, 255, 230);
      if (cyclone_clock > 0)
        cyclone[cyclone_clock-1].setHSV(24, 255, 210);
      if (cyclone_clock > 1)
        cyclone[cyclone_clock-2].setHSV(24, 255, 190);
      if (cyclone_clock > 2)
        cyclone[cyclone_clock-3].setHSV(24, 255, 170);
      if (cyclone_clock > 3)
        cyclone[cyclone_clock-4].setHSV(24, 255, 150);
      break;

    case 3: // chase white
      cyclone_clock += CYCLONE_LED_SPEED;
      if (cyclone_clock > CYCLONE_NUM_LEDS)
         cyclone_clock = 0;
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i].setRGB(80, 80, 60);
      }
      cyclone[cyclone_clock].setRGB(255, 255, 230);
      if (cyclone_clock > 0)
        cyclone[cyclone_clock-1].setRGB(220, 220, 220);
      if (cyclone_clock > 1)
        cyclone[cyclone_clock-2].setRGB(190, 190, 160);
      if (cyclone_clock > 2)
        cyclone[cyclone_clock-3].setRGB(150, 150, 130);
      if (cyclone_clock > 3)
        cyclone[cyclone_clock-4].setRGB(100, 100, 80);
      break;

    case 4: // chase "screamin" yellow
      cyclone_clock += CYCLONE_LED_SPEED;
      if (cyclone_clock > CYCLONE_NUM_LEDS)
         cyclone_clock = 0;
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i].setHSV(39, 255, 156);
      }
      cyclone[cyclone_clock].maximizeBrightness();
      break;

    case 5: //rainbow
        cyclone_clock += 2;
        if (cyclone_clock >= 255)
          cyclone_clock = 0;

        for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
        {
          cyclone[i].setHue(cyclone_clock);
        } break;

      case 6: // chasing rainbow
        cyclone_clock += 2;
        if (cyclone_clock >= CYCLONE_NUM_LEDS)
          cyclone_clock = 0;
        for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
        {
          if ((i + cyclone_clock) < CYCLONE_NUM_LEDS)
            cyclone[i + cyclone_clock].setHue((i * floor(255/CYCLONE_NUM_LEDS)) + 1);

          else if ((i + cyclone_clock) > CYCLONE_NUM_LEDS)
            cyclone[(i + cyclone_clock - CYCLONE_NUM_LEDS) - 1].setHue(i * floor(255/CYCLONE_NUM_LEDS));
        }
        break;
  }

  //hopper

  switch (hopper_pattern)
  {
    case 0: //off
      for (int i = 0; i < HOPPER_NUM_LEDS; i++)
      {
        hopper[i] = CRGB::Black;
      } break;

    case 1:
      break;

    case 5: //rainbow
        hopper_clock += 2;
        if (hopper_clock >= 255)
          hopper_clock = 0;

        for (int i = 0; i < HOPPER_NUM_LEDS; i++)
        {
          hopper[i].setHue(hopper_clock);
        } break;

      case 6: // chasing rainbow
        hopper_clock += 2;
        if (hopper_clock >= HOPPER_NUM_LEDS)
          hopper_clock = 0;
        for (int i = 0; i < HOPPER_NUM_LEDS; i++)
        {
          if ((i + hopper_clock) < HOPPER_NUM_LEDS)
            hopper[i + hopper_clock].setHue((i * floor(255/HOPPER_NUM_LEDS)) + 1);

          else if ((i + hopper_clock) > HOPPER_NUM_LEDS)
            hopper[(i + hopper_clock - HOPPER_NUM_LEDS) - 1].setHue(i * floor(255/HOPPER_NUM_LEDS));
        }break;

  }
    
}








void setupPatterns(int pattern)
{
  switch (pattern)
  {
    case 3: //orange underglow pulse
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
      {
        underglow[i].setHSV(24, 255, 127);
      }
      underglow_clock = 15;
      break;
    case 4: //orange underglow chase
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
      {
        underglow[i].setHSV(24, 255, 127);
      }
      underglow_clock = 0;
      break;
    case 5: //rainbow underglow
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
        {
          underglow[i].setHSV(1, 255, 255);
        }
      underglow_clock = 0;
      break;
    case 6: //rainbow chase underglow
      for (int i = 0; i < UNDERGLOW_NUM_LEDS; i++)
        {
          underglow[i] = CRGB::Black;
        }
      underglow_clock = 0;
      break;
    case 12: // orange chase cyclone
      cyclone_clock = 0;
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i].setHSV(24, 255, 130);
      }break;

    case 13: // white chase cyclone
      cyclone_clock = 0;
      for (int i = 0; i < CYCLONE_NUM_LEDS; i++)
      {
        cyclone[i].setRGB(127, 127, 127);
      }break;
  }
}
















