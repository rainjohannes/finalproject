int pot = A14;
int bendToggle = 35;
int modToggle = 34;

int lastStretchVal;
int currentStretchVal;

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
  lastStretchVal = currentStretchVal;
  lastBendVal = currentBendVal;
  lastModVal = currentModVal;

  if (millis() >= lastSmoothTime + smoothTime) {
    if ((analogRead(pot) - lastStretchVal >= 4) || (lastStretchVal - analogRead(pot) >= 4)) {
      currentStretchVal = (analogRead(pot));
    }

    if (currentStretchVal <= lowReadVal) {
      currentBendVal = 0;
      currentModVal = 0;
    }

    else if (currentStretchVal <= highReadVal) {
      currentModVal = map(currentStretchVal, lowReadVal, highReadVal, modLow, modHigh);

      if (digitalRead(bendToggle) == HIGH) {
        currentBendVal = map(currentStretchVal, lowReadVal, highReadVal, 0, bendHigh);
      }

      else if (digitalRead(bendToggle) == LOW) {
        currentBendVal = map(currentStretchVal, lowReadVal, highReadVal, 0, bendLow);
      }
    }

    if (digitalRead(modToggle) == LOW) {
      if (lastBendVal != currentBendVal); {
        Serial.println(currentBendVal);
        usbMIDI.sendPitchBend(currentBendVal, 1);
      }
    }

    else if (digitalRead(modToggle) == HIGH) {
      if (lastModVal != currentModVal); {
        Serial.println(currentModVal);
        usbMIDI.sendControlChange(1, currentModVal, 1);
      }
    }

    lastSmoothTime = millis();

  }

}
