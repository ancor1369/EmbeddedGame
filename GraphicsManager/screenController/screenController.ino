#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     5 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define MISSILE_HEIGHT   8
#define MISSILE_WIDTH    16

String inputString = "";
int x,y=0;
int xt,yt=0;
int xmi, ymi = 0;
int scene ;
int Bitmap;


static const unsigned char PROGMEM logo_bmp[] = 
{
  0x00, 0x00, 0x04, 0x80, 0x0f, 0xc0, 0x0e, 0xe0, 0xcf, 0xf3, 0xc8, 0x03, 0xce, 0x03, 0xc8, 0x13, 
  0x8f, 0xf3, 0x9f, 0xf9, 0xaf, 0xf5, 0x9e, 0x79, 0xfe, 0x7f, 0x0f, 0xf0, 0x02, 0x20, 0x02, 0x20
};

static const unsigned char PROGMEM missile_bmp[] =
{
  0xff, 0xf0, 0xfb, 0xa8, 0xf9, 0x54, 0xfc, 0x2a, 0xf9, 0x54, 0xfb, 0xa8, 0xff, 0xf0, 0x00, 0x00
};

void setup() { 
  
  Serial.begin(115200);
   inputString.reserve(300);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) 
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }    
  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void loop() 
{
  
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void serialEvent();

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

 

for(;;) { // Loop forever...
  display.clearDisplay(); // Clear the display buffer
  serialEvent();
  // Draw each snowflake:
  for(f=0; f< NUMFLAKES; f++) {
    if(f==1)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }
    if(f==2)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], missile_bmp, MISSILE_WIDTH, MISSILE_HEIGHT, SSD1306_WHITE);
    }
  }

  display.display(); // Show the display buffer on the screen
  delay(15);        // Pause for 1/10 second    
  // Then update coordinates of each flake...
  for(f=0; f< NUMFLAKES; f++) {
      
      if(f==1)
      {
        icons[f][XPOS]   = x;
        icons[f][YPOS]   = y;    
      }
      if(f==2)
      {
        icons[f][XPOS]   = xmi;
        icons[f][YPOS]   = ymi;       
      }
    }
  }
}




void serialEvent() {  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();    
    // add it to the inputString:
    if(inChar != 'Z')
    {
        inputString += inChar; 
    }
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == 'Z') 
    {
      Serial.println(inputString);            
      sscanf(inputString.c_str(),"%d,%d,%d,%d",&scene,&Bitmap,&xt,&yt);
      //This needs to go into the queue and then we can make this program to work as expected
      //This is just a test to mive the characters as desired
      if(Bitmap==1)
      {
        x=xt;
        y=yt;
      }
      if(Bitmap==2)
      {
        xmi=xt;
        ymi=yt;
      }
      inputString = "";
      
    }
  }
}
