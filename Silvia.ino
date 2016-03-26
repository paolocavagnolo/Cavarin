#include <math.h>
//SILVIA 10 NOTE
//LUCA 5 NOTE
#define NUMERO_NOTE 10 //MAI SUPERIORE A 14!
#define NUMERO_PRIME_NOTE 36
#define NUMERO_SCALE 5

//LUCA
#define DIST_MIN 0
#define DIST_MAX 45


#define IST 2

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

int triggerPort = 5;
int echoPort = 2;

int primaNota[36];

int scalaON[NUMERO_NOTE];
/*
int dorian[] = {2, 1, 2, 2, 2, 1, 2};
int phrygian[]  = {1, 2, 2, 2, 1, 2, 2};
int lydian[]    = {2, 2, 2, 1, 2, 2, 1};
int mixolydian[] = {2, 2, 1, 2, 2, 1, 2};
int aeolian[]   = {2, 1, 2, 2, 1, 2, 2};
int locrian[]   = {1, 2, 2, 1, 2, 2, 2};

int lydian_domiant[] = {8, 2, 2, 2, 1, 2, 1, 2};
int super_locrian[]  = {8, 1, 2, 1, 2, 2, 2, 2};
*/
int minor_pentatonic[] = {0, 3, 5, 7, 10, 12, 15, 17, 19, 22, 24, 27, 29, 31};
int major_pentatonic[] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31};
int minor_blues[]    = {0, 3, 5, 6, 7, 10, 12, 15, 17, 18, 19, 22, 24, 27};
int major_blues[]    = {0, 2, 3, 4, 7, 9, 12, 14, 15, 16, 19, 21, 24, 26};

int sol_arm_min[] = {0, 1, 4,5, 7,8, 10,12, 13, 16, 17, 19, 20,22,24};
int major[] = {0,2,4,5,7,9,11,12,14,16,17,19,21,23,24};


int* scales[NUMERO_SCALE] = {major, minor_blues, sol_arm_min, minor_pentatonic, major_pentatonic};

int l_max = 9;
bool on = false;
long duration = 0;


int dist = 0;
int old_dist = 0;

int nota = 36;

#define trigPin  5   // Trigger Pin

#define echoPin1 2    // Echo Pin for sensor # 1

#define MAXIMUM_RANGE  200  // Maximum range needed
#define MINIMUM_RANGE  0    // Minimum range needed

volatile boolean time_elapsed;
volatile byte i;

volatile float vect = -2;

void read_distance(byte sensor_trig_pin, byte sensor_echo_pin);






























float threshold[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int isteresi;

void setup()
{
  //setta le soglie in base al numero di note e allo spazio occupato dalla tua mano
  for (int i = 0; i < NUMERO_NOTE; i++) {
    threshold[i] = (i + 1) * (DIST_MAX - (DIST_MIN + 2)) / NUMERO_NOTE + (DIST_MIN + 5);
  }

  //crea il vettore primaNota
  for (int i = 0; i < 36; i++) {
    primaNota[i] = i + 48;
  }



  pinMode(trigPin, OUTPUT);

  pinMode(echoPin1, INPUT);

  // open the serial port at 9600 bps:
  //Serial.begin(9600);

  // initialize Timer1
  cli();          // disable global interrupts

  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // set entire TCCR1B register to 0

  // set compare match register to desired timer count, corresponds to 1 second
  OCR1A = 1024;   // Timer 1 Output Compare Register A

  // turn on CTC mode on Timer1:
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);

  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);

  // enable Timer1 overflow interrupt:
  //TIMSK1 = (1 << TOIE1);

  // enable global interrupts:
  sei();

  time_elapsed = false;
  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)
{
  read_distance(trigPin, echoPin1);
  time_elapsed = true;
}

int iPlay = 0;
int iPlay_o = 0;

float ist[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};












int potDX;
int potSX;
int potDX_old;
int potSX_old;

int cc;
long int cc_time;

bool appena_finito = false;



