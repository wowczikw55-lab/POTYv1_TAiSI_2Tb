#include <Arduino.h>
#include <TM1637Display.h>
// pin definitions for TM1637 4-digit 7-segment display
#define CLK A4
#define DIO A5
// pin definitions for 74HC595 shift register
#define DATA_PIN 11  // DS
#define CLOCK_PIN 13 // SHCP
#define LATCH_PIN 10 // STCP

TM1637Display display(CLK, DIO);
bool ledState[8];
bool buttonState[8];
int8_t work_mode;
// --------------------****------------------- function prototypes
void showLedValue(int8_t value); 
void CounterMode();
void testMode();
void binaryGame();
void knightRiderMode();
void strobeMode();
void countupAnimationMode();
void bounceMode();

void setup() {
  // -------------------****------------------- setup for pins
  Serial.begin(9600);
  for (int i = 2; i < 10; i++)
  { // start from pin 2 to pin 9 for buttons
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  // -------------------****------------------- setup for 7-segment display
  display.setBrightness(7);
  display.showNumberDec(0); // initialize 7-segment to 0

  showLedValue(0); // initialize LEDs to 0

  // -------------------****------------------- initialize work mode
   Serial.print("triggered: ");
  if(analogRead(A0) == 1016){work_mode |= (1 << 3); Serial.print("0; ");}
  if(analogRead(A1) == 1016){work_mode |= (1 << 2); Serial.print("1; ");}
  if(analogRead(A2) == 1016){work_mode |= (1 << 1); Serial.print("2; ");}
  if(analogRead(A3) == 1016){work_mode |= (1 << 0); Serial.print("3; ");}
  Serial.println(); Serial.print("value is: ");
  Serial.println(work_mode); //debugging: print the work mode value to serial monitor

  randomSeed(analogRead(A7)); // seed random number generator with noise from unconnected pin

}

bool flag1;       // flag to prevent multiple triggers from a single button press
int value;        // stored input value
int value_mirror; // i fucked wiring so i need it
int8_t value8;    // 8-bit version of value for LEDs

void loop()
{
  switch (work_mode)
  {
  case 0:
    CounterMode();
    break;
  case 1:
    binaryGame();
    break;
  case 2:
    knightRiderMode();
    break;
  case 3:
    strobeMode();
    break;
  case 4:
    countupAnimationMode();
    break;
  case 5:
    bounceMode();
    break;
  default:
    testMode();
    break;
  }
}
void showLedValue(int8_t value)
{
  digitalWrite(LATCH_PIN, LOW);                   // open gate
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, value); // data
  digitalWrite(LATCH_PIN, HIGH);                  // close gate
}
void CounterMode(){// inputs from buttons displayed on LEDs and 7-segment, buttons act as binary counter
  for (int i = 2; i < 10; i++)
  {
    // compute indices: button 2→0 … 9→7 and mirror for LEDs
    int buttonIndex = i - 2;
    int ledIndex = 7 - (i - 2); // mirror index for LEDs

    if ((!digitalRead(i)) != buttonState[buttonIndex])
    { // check if button state has changed
      if (flag1 == 0)
      { // trigger only once per press

        ledState[ledIndex] = !ledState[ledIndex];
        if (ledState[ledIndex])
        {
          value += 1 << ledIndex;              // set the bit corresponding to physical LED
          value_mirror += 1 << (7 - ledIndex); // set the mirrored bit
        }
        else
        {
          value -= 1 << ledIndex;              // clear that bit
          value_mirror -= 1 << (7 - ledIndex); // clear the mirrored bit
        }
        value8 = value_mirror;
       
        showLedValue(value8);         // displaying value on LEDs
        display.showNumberDec(value); // displaying value on 7-segment
        flag1 = 1;
      }
      else
      {
        flag1 = 0;
      }
    }

    buttonState[buttonIndex] = !digitalRead(i);
  }
  delay(10);
}
void testMode(){// for debugging: print max values set as defoult
  showLedValue(255);         // max value
  display.showNumberDec(9999); // max value
  delay(100);
  }


  int randomValue;
  bool victory = 1;
void binaryGame(){ // number shows on 7-segment, player has to press the corresponding buttons to match the number in binary, then a new number is generated
  if (victory)  {
    randomValue = random(0, 256); // generate a random number between 0 and 255
    display.showNumberDec(randomValue); // show the random number on 7-segment
    victory = 0; // reset victory flag
  }
    for (int i = 2; i < 10; i++)
  {
    // compute indices: button 2→0 … 9→7 and mirror for LEDs
    int buttonIndex = i - 2;
    int ledIndex = 7 - (i - 2); // mirror index for LEDs

    if ((!digitalRead(i)) != buttonState[buttonIndex])
    { // check if button state has changed
      if (flag1 == 0)
      { // trigger only once per press

        ledState[ledIndex] = !ledState[ledIndex];
        if (ledState[ledIndex])
        {
          value += 1 << ledIndex;              // set the bit corresponding to physical LED
          value_mirror += 1 << (7 - ledIndex); // set the mirrored bit
        }
        else
        {
          value -= 1 << ledIndex;              // clear that bit
          value_mirror -= 1 << (7 - ledIndex); // clear the mirrored bit
        }
        value8 = value_mirror;
       if (value == randomValue) victory = 1; // check for victory condition
        showLedValue(value8);         // displaying value on LEDs
        flag1 = 1;
      }
      else
      {
        flag1 = 0;
      }
    }

    buttonState[buttonIndex] = !digitalRead(i);
  }
  delay(10);
}

// ==================== AI GENERATED SLOP ANIMATIONS! ====================

void knightRiderMode() { // LED scanning effect (like KITT from Knight Rider)
  static uint8_t pos = 0;
  static int8_t direction = 1;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 80) {
    pos += direction;
    if (pos >= 7) { pos = 7; direction = -1; }
    if (pos <= 0) { pos = 0; direction = 1; }
    
    showLedValue(1 << pos);
    display.showNumberDec(pos);
    lastUpdate = millis();
  }
  delay(10);
}

void strobeMode() { // Alternating pattern strobe
  static unsigned long lastUpdate = 0;
  static bool state = 0;
  
  if (millis() - lastUpdate > 200) {
    state = !state;
    showLedValue(state ? 0xAA : 0x55); // alternating bit pattern
    display.showNumberDec(state ? 9999 : 0);
    lastUpdate = millis();
  }
  delay(10);
}

void countupAnimationMode() { // Count 0-255 in binary, display on 7-segment, LEDs in mirrored direction
  static unsigned long lastUpdate = 0;
  static uint16_t counter = 0;
  
  if (millis() - lastUpdate > 100) {
    // Calculate mirrored (bit-reversed) value for LEDs
    uint8_t mirrored = 0;
    uint8_t temp = counter & 0xFF;
    for (int i = 0; i < 8; i++) {
      mirrored = (mirrored << 1) | (temp & 1);
      temp >>= 1;
    }
    
    showLedValue(mirrored);           // show reversed binary on LEDs
    display.showNumberDec(counter);   // show counter 0-255 on 7-segment
    counter++;
    if (counter > 255) counter = 0;
    lastUpdate = millis();
  }
  delay(10);
}

void bounceMode() { // LED bouncing between ends
  static uint8_t pos = 0;
  static int8_t direction = 1;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 120) {
    pos += direction;
    if (pos >= 7) { pos = 7; direction = -1; }
    if (pos <= 0) { pos = 0; direction = 1; }
    
    uint8_t pattern = (1 << pos) | (1 << (7 - pos));
    showLedValue(pattern);
    lastUpdate = millis();
  }
  delay(10);
}