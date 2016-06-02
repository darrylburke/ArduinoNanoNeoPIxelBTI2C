
#include <stdlib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

long strtol (const char *__nptr, char **__endptr, int __base);

//i2c stuff
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

String inString = "";


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN           6

//Pins for debug more..  short 3 & 4 and demo mode starts automatically.. if not then it waits for serial input
//
#define LPIN          3
#define IPIN          4
#define HPIN          5
//demo modes
#define CYCLONE       0
#define COP           1
#define STROBE        2
#define COPCAR        3
#define RAINBOW       4
#define RAINBOWS      5
#define FIRE          6
#define WORM          7
int currentmode = 0;
// How many NeoPixels are attached to the Arduino?  you can update this via the serial command as well
int NUMPIXELS =     10;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel pixelss you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//https://github.com/adafruit/Adafruit_NeoPixel

int delayval = 75; // delay for half a second
int intensity=50;
boolean demomode=false;
int democounter=0;

//String tmpinput="";
boolean run = false;
//String X = "";
long cr,cg,cb;


void setup() {
   pinMode(13, OUTPUT);
   // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);
 
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
 
   pixels.begin(); // This initializes the NeoPixel library.
   pinMode(1, OUTPUT); //on board LED used for diag if you need it
   
   pinMode(HPIN, OUTPUT);
   digitalWrite(HPIN,HIGH);
   pinMode(LPIN, OUTPUT);
   digitalWrite(LPIN,LOW);

   pinMode (IPIN,INPUT);
   //testmode = digitalRead(IPIN);
   Serial.begin(9600);
   Serial.setTimeout(500);
   
   stop();
   setall(0,NUMPIXELS,255,255,255,true);
   delay(2000); // Delay for a period of time (in milliseconds).

   stop();
}

void setall(int start, int end, int r,int g, int b,boolean refresh) {
 
  for(int i=start;i<=end;i++){
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
   
  }
   
   if (refresh) {
      pixels.show(); // This sends the updated pixel color to the hadware.
    }
}
// used when you delay the rehresh to send multiple commands to the LED
void refreshleds(){
  pixels.show();
}

