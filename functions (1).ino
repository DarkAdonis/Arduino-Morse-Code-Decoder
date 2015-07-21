void recordCharacter() {
  //Serial.println(dotDashNumber);
  if (dotDashNumber == 6) {
    float messageDuration = ((float)(myMillis() - startOfMessage)) / 1000.0;
    Serial.println("Stop inputting! Converting your message");
    noInterrupts();
    int startValue = 0;
    thePhrase = "";
    for (int i = 0; i < currentCharacter && i < 75; i++) { //75 is max number of characters
      if (messageArray[i] == 2) {
        //find character and add to string
        thePhrase = thePhrase + makeIntoLetter(i, startValue);
        startValue = i + 1;
      }
      else if (messageArray[i] == 3) {
        //add space
        thePhrase = thePhrase + " ";
        startValue = i + 1;
      }
    }
    Serial.print("Your phrase is: ");
    Serial.println(thePhrase);
    Serial.print("You inputted your phrase in: ");
    Serial.print(messageDuration);
    Serial.println(" seconds.");
    reset();
    interrupts();
  }
  else {
    if (currentCharacter + dotDashNumber < 74) { //must be less than 74 in order to leave room for the end of character character
      for (int i = 0; i < dotDashNumber; i++) {
        duration = (end_count[i] - start_count[i]);
        secDuration = (float)duration / 1000.0;
        if (secDuration > minLengthDot/1000.0 && secDuration <= maxLengthDot/1000.0) {
          messageArray[currentCharacter] = 0;
          /*Serial.print("I've run Dot at a length of: ");
          Serial.print(secDuration);
          Serial.println(" seconds");*/
          currentCharacter++;
        }
        else if (secDuration > minLengthDash/1000.0 && secDuration <= maxLengthDash/1000.0) {
          messageArray[currentCharacter] = 1;
          /*Serial.print("I've run Dash at a length of: ");
          Serial.print(secDuration);
          Serial.println(" seconds");*/
          currentCharacter++;
        }
      }
      Serial.println("End of character");
      messageArray[currentCharacter] = 2;
      currentCharacter++;
      dotDashNumber = 0;
      Serial.print("Current message is: ");
      for (int i = 0; i < currentCharacter; i++) {
        Serial.print(messageArray[i]);
      }
      Serial.println();
    }
    else {
      Serial.println("You have run out of space. Please enter a full-stop code to end message");
    }

  }
}
void endOfWord() {
  wordDone = true;
}

void deletingCharacter() {
  deleteCharacter = true;
}

void playback() {
  playbackMessage = true;
}

void reset() {
  for (int i = 0; i < currentCharacter; i++) {
    playbackArray[i] = messageArray[i];
    messageArray[i] = ' ';
  }
  storedPhrase = thePhrase;
  thePhrase = "";
  playbackLength = currentCharacter;
  dotDashNumber = 0;
  currentCharacter = 0;
}
void endOfCharacter() {
  characterDone = true;
}
