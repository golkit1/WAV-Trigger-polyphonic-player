//Code was written by Omer Goldberg. August 2021//

#include <wavTrigger.h>

/* Arduino Mega Serial port to use: Serial Port 3:
Pin 14: TX
Pin 15: RX
*/

// Buttons //
const int buttonPins[] = {2,3,4,5,6,7,8,9,10,11,12,16,17,18,19,68,69,22,23,24,25,26,27,28}; //array of buttons pins
const int numOfButtons =  (sizeof(buttonPins) / sizeof(buttonPins[0])); // size of push buttons

// Leds //
const int ledPins[] ={30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53}; // array of leds pins
const int numOfLeds =  (sizeof(ledPins) / sizeof(ledPins[0])); // size of Leds

// Track Lengths //
// formula for milliseconds =  (minutes * 60 + seconds) * 1000 = milliseconds //
const unsigned long trackLength[] = {19000,66000,75000,104000,29000,49000,49000,107000,52000,97000,35000,57000,157000,82000,86000,104000,39000,37000,81000,103000,45000,96000,30000,115000};


// array to store the time when button is pressed //
unsigned long timeButtonPressed[numOfButtons] = {0}; 

//for first run code //
bool firstRun[numOfButtons] = {0};

//for accurate fading track //
bool preButtonState[numOfButtons] = {0};

bool buttonState[numOfButtons] = {0};

// to count how many track are currently playing //
int counter = 0;

const int fadeTime = 5000;


// WAV Trigger object //
wavTrigger wTrig;            

//track number//
int  gNumTracks;  

// firmware version//
char gWTrigVersion[VERSION_STRING_LEN]; 


//------------------------------------------//

 
void setup() {

  // starting a commuincation with the serial port //
  
  Serial.begin(9600);
  // setting up the digital pins for the buttons to be an input //
  for (int i = 0; i <  numOfButtons; i++)
  {
    pinMode(buttonPins[i], INPUT);
  }

  // setting up the digital pins for the leds to be an output //
  for (int j = 0; j < numOfLeds; j++)
  {
    pinMode(ledPins[j], OUTPUT);
  }

  delay(1000);

  // WAV Trigger startup at 57600
  wTrig.start();
  delay(10);
 
  // Send a stop-all command and reset the sample-rate offset, in case we have
  // reset while the WAV Trigger was already playing.
  wTrig.stopAllTracks();
  wTrig.samplerateOffset(0);
  
  // Enable track reporting from the WAV Trigger
  wTrig.setReporting(true);
  
  delay(1000);
  
   /* checking bi-directional communication. should be able
   to return the version of the board and number of tracks on the SD card.*/
   
  if (wTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN))
  {
      Serial.print(gWTrigVersion);
      Serial.print("\n");
      gNumTracks = wTrig.getNumTracks();
      Serial.print("Number of tracks = ");
      Serial.print(gNumTracks);
      Serial.print("\n");
  }
  else
  Serial.print("WAV Trigger response not available\n");
    
  Serial.print("\n");  
  Serial.print("Num of Buttons: ");
  Serial.println(numOfButtons);
  Serial.print("\n");   
  Serial.print("Num of Leds: ");
  Serial.println(numOfLeds);
  Serial.print("\n");    


  // print the length of each track //
  for (int i = 0;i < numOfButtons; i++)
  {
    Serial.print("The Length of track number ");
    Serial.print(i+1);
    Serial.print(" is: ");
    Serial.print(trackLength[i]);
    Serial.println(" Milliseconds");
  }

  // set master gain to 0db;
  wTrig.masterGain(0);

}
  
//------ loop ------//
  
void loop()
{
  wTrig.update();
  
  for (int i = 0; i < numOfButtons; i++)
  {
    if (digitalRead(buttonPins[i]) == HIGH)
    {
      if(millis() - timeButtonPressed[i] >= trackLength[i] || firstRun[i] == 0)
      {
        timeButtonPressed[i] = millis(); //storing the time of button pressed
        wTrig.trackPlayPoly(i+1); //play track number i
        Serial.print("Track number ");
        Serial.print(i+1);
        Serial.println(" is playing");
        Serial.println("");
        digitalWrite(ledPins[i],HIGH); // turn on led corresponding to the track that is playing
        buttonState[i] = 1; // store 1 to indicate that the track number i  is currently playing
        counter++; // count how many track playing
        Serial.print("Number of tracks playing: ");
        Serial.print(counter);
        Serial.println("\n");
        firstRun[i] = 1;
        Serial.print("counter: ");
        Serial.println(counter);
      }
    }
    if (millis() > timeButtonPressed[i] + trackLength[i]) // check if the track number i is finished playing
    {
      digitalWrite(ledPins[i],LOW); // if so - turn off led
      if (millis() > timeButtonPressed[i] + trackLength[i] && buttonState[i] == 1) // check if track number i that finisihed was previusly on
      {
        counter--; // if so - reduce 1 from counting
        buttonState[i] = 0; //store 0 for that button state at track index i
           
        Serial.print("Track number ");
        Serial.print(i+1);
        Serial.println(" stopped");
        Serial.println("");
        Serial.print("Number of tracks playing: ");
        Serial.print(counter);
        Serial.println("\n");
      }
    }
    if(buttonState[i] != preButtonState[i])
    {
      for (int i = 0; i < numOfButtons; i++)
      {
    switch (counter)
    {
      case 1:
      wTrig.trackFade(i+1, 0, fadeTime, 0);
      break;
      case 2:
      wTrig.trackFade(i+1, -6, fadeTime, 0);
      break;
      case 3:
      wTrig.trackFade(i+1, -10, fadeTime, 0);
      break;
      case 4:
      wTrig.trackFade(i+1, -12, fadeTime, 0);
      break;
      case 5:
      wTrig.trackFade(i+1, -14, fadeTime, 0);
      break;
      case 6:
      wTrig.trackFade(i+1, -16, fadeTime, 0);
      break;
      case 7:
      wTrig.trackFade(i+1, -17, fadeTime, 0);
      break;
      case 8:
      wTrig.trackFade(i+1, -18, fadeTime, 0);
      break;
      case 9:
      wTrig.trackFade(i+1, -19, fadeTime, 0);
      break;
      case 10:
      wTrig.trackFade(i+1, -20, fadeTime, 0);
      break;
      case 11:
      wTrig.trackFade(i+1, -21, fadeTime, 0);
      break;
      case 12:
      wTrig.trackFade(i+1, -22, fadeTime, 0);
      break;
      case 13:
      wTrig.trackFade(i+1, -22, fadeTime, 0);  
      break;
      case 14:
      wTrig.trackFade(i+1, -24, fadeTime, 0);
      break;
    }
   preButtonState[i] = buttonState[i];
   }
  }
 }
}





  