void blinkOnboardLED(){
  digitalWrite(1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(1, LOW);    // turn the LED off by making the voltage LOW
  delay(100);  
}
// use dto start any background demo mode which requires cycling of the leds
void start() {
  run=true;
}
//stop any demo mode
void stop() {
  run=false;
  currentmode=0;
  setall(0,NUMPIXELS,0,0,0,true);
}
// get the colors from an serial input in the fomat RRGGBB in hex
void getColors(String gcinput){

  if (gcinput.length() != 6)
  {
    Serial.print("Length != 6 [");
    Serial.print(gcinput);
    Serial.println("]");
    return;
  }

  long number = strtol( &gcinput[0], NULL, 16);
  
  cr = number >> 16;
  cg = number >> 8 & 0xFF;
  cb = number & 0xFF;

  Serial.println("Colors:");
  Serial.print("Green:[");
  Serial.print(cr);
  Serial.print("] Red:[");
  Serial.print(cg);
  Serial.print("] Blue:[");
  Serial.print(cb);
  Serial.println("]");
}
// cycle through on  demo mode pattern
void cycle() {
// For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  //setall(0,50,0)
  
  if (currentmode == CYCLONE){
      
      
      for(int i=0;i<NUMPIXELS;i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        setall(0,NUMPIXELS,0,intensity,0,true);
        pixels.setPixelColor(i, pixels.Color(0,0,intensity)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(delayval/2); // Delay for a period of time (in milliseconds).
      }
      //delay(1000);
      for(int i=0;i<NUMPIXELS;i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        setall(0,NUMPIXELS,0,intensity,0,true);
        pixels.setPixelColor(NUMPIXELS-1-i, pixels.Color(0,0,intensity)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(delayval/2); // Delay for a period of time (in milliseconds).
      }
  }else if (currentmode == COPCAR){

     copcar();
     
  }else if (currentmode == RAINBOW){

     
   rainbow(20);
  
     
  }else if (currentmode == RAINBOWS){
    
    rainbowCycle(15);
 
     
  }else if (currentmode == WORM){

     worm();
     
  }else if (currentmode == FIRE){

     fire();
     
  } else if (currentmode == STROBE){

    
      setall(0,NUMPIXELS,0,0,0,true);
      for(int i=0;i<NUMPIXELS;i++){

        for (int x=0;x<17;x++) {
          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          setall(i,i,x*15,x*15,x*15,true);
          delay(15); // Delay for a period of time (in milliseconds).
        }
        for (int x=16;x>=0;x--) {
          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          setall(i,i,x*15,x*15,x*15,true);
          delay(15); // Delay for a period of time (in milliseconds).
        }

       
      }

      
  } else if (currentmode == COP) {

    
    boolean first = true;
    
    for (int x=0;x<NUMPIXELS;x++){
      if (first) {
        pixels.setPixelColor(x, pixels.Color(0,intensity,0)); 
        first = false;  
      } else {
        pixels.setPixelColor(x, pixels.Color(0,0,intensity)); 
        first = true;
      }
    }
    
    pixels.show();
    delay(delayval*2);
    first=false;
    
    for (int x=0;x<NUMPIXELS;x++){
      if (first) {
        pixels.setPixelColor(x, pixels.Color(0,intensity,0)); 
        first = false;  
      } else {
        pixels.setPixelColor(x, pixels.Color(0,0,intensity)); 
        first = true;
      }
    }
    pixels.show();
    delay(delayval*2);
    
    
  }
  
}
// confert string to long int
long sToI(String in){
return (strtol( &in[0], NULL, 16));
}

void setLEDs(String slinput){

      
 int llen=slinput.length();
 int groups = llen / 14;
 Serial.print("Setting LEDs:[");
 Serial.print(slinput);
 Serial.print("] Len:[");
 Serial.print(llen);
 Serial.print("] Groups:[");
 Serial.print(groups);
 Serial.println("]");
  for (int x=0; x< groups;x++) {

      String group = slinput.substring(x*15,x*15+14);
      Serial.print("Group: [");
      Serial.print(x);
      Serial.print("] ");
      Serial.println(group);

      String tmpdelay=group.substring(0,4);
     // String tmpLEDS=group.substring(4,14);

      String startled=group.substring(4,6);
      String endled=group.substring(6,8);
      String colors=group.substring(8,14);
      int mydelay = group.substring(0,4).toInt();
      
      
      Serial.print("Delay:[");
      Serial.print(mydelay);
      Serial.print("] Tmp:[");
      Serial.print(tmpdelay);
      Serial.println("]");
      
  
    

      
      Serial.print("Start LED:[");
      Serial.print(startled);
      Serial.print("] End LED:[");
      Serial.print(endled);
      Serial.print("] Green:[");
      Serial.print(colors.substring(0,2));
      Serial.print("] Red:[");
      Serial.print(colors.substring(2,4));
      Serial.print("] Blue:[");
      Serial.print(colors.substring(4,6));
      Serial.println("]");

      delay(mydelay);

      long StartLED = sToI(startled);
      long EndLED = sToI(endled);
      Serial.print("Requesting Colors:[");
      Serial.print(slinput);
      Serial.print("] S:[");
      Serial.print(StartLED);
      Serial.print(" / ");
      Serial.print(startled);
      Serial.print("] E:[");
      Serial.print(EndLED);
      Serial.print(" / ");
      Serial.print(endled);
      Serial.print("] C:[");
      Serial.print(colors);
      Serial.println("]");
      getColors(colors);
      Serial.print("Setting LEDs [");
      Serial.print(StartLED);
      Serial.print("] to [");
      Serial.print(EndLED);
      Serial.print("] as G[");
      Serial.print(cr);
      Serial.print("] R[");
      Serial.print(cg);
      Serial.print("] B[");
      Serial.print(cb);
      Serial.println("]");
      setall(StartLED,EndLED,cr,cg,cb,false);
      refreshleds();
  }
  
 
}
void processInput(String input){
  
   Serial.print("Read:[");
   Serial.print(input);
   Serial.println("]");
   if (input.startsWith("?")){
      Serial.println(" Usage: End all Commands with '|' ");
      Serial.println(" ARRGGBB - Set all pixels to the same RGB values");
      Serial.println(" A000000 - Set all pixels to Off");
      Serial.println(" COP - Police Lights Mode");
      Serial.println(" COPCAR - Police Car Lights Mode");
      Serial.println(" WORM - Worm Mode");
      Serial.println(" CYCLONE - Cyclone Lights Mode");
      Serial.println(" STROBE - Strobe Lights Mode");
      Serial.println(" RAINBOWS - Rainbow Lights Mode - Individual Pixels");
      Serial.println(" RAINBOW - Rainbow Lights Mode - all Pixels");
      Serial.println(" FIRE - Fire Lights Mode");
      Serial.println(" LDDDDPSPERRGGBB[DDDDPSPERRGGBB] - Set Pixel Mode");
      Serial.println("  Groups of 10 Digits:");
      Serial.println("  DDDD = Miliseconds to delay before execution");      
      Serial.println("  PS = Start Pixel");
      Serial.println("  PE = End Pixel");
      Serial.println("  RR = Red Color");
      Serial.println("  GG = Green Color");
      Serial.println("  BB = Blue Color");
 
      
   }else if (input.startsWith("A")){
      stop();
      Serial.println("Seting All LEDs");
      if (input.length() != 8){
        Serial.print("Invalid: A length must be 7. found ");
        Serial.println(input.length());
        
        return;
      }
      getColors(input.substring(1,7));
      setall(0,NUMPIXELS,cr,cg,cb,true);
     
   } else if (input.startsWith("L")){
      if (run==true){
        stop();
      }
      String newinput=input.substring(1,input.length());//String Leading L
      int len = newinput.length();
      Serial.print("Command Len:");
      Serial.println(len);
      int rem = len % 15;
      Serial.print("Remainder:");
      Serial.println(rem);
      if (rem != 0){
        Serial.println("Invalid parameters != 15");
        return;
      }
      
      setLEDs(newinput);

   } else if (input.startsWith("COPCAR")){
        Serial.println("Starting COPCAR");
        currentmode=COPCAR;
        start();
       
   }else if (input.startsWith("FIRE")){
        Serial.println("Starting FIRE");
        currentmode=FIRE;
        start();
       
   }else if (input.startsWith("CYCLONE")){
        Serial.println("Starting CYCLONE");
        currentmode=CYCLONE;
        start();
       
   }else if (input.startsWith("WORM")){
        Serial.println("Starting WORM");
        currentmode=WORM;
        start();
        
   }else if (input.startsWith("COP")){
        Serial.println("Starting COP");
        currentmode=COP;
        start();
        
   }else if (input.startsWith("STROBE")){
        Serial.println("Starting STROBE");
        currentmode=STROBE;
        start();
       
   }else if (input.startsWith("RAINBOWS")){
        Serial.println("Starting RAINBOWS");
        currentmode=RAINBOWS;
        start();
       
   }else if (input.startsWith("RAINBOW")){
        Serial.println("Starting RAINBOW");
        currentmode=RAINBOW;
        start();
       
   }
   
   else if (input.startsWith("C")){
      int pixelcount = input.substring(1,2).toInt();
      Serial.print("Setting LED Count.[");
      Serial.print(pixelcount);
      Serial.println("]");
      pixels.updateLength(pixelcount);
      input="";
   }
  

   
}


void copcar(){


  for (int i = (pixels.numPixels() / 2); i < pixels.numPixels(); i = i + 1) {
        pixels.setPixelColor(i, 0);
      }
      
  for (int x=0;x<12;x++){
     
        setall(0,(pixels.numPixels() / 2)-1,0,0,255,true);
        delay(50);
        setall(0,(pixels.numPixels() / 2)-1,0,0,0,true);
        delay(50);
        setall(0,(pixels.numPixels() / 2)-1,0,0,255,true);
        delay(50);
        setall(0,(pixels.numPixels() / 2)-1,0,0,0,true);
        delay(50);
        setall(0,(pixels.numPixels() / 2)-1,0,0,255,true);
        delay(50);
        setall(0,(pixels.numPixels() / 2)-1,0,0,0,true);
        delay(250);
        setall(pixels.numPixels() / 2,NUMPIXELS,0,255,0,true);
        delay(50);
        setall(pixels.numPixels() / 2,NUMPIXELS,0,0,0,true);
        delay(50);
        setall(pixels.numPixels() / 2,NUMPIXELS,0,255,0,true);
        delay(50);
        setall(pixels.numPixels() / 2,NUMPIXELS,0,0,0,true);
        delay(50);
        setall(pixels.numPixels() / 2,NUMPIXELS,0,255,0,true);
        delay(50);
        setall(pixels.numPixels() / 2,NUMPIXELS,0,0,0,true);
        delay(50);
  }
  delay(500);
//  ArrowLeft(pixels.Color(0, 0, 255), 100); // Blue
//  ArrowRight(pixels.Color(0, 255, 0), 100); //Red
//
//  // These are side to side or wig/wag
//  WigWag(pixels.Color(0,255, 0), 200); // Red
//  WigWag(pixels.Color(255, 255, 255), 30); // Blue faster
//  WigWag(pixels.Color(255, 255, 255), 30); // Blue faster
//  WigWag(pixels.Color(255, 255, 255), 30); // Blue faster
//  WigWag(pixels.Color(255, 255, 255), 30); // Blue faster
//  WigWag(pixels.Color(0, 0, 255), 63); // Blue faster
//  WigWag(pixels.Color(0, 0, 255), 127); // Blue medium
//  WigWag(pixels.Color(0, 0, 255), 255); // Blue slowest
//  ClearLights();
//  delay(1000);
//  // This is a 2 color wigwag
//  WigWag2(pixels.Color(0, 0, 255), pixels.Color(0, 255, 0), 200); // Blue and Red
//  ClearLights();
//  delay(1000);
//
//  // Blinks the outer most lights
//  BlinkOuter(pixels.Color(0, 0, 255), 200,15); //Blue
//  BlinkOuter(pixels.Color(0, 0, 255), 50,50); //Blue faster
//  //BlinkOuter(pixels.Color(255, 255, 0), 200,10); //Yellow
//  ClearLights();
//  delay(1000);
//
//  // Turns the outer most lights on
//  OnOuter(pixels.Color(0,63, 0)); // Red 25%
//  OnOuter(pixels.Color(0,127, 0)); // Red 50%
//  OnOuter(pixels.Color(0,191, 0)); // Red 75%
//  OnOuter(pixels.Color(0,255, 0)); // Red 100%
//  ClearLights();
//  delay(1000);
//  OnOuter(pixels.Color(0, 0, 255)); // Blue
//  //OnOuter(pixels.Color(255, 255, 0)); // Green
//  // Turns off all lights
  ClearLights();
  delay(5000);
}

void ArrowRight(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 4; j++) { // The j<# determines how many cycles
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(wait);
    }
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, 0);
    }
    pixels.show();
    delay(wait);
  }
}

