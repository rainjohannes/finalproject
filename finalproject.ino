int stretch = A14;
int bendToggle = 35;
int modToggle = 34;

int lastStretchVal;
int currentStretchVal;
int midiStretchVal;

int lastBendVal;
int currentBendVal;

int lastModVal;
int currentModVal;

int highReadVal = 100;
int lowReadVal = 50;

int bendHigh = 8191;
int bendLow = -8192;

int modHigh = 127;
int modLow = 0;

int smoothTime = 1;
unsigned long lastSmoothTime = millis();

void setup() {
  Serial.begin(9600);
  pinMode(bendToggle, INPUT);
  pinMode(modToggle, INPUT);

}

void loop() {
  checkStretch();

}

void checkStretch() {
  //lastStretchVal = currentStretchVal;
  //currentStretchVal = (analogRead(stretch));
  //lastBendVal = currentBendVal;
  //lastModVal = currentModVal;

  //if (millis() >= lastSmoothTime + smoothTime) {
  //if ((currentStretchVal - lastStretchVal >= 4) || (lastStretchVal - currentStretchVal >= 4)) {
  midiStretchVal = (analogRead(stretch));

  if (midiStretchVal <= lowReadVal) {
    currentBendVal = 0;
    currentModVal = 0;
  }

  else if (midiStretchVal <= highReadVal) {
    currentModVal = map(midiStretchVal, lowReadVal, highReadVal, modLow, modHigh);

    if (digitalRead(bendToggle) == HIGH) {
      currentBendVal = map(midiStretchVal, lowReadVal, highReadVal, 0, bendHigh);
    }

    else if (digitalRead(bendToggle) == LOW) {
      currentBendVal = map(midiStretchVal, lowReadVal, highReadVal, 0, bendLow);
    }
  }

  if (digitalRead(modToggle) == LOW) {
    //Serial.println(currentBendVal);
    usbMIDI.sendPitchBend(currentBendVal, 1);
  }

  else if (digitalRead(modToggle) == HIGH) {
    //Serial.println(currentModVal);
    usbMIDI.sendControlChange(1, currentModVal, 1);
  }

  //lastSmoothTime = millis();
}
