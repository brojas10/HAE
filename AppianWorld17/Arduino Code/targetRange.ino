/*
TODO

* Bug - random led pin does not change when target is hit
*/

const int numberTargetButtons = 2; // UPDATE with count of buttons, sizeof() doesn't seem to cooperate and apparently there is no count / array length function!?
const int targetButtonScore[] = {50, 100}; // UPDATE for new button
const int targetButtonPin[] = {10,11}; // UPDATE for new button KEEP IN SEQUENTIAL ORDER FOR RANDOM BONUS!
const int targetLedPin[] = {6,7}; // UPDATE for new button
int targetLedFlashCount[] = {0,0}; // UPDATE for new button
unsigned long targetLedPreviousMillis[] = {0,0}; // UPDATE for new button

const int newGameButtonPin = 9;
const int newGameLedPin = 5;

const int newGameTimesFlash = 5;
const int newGameFlashDelay = 500;

const int hitTimesFlash = 4;
const int hitTimeFlashDelay = 250;

const int buttonCheckDelay = 1000;

int bonusIndex = 0;
int previousBonusIndex = 100;
unsigned long bonusLedPreviousMillis = 0;
int bonusLedFlashCount = 0;

const int bonusMultiplier = 2;
const int bonusTimesFlash = 5;
const int bonusTimeFlashDelay = 1000;

void setup() {
  Serial.begin(9600);
  
  pinMode(newGameButtonPin, INPUT);
  pinMode(newGameLedPin, OUTPUT);
  
  for (int i = 0; i < numberTargetButtons; i++){
    pinMode(targetButtonPin[i], INPUT);
  }
  
  for (int i = 0; i < numberTargetButtons; i++){
    pinMode(targetLedPin[i], OUTPUT);
  }
  
  randomSeed(analogRead(0));
  setBonusIndex();
  
}

void loop() {
  
  /*checkNewGameButton();*/
  checkBonusIndex();
  flashBonusLed();
  checkTargetButtons();
  flashTargetLeds();
  // delay(100); // TODO: THIS IS FOR DEBUGGING ONLINE ONLY!!!! REMOVE ME!!!!!
}

void checkNewGameButton() {
  if (digitalRead(newGameButtonPin) == HIGH) {
    Serial.println("NewGame");
    for (int i = 0; i < numberTargetButtons; i++) {
      digitalWrite(targetLedPin[i], LOW);
      targetLedFlashCount[i] = 0;
    }
    for (int i = 0; i <newGameTimesFlash; i++) {
      digitalWrite(newGameLedPin, HIGH);
      delay(newGameFlashDelay);
      digitalWrite(newGameLedPin, LOW);
      delay(newGameFlashDelay);
    }
    setBonusIndex();
  } else {
    
  }
}

void checkTargetButtons() {
  for (int i = 0; i < numberTargetButtons; i++) {
    if (digitalRead(targetButtonPin[i]) == HIGH) {
      targetLedFlashCount[i] += hitTimesFlash;
      if (i == bonusIndex) {
        Serial.println(targetButtonScore[i] * bonusMultiplier);
      }
      else {
        Serial.println(targetButtonScore[i]);
      }
      setBonusIndex();
      delay(buttonCheckDelay);
    }
    else {
      
    }
  }
}

void flashTargetLeds() {
  for (int i = 0; i < numberTargetButtons; i++) {
    if (targetLedFlashCount[i] > 0 && digitalRead(targetLedPin[i]) == LOW) {
      if (millis() - targetLedPreviousMillis[i] >= hitTimeFlashDelay) {
        targetLedPreviousMillis[i] = millis();
        digitalWrite(targetLedPin[i], HIGH);
      }
    } 
    else {
      if (targetLedFlashCount[i] > 0 && digitalRead(targetLedPin[i]) == HIGH && millis() - targetLedPreviousMillis[i] >= hitTimeFlashDelay) {
        targetLedPreviousMillis[i] = millis();
        digitalWrite(targetLedPin[i], LOW);
        targetLedFlashCount[i]--;
      } 
    }
  }
}

void checkBonusIndex() {
  if (bonusLedFlashCount == 0 && millis() - bonusLedPreviousMillis >= bonusTimeFlashDelay) {
    setBonusIndex();
    bonusLedFlashCount = bonusTimesFlash;
  }
}

void setBonusIndex() {
  bonusLedFlashCount = 0;
  digitalWrite(targetLedPin[bonusIndex], LOW);
	while(bonusIndex == previousBonusIndex) {
		bonusIndex = random(0, numberTargetButtons);
	}
	previousBonusIndex = bonusIndex;
}

void flashBonusLed() {
  if (bonusLedFlashCount > 0 && digitalRead(targetLedPin[bonusIndex]) == LOW) {
    if (millis() - bonusLedPreviousMillis >= bonusTimeFlashDelay) {
      bonusLedPreviousMillis = millis();
      digitalWrite(targetLedPin[bonusIndex], HIGH);
    }
  } 
  else {
    if (bonusLedFlashCount > 0 && millis() - bonusLedPreviousMillis >= bonusTimeFlashDelay && digitalRead(targetLedPin[bonusIndex]) == HIGH) {
      bonusLedPreviousMillis = millis();
      digitalWrite(targetLedPin[bonusIndex], LOW);
      bonusLedFlashCount--;
    } 
  }
  
}