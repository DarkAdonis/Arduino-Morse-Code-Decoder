ISR(TIMER1_OVF_vect) {
  overflowCycles += 1;
  //Serial.println(overflowCycles);
}

unsigned long myMillis() {
  volatile unsigned long tempNumberofCycles = (TCNT1 + overflowCycles * 512);
  volatile unsigned long  theTime = (tempNumberofCycles * (1 / (16e6 / 64))) * 1000;
  return theTime;
}

void myDelay(int waitTime) {
  volatile unsigned long theDelay = myMillis() + (unsigned long) waitTime;
  while (myMillis() < theDelay) {
    //do Nothing
  }
}