void ArrowLeft(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 4; j++) { // The j<# determines how many cycles
    for (uint16_t i = pixels.numPixels(); i + 1 > 0 ; i--) {
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(wait);
    }
    for (uint16_t i = pixels.numPixels(); i + 1 > 0 ; i--) {
      pixels.setPixelColor(i, 0);
    }
    pixels.show();
    delay(wait);
  }
}

void WigWag(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { // The j<# determines how many cycles
    for (int i = 0; i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, c);
    }
    for (int i = (pixels.numPixels() / 2); i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, 0);
    }
    pixels.show();
    delay(wait);

    for (int i = 0; i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, 0);
    }
    for (int i = (pixels.numPixels() / 2); i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, c);
    }
    pixels.show();
    delay(wait);
  }
}


void WigWag2(uint32_t c, uint32_t c2, uint8_t wait) {
  for (int j = 0; j < 20; j++) { // The j<# determines how many cycles
    for (int i = 0; i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, c);
    }
    for (int i = (pixels.numPixels() / 2); i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, 0);
    }
    pixels.show();
    delay(wait);

    for (int i = 0; i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, 0);
    }
    for (int i = (pixels.numPixels() / 2); i < pixels.numPixels(); i = i + 1) {
      pixels.setPixelColor(i, c2);
    }
    pixels.show();
    delay(wait);
  }
}

