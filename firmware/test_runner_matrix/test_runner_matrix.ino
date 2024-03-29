#include <Wire.h>
#include "TimerOne.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "test_runner_matrix.h"

/*
  Test Runner Matrix
  
  Display status of running tests

  Protocol:

  MODE,SUITE - show suite display
  MODE,TEST - show test display

  SUITE,PASS - suite is passing
  SUITE,FAIL - suite is failing
  SUITE,PENDING - suite is pending

  TEST,START - start another test
  TEST,PASS - pass test
  TEST,FAIL - fail test
  TEST,PENDING - pending test
*/

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

static uint8_t __attribute__ ((progmem)) pass_image[]={0x3C, 0x42, 0x95, 0xA1, 0xA1, 0x95, 0x42, 0x3C};
static uint8_t __attribute__ ((progmem)) fail_image[]={0x3C, 0x42, 0xA5, 0x91, 0x91, 0xA5, 0x42, 0x3C};
static uint8_t __attribute__ ((progmem)) pending_image[]={0x3C, 0x42, 0x95, 0x91, 0x91, 0x95, 0x42, 0x3C};

enum StatusType  status = PASS;
enum DisplayMode mode   = SUITE;

uint8_t  colors[] = { LED_GREEN, LED_RED, LED_YELLOW};
uint8_t* images[] = { pass_image, fail_image, pending_image};

#define TEST_COUNT 40

enum StatusType recentTests[TEST_COUNT];

uint16_t currentTest = 0;
volatile bool blinkState = false;

char data[20]; // Allocate some space for the string
char c=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

char readLineFromSerial() {
  while (Serial.available() > 0) {
    c = Serial.read();

    if (c == '\n') {
      data[index] = '\0';
      index = 0;
      Serial.println(data);
      return 1;
    } else {
      data[index] = c;
      index++;
      data[index] = '\0';
    }
  }

  return 0;
}

char commandEntered(char* command) {
  return strcmp(data, command);
}

void setup() {
  Timer1.initialize(150000);
  Timer1.attachInterrupt(blink);
  Timer1.start();

  Serial.begin(9600);
  Serial.println("8x8 LED Matrix");
  
  matrix.begin(0x70);  // pass in the address
  matrix.setRotation(3);
  matrix.clear();
}

void loop() {
  readSerial();
  updateMatrix();
}

void blink()
{
  blinkState = !blinkState;
}

void updateSuiteStatus(StatusType current) {
  mode   = SUITE;
  status = current;
}

void updateTestStatus(StatusType current) {
  recentTests[currentTest % TEST_COUNT] = current;

  switch(current) {
    case FAIL:
      status = current;
      break;
    case PENDING:
      if (status != FAIL) status = current;
      break;
    case PASS:
      if (currentTest == 0) status = PASS;
      break;
  }

  if (current != RUNNING) {
    currentTest++;
    if (currentTest % TEST_COUNT == 0)
      resetRecentTests();
  }
}

void readSerial() {
  if (readLineFromSerial() != 1)
    return;

  if (commandEntered("MODE,SUITE") == 0) {
    updateSuiteStatus(status);
  }
  else if (commandEntered("MODE,TEST") == 0) {
    mode        = TEST;
    currentTest = 0;

    resetRecentTests();
  }
  else if (commandEntered("SUITE,PASS") == 0) {
    updateSuiteStatus(PASS);
  }
  else if (commandEntered("SUITE,FAIL") == 0) {
    updateSuiteStatus(FAIL);
  }
  else if (commandEntered("SUITE,PENDING") == 0) {
    updateSuiteStatus(PENDING);
  }
  else if (commandEntered("TEST,START") == 0) {
    updateTestStatus(RUNNING);
    blinkState = false;
    Timer1.restart();
  }
  else if (commandEntered("TEST,PASS") == 0) {
    updateTestStatus(PASS);
  }
  else if (commandEntered("TEST,FAIL") == 0) {
    updateTestStatus(FAIL);
  }
  else if (commandEntered("TEST,PENDING") == 0) {
    updateTestStatus(PENDING);
    Timer1.restart();
  }
}

void updateMatrix() {
  matrix.clear();

  if (mode == TEST)
    drawTests();
  else
    drawStatus();
}

void drawStatus() {
  matrix.setRotation(2);
  matrix.drawBitmap(0, 0, images[status], 8, 8, colors[status]);
  matrix.writeDisplay();
  matrix.setRotation(3);
}

void resetRecentTests() {
  for (int i=0; i < TEST_COUNT; i++)
    recentTests[i] = NOT_RUN;
}

void drawTests() {
  int x,y,i;
  uint8_t color;
  enum StatusType testStatus;

  matrix.fillRect(0, 0, 8, 2, colors[status]);

  for (i=0; i < TEST_COUNT; i++) {
    x = i%8;
    y = 3 + (i/8);

    testStatus = recentTests[i];
  
    if (testStatus == RUNNING) {
      if (blinkState) matrix.drawPixel(x,y,LED_YELLOW);
    }
    else if (testStatus != NOT_RUN) {
      matrix.drawPixel(x,y,colors[testStatus]);
    }
  }

  matrix.writeDisplay();
}
