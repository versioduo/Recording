#include "MIDIUSB.h"

enum {
  PIN_SWITCH = A0
};

enum {
  MIDI_STATUS_NOTE_OFF =  8,
  MIDI_STATUS_NOTE_ON =   9,
  MIDI_STATUS_CC =       11
};

enum {
  MIDI_CC_RECORD =          3, // The first undefined
  MIDI_CC_ALL_NOTES_OFF = 123
};


void setup() {
  Serial.begin(9600);
  pinMode(PIN_SWITCH, OUTPUT);
  digitalWrite(PIN_SWITCH, HIGH);
}

void loop() {
  midiEventPacket_t rx = MidiUSB.read();

  uint8_t command = (rx.byte1 >> 4);
  if (command == 0)
    return;

  switch (command) {
    case MIDI_STATUS_NOTE_ON:
      Serial.println("NoteOn");
      digitalWrite(PIN_SWITCH, LOW);
      break;

    case MIDI_STATUS_NOTE_OFF:
      Serial.println("NoteOff");
      digitalWrite(PIN_SWITCH, HIGH);
      break;

    case MIDI_STATUS_CC:
      switch (rx.byte2) {
        case MIDI_CC_RECORD:
          Serial.print("CC Record ");
          Serial.println(rx.byte3 < 64 ? "Off" : "On");
          digitalWrite(PIN_SWITCH, rx.byte3 < 64);
          break;

        case MIDI_CC_ALL_NOTES_OFF:
          Serial.print("CC AllNotesOff");
          digitalWrite(PIN_SWITCH, HIGH);
          break;

        default:
          Serial.print("CC ");
          Serial.print(rx.byte2);
          Serial.print(" ");
          Serial.println(rx.byte3);
      }
  }
}