void ClearLights() {
  for (int i = 0; i < pixels.numPixels(); i = i + 1) {
    pixels.setPixelColor(i, 0);    //turn every pixel off
  }
  pixels.show();
}

void BlinkOuter(uint32_t c, uint8_t wait, int count) {
  for (int j = 0; j < count; j++) { // The j<# determines how many cycles
    pixels.setPixelColor(pixels.numPixels() - 1, c);
    pixels.setPixelColor(0, c);
    pixels.show();
    delay(wait);
    pixels.setPixelColor(pixels.numPixels() - 1, 0);
    pixels.setPixelColor(0, 0);
    pixels.show();
    delay(wait);
  }
}

void OnOuter(uint32_t c) {
  pixels.setPixelColor(pixels.numPixels() - 1, c);
  pixels.setPixelColor(0, c);
  pixels.show();
  delay(3000);
}

void rainbow(uint8_t wait) {
  uint16_t i, j,x;
  for (x=0;x<1;x++) {
    for(j=0; j<256; j++) {
      for(i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, Wheel((j) & 255));
      }
      pixels.show();
      delay(wait);
    }
  }
  
}
uint32_t Wheel(byte WheelPos)
{
  if(WheelPos < 85)
  {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if(WheelPos < 170)
  {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void worm() { 
  int mydelay=100;
  int max=100;
  int r=max;
  int g=max;
  int b=max;
  int tail=3;
  int drop =20;
  for (int z=0;z<1;z++) {
    ClearLights();
    for (int x=0;x<NUMPIXELS+6;x++){
        pixels.setPixelColor(x, pixels.Color(r,g,b));
        for (int y=x-1;y>=0;y--){
          int col = max - (drop * y);
          if (col < 0 ) {
            col=0;
          }
          pixels.setPixelColor(x-y-1, col,col,col);  
        }
        pixels.show();
        delay(mydelay);
    }
  }
    
}


void fire() {
  int r = 200;
  int g = r-40;
  int b = 40;
  
  for (int y=0;y<10;y++){
    
  
    for(int x = 0; x <NUMPIXELS; x++)
    {
      int flicker = random(0,150);
      int r1 = r-flicker;
      int g1 = g-flicker;
      int b1 = b-flicker;
      if(g1<0) g1=0;
      if(r1<0) r1=0;
      if(b1<0) b1=0;
      pixels.setPixelColor(x,g1,r1, b1);
    }
  pixels.show();
  delay(random(50,150));
  }
}
void rainbows() {
   rainbow(20);
   rainbowCycle(15);
}
void testmap() {

ClearLights();

 for (int x=0;x<NUMPIXELS;x++){
  //g , r , b
      pixels.setPixelColor(x, pixels.Color(0,50,0));
      pixels.show();
      delay(50);
  }
  
}
void loop() {
   
int testmode =0;
  testmode = digitalRead(IPIN);
  //Serial.println(testmode);

   if (testmode == HIGH) {
      demomode = true;
   
      if (!run){
      start();
      }
      democounter++;

   }else {
    
     String newinput = "";
     if (Serial.available())  {
  
          String tmpinput = tmpinput + Serial.readString();
          Serial.print("Read:");
          Serial.println(tmpinput);
          
          if (tmpinput.endsWith("|")){
              String newinput=tmpinput;
              tmpinput="";
              processInput(newinput.substring(0,newinput.length()));  
          }
          
     }
     //blinkOnboardLED();
    
      
   }
   if (demomode && democounter==1){
     processInput("STROBE");  
   }else if (demomode && democounter == 5){
     processInput("COP");  
   }else if (demomode && democounter == 20){
     processInput("CYCLONE");  
   } else if (demomode && democounter == 28){
     processInput("CAR");  
   }else if (demomode && democounter == 29){
     processInput("RAINBOWS");
   }else if (demomode && democounter == 31){
     processInput("FIRE");
   }else if (demomode && democounter > 32){
     democounter=0;
   }
    if (run) {
       cycle();
     }
  
}
//I2C stuff

void receiveData(int byteCount){
   int inChar;
   run=false;
   while(Wire.available()){ 
          inChar = Wire.read();
          Serial.print("I2C Read Char:[");
          Serial.println(inChar);
          Serial.print("]");
          inString += char(inChar);
   }
   Serial.print("I2C Read:");
   Serial.println(inString);
   if (inString.endsWith("|")){
       String newinput=inString;
       inString = "";
       processInput(newinput.substring(0,newinput.length()));  
   }

}
 
// callback for sending data
void sendData(){
  Wire.write(number);
}


