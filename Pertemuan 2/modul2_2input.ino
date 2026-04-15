#include <Arduino.h>

// Pin 7-Segment (a b c d e f g dp)
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Push button
const int buttonPin2 = 2;
const int buttonPin3 = 3;

// Counter
int counter = 0;

// State button
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;

// Pola digit 0-F
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, //0
  {0,1,1,0,0,0,0,0}, //1
  {1,1,0,1,1,0,1,0}, //2
  {1,1,1,1,0,0,1,0}, //3
  {0,1,1,0,0,1,1,0}, //4
  {1,0,1,1,0,1,1,0}, //5 
  {1,0,1,1,1,1,1,0}, //6
  {1,1,1,0,0,0,0,0}, //7
  {1,1,1,1,1,1,1,0}, //8
  {1,1,1,1,0,1,1,0}, //9
  {1,1,1,0,1,1,1,0}, //A
  {0,0,1,1,1,1,1,0}, //b
  {1,0,0,1,1,1,0,0}, //C
  {0,1,1,1,1,0,1,0}, //d
  {1,0,0,1,1,1,1,0}, //E
  {1,0,0,0,1,1,1,0}  //F
};

// Tampilkan digit
void displayDigit(int num)
{
  for(int i=0; i<8; i++)
  {
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup()
{
  for(int i=0; i<8; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }

  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);

  displayDigit(counter); // tampilkan awal
}

void loop()
{
  bool currentButtonState2 = digitalRead(buttonPin2);
  bool currentButtonState3= digitalRead(buttonPin3);

  // deteksi tekan (HIGH -> LOW)
  if (lastButtonState2 == HIGH && currentButtonState2 == LOW)
  {
    counter++;
    if(counter > 15) counter = 0;

    displayDigit(counter); // update hanya saat ditekan

    delay(200); // debounce sederhana
  }
  if (lastButtonState3 == HIGH && currentButtonState3 == LOW)
  {
    counter--;
    if(counter > 15) counter = 0;

    displayDigit(counter); // update hanya saat ditekan

    delay(200); // debounce sederhana
  }

  lastButtonState2 = currentButtonState2;
  lastButtonState3 = currentButtonState3;
}