
#define NOTE_TOTAL_NUMBER 5   //min 1 max 14
#define SCALE_TOTAL_NUMBER 5  //min max
#define DIST_MIN 50           //cm of a bottom dead bound
#define DIST_MAX 100          //cm of the upper limit
#define IST 2                 //accuracy of the isteresys cycle on the 'distance to note interval'

//Definition of a function
int istRead(int inputValue, int inputMax, int inputMin, int ist, int intervalTotal){

  if ((inputValue < inputMax) && (input > inputMin)) {

    if ((inputValue < threshold[0] + ist)) {
      interval = 0;
    }

    for (int k = 1; k < intervalTotal; k++) {
      if ((inputValue >= (threshold[k - 1] + ist)) && (vect < (threshold[k] + ist))) {
        interval = k;
      }
    }

    return interval;
}

void noteOn(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MIDIUSB.write(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MIDIUSB.write(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  MIDIEvent event = {0x0B, 0xB0 | channel, control, value};
  MIDIUSB.write(event);
}

void setup()
{
  //initialize ist[]
  for (int i = 0; i < NOTE_TOTAL_NUMBER; i++) {
    ist[i] = 0;
  }

  //threshold set
  for (int i = 0; i < NOTE_TOTAL_NUMBER; i++) {
    threshold[i] = (i + 1) * (DIST_MAX - (DIST_MIN + 2)) / NOTE_TOTAL_NUMBER + (DIST_MIN + 3);
  }

}

void loop()
{
}
