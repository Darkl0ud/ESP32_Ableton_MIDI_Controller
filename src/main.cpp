//  button Button[i]; // Creates Button from button class
//  Button[i].iNote = kbOutArr[i]; //  Sets the note for the Button to the corresponding note in note array
//  buttonObjArr[i] = Button[i];  // Sets the Button object into the button object array

//void checkPins()
//{
//  for (int i = 0; i < sizeof(kbOutArr) / sizeof(kbOutArr[0]); i++)
//  {
//    if (analogRead(kbOutArr[i]) > 0 && !buttonObjArr[i].bActive)  // If our input is greater than one and button is not already active...
//    {
//      MIDI.sendNoteOn(buttonObjArr[i].iNote,127,1);
//      buttonObjArr[i].bActive = true;
//      buttonObjArr[i].bInactive = false;
//    }
//    else if(digitalRead(kbOutArr[i]) == 0 && !buttonObjArr[i].bInactive)
//    {
//      MIDI.sendNoteOff(buttonObjArr[i].iNote,127,1);
//      buttonObjArr[i].bActive = false;
//      buttonObjArr[i].bInactive = true;
//    }
//  }
//}

#include <main.h>

void scanKB()
{
  int kbMIDINoteOffset = 0;

  for (int OutPin : kbOutArr)  // For each kb OUTPUT pin...
  {
    digitalWrite(OutPin, 1);  // Set each collumn / row HIGH

    for (int inPin : kbInArr)  // For each kb INPUT pin...
    {
      int currentKbMIDINote = kbMIDIStartNote + kbMIDINoteOffset;
      
      if (digitalRead(inPin) == 1 && !keyObjArr[currentKbMIDINote].bActive)
      {
        MIDI.sendNoteOn(currentKbMIDINote,127,1);
        keyObjArr[currentKbMIDINote].bActive=true;
      }
      else if (digitalRead(inPin) == 0 && keyObjArr[currentKbMIDINote].bActive)
      {
      MIDI.sendNoteOff(currentKbMIDINote,127,1);
       keyObjArr[currentKbMIDINote].bActive=false;
      }

      kbMIDINoteOffset++;
    }
    digitalWrite(OutPin, 0);  // Set each collumn / row LOW
  }
}

void setupKeys()
{
  int iter = 0;
  for (key inKey : keyObjArr) // For each key in the key array...
  {
    inKey.iNote = inKey.iNote + iter; // Set each keys note, increasing each iteration of loop starting at middle c
    iter++;
  }
}

void setupGPIO()  // Sets GPIO pins as INPUT or OUTPUT.
{
  for (int pin : kbOutArr)  // For each kb OUTPUT...
  {
    pinMode(pin, OUTPUT); // Set GPIO pin mode as OUTPUT
  }

  for (int pin : kbInArr)  // For each kb INPUT...
  {
    pinMode(pin, INPUT_PULLDOWN); // Set GPIO pin mode as INPUT
  }
}

void setup()
{
  setupGPIO();
  setupKeys();
  usb_midi.setStringDescriptor("TinyUSB MIDI");
  MIDI.begin(1);                                  // Launch MIDI and listen to channel 1 
  while( !TinyUSBDevice.mounted() ) delay(1);        // wait until device mounted
}

void loop()
{
  MIDI.read();
  scanKB();
}