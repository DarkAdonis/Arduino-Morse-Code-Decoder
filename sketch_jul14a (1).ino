//Pin port
#define characterPin 4
#define wordPin 5
#define playbackPin 6
#define lightPin1 7
#define lightPin2 10
#define speakerPin 13
#define dotDashPin 2
#define minLengthDot 100
#define maxLengthDot 1000
#define minLengthDash 1000
#define maxLengthDash 3000

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

volatile unsigned long overflowCycles = 0;

//Variables for storing a single character
volatile int previousState; //error checking
volatile int start_count[6];//stores register value at first rising edge of character
volatile int end_count[6];//stores register value at second rising edge of character
volatile int dotDashNumber = 0;//what number of dot/dash is it
volatile int dotDashPinState = 0;
volatile int previousDotDashTime = myMillis();
volatile int previousDotDashState = 0;

//End of Character, Word and light variables
volatile int startOfMessage = 0;
int messageArray[75];//0 for dot, 1 for dash, 2 for end of character, 3 for space
int playbackArray[75];
int playbackLength = 0;
volatile int currentCharacter = 0;//number of characters
volatile boolean characterDone = false;
volatile boolean wordDone = false;
volatile boolean deleteCharacter = false;
String thePhrase = "";
volatile int debounceOtherTime = 0;


//playback variables

long duration;
float secDuration;
long counter_freq;//frequency of the counter in Arduino
boolean playbackMessage = false;
String storedPhrase = "";

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(0, dotDash, CHANGE);
  attachInterrupt(1, otherButtonPressed, FALLING);
  pinMode(characterPin, INPUT_PULLUP);
  pinMode(wordPin, INPUT_PULLUP);
  pinMode(playbackPin, INPUT_PULLUP);
  pinMode(lightPin1, OUTPUT);
  pinMode(lightPin2, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  sbi(TIMSK1, TOIE1);
  Serial.begin(9600);
}

void loop() {
  //If a character was inputted (red button)
  if (characterDone == true) {
    tone(speakerPin, 600);
    recordCharacter();
    characterDone = false;
    myDelay(500);
    noTone(speakerPin);
  }
  //If word is complete (blue button)
  if (wordDone == true) {
    if (currentCharacter < 75) {
      messageArray[currentCharacter] = 3;
      currentCharacter++;
      Serial.println("You entered a space");
    }
    else {
      Serial.println("You have run out of space. Please enter a full-stop code to end message");
    }
    tone(speakerPin, 700);
    myDelay(500);
    noTone(speakerPin);
    dotDashNumber = 0;
    wordDone = false;
  }

  if (deleteCharacter == true) {
    Serial.println("Deleted Character");
    tone(speakerPin, 700);
    myDelay(500);
    noTone(speakerPin);
    dotDashNumber = 0;
    deleteCharacter = false;
  }
  if (playbackMessage == true) {
    Serial.print("Stored phrase is: ");
    Serial.println(storedPhrase);
    for (int i = 0; i < playbackLength; i++) {
      if (playbackArray[i] == 0) {
        digitalWrite(lightPin1, HIGH);
        tone(speakerPin, 500);
        myDelay(500);
        digitalWrite(lightPin1, LOW);
        noTone(speakerPin);
        myDelay(250);
      }
      else if (playbackArray[i] == 1) {
        digitalWrite(lightPin1, HIGH);
        digitalWrite(lightPin2, HIGH);
        tone(speakerPin, 500);
        myDelay(1000);
        digitalWrite(lightPin1, LOW);
        digitalWrite(lightPin2, LOW);
        noTone(speakerPin);
        myDelay(250);
      }
      else if (playbackArray[i] == 2) {
        noTone(speakerPin);
        myDelay(500);
      }
      else {
        noTone(speakerPin);
        myDelay(1000);
      }
    }
    playbackMessage = false;
  }
}




