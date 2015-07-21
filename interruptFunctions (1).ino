void dotDash() {
  dotDashPinState = digitalRead(dotDashPin);
  volatile int currentTime = myMillis();
  if ((previousDotDashState != dotDashPinState) && (currentTime - previousDotDashTime > 200) && dotDashNumber < 6) {
    previousDotDashState = dotDashPinState;
    previousDotDashTime = currentTime;
    volatile int tempDuration;
    switch (dotDashPinState) {
      case 0:
        end_count[dotDashNumber] = currentTime;
        noTone(speakerPin);
        tempDuration = end_count[dotDashNumber] - start_count[dotDashNumber];
        if (tempDuration > minLengthDot && tempDuration <= maxLengthDot) {
          Serial.print("You just entered a dot at a time of: ");
          Serial.print((float)tempDuration / 1000.0);
          Serial.println(" seconds");
        }
        else {
          Serial.print("You just entered a dash at a time of: ");
          Serial.print((float)tempDuration / 1000.0);
          Serial.println(" seconds");
        }
        dotDashNumber++;
        break;
      case 1:
        start_count[dotDashNumber] = currentTime;
        tone(speakerPin, 500);
        Serial.print("The dot dash number is: ");
        Serial.println(dotDashNumber);
        break;
    }
    if (dotDashNumber == 0 && currentCharacter == 0) {
      startOfMessage = currentTime;
    }
  }

}

void otherButtonPressed() {
  if ((millis() - debounceOtherTime) > 2000) {
    if (digitalRead(characterPin) == 0 && dotDashNumber != 0) {
      endOfCharacter();
    }
    else if (digitalRead(wordPin) == 0) {
      if (dotDashNumber == 0) {
        endOfWord();
      }
      else {
        deletingCharacter();
      }
    }
    else if ((digitalRead(playbackPin) == 0)) {
      playback();
    }
  }
  debounceOtherTime = millis();
}

