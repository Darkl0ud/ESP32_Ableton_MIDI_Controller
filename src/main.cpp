#include <main.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

int buttonPinArr[4] = {41,38,35,47};  // These are the pins on MC used for note input buttons
int buttonNoteArr[4] = {48,55,62,69}; //  These are the notes that correspond to the input buttons
button buttonObjArr[4];  // Creates an array of 4 buttons. This must be increased as button inputs increase

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
    
  delay(1);
  checkPins();
}