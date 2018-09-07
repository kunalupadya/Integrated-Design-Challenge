//line follow and servo setup
#include <Servo.h>                           // Include servo library
Servo servoLeft;                             // Declare left servo signal
Servo servoRight;                            // Declare right servo signal
//int leftPin = 4;
//int centerLeftPin = 5;
//int centerRightPin = 6;
//int rightPin = 7;
int oL = 4;
int iL = 5;
int iR = 6;
int oR = 7;
//storage variables for the communication
int x1= 0;
int x2= 0;
int x3= 0;
int x4= 0;
int x5= 0;
int c1 = 1;
int c2 = 0;
int c3 = 0;
int c4 = 0;
int c5 = 0;
int total = 0;
char outgoing = ' ';
// tone values
const int c = 261;
const int d = 294;
const int dSH = 622;
const int dS = dSH/2;
const int e = 329;
const int f = 349;
const int fSH = 740;
const int fS = fSH/2;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 493;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int eH = 659;
const int fH = 698;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
const int bH = 987;
 
const int buzzerPin = 3;
//length of tones
const int q = 400;
const int qh = q*1.5;
const int ha = 2*q;
const int o = 4*q;
const int ei = q/2;

const int lineFollowThreshold = 300;                 // used for defining black v white
boolean previouslyReadIntersection = false;    // Was the previous hash the first one? if so, then keep going
int numHashes = 0;
int updateLEDFeed = 0; // for now, colorDetect is not implemented so 0 is the case;


//Sensing setup
#include <Wire.h>
#include "Adafruit_TCS34725.h"
/* Initialise with specific int time and gain values */
//Set up for RGB sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
//Set pins for IR receivers/emitters and the indicator LEDs
int receiverHigh = 9;
int irLEDHigh = 8;
//int receiverLow = x;
//int irLEDLow = y;
//int qtiPin;
int onesLED = 11;
int twosLED = 10;

void setup() {
  //buzzer setup
  pinMode(buzzerPin, OUTPUT);
  
  //SERVO SETUP
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                      // Attach left signal to pin 12
 
  //SENSOR SETUP
  // code for IR Receivers and emitters
  pinMode(receiverHigh, INPUT);   //IR Receiver high
  pinMode(irLEDHigh,   OUTPUT);   //IR LED high
  // Indicator LED setup
  pinMode(onesLED,     OUTPUT);   //Indicator LED (1's place)
  pinMode(twosLED,     OUTPUT);   //Indicator LED (2's place)
  Serial.begin(9600);
  Serial2.begin(9600);
}

  

