#include <main.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

int buttonPinArr[4] = {41,38,35,47};  // These are the pins on MC used for keyboard note input buttons
int buttonNoteArr[4] = {48,55,62,69}; //  These are the MIDI note values that correspond to the input buttons
button buttonObjArr[4];  // Creates an array of 4 buttons. This must be increased as button inputs increase
int ccPinArr[1] = {7};
int ccNumArr[1] = {1};
cc ccObjArr[1];
void setupPins()
{
  for (int i = 0; i < sizeof(buttonPinArr) / sizeof(buttonPinArr[0]); i++)
  {
    button Button[i]; // Creates Button from button class
    Button[i].iNote = buttonNoteArr[i]; //  Sets the note for the Button to the corresponding note in note array
    buttonObjArr[i] = Button[i];  // Sets the Button object into the button object array
  }
}

void checkPins()
{
  for (int i = 0; i < sizeof(buttonPinArr) / sizeof(buttonPinArr[0]); i++)
  {
    if (digitalRead(buttonPinArr[i]) == 1 && !buttonObjArr[i].bActive)
    {
      MIDI.sendNoteOn(buttonObjArr[i].iNote,127,1);
      buttonObjArr[i].bActive = true;
      buttonObjArr[i].bInactive = false;
    }
    else if(digitalRead(buttonPinArr[i]) == 0 && !buttonObjArr[i].bInactive)
    {
      MIDI.sendNoteOff(buttonObjArr[i].iNote,127,1);
      buttonObjArr[i].bActive = false;
      buttonObjArr[i].bInactive = true;
    }
  }
  
  for (int i = 0; i < sizeof(ccPinArr) / sizeof(ccPinArr[0]); i++)
  {
    if (digitalRead(ccPinArr[i]) == 1 && !ccObjArr[i].bActive)
    {
      MIDI.sendControlChange(1,127,1);
      ccObjArr[i].bActive = true;
      ccObjArr[i].bInactive = false;
    }
    else if(digitalRead(ccPinArr[i]) == 0 && !ccObjArr[i].bInactive)
    {
      MIDI.sendControlChange(1,0,1);
      ccObjArr[i].bActive = false;
      ccObjArr[i].bInactive = true;
    }
  }
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//-----------------------
//  Setup
//-----------------------
void setup()
{
  usb_midi.setStringDescriptor("TinyUSB MIDI");
  pinMode(LED_BUILTIN, OUTPUT);
  MIDI.begin(4);                                  // Launch MIDI and listen to channel 4

  pinMode(41, INPUT_PULLDOWN);
  pinMode(38, INPUT_PULLDOWN);
  pinMode(35, INPUT_PULLDOWN);
  pinMode(47, INPUT_PULLDOWN);

  while( !TinyUSBDevice.mounted() ) delay(1);        // wait until device mounted
  setupPins();

}

//-----------------------
//  Loop
//-----------------------
void loop()
{
  checkPins();
  MIDI.read();

  int temp = analogRead(7);
  int weeed = floatMap(temp,0,4095,0,127);
  Serial.println(weeed);
}