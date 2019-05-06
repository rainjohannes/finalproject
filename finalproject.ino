int stretch = A14; // the stretch sensor
int bendToggle = 35; // the toggle controlling direction of pitchbend
int modToggle = 34; // the toggle to alternate between pitchbend and modulation

int lastStretchVal;
int currentStretchVal;

int delayTime = 8;

int highReadVal = 410; // highest usable stretch sensor value
int lowReadVal = 260; // lowest usable stretch sensor value
int stepVal = 1;

int bendValUp; // mapped value for pitchbend up
int bendValDown; // mapped value for pitchbend down
int bendMax = 8191; // MIDI value for maximum pitchbend up
int bendNone = 0; // MIDI value for no applied pitchbend
int bendMin = -8192; // MIDI value for maximum pitchbend down

int modVal; // amount of modulation mapped to MIDI
int modMax = 127; // MIDI value for maximum modulation
int modMin = 0; // MIDI value for no applied modulation

void setup() {
  pinMode(bendToggle, INPUT); // use bendToggle as input
  pinMode(modToggle, INPUT); // use modToggle as input
}

void loop() {
  checkStretch();

}

void checkStretch() {
  lastStretchVal = currentStretchVal; // set lastStretchVal to value of stretch sensor as it was in the previous loop
  delay(delayTime); // wait a little
  currentStretchVal = (analogRead(stretch)); // update currentStretchVal to the present value of the stretch sensor

  if (lastStretchVal == currentStretchVal) { // if the current raw value is the saw as it was the last time around...
    return; // do nothing and start the loop again
  }

  else if (currentStretchVal <= lowReadVal) { // otherwise, if the currentStretchVal is below our readable threshold...
    if (digitalRead(modToggle) == LOW) { // check if we're set to pitchbend. If so...
      usbMIDI.sendPitchBend(bendNone, 1); // send a pitchbend value of 0
    }

    else if (digitalRead(modToggle) == HIGH) { // if we're set to modulation...
      usbMIDI.sendControlChange(1, modMin, 1); // send a modulation value of 0
    }

    return; // just to be safe, don't use the code below if currentStretchVal is below the readable threshold

  }


  else if (currentStretchVal <= highReadVal) {
    // if current value is not equal to lastStretchVal, not lower than lowReadVal, and less than highReadVal

    bendValUp = map(currentStretchVal, lowReadVal, highReadVal, bendNone, bendMax); //map currentStretchVal to upwards MIDI pitchbend values
    bendValDown = map(currentStretchVal, lowReadVal, highReadVal, bendNone, bendMin); //map currentStretchVal to downwards MIDI pitchbend values
    modVal = map(currentStretchVal, lowReadVal, highReadVal, modMin, modMax); //map currentStretchVal to MIDI modulation values

    if (digitalRead(modToggle) == LOW) { //if modToggle is OFF:

      if (digitalRead(bendToggle) == LOW) { // is bendToggle OFF?
        if (currentStretchVal >= highReadVal) {
          usbMIDI.sendPitchBend(bendMin, 1); //if currentStretchVal is over highReadVal, send bendMin
        }

        usbMIDI.sendPitchBend(bendValDown, 1); //if currentStretchVal is not over highReadVal, send bendValDown
      }

      else if (digitalRead(bendToggle) == HIGH) { // is bendToggle ON?
        if (currentStretchVal >= highReadVal) {
          usbMIDI.sendPitchBend(bendMax, 1); //if currentStretchVal is over highReadVal, send bendMax
        }
        
        usbMIDI.sendPitchBend(bendValUp, 1); //if currentStretchVal is not over highReadVal, send bendValUp
      }
    }

    else if (digitalRead(modToggle) == HIGH) { //If modToggle is ON:
      if (currentStretchVal >= highReadVal) {
        usbMIDI.sendControlChange(1, modMax, 1); //if currentStretchVal is over highReadVal, send modMin
      }
      
      usbMIDI.sendControlChange(1, modVal, 1); //if currentStretchVal is not over highReadVal, send modVal

    }

  }

}
