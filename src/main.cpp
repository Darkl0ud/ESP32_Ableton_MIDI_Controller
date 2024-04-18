#include <main.h>

void scanKB() //  CHANGE SO WE ITERATE OVER ALL KEYS OF THE KB AND CHANGE THE STATE OF THE MATCHING KEYOBJARR INDEX AND SET THE MODE OF THAT NOTE DEPENDING ON THE MODIFIER KEYS HELD
{
  kbMIDINoteOffset = 0; // Reset offset for which MIDI note we are checking for next.. Added to kbLowestPhysicalKey to determine which key to sense for

  for (uint8_t& OutPin : kbOutArr)  // For each kb OUTPUT pin...
  {
    digitalWrite(OutPin, 1);  // Set each collumn / row HIGH

    for (uint8_t& inPin : kbInArr)  // For each kb INPUT pin...
    {
      
      currentKbMIDINote = kbLowestPhysicalKey + kbMIDINoteOffset - 21;  // Determines which key we are sensing for. Subtracting 21 because kbObjArr.iNote starts at 21, which corresponds to MIDI value lowest note
      /*
      if (digitalRead(inPin) == 1 && !keyObjArr[currentKbMIDINote].bActive) // If we push a kb button and the corresponding keyObj is not active...
      {
        MIDI.sendNoteOn(keyObjArr[currentKbMIDINote].iNote,127,1);
        keyObjArr[currentKbMIDINote].bActive=true;
      }
      else if (digitalRead(inPin) == 0 && keyObjArr[currentKbMIDINote].bActive  // If we release a kb button, it was active,
              && keyObjArr[currentKbMIDINote].noteMode == 0)  //  If note mode is not toggle
      {
        MIDI.sendNoteOff(keyObjArr[currentKbMIDINote].iNote,127,1);
        keyObjArr[currentKbMIDINote].bActive=false;
      }
      */

      kbMIDINoteOffset++;
    }
    digitalWrite(OutPin, 0);  // Set each collumn / row LOW
  }
}

void setupKeys()
{
  int midiNoteIter = 21;  // Lowest MIDI note value... This is incremented and set to the iNote val for each key in kb array.
  for (key& inKey : keyObjArr) // For each key in the key array...
  {
    inKey.iNote = midiNoteIter; // Set each keys note, increasing each iteration of loop starting at middle C
    midiNoteIter++;
  }
}

void setupGPIO()  // Sets GPIO pins as INPUT or OUTPUT.
{
  for (uint8_t& pin : kbOutArr)  // For each kb OUTPUT...
  {
    pinMode(pin, OUTPUT); // Set GPIO pin mode as OUTPUT
  }

  for (uint8_t& pin : kbInArr)  // For each kb INPUT...
  {
    pinMode(pin, INPUT_PULLDOWN); // Set GPIO pin mode as INPUT
  }

  for (uint8_t& pin : absolutePotArr)  // For each Absolute Potentiometer OUTPUT...
  {
    pinMode(pin, INPUT_PULLDOWN); // Set GPIO pin mode as INPUT
  }
}

bool setupGPIOtest()
{
    for (uint8_t& pin : kbOutArr)  // For each kb OUTPUT...
  {
    pinMode(pin, OUTPUT); // Set GPIO pin mode as OUTPUT
  }

  for (uint8_t& pin : kbInArr)  // For each kb INPUT...
  {
    pinMode(pin, INPUT_PULLDOWN); // Set GPIO pin mode as INPUT
  }

  for (uint8_t& pin : absolutePotArr)  // For each Absolute Potentiometer OUTPUT...
  {
    pinMode(pin, INPUT_PULLDOWN); // Set GPIO pin mode as INPUT
  }
  return 0;
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
  //EMA_S = (EMA_a*analogRead(7)) + ((1-EMA_a)*EMA_S);    // Smooths analog reading
  //int EMA_SintConv = EMA_S;  // Shift bits right, 12 bit to 7 bit
  //int latest = EMA_SintConv >> 5;
  //Serial.println(EMA_SintConv);
  //Serial.println(latest);
  //Serial.println("Free memory: " + String(esp_get_free_heap_size()) + " bytes");
}