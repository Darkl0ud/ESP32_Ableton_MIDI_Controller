#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <key.h>

Adafruit_USBD_MIDI usb_midi;  // USB MIDI object
key keyObjArr[88];  // Array to store created keyboard keys

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);// Create a new instance of the Arduino MIDI Library, usb_midi as transport
int kbMIDINoteOffset;   // Offset applied to kbLowestPhysicalKey for currentKbMIDINote
int currentKbMIDINote;  // Current MIDI note scanKB() is checking for activity
static int kbLowestPhysicalKey = 48;    // Sets the lowest note on the keyboard, 48 = C3
static int kbOutArr[3] = {12,13,14};  // These are the GPIO OUTPUT on MC used for keyboard matrix INPUT, +VCC
static int kbInArr[3] = {35,36,37};  // These are the GPIO INPUT on MC used for keyboard matrix OUTPUT, DATA
static int absolutePotArr[1] = {7}; // Array to store absolute potentiometer obj

float EMA_a = 0.04;      // EMA Alpha
int EMA_S;          // EMA S