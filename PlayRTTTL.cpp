#include "pitches.h"
#include "PlayRTTTL.h"

#define isdigit(n) (n >= '0' && n <= '9')

static int notes[] = { 0,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};


PlayRTTTL::PlayRTTTL(int buzzer_pin) {
  is_playing = false;
  buzzerpin = buzzer_pin;
}

void PlayRTTTL::setOctaveOffset(uint8_t offset) {
  octave_offset = offset;
}

bool PlayRTTTL::isPlaying() {
  return is_playing;
}

void PlayRTTTL::stop() {
  noTone(buzzerpin);
  is_playing = false;
}

// call as often as possible!
void PlayRTTTL::update() {
  if (is_playing) {
    if (millis() > nextTone) {
      playNextNote();
    }
  }
}

void PlayRTTTL::mymelodytone(unsigned int frequency, unsigned long duration) {
  tone(buzzerpin, frequency, duration);
  nextTone = millis() + duration;
}

void PlayRTTTL::beep() {
  tone(buzzerpin, NOTE_E5, 200);
}

void PlayRTTTL::tok() {
  tone(buzzerpin, NOTE_E5, 20);
}

void PlayRTTTL::wakawaka() {
  for(int r = 0; r < 5; r++) {
    for(int c = 1000; c > 340; c-=80) {
      tone(buzzerpin, c, 20);
      delay(20);
    }
    for(int d = 340; d < 1000; d+=80) {
      tone(buzzerpin, d, 20);
      delay(20);
    }
  }
  noTone(buzzerpin);
}

void PlayRTTTL::play(const char *thesong) {
  play(thesong, 0);
}

void PlayRTTTL::play(const char *thesong, uint16_t startdelay) {
  if (is_playing) {
    stop();
  }
  song = thesong;
  parseHeader();
  is_playing = true;
  nextTone = millis() + startdelay;
}

void PlayRTTTL::parseHeader() {
  int num;
  int bpm = 63;

  p = song;

  default_dur = 4;
  default_oct = 6;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while (*p != ':') {
    p++;    // ignore name
  }
  //XXX *p = '\0';               // buffer holds name :)
  p++;                     // skip ':'

  // get default duration
  if (*p == 'd') {
    p++; p++;              // skip "d="
    num = 0;
    while (isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    if (num > 0) {
      default_dur = num;
    }
    p++;                   // skip comma
  }

  // get default octave
  if (*p == 'o') {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if (num >= 3 && num <=7) {
      default_oct = num;
    }
    p++;                   // skip comma
  }

  // get BPM
  if (*p == 'b') {
    p++; p++;              // skip "b="
    num = 0;
    while (isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)
}

void PlayRTTTL::playNextNote() {
  if (*p) {
    // first, get note duration, if available
    long duration;
    byte note;
    byte scale;
    int num = 0;
    while (isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }

    if (num) {
      duration = wholenote / num;
    } else {
      duration = wholenote / default_dur;  // we will need to check if we are a dotted note after
    }

    // now get the note
    note = 0;

    switch (*p) {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if (*p == '#') {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if (*p == '.') {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if (isdigit(*p)) {
      scale = *p - '0';
      p++;
    } else {
      scale = default_oct;
    }

    scale += octave_offset;

    if (*p == ',') {
      p++;       // skip comma for next note (or we may be at the end)
    }

    // now play the note
    if (note) {
      mymelodytone(notes[(scale - 4) * 12 + note], duration);
    } else {
      mymelodytone(0, duration);
    }
  } else {
    stop();
  }
}