void loop()
{
  //check soglie
  for (int j = 0; j < NUMERO_NOTE; j++) {
    if (j == iPlay) {
      ist[j] = IST;
    }
    else {
      ist[j] = 0;
    }
  }

  potSX = map(analogRead(1), 680, 0, 0, 35);

  potDX = (int)fscale(0, 680, 0, NUMERO_SCALE - 1, map(analogRead(0), 680, 0, 0, 680), 5);

  if (potSX != potSX_old) {
    noteOff(1, primaNota[potSX_old], 125);
//    Serial.print("noteOff");
//    Serial.print(" - ");
//    Serial.print(primaNota[potSX_old]);
//    Serial.print(" - ");
//    Serial.println("potSX_old");
    MIDIUSB.flush();
    noteOn(1, primaNota[potSX], 125);
//    Serial.print("noteOn");
//    Serial.print(" - ");
//    Serial.print(primaNota[potSX]);
//    Serial.print(" - ");
//    Serial.println("potSX");
    MIDIUSB.flush();
    cc_time = millis();
  }

  if (((millis() - cc_time) > 1000) && ((millis() - cc_time) < 1050) ) {
    noteOff(1, primaNota[potSX], 125);
//    Serial.print("noteOff");
//    Serial.print(" - ");
//    Serial.print(primaNota[potSX]);
//    Serial.print(" - ");
//    Serial.println("potSX");
    MIDIUSB.flush();
    clearall();
  }

  for (int ww = 0; ww < NUMERO_NOTE; ww++) {
    scalaON[ww] = primaNota[potSX] + scales[potDX][ww];
  }

  potDX_old = potDX;
  potSX_old = potSX;

  //check mano
  if ((vect < DIST_MAX) && (vect > DIST_MIN)) {

    if ((vect < threshold[0] + ist[0])) {
      iPlay = 0;
    }

    for (int k = 1; k < NUMERO_NOTE; k++) {
      if ((vect >= threshold[k - 1] + ist[k - 1]) && (vect < threshold[k] + ist[k])) {
        iPlay = k;
      }
    }

    if (iPlay != iPlay_o) {
      noteOn(1, scalaON[iPlay], 125);
      int suonata = iPlay;
      Serial.print("noteOn");
      Serial.print(" - ");
      Serial.print(scalaON[iPlay]);
      Serial.print(" - ");
    Serial.println("mano cambia_iplay");
      MIDIUSB.flush();
      noteOff(1, scalaON[iPlay_o], 125);
//      Serial.print("noteOff");
//      Serial.print(" - ");
//      Serial.print(scalaON[iPlay_o]);
//      Serial.print(" - ");
//    Serial.println("mano cambia_iplay_o");
      MIDIUSB.flush();
      iPlay_o = iPlay;
    }

    

    /*
    Serial.print(vect);
    Serial.print(" - ");
    if (iPlay == 0) {
      Serial.print("0");
    }
    else
    {
      Serial.print(threshold[iPlay - 1]);
    }
    Serial.print(" - ");
    Serial.print(iPlay);
    Serial.print(" - ");
    Serial.print(threshold[iPlay]);
    Serial.print(" // ");
    Serial.print(iPlay_o);*/
    
    appena_finito = true;
  }
  else if (appena_finito) {
    noteOff(1, scalaON[iPlay], 125);
//    Serial.print("noteOff");
//      Serial.print(" - ");
//      Serial.print(scalaON[iPlay]);
//      Serial.print(" - ");
//    Serial.println("appena_finito iPlay");
    MIDIUSB.flush();
    noteOff(1, scalaON[iPlay_o], 125);
//    Serial.print("noteOff");
//      Serial.print(" - ");
//      Serial.print(scalaON[iPlay_o]);
//      Serial.print(" - ");
//    Serial.println("appena_finito iPlay_o");
    MIDIUSB.flush();
    iPlay_o = -1;
    appena_finito = false;
  }

}



void read_distance(byte sensor_trig_pin, byte sensor_echo_pin) {

  long duration; // Duration used to calculate distance
  float distance;

  digitalWrite(sensor_trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(sensor_trig_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(sensor_trig_pin, LOW);
  duration = pulseIn(sensor_echo_pin, HIGH, 100000);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;

  if (!(distance >= MAXIMUM_RANGE || distance <= MINIMUM_RANGE)) {
    vect = distance;
  }
  else {
    vect = -1;
  }
}

float fscale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue, float curve) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution
   Serial.println();
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  /*
  Serial.print(OriginalRange, DEC);
   Serial.print("   ");
   Serial.print(NewRange, DEC);
   Serial.print("   ");
   Serial.println(zeroRefCurVal, DEC);
   Serial.println();
   */

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}

void clearall() {

  for (int i = 10; i < 130; i++) {
    noteOff(1, i, 125);
    MIDIUSB.flush();
  }

}

