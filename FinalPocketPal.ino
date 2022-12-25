/* 
 *  FinalPocketPal.ino
 *  Author: Selin Butun
 *  Last Update: December 14, 2022
 *  
 *  Description: Displays animations on an 8x8 LED matrix of
 *  a creature that gets hungry and sad, and displays happiness,
 *  motions of eating, and excitement when pet. This program
 *  runs on an Adafruit Classic Circuit Plyaground and makes use of
 *  the buttons. 
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_CircuitPlayground.h>
#include <TimerOne.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix(); // adress to the matrix

// global variables for the timers
volatile char timerFlag = 0;
unsigned long prevFedMillis = 0;
unsigned long prevPetMillis = 0;
unsigned long feedInterval = 50000;
unsigned long petInterval = 30000;
bool hungerState = false;
bool petState = false;

// Create a new data called "states" type with 5 values.
enum states
{
    Happy,
    Fed,
    Pet,
    Hungry,
    Sad
}; // enum types

// Create a variable "myState" whose type is "states"
enum states myState; // enum variable

void setup()
{
    CircuitPlayground.begin();
    matrix.begin(0x70);
    Timer1.initialize(100000);      // value is in microseconds
    Timer1.attachInterrupt(ticker); // call tick state machine
    myState = Happy;                // beginning state
}

void ticker()
{
    timerFlag = 1; // timer
}

static const uint8_t PROGMEM
    // the layouts for the 8-bit face designs for the animations
    smile_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00100100,
         B00011000,
         B00000000,
         B00000000},
    grin_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B01111110,
         B00100100,
         B00011000,
         B00000000},
    eat1_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00011000,
         B00100100,
         B00011000,
         B00000000},
    eat2_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00100000,
         B00011100,
         B00100000,
         B00000000},
    eat3_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00100100,
         B00011100,
         B00100000,
         B00000000},
    sad1_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00011000,
         B00100100,
         B00000000,
         B00000000},
    sad2_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000010,
         B00011000,
         B00100100,
         B00000000,
         B00000000},
    sad3_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00011010,
         B00100100,
         B00000000,
         B00000000},
    sad4_bmp[] =
        {B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00011000,
         B00100110,
         B00000000,
         B00000000},
    hungry1_bmp[] =
        {B01100001,
         B10010110,
         B00000000,
         B00100100,
         B00100100,
         B00000000,
         B00011000,
         B00100000},
    hungry2_bmp[] =
        {B00000001,
         B01100110,
         B10010000,
         B00100100,
         B00100100,
         B00000000,
         B00011000,
         B00100000},
    pet1_bmp[] =
        {B00000000,
         B01000010,
         B00100100,
         B00100100,
         B00000000,
         B00100100,
         B00011000,
         B00000000},
    pet2_bmp[] =
        {B00000000,
         B01100011,
         B10100101,
         B00000000,
         B00100100,
         B00011000,
         B00000000,
         B00000000},
    pet3_bmp[] =
        {B00000000,
         B00000000,
         B01100011,
         B10100101,
         B00000000,
         B00100100,
         B00011000,
         B00000000};

void running()
{
    // the method that contains all the codes for the different states and timers
    bool feeding = CircuitPlayground.leftButton();  // left button variable
    bool petting = CircuitPlayground.rightButton(); // right button variable
    unsigned long currentMillis = millis();         // starting timer

    switch (myState)
    {
    case Happy:
        // loops the "Happy" animation until further action is taken
        for (int i = 0; i < 4; i++)
        {
            matrix.clear();
            matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);

            matrix.clear();
            matrix.drawBitmap(0, 0, grin_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);
        }
        if (feeding)
        {
            // switches states if left button is pressed
            myState = Fed;
        }
        if (petting)
        {
            // switches states if right button is pressed
            myState = Pet;
        }
        if (currentMillis - prevFedMillis >= feedInterval)
        {
            // if it has been too long since feeding, creature gets hungry and
            // state is switched to "Hungry"
            prevFedMillis = currentMillis;
            hungerState = !hungerState;
            if (hungerState)
            {
                myState = Hungry;
            }
        }
        if (currentMillis - prevPetMillis >= petInterval)
        {
            // if it has been too long since petting, creature gets sad and
            // state is switched to "Sad"
            prevPetMillis = currentMillis;
            petState = !petState;
            if (petState)
            {
                myState = Sad;
            }
        }
        break;

    case Fed:
        // loops the "Fed" (eating) animation, then switches state back to "Happy"
        // if no further actions taken
        matrix.clear();
        matrix.drawBitmap(0, 0, eat1_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        matrix.setTextSize(1);
        matrix.setTextWrap(false); // we dont want text to wrap so it scrolls nicely
        matrix.setTextColor(LED_ON);
        for (int8_t x = 0; x >= -36; x--)
        {
            matrix.clear();
            matrix.setCursor(x, 0);
            matrix.print("Yum!");
            matrix.writeDisplay();
            delay(50);
        }
        matrix.setRotation(0);

        for (int j = 0; j < 4; j++)
        {
            matrix.clear();
            matrix.drawBitmap(0, 0, eat2_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);

            matrix.clear();
            matrix.drawBitmap(0, 0, eat3_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);
        }
        prevFedMillis = currentMillis; // sets last feeding time to current time
        hungerState = !hungerState;    // switches hungerState to not hungry

        if (feeding)
        {
            myState = Fed;
        }
        else if (petting)
        {
            myState = Pet;
        }
        else
        {
            myState = Happy;
        }
        break;

    case Pet:
        // loops the "Pet" animation, then switches state back to "Happy"
        // if no further actions taken
        for (int i = 0; i < 4; i++)
        {
            matrix.clear();
            matrix.drawBitmap(0, 0, pet1_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);

            matrix.clear();
            matrix.drawBitmap(0, 0, pet2_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);

            matrix.clear();
            matrix.drawBitmap(0, 0, pet3_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            delay(200);
        }

        if (feeding)
        {
            myState = Fed;
        }
        else if (petting)
        {
            myState = Pet;
        }
        else
        {
            myState = Happy;
        }
        break;

    case Hungry:
        // Creature stays in "Hungry" state until fed (left button press)
        // no other action is accepted
        matrix.clear();
        matrix.drawBitmap(0, 0, hungry1_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        matrix.clear();
        matrix.drawBitmap(0, 0, hungry2_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        if (feeding)
        {
            // if left button pressed, set previous feeding time to current time
            // then switch to "Fed" state
            prevFedMillis = currentMillis;
            myState = Fed;
        }
        else
        {
            myState = Hungry;
        }
        break;

    case Sad:
        // Creature stays in "Sad" state until pet (right button press)
        // no other action is accepted
        matrix.clear();
        matrix.drawBitmap(0, 0, sad1_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        matrix.clear();
        matrix.drawBitmap(0, 0, sad2_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        matrix.clear();
        matrix.drawBitmap(0, 0, sad3_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        matrix.clear();
        matrix.drawBitmap(0, 0, sad4_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();
        delay(200);

        if (petting)
        {
            // if right button is pressed, set last petting time to current time
            // then, change state to "Pet"
            prevPetMillis = currentMillis;
            myState = Pet;
        }
        else
        {
            myState = Sad;
        }
        break;
    }
}

void loop()
{
    // calls on running method state machine as ticker goes off
    if (timerFlag)
    {
        running();
        timerFlag = 0;
    }
}
