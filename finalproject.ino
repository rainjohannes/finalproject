int pot = A14;

int lastPotVal;
int currentPotVal;
int mappedPot;

int lowReadVal = 50;
int highReadVal = 100;

int pitchBendHigh = 8191;
int pitchBendLow = -8192;

int smoothTime = 1;
unsigned long lastSmoothTime = millis();

void setup() {
  Serial.begin(9600);

}

void loop() {
  checkPot();

}

void checkPot() {
  lastPotVal = currentPotVal;

  //if (millis() >= lastSmoothTime + smoothTime) {
  currentPotVal = (analogRead(pot));

  if (currentPotVal <= lowReadVal) {
    mappedPot = 0;
  }

  else if (map(currentPotVal, lowReadVal, highReadVal, 0, pitchBendHigh) >= 0) {
    mappedPot = map(currentPotVal, lowReadVal, highReadVal, 0, pitchBendHigh);
  }

  //lastSmoothTime = millis();


  if (lastPotVal != currentPotVal) {
    Serial.println(currentPotVal);
    usbMIDI.sendPitchBend(mappedPot, 1);
  }

}
