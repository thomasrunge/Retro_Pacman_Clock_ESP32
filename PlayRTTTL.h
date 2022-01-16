#ifndef _play_rtttl_h
#define _play_rtttl_h

#include <Arduino.h>

class PlayRTTTL {
 public:
  PlayRTTTL(int buzzer_pin);
  void setOctaveOffset(uint8_t offset);
  void play(const char *song);
  void play(const char *song, uint16_t startdelay);
  bool isPlaying();
  void stop();
  void beep();
  void tok();
  void wakawaka();
  void update();

 private:
  void mymelodytone(unsigned int frequency, unsigned long duration);
  int buzzerpin;
  const char *song;
  uint8_t octave_offset;

  void parseHeader();
  void playNextNote();
  bool is_playing;
  byte default_dur;
  byte default_oct;
  long wholenote;
  const char *p;
  unsigned long nextTone;
};

#endif // _play_rtttl_h

