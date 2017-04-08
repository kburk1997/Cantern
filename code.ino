#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

const int TOGGLE_MODE_PIN =2;
const int TOGGLE_INTERACTIVE_PIN= 3;
const int SOUND_SENSOR_PIN=A0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int mode=HIGH; //default - calm mode
int modeButtonState;
int lastModeButtonState=LOW;

unsigned long lastModeDebounceTime = 0;  // the last time the output pin was toggled
unsigned long modeDebounceDelay = 50;    // the debounce time; increase if the output flickers

int interactive=HIGH; //default - standalone

int interactiveButtonState;
int lastInteractiveButtonState=LOW;

unsigned long lastInteractiveDebounceTime = 0;  // the last time the output pin was toggled
unsigned long interactiveDebounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  Serial.begin(9600);
  pinMode(TOGGLE_MODE_PIN, INPUT);
  pinMode(TOGGLE_INTERACTIVE_PIN, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //readSound();
  selectColor();
  //rainbow(20);
  
  //
  //gradientRainbow(50);
}

int getMode(int oldMode){
  int newMode=oldMode;
    // read the state of the switch into a local variable:
  int reading = digitalRead(TOGGLE_MODE_PIN);
  //Serial.println(modeButtonState);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastModeButtonState) {
    // reset the debouncing timer
    lastModeDebounceTime = millis();
  }

  if ((millis() - lastModeDebounceTime) > modeDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != modeButtonState) {
      modeButtonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (modeButtonState == HIGH) {
        newMode= !oldMode;
      }
    }
  }
  lastModeButtonState = reading;
  //Serial.println(mode);
  delay(1);
  return newMode;
  //mode=digitalRead(TOGGLE_MODE_PIN);
  //interactive=digitalRead(TOGGLE_INTERACTIVE_PIN);
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue  
}

int getInteractive(int oldInteractive){
  int newInteractive=oldInteractive;
    // read the state of the switch into a local variable:
  int reading = digitalRead(TOGGLE_INTERACTIVE_PIN);
  Serial.println(interactiveButtonState);
  Serial.println(reading);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastInteractiveButtonState) {
    // reset the debouncing timer
    lastInteractiveDebounceTime = millis();
  }

  if ((millis() - lastInteractiveDebounceTime) > interactiveDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != interactiveButtonState) {
      interactiveButtonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (interactiveButtonState == HIGH) {
        newInteractive= !oldInteractive;
      }
    }
  }
  lastInteractiveButtonState = reading;
  Serial.println(newInteractive);
  delay(1);
  return newInteractive;
  //mode=digitalRead(TOGGLE_MODE_PIN);
  //interactive=digitalRead(TOGGLE_INTERACTIVE_PIN);
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue  
}

void selectColor(){
  
  for(uint16_t j=0;j<256;j++){
    mode=getMode(mode);
    interactive=getInteractive(interactive);
    //Serial.println(mode);
    lightUp(mode,interactive,j);
  }
}

void lightUp(int mode,int interactive, uint16_t j){
  uint16_t i;
  uint32_t color;

  if(mode==LOW){
    int loop_stop=1;
    uint32_t oldColor=color;
    int light_delay=20;
    if(interactive==LOW){
      light_delay=1;
       color = readSound(1);
       loop_stop=256;
    }
    /*for(int k=0;k<loop_stop;k++){
      
    }*/
    for(i=0; i< strip.numPixels(); i++) {
      if(interactive==HIGH){
        calmColor(i,j,interactive);
      }
      else{
        //calmColorInteractive(i,j,color-oldColor);
        strip.show();
        calmColorInteractive(i,j,color);
        
      }
    }
    strip.show();
    if(interactive==HIGH){
    
    delay(light_delay);
    }
  }
  else{
    for (int q=0; q < 3; q++) {
      if(interactive==HIGH){
        strip.show();
       }
      for (i=0; i < strip.numPixels(); i=i+3) {
        partyColor(i,j,q,interactive);    //turn every third pixel on
      }
      strip.show();

      if(interactive==HIGH){
        delay(40);
      }
      for (i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      if(interactive==HIGH){
        delay(10);
      }
    }
  }

}

uint32_t readSound(int precision){
  uint32_t sum=0;
  for(int i=0;i<=pow(2,precision);i++){
    int soundReading=analogRead(SOUND_SENSOR_PIN);
    sum+=(soundReading);
  }
  sum>>precision;
  sum=sum%256;
  Serial.println((sum));
  return (sum);
}

void calmColor(uint16_t i, uint16_t j, int interactive){
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
}

void calmColorInteractive(uint16_t i, uint16_t j, uint32_t color){
    Serial.println(color);
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & ((color%256))));
}

void partyColor(uint16_t i, uint16_t j, int q, int interactive){
  if(interactive==HIGH){
    strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
  }
  else{
    int color = readSound(1);
    Serial.println(color);
    strip.setPixelColor(i+q,Wheel((j+i)%(color%k)));
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


//Theatre-style crawling lights with rainbow effect
void gradientRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
       
        printf("%d", i+q);
        for (int r =0; r<(strip.numPixels()-i-q); r++){
          printf("%d",r);
           strip.setPixelColor(i+q+r, Wheel( (i+j+(40*r)) % 255));    //turn every third pixel on
        }
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        //strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
