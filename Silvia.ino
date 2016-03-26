
#define NOTE_TOTAL_NUMBER 5           //min 1 max 14
#define SCALE_TOTAL_NUMBER 5          //min max
#define FIRSTNOTE_TOTAL_NUMBER 36     //firstnote pot
#define DIST_MIN 50                   //cm of a bottom dead bound
#define DIST_MAX 100                  //cm of the upper limit
#define IST 2                         //accuracy of the isteresys cycle on the 'distance to note interval'

int thresholdDistance[NOTE_TOTAL_NUMBER];
int thresholdPotNote[FIRSTNOTE_TOTAL_NUMBER];
int thresholdPotScale[SCALE_TOTAL_NUMBER];

int intervalDistance;
int intervalPotNote;
int intervalPotScale;

int istRead(int inputValue, int thresholdArray[], int thresholdArrayNum, int inputMax, int inputMin, int ist, int intervalTotal){

  for (int z=0; z < thresholdArrayNum; z++) {
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
      for (int k = 1; k < thresholdArrayNum; k++) {
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

void setup()
{
  for (int i=0; i < NOTE_TOTAL_NUMBER; i++){
    thresholdDistance[i] = 0;
  }
  for (int ii=0; i < FIRSTNOTE_TOTAL_NUMBER; i++){
    thresholdPotNote[ii] = 0;
  }
  for (int iii=0; i < SCALE_TOTAL_NUMBER; i++){
    thresholdPotScale[iii] = 0;
  }
}

void loop()
{
}
