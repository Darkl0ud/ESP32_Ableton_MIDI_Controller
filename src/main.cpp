#include <main.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

class button
{
private:
 
public:
  button();
  ~button();
  bool bActive = false;

  void Activate()
  {
      bActive = true;
  }

  void Deactivate()
  {
      bActive = false;
  }
};

button::button()
{
}

button::~button()
{
}

button NewButton;

void button41Pressed() 
{
  if (digitalRead(41) == 1 && !NewButton.bActive)  {
  MIDI.sendNoteOn(42,127,1);
  digitalWrite(LED_BUILTIN, HIGH);
  NewButton.Activate();
  }
  else  if (digitalRead(41) == 0)
  {
  MIDI.sendNoteOff(42,127,1);
  digitalWrite(LED_BUILTIN, LOW);
  NewButton.Deactivate();  }
}

void setup()
{
    usb_midi.setStringDescriptor("TinyUSB MIDI");
    pinMode(LED_BUILTIN, OUTPUT);
    MIDI.begin(4);                                  // Launch MIDI and listen to channel 4

    pinMode(41, INPUT_PULLDOWN);

  while( !TinyUSBDevice.mounted() ) delay(1);        // wait until device mounted

}

//-----------------------
//
//-----------------------
void loop()
{
  delay(1);
  button41Pressed();
}