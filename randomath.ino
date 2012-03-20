/*
RandoMath
*/
#include <SoftwareSerial.h>

#define LED 13
#define CLK 9
#define DAT 12
#define txPin 7 //pin for serial lcd

// if I put the keyboard map in here then keys[scan code] would return 
// the ascii value of the scan code.

unsigned char keys[128] =
{
};

int ans=0;
int key=0;
int sum=0;
int answered=1;
int i =0;
int pass=0;

SoftwareSerial LCD = SoftwareSerial(0, txPin);
// since the LCD does not send data back to the Arduino, we should only define the txPin
const int LCDdelay=10;  // conservative, 2 actually works

// wbp: goto with row & column
void lcdPosition(int row, int col) {
  LCD.write(0xFE);   //command flag
  LCD.write((col + row*64 + 128));    //position 
  delay(LCDdelay);
}
void clearLCD(){
  LCD.write(0xFE);   //command flag
  LCD.write(0x01);   //clear command.
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(157);    //light level.
  delay(LCDdelay);
}
void backlightOff(){  //turns off the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(128);     //light level for off.
   delay(LCDdelay);
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  LCD.write(0xFE);
}

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(DAT, INPUT);
  digitalWrite(CLK, HIGH);
  digitalWrite(DAT, HIGH);
  delayMicroseconds(50);
  Serial.begin(9600);
  
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
  lcdPosition(0,2);
  LCD.print("ThinkerToys!");
  delay(1500);
  lcdPosition(1,3);
  LCD.print("RandoMath");
  delay(2500);
  clearLCD();
  lcdPosition(0,1);
  LCD.print(" Let's start    with addition!");
  delay(2500);
  clearLCD();

  
}

void loop()
{
  
  if(i%5==0 && i!=0 && answered == 1)
  {
  lcdPosition(0,0);
  LCD.print("Hooray! 5 quest-ions completed!");
  delay(5000);
  clearLCD();
   
  }
  
  
  if(answered==1){
    int x = random(10,49);
    int y = random(10,49);
    String operand = "+";
    String equalto = "=";
    String question = "?";
    sum = x + y;
    String whole = x + operand + y + equalto + question;
    lcdPosition(0,4);
    LCD.print(whole);
    answered=0;
    i++;
  }
  
  byte b = readkey(); 
  if (b == 0xF0)
  {
    // key released
    b = readkey();
  }
  else
  {
    key=getKey(b);
    if(key==11){
      pass++;
      if(pass==2){
      lcdPosition(0,10);
      LCD.print(ans);
      delay(0);
      
      answered=1;
       
      if(sum==ans){
        lcdPosition(1,4);
        LCD.print("Correct!");
        delay(2500);
        clearLCD();
      }
      else{
       lcdPosition(1,3);
       LCD.print("It's");
       lcdPosition(1,8);
       LCD.print(sum);      
       lcdPosition(1,11);
       LCD.print(":)");      
       delay(2500);
       clearLCD();
      }
       
      Serial.print(ans, DEC); 
      Serial.print(" ");
      ans=0;
      sum=0;
      key=0;
      pass=0;
    }
   }
    if(key!=11){    
    ans = ans*10 + key;  
    }
  }                   
}

int getKey(byte b)
{
 switch(b) {
 
 case 105:
  return 1; 
  break;
  
 case 114:
  return 2; 
  break;

 case 122:
  return 3; 
  break;

 case 107:
  return 4; 
  break;

 case 115:
  return 5; 
  break;

 case 116:
  return 6; 
  break;

 case 108:
  return 7; 
  break;

 case 117:
  return 8; 
  break;

 case 125:
  return 9; 
  break;

 case 112:
  return 0; 
  break;
  
 default:
   return 11;
   break;
 }
}


byte readkey()
{
  byte _start = 0;
  byte buf = 0;
  byte _parity = 0;
  byte _stop = 0;
  
  waitClockLow();
    
  _start = digitalRead(DAT);

  if (_start == 0)
  {
    waitClockLow();
    for (byte c=0; c<8; c++)
    {
      buf = buf | (digitalRead(DAT) << c);
      waitClockLow();
    }
  
    _parity = digitalRead(DAT);
    waitClockLow();
  
    _stop = digitalRead(DAT);
  }
  return buf;
}

void waitClockLow()
{
  if (digitalRead(CLK) == LOW)
    while (digitalRead(CLK) == LOW)
    {
    }
  while (digitalRead(CLK) == HIGH)
  {
  }
}
