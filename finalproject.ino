int pot = A1;

int lastPotVal;
int currentPotVal;
int mappedPot;

int smoothTime = 20;
unsigned long lastSmoothTime = millis();

void setup() {
  Serial.begin(9600);

}

void loop() {
  checkPot();

}

void checkPot() {
  lastPotVal = currentPotVal;
  
  if (millis() >= lastSmoothTime + smoothTime) {
    currentPotVal = (analogRead(pot));
    mappedPot = map(currentPotVal, 1, 1023, -8192, 8191);
    lastSmoothTime = millis();
  }

  if (lastPotVal != currentPotVal) {
    Serial.println(mappedPot);
    usbMIDI.sendPitchBend(mappedPot, 1);
  }
}
