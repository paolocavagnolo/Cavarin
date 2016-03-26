//Arduino definition
#define trigPin  5          // Trigger Pin
#define echoPin 2           // Echo Pin

//User definition
#define NOTE_TOTAL_NUMBER 5           //min 1 max 14
#define SCALE_TOTAL_NUMBER 5          //min max
#define FIRSTNOTE_TOTAL_NUMBER 36     //firstnote pot
#define DIST_MIN 50                   //cm of a bottom dead bound
#define DIST_MAX 100                  //cm of the upper limit
#define IST 2                         //accuracy of the isteresys cycle on the 'distance to note interval'



//Definition of the 3 analog measure we need, as interval, in such a way we can apply the isteresys function instead of the MAP function
int intervalDistance = 0;
int intervalPotNote = 0;
int intervalPotScale = 0;

int thresholdDistance[NOTE_TOTAL_NUMBER];
int thresholdPotNote[FIRSTNOTE_TOTAL_NUMBER];
int thresholdPotScale[SCALE_TOTAL_NUMBER];

int istRead(int interval, int inputValue, int thresholdArray[], int inputMax, int inputMin, int ist, int intervalTotal){

  for (int z=0; z < intervalTotal; z++) {
    thresholdArray[z] = (z + 1) * (inputMax - (inputMin + 2)) / intervalTotal + inputMin;
    if (z == interval) {
      thresholdArray[z] += ist;
    }
  }

  if ((inputValue <= inputMax) && (inputValue >= inputMin)) {
    if (inputValue < thresholdArray[0]) {
      interval = 0;
    }
    else {
      for (int k = 1; k < intervalTotal; k++) {
        if ((inputValue >= thresholdArray[k-1]) && (inputValue < thresholdArray[k])) {
          interval = k;
        }
      }
    }
  }
  else {
    return -1;
  }

}

//Definition of the measurement from the sensor
volatile float vect = -2;

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
  vect = duration / 58.2;
}

ISR(TIMER1_COMPA_vect)
{
  read_distance(trigPin, echoPin);
}


void setup()
{
  //isteresys part
  for (int i=0; i < NOTE_TOTAL_NUMBER; i++){
    thresholdDistance[i] = 0;
  }
  for (int ii=0; ii < FIRSTNOTE_TOTAL_NUMBER; ii++){
    thresholdPotNote[ii] = 0;
  }
  for (int iii=0; iii < SCALE_TOTAL_NUMBER; iii++){
    thresholdPotScale[iii] = 0;
  }

  //distance sensor part
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  cli();                          // disable global interrupts
  TCCR1A = 0;                     // set entire TCCR1A register to 0
  TCCR1B = 0;                     // set entire TCCR1B register to 0
  OCR1A = 1024;                   // Timer 1 Output Compare Register A set to 1 second
  TCCR1B |= (1 << WGM12);         // turn on CTC mode on Timer1
  TCCR1B |= (1 << CS10);          // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12);          // Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);        // enable timer compare interrupt
  sei();                          // enable global interrupts:
}

void loop()
{
  intervalDistance = istRead(intervalDistance, vect, thresholdDistance, DIST_MIN, DIST_MAX, NOTE_TOTAL_NUMBER);
}
