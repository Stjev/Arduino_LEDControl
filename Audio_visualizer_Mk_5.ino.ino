  /*
  * This version works the same as Mk 1, except this one uses the FFT (Fast Fourier Transform) to map the colors / brightness to the leds.
  */

  /**
   * Leds
   */
  #include <FastLED.h>
  
  #define JACK A0
  #define NUM_LEDS 150
  #define DATA_PIN_1 5
  #define DATA_PIN_2 6
  #define SENSITIVITY 15 // How sensitive are the leds
  #define MAX_DATA_COUNT 1

  // COLOR CYCLE MODE
  #define CYCLESPEED 0.5

  // STAR MODE
  #define SPEED 30000 // The speed the stars get brighter / dimmer, lower is faster
  #define STARCOUNT_PER_STRIP 20 // The amount of stars displayed at a time

  struct CRGB leds1[NUM_LEDS];
  struct CRGB leds2[NUM_LEDS];

  /**
   * FFT
   * 
   * Bass -> bands 0 - 9
   * Mid -> bands 10 - 63
   * Treble -> bands 64 - 127
   */
  #include "fix_fft.h"
  
  const int n = 7;
  const int bands = pow(2, n);
  
  char im[bands], data[bands];
  
  int b = 0, val;
  static long tt=0;

  /**
   * Custom
   */   
  float bass = 0;

  int brightness = 255;
  uint8_t mode = 4;
  uint8_t data_counter = 0;
  
  float red = 255.0, green = 0.0, blue = 0.0;
   
   
  /**
   * Star leds
   */    
  // First element in array is the led active
  // Second is the brightness of that led
  // Third is how fast the led is getting brighter
  int dots1[STARCOUNT_PER_STRIP][3];
  int dots2[STARCOUNT_PER_STRIP][3];

  /**
   * Static
   */
   CRGB color = CRGB(140, 40, 0);

  /**
   * Runs only once when the arduino starts
   */
  void setup() {    
    pinMode(JACK, INPUT);
    
    FastLED.addLeds<WS2812, DATA_PIN_1, GRB>(leds1, NUM_LEDS);
    FastLED.addLeds<WS2812, DATA_PIN_2, GRB>(leds2, NUM_LEDS);
    FastLED.setBrightness(brightness);
    for(int i = 0; i < STARCOUNT_PER_STRIP / 2; i++){
      dots1[i][0] = -1;
      dots2[i][0] = -1;
    }

    Serial.begin(9600);
  }

  /**
   * Infinitely loops
   */
  void loop(){        
    bluetooth_settings();
    
    // Mode 1: The leds react to the music
    if(mode == 0){
      soundReactive();
    }
    // Mode 2: Star mode
    else if (mode == 1) {      
      starmode();
    } 
    else if (mode == 2) {
      staticMode();
    }
    else if (mode == 3) {
      colorCycleMode();
    }
  }

  /**
   * This is responsible for the communication with the bluetooth module
  */
  void bluetooth_settings(){
    // Check if there is data available and it is not 10 (since 10) always gets send right after data
    if(Serial.available()){
      int b = Serial.read();

      // check if this is the first byte sent
      if(data_counter == 0) {
        setMode(b);
      } else if(data_counter == 1) {
        setDataBrightness(b);
      }

      // update the data_counter
      if(data_counter < MAX_DATA_COUNT) {
        data_counter++;
      } else {
        data_counter = 0; // reset the datacounter
      }
      
      Serial.println(b);      
    }
  }

  // this will set the mode to the incoming byte
  void setMode(int b) {
    if(b >= 0 && b < 4) { mode = (b & 0xFF); }
   
    resetLEDS();
  }

  // set the brightness of the to the incoming byte
  void setDataBrightness(int b){
    brightness = b;

    FastLED.setBrightness(brightness);
  }

  void resetLEDS() {
    for(int i = 0; i < NUM_LEDS; i++) {
      leds1[i] = CRGB::Black;
      leds2[i] = CRGB::Black;
    }
  }

  /**  
   * Sound reactive Mode
   */
  void soundReactive(){

    // Get a beautified version of the lowest band
    /*bass = min(nextValue(analogRead(JACK) * 1.5, bass), 150);
    
    drawLeds();*/
    if (b < bands) {
        // Get (a beautified version of) the value of the audio jack
        data[b] = nextValue(analogRead(JACK), data[b] - bands * 4);
        im[b] = 0;
        b++;
      } 
      else {
        // Run the fft algorithm
        fix_fft(data,im,n,0);
    
        if (millis() - tt > 100) {
            tt = millis();
        }
  
        // Get the absolute value
        for (int i = 0; i < bands / 2; i++) {
            data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
        }
        
        // Get a beautified version of the lowest band
        bass = min(nextValue(getAverageTone(0, 9, data), bass), 150);
        
        drawLeds();
        
        b = 0;
      }
  }

  // Calculate the next value for the band, this function will smoothen out the graph.
  float nextValue(float data, float band) {
    if(data > band) {
      return data;
    } else {
      return band - band / 50;  
    }
  }

  // Method to draw the leds
  void drawLeds() {
    FastLED.clear(); // Turn all leds off

    FastLED.setBrightness(map(bass, 0, 150, 20, brightness));

    if(bass < 1) bass = 0;

    // Turn the leds on by bass on strip 1
    for(int i = 0; i < bass; i++) { 
      // Color the leds, more blue at the bottom and more red at the top.
      leds1[i] = CRGB(
            map(i, 0, bass, 0, 255),    // Red             
            0,                          // Green
            map(i, 0, bass, 255, 0)     // Blue
            );  
    } 

    // Turn the leds on by bass on strip 2
    for(int i = 0; i < bass; i++) { 
      // Color the leds, more blue at the bottom and more red at the top.
      leds2[i] = CRGB(
            map(i, 0, bass, 0, 255),    // Red             
            0,                          // Green
            map(i, 0, bass, 255, 0)     // Blue
            );  
    } 
    
    FastLED.show();
  }

  // Gets the avarage of bands
  float getAverageTone(int from, int to, char data[]) {
    float sum = 0; 
    
    for(int i = from; i <= to; i++) {
      sum += data[i] * SENSITIVITY;
    }

    return sum / (to - from + 1);
  }

  /**
   * Star Mode
   */
  void starmode(){
    for(int i = 0; i < STARCOUNT_PER_STRIP; i++){
        // Generate stars for strip 1
        if(dots1[i][0] == -1){
          newRandomStar(i, 1);
        } else {
          // The led is at its max brightness
          if(dots1[i][1] > SPEED){
             dots1[i][2] *= -1;
          }

          dots1[i][1] += dots1[i][2];
          leds1[dots1[i][0]] = CRGB(0, 0,
              map(dots1[i][1], 0, SPEED, 0, brightness)
          );
          
          // The led dimmed out, reset it
          if (dots1[i][1] < 1) {
             newRandomStar(i, 1);
          }
        }
        
        // Generate stars for strip 2
        if(dots2[i][0] == -1){
          newRandomStar(i, 2);
        } else {
          // The led is at its max brightness
          if(dots2[i][1] > SPEED){
             dots2[i][2] *= -1;
          }

          dots2[i][1] += dots2[i][2];
          leds2[dots2[i][0]] = CRGB(0, 0,
              map(dots2[i][1], 0, SPEED, 0, brightness)
          );
          
          // The led dimmed out, reset it
          if (dots2[i][1] < 1) {
             newRandomStar(i, 2);
          }
        }
      }

      FastLED.show();
  }

  // Generates random stars for the starmode
  void newRandomStar(int pos, int strip){
    // I know passing the array would be more beautiful, but it bugs tf out
    if(strip == 1){
      dots1[pos][0] = random(pos * NUM_LEDS / STARCOUNT_PER_STRIP, min((pos+1) * NUM_LEDS / STARCOUNT_PER_STRIP, NUM_LEDS - 1));
      dots1[pos][1] = 1;
      dots1[pos][2] = random(50, 150);
    } else if (strip == 2) {
      dots2[pos][0] = random(pos * NUM_LEDS / STARCOUNT_PER_STRIP, min((pos+1) * NUM_LEDS / STARCOUNT_PER_STRIP, NUM_LEDS - 1));
      dots2[pos][1] = 1;
      dots2[pos][2] = random(50, 150);
    }
    
  }

  /**
   * Static Mode
   */
   void staticMode() {
      for(int i = 0; i < NUM_LEDS; i++) {
        leds1[i] = color;
        leds2[i] = color;
      } 
      FastLED.show();
   }

   /**
    * Color cycle Mode
    */
    void colorCycleMode(){      
      if(red >= 255) {
        if(blue > 0){
          blue -= CYCLESPEED;
        } else if(green >= 255) {
          red -= CYCLESPEED;  
        } else {
          green += CYCLESPEED;
        }
      } else if(green >= 255) {
        if(red > 0) {
          red -= CYCLESPEED;
        } else if (blue >= 255){
          green -= CYCLESPEED;
        } else {
          blue += CYCLESPEED;
        }
      } else if (blue >= 255) {
        if(green > 0) {
          green -= CYCLESPEED;
        } else if (red >= 255) {
          blue -= CYCLESPEED;
        } else {
          red += CYCLESPEED;
        }
      }
      
      for(int i = 0; i < NUM_LEDS; i++) {
        leds1[i] = CRGB(red, green, blue);
        leds2[i] = CRGB(red, green, blue);
      } 
      FastLED.show();
    }
