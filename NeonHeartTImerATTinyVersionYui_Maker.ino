#include <FastLED.h>
#define NUM_LEDS 19
#define LED_PIN 1

CRGB leds[NUM_LEDS];

uint8_t hue = 0; // uint8_t is a unsigned number between 0 to 255.
int timerHue;   //input time for how long you would like to set the timer for. 

// for timer
#define buttPin 3
//unsigned long Timestamp_Button_Pressed;
int state = 0;
long timeGap;
unsigned long lastButtonPress = 0;
unsigned long changeTime;
int AddedMinutes = 10;
int timerHueChange = 20;

void setup()
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(150);
  changeTime = millis();

  // for timer
  pinMode(buttPin, INPUT_PULLUP);
}

void loop()
{
  randomSeed(millis());
  if (state == 0 && digitalRead(buttPin) == HIGH)
  {
    waiting();
  }
  else if (state == 0 && digitalRead(buttPin) == LOW)
  {
    if (millis() - lastButtonPress > 500)
    {
      buttPressed();
    }
    else
    {
      waiting();
    }
  }
  else if (state == 1 && digitalRead(buttPin) == HIGH)
  {
    timerRunning();
  }
  else if (state == 1 && digitalRead(buttPin) == LOW)
  {
    interrupted();
  }
}

void timerRunning()
{
  allInOneColor();
  if (millis() - timeGap > AddedMinutes * 60000 && timerHue > 0)
  {
    timerHue = timerHue - timerHueChange;
    allInOneColor();
    timeGap = millis();
  }
  if (timerHue <= 0)
  {
    timerEnded();
  }
}

void buttPressed()
{
  while (digitalRead(buttPin) == LOW)
  {

  }
  timerDance();
  state = 1;
  timerHue = 103;
  timeGap = millis();
}

void interrupted()
{
  while (digitalRead(buttPin) == LOW)
  {
  }
  timerDance();
  state = 0;
  hue = 0;
}

void waiting()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(hue, 255, 255); // this fills all of the LEDs with a particular HSB color. The saturation and brightness is full
  }

  EVERY_N_MILLISECONDS(200)
  {
    hue++;
  }

  FastLED.show();
}

void allInOneColor()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(timerHue, 255, 255);
  }
  FastLED.show();
}

void timerEnded()
{
  while (digitalRead(buttPin) == HIGH)
  {
    for (int i = 0; i < 19; i++)
    {
      leds[i] = CHSV(0, 255, 255);
      leds[i + 1] = CHSV(0, 255, 255);

      FastLED.show();
      delay(10);
      FastLED.clear();
    }
  }
  state = 0;
  lastButtonPress = millis();
  waiting();
}

void timerDance()
{
  for (int a = 0; a < 2; a++)
  {
    for (int i = 0; i < 19; i++)
    {
      leds[i] = CHSV(i * random(0, 255), 255, 255);
      leds[i + 1] = CHSV(i * random(0, 255), 255, 255);
      FastLED.show();
      delay(5);
      FastLED.clear();
    }
  }
}