void loop() {
  //bot starts on hashmark
  
  boolean go = true;
  delay(1000);
  while(go){ //keep line following until the bot reaches the hash
    go = lineFollow(previouslyReadIntersection);
  }
  
  goStraight();//after hitting hash, go slightly forward
  delay(200); // for x amount of milliseconds
  stayStill();
  delay(2000);
  
  turn90CCW();//turn 90 degrees counter clockwise to fase the center of the ring
  delay(1000); // wait for next command
  
  goStraight();//move towards center of ring
  delay(1550);
  stayStill();
  delay(1000);

  for(int i = 0; i <= 3; i++){
    delay(50);
    int irHigh = irDetect(irLEDHigh, receiverHigh, 38000);     //check if something is present high up
    delay(50);
    int lowResult = lowObjectDetect(getC());                 //check if low object is duck or 8-ball
    delay(50);
    updateLEDFeed = checkObject(irHigh, lowResult); // for now, colorDetect is not implemented so 0 is the case;
    delay(50);
    Serial.print(i);
    Serial.print("\n");
    Serial.print(updateLEDFeed);
    Serial.print("\n");
    delay(30);
    if (i == 3){
      updateLED(updateLEDFeed);  //LED lights up binary result of updateLEDFeed
      delay(1000);
      digitalWrite(onesLED, LOW);
      digitalWrite(twosLED, LOW);
    }
  }

  //communication part

if (updateLEDFeed == 1){
  outgoing = '2';
  x1 = 1;
  
}
if (updateLEDFeed == 2){
  outgoing = '3';
  x1 = 2;
}
if (updateLEDFeed == 3){
  outgoing = '1';
  x1 = 0;
  
}



 while(true){ //loop runs until break statement-all values recieved
  Serial2.print(outgoing);
  // !((c1 == 1)  && (c2 == 1)  && (c3 == 1)  && (c4 == 1)  && (c5 == 1))
  
if (Serial2.available()) {
      char incoming = Serial2.read();

      Serial.println(incoming);
   if ((incoming-48 < 7) && (incoming-48 >3)) {
    x2 = incoming-52;
    c2 = 1;
    Serial.println(x2);
    Serial.println(c2);
  }
  else if ((incoming-96 < 4) && (incoming-96 >0)) {
    x3 = incoming-97;
    c3 = 1;
    Serial.println(x3);
    Serial.println(c3);
  }
  else if ((incoming-96 < 7) && (incoming-96 > 3)) {
    x4 = incoming-100;
    c4 = 1;
    Serial.println(x4);
    Serial.println(c4);
  }
  else if ((incoming-96 < 10) && (incoming-96 > 6)) {
    x5 = incoming-103;
    c5 = 1;
    Serial.println(x5);
    Serial.println(c5);
  }
}
   
 
 if((c1 == 1)  && (c2 == 1)  && (c3 == 1)  && (c4 == 1)  && (c5 == 1)) {
  break;
 }
 delay(350);
 Serial.print(c1);
 Serial.print(c2);
 Serial.print(c3);
 Serial.print(c4);
 Serial.print(c5);
Serial.print("\n"); 
 }
 

   total = (x1 + x2 + x3 + x4 +x5) % 3;
   tone(4, 440, 500);
   Serial.print("Total: ");
   Serial.println(total);
   if(total == 0) {

    for(int k = 0; k <= 20; k++){
    servoRight.writeMicroseconds(1450);                      
    servoLeft.writeMicroseconds(1450);
    delay(500);
    servoRight.writeMicroseconds(1550);                      
    servoLeft.writeMicroseconds(1550);
    delay(500);
    }
    
    
   }
   if(total == 1){
    for(int k = 0; k <= 100; k++){
    digitalWrite(11, !digitalRead(11));
    digitalWrite(10, !digitalRead(12));
    delay(200);
    }
   }
    if(total == 2){
      beep(b,q);
beep(cSH,qh);
beep(cSH,qh);
beep(fSH,q);
beep(eH, qh);
beep(dH, qh);
beep(cSH, q);
beep(b, qh);
beep(b, qh);
beep(cSH, q);
beep(a, ha*1.5);
beep(b,q);
beep(cSH,qh);
beep(cSH,qh);
beep(fSH,q);
beep(eH, qh);
beep(cSH, qh);
beep(eH, q);
beep(b, o);
delay(q*3);
beep(gSH, q);
beep(aH, qh);
beep(gSH, qh);
beep(fSH, q);
beep(eH, qh);
beep(dH, qh);
beep(cSH, q);
beep(b, qh);
beep(eH,qh);
beep(dH, q);
beep(cSH, ha*1.5);
beep(gSH, q);
beep(aH, qh);
beep(gSH, qh);
beep(fSH, q);
beep(eH, qh);
beep(aH, qh);
beep(bH, qh);
beep(bH, o+q);
delay(ei);
beep(aH, ei);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, ei);
beep(eH, ei);
beep(fSH, qh);
beep(fSH, ei);
beep(gSH, q);
beep(gSH, ei);
beep(gSH, q);
beep(aH, ei);
beep(aH, q);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, ei);
beep(eH, ei);
beep(fSH, q);
beep(aH, ei);
beep(bH, q);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, q);
beep(fSH, qh);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, ei);
beep(eH, ei);
beep(fSH, q);
beep(aH, q);
beep(eH,q);
delay(ei);
beep(aH, ei);
beep(gSH, ei);
beep(fSH, ei);
beep(gSH, ei);
beep(aH, ha+ei);
beep(aH, q);
beep(eH, ei);
beep(eH, q);
beep(cSH, ei);
beep(eH, ei);
beep(b, qh);
beep(aH, ei);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, ei);
beep(eH, ei);
beep(fSH, qh);
beep(fSH, ei);
beep(gSH, q);
beep(gSH, ei);
beep(gSH, q);
beep(aH, ei);
beep(aH, q);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, ei);
beep(eH, ei);
beep(fSH, q);
beep(aH, ei);
beep(bH, q);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, q);
beep(fSH, qh);
beep(aH, ei);
beep(gSH, ei);
beep(gSH, ei);
beep(eH, ei);
beep(fSH, q);
beep(aH, q);
beep(eH,q);
delay(ei);
beep(aH, ei);
beep(gSH, ei);
beep(fSH, ei);
beep(gSH, ei);
beep(aH, ha+ei);
beep(aH, q);
beep(eH, ei);
beep(eH, q);
beep(cSH, ei);
beep(eH, ei);
beep(b, qh);
delay(3*q);

    }

    
while(true){
  Serial2.print(outgoing);
  delay(350);
}
  delay(100000);
  
  
  
/*  
  if(Serial2.available()) { // Is data available from XBee?
    char incoming = Serial2.read(); // Read character,
    digitalWrite(onesLED, HIGH);
    digitalWrite(twosLED, HIGH);
    delay(1000);
    digitalWrite(onesLED, LOW);
    digitalWrite(twosLED, LOW);
  }
  delay(50);
  */
 }

//METHODS


//establishes the time delay for the qti
long RCtime(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(230);
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
  long time = micros();
  while (digitalRead(pin));
  time = micros() - time;
  return time;
}

