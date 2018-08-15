
void StepperRotate(int dir, int rpm, int arc) {

  //1=00
  //2=10
  //3=01
  //4=11

  StepperTick = micros();
  DirA = (dir % 2) > 0 ? -1 : 1 ;
  DirB = (dir > 2) ? 1 : -1;
  StepperDelay = (1000000 / ((rpm * StepsPerRev) / 60));

  if (StepperTick > (LastStep + StepperDelay)) {
    Serial.println(Steps);

    if (FullStep = 2) {
      FullStepSequence(DirA);
    }
    else {
      HalfStepSequence(DirA);
    }
    digitalWrite(motorPin1, w1);
    digitalWrite(motorPin2, w2);
    digitalWrite(motorPin3, w3);
    digitalWrite(motorPin4, w4);

    if (FullStep = 2) {
      FullStepSequence(DirB);
    }
    else {
      HalfStepSequence(DirB);
    }
    digitalWrite(motorPin5, w1);
    digitalWrite(motorPin6, w2);
    digitalWrite(motorPin7, w3);
    digitalWrite(motorPin8, w4);


    LastStep = micros();

    if (Steps > 0) {
      --Steps;
    }
    // Queue();
  }

}

void HalfStepSequence(int dir) {
  w1 = (Steps % 8) > 4 ? 1 : 0;
  w2 = ((Steps + (2 * dir)) % 8) > 4 ? 1 : 0;
  w3 = ((Steps + (4 * dir)) % 8) > 4 ? 1 : 0;
  w4 = ((Steps + (6 * dir)) % 8) > 4 ? 1 : 0;
}

void FullStepSequence(int dir) {
  w1 = (Steps % 4) > 1 ? 1 : 0;
  w2 = ((Steps + (1 * dir)) % 4) > 1 ? 1 : 0;
  w3 = ((Steps + (2 * dir)) % 4) > 1 ? 1 : 0;
  w4 = ((Steps + (3 * dir)) % 4) > 1 ? 1 : 0;
}

void SteppersOff () {
  digitalWrite(motorPin1, 0);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, 0);
  digitalWrite(motorPin4, 0);
  digitalWrite(motorPin5, 0);
  digitalWrite(motorPin6, 0);
  digitalWrite(motorPin7, 0);
  digitalWrite(motorPin8, 0);
}

//Wheel Step Queue
void Queue () {
  if (Steps > 0) {
    --Steps;
  }
}

