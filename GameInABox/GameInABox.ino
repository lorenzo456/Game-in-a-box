int tempScore = 0;
int tempPin = 0;

int pushButton = 2;
int pushButton2 = 3;

int LED1 = 13;
int LED2 = 12;
int LED3 = 11;
int LED4 = 10;
int LED5 = 9;
int LED6 = 8;
int ledPins[] = {8,9,10,11,12,13};
int pinCount = 6;

bool currentRound = true;
bool reverse = false;
bool isInitialized;
int buttonState;
int buttonState2;

int score1 = 0;
int score2 = 0;

unsigned long previousMillis = 0;
const long interval = 500; 
long gameInterval = 500;
int ledState = LOW;

int lastButtonState = LOW;
int lastButtonState2 = LOW;
unsigned long lastDebounceTime = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;
int read1;
int read2;

int currentLed;
bool afterDelay = true;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  isInitialized = false;

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(pushButton, INPUT);
  pinMode(pushButton2, INPUT);
  currentLed = 9;
 // Initialize();
}
void StartGame()
{
  if(digitalRead(pushButton) && digitalRead(pushButton2) && afterDelay)
  {
    isInitialized = true;
  }
}

void Initialize()
{

  if(!isInitialized && afterDelay)
  {
    isInitialized  = true;
  }

}
void BlinkWithoutDelay()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= gameInterval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
      gameInterval = random(100, 400);
    }
    digitalWrite(currentLed, ledState);
    
    if(ledState == LOW)
    {
      if(!reverse)
      {
        currentLed++; 
        if(currentLed > 13){
            currentLed = 12;
            reverse = true;
          }
      }else
        {
        currentLed--; 
        if(currentLed < 8){
            currentLed = 9;
            reverse = false;
          }
        }
    }
    
  }
}

void EndRound(bool winP1)
{
  digitalWrite(13, LOW);
  digitalWrite(8, LOW);
  currentRound = false;
  if(winP1)
  {
    score1++;
    tempScore = score1;
  }else
  {
    score2++;
    tempScore = score2;
  }

  if(winP1)
  {
    tempPin = 8;
  }else
  {
    tempPin = 13;
  }

  Serial.println(tempPin + " " + tempScore);
 // currentRound = true;
}
void EndGame()
  {
      score1 = 0;
      score2 = 0;
      tempScore = 0;
      isInitialized = false;
      currentRound = true;
      reverse = false;
      digitalWrite(currentLed, LOW);
      currentLed = 9;
      afterDelay = false;
      delay(2000);
      afterDelay = true;
  }
void checkForInput()
{
  read1 = digitalRead(pushButton);

  if(read1 != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if((millis() - lastDebounceTime) > debounceDelay)
  {
    if(read1 != buttonState){
      buttonState = read1;
      Serial.println("BTN 1");
      if(digitalRead(13))
      {
       // Serial.println("13"); 
        EndRound(true);
      }
      
      if(buttonState == HIGH)
      {
         //  Serial.println("BUTTON 1");           
      }
    }
  }
}

void checkForInput2()
{
  read2 = digitalRead(pushButton2);

  if(read2 != lastButtonState2)
  {
    lastDebounceTime2 = millis();
  }

  if((millis() - lastDebounceTime2) > debounceDelay)
  {
    if(read2 != buttonState2){
      Serial.println("BTN 2");
      buttonState2 = read2;
      if(digitalRead(8))
      {
        Serial.println("8");
        EndRound(false);
      }
      
      if(buttonState2 == HIGH)
      {
           Serial.println("BUTTON 2");           
      }
    }
  }
}

unsigned long endPreviousMillis = 0;
void CheckEndGame()
{
  bool endgame = false;
  if(!digitalRead(pushButton) || !digitalRead(pushButton2))
    {
      return;
    }

  int count = 0;
  while(digitalRead(pushButton) && digitalRead(pushButton2) && isInitialized)
  {
    if(millis()-endPreviousMillis >= 5000)
    {
      endPreviousMillis = millis();
      Serial.println("LONG PRESS AND END GAME");
      EndGame();
    }
    
  }

}

int winLedState = LOW;
unsigned long winPreviousMillis = 0;
int winCount = 0;

void blinkWin(int Score, int Pin)
{
unsigned long currentMillis = millis();

  if (currentMillis - winPreviousMillis >= interval) {
      Serial.println("BLINK WIN");
        winPreviousMillis = currentMillis;
        
        if (winLedState == LOW) {
          winLedState = HIGH;
          winCount++;
        } else {
          winLedState = LOW;
        }
        digitalWrite(tempPin, winLedState);
        if(winCount == tempScore)
        {
          winCount = 0;
          currentRound = true;
          digitalWrite(tempPin, LOW);
        }
      }
}

void loop() {
  
  checkForInput();
  checkForInput2();

  if(isInitialized)
  {
    CheckEndGame();
    if(currentRound)
    {
      BlinkWithoutDelay();
    }
  }else
  {
    StartGame();
  }


  if(!currentRound)
  {
    blinkWin(tempScore,tempPin);
  }
 lastButtonState = read1;
 lastButtonState2 = read2;
}
