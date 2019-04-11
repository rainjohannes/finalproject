int pot = A1;

int lastPotVal;
int currentPotVal;
int mappedPot;

void setup() {
  Serial.begin(9600);

}

void loop() {
  checkPot();

}

void checkPot() {
  lastPotVal = currentPotVal;
  currentPotVal = (analogRead(pot));
  mappedPot = map(currentPotVal, 1, 1023, -8192, 8191);

  if (lastPotVal != currentPotVal) {
    Serial.println(currentPotVal);
    usbMIDI.sendPitchBend(mappedPot, 1);
  }
}