//line following
boolean lineFollow(boolean previous){
  int whichQTIs = getCase(oL, iL, iR, oR);
  //case 0000
  if (whichQTIs == 0){
    goStraight();
  }
  //case 0001
  if (whichQTIs == 1){
    turnRightHard();
  }
  //case 0010
  if (whichQTIs == 2){
    turnLeft();
  }
  //case 0011
  if (whichQTIs == 3){
    turnRight();
  }
  //case 0100
  if (whichQTIs == 4){
    turnRight();
  }
  //case 0101
  //if (whichQTIs == 5){
    //turnRight(); //weird case
  //}
  //case 0110
  if (whichQTIs == 6){
    goStraight();
  }
  //case 0111
  if (whichQTIs == 7){
    turnLeft();
  }
  //case 1000
  if (whichQTIs == 8){
    turnLeftHard();
  }
  //case 1001
  if (whichQTIs == 9){
    goStraight(); //weird case
  }
  //case 1010
  if (whichQTIs == 10){
    turnLeft();
  }
  //case 1011
  if (whichQTIs == 11){
    goStraight(); //weird case
  }
  //case 1100
  if (whichQTIs == 12){
    turnLeft();
  }
  //case 1101
  if (whichQTIs == 13){
    goStraight(); //weird case
  }
  //case 1110
  if (whichQTIs == 14){
    turnRight();
  }
  //case 1111
  if (whichQTIs == 15){
    if(previouslyReadIntersection == false){
      numHashes++;
      previouslyReadIntersection == true;
    }

    if(numHashes >= 2){
      return false;
    }
      stayStill();
      delay(1000);
      goStraight();
      delay(200);
  }
  if(!whichQTIs == 15){
    previouslyReadIntersection = false;
  }
  delay(10);
  return true;

}
//tone generator
 void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
  Serial2.print(outgoing);
  delay(duration+10);
}
//turning functions
void turnLeft() {
  servoRight.writeMicroseconds(1500);                    
  servoLeft.writeMicroseconds(1450);
}
void turnRight() {
 servoRight.writeMicroseconds(1550);                      
 servoLeft.writeMicroseconds(1500);
}

void turnRightHard() {
 servoRight.writeMicroseconds(1500);                      
 servoLeft.writeMicroseconds(1450);
}

void turnLeftHard() {
 servoRight.writeMicroseconds(1550);                      
 servoLeft.writeMicroseconds(1500);
}

void goStraight(){
 servoRight.writeMicroseconds(1550);                      
 servoLeft.writeMicroseconds(1450); 
}
void stayStill(){
  servoRight.writeMicroseconds(1500);
  servoLeft.writeMicroseconds(1500);
}

void turn90CCW(){
 servoRight.writeMicroseconds(1450);                      
 servoLeft.writeMicroseconds(1450); 
 delay(1150);
 stayStill();
}

int getCase(int leftPin, int centerLeftPin, int centerRightPin, int rightPin){
  int binaryResult = 0;
  if (RCtime(leftPin) > lineFollowThreshold)
    binaryResult += 1;

  if (RCtime(centerLeftPin) > lineFollowThreshold)
    binaryResult += 2;

  if (RCtime(centerRightPin) > lineFollowThreshold)
    binaryResult += 4;

  if (RCtime(rightPin) > lineFollowThreshold)
    binaryResult += 8;

  return binaryResult;
}

//SENSING METHODS
// IR Object Detection Function
int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  

/**
 * Args
 *  int luxValue : lux value from RGB sensor
 * Returns
 *  3 iff 8-ball is present
 *  2 iff duck is present
 *  0 else
 */
int lowObjectDetect(int c){ //differentiates between duck and 8-ball
  if(c < 2400)
    return 3;
  if(c > 2400)
    return 2;
  else
    return 0;
}

/**
 * Args
 *  irDetectResult : detection from higher IR sensor
 *  lowerResult : result from lowObjectDetect
 * Returns
 *  1 iff pencil present
 *  2 iff 8-ball present
 *  3 iff duck present
 *  0 else
 */
int checkObject(int irDetectResult, int lowerResult)
{
  if (!irDetectResult == 1){ // detect if the pencil is there
    return 1; 
  }
  else if(lowerResult == 2){ //detect if the duck is there
    return 2; //result to display "2" in binary using LEDs
  }
  else if(lowerResult == 3){ // detect if the 8-ball is there
    return 3; //result to display "3" in binary using LEDs
  }
  else{
    return 0; //returns 0 if no object has been detected so far
  }
}

void updateLED(int whichObject){ //displays binary result of int returned from checkObject
  if (whichObject == 1){ // pencil
    digitalWrite(onesLED, HIGH);
    digitalWrite(twosLED, LOW);
  }
 else if (whichObject == 2){ // duck
    digitalWrite(onesLED, LOW);
    digitalWrite(twosLED, HIGH);
  }
 else if (whichObject == 3){ // ball
    digitalWrite(onesLED, HIGH);
    digitalWrite(twosLED, HIGH);
  }
  else{
    digitalWrite(onesLED, LOW);
    digitalWrite(twosLED, LOW);
  }
}

/**
 * Returns C read from RGB sensor
 */
int getC()
{
    uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  return c;
}
