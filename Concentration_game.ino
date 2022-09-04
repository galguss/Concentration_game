#define joysticlX A1
#define joysticlY A2

#define ledBlue 6
#define ledGreen 9
#define ledYellow 10
#define ledRed 11

//----------Leds------------
#define LEDS_NUMBERS  4
int redRate;
int randomLeds;
byte isNumberLedOn;
bool isLedsOn;


int leds[LEDS_NUMBERS] = {
  ledBlue, ledGreen, ledYellow, ledRed
};

unsigned long TimeForTurningOnTheLight;



void Led_ON(int n){
  digitalWrite(leds[n], HIGH);
  isLedsOn = true;
  isNumberLedOn = n;
}

void Led_Off(int n){
  digitalWrite(leds[n], LOW);
  isLedsOn = false;
}

void allLedsOff(){
    for(int k = 0; k < LEDS_NUMBERS; k++){
    digitalWrite(leds[k], LOW);
  }
}

void endGame(){
  for(int k = 0; k < LEDS_NUMBERS; k++){
    digitalWrite(leds[k], HIGH);
  }
}

 void toggleLeds(){
  randomLeds = random(0, LEDS_NUMBERS);
  redRate = chooseRandomValueInSmartRange();
  TimeForTurningOnTheLight = millis();
     if(!isLedsOn){
      Led_ON(randomLeds);
    }else{
      Led_Off(isNumberLedOn);
    } 
}


//-------joystick-------
#define JoyInTheMiddle 15
unsigned long holdingTimeOfTheJoystick;
byte startGame;


int joySituation(){
  int axisX = analogRead(joysticlX);
  int axisY = analogRead(joysticlY);
  byte joyPos;
  
    if(axisX < 500){
      joyPos = 1;
    }else if(axisX > 550){
      joyPos = 3;
    }else{
      joyPos = 15; 
    }
    
    if(axisY < 400){
      joyPos = 0;
    } else if(axisY > 700){
      joyPos = 2;
    }

    if((axisX > 750) && (axisY < 450) && ((millis() - holdingTimeOfTheJoystick) > 1000)){
      startGame = 1;
      holdingTimeOfTheJoystick = millis();
    }
    
    if((axisX < 450) && (axisY > 750) && ((millis() - holdingTimeOfTheJoystick) > 1000)){
      if(startGame == 1){
        startGame = 2;
      }else{
        startGame = 0;
      }
        holdingTimeOfTheJoystick = millis();
      }
  return joyPos;
}

//-------Sorces gamse-----
#define lampDoesNotExist 100
int points = 0;
int displacementTimeInMsec = 300;
 
int chooseRandomValueInSmartRange(){
  int minRangeVal = max(500, 2000 -500 * points );
  int maxRangeVal = max(1000, 3000 -500 * points );
  int val = random(minRangeVal, maxRangeVal);

  return val;
}

void gameManagement(){
    if((isNumberLedOn == joySituation()) && ((millis() - holdingTimeOfTheJoystick) > displacementTimeInMsec)){
      points++;
      Led_Off(isNumberLedOn);
      isNumberLedOn = lampDoesNotExist;
      holdingTimeOfTheJoystick = millis();
    }else if(((joySituation() != JoyInTheMiddle) && (isNumberLedOn != joySituation()) && ((millis() - holdingTimeOfTheJoystick) > displacementTimeInMsec)) || ((!isLedsOn) && ((millis() - holdingTimeOfTheJoystick) > redRate))){
      if(points <= 0){
        points = 0;
      }else{
        points--;
      }
      holdingTimeOfTheJoystick = millis();
    }
    
 if((millis() - TimeForTurningOnTheLight) > redRate){
  toggleLeds();
 }

 
 Serial.print("points  = ");
 Serial.println(points);
}

//------State machine----
#define WAIT_4_START  21
#define START_GAME    22
#define GAME_ON       23
#define GAME_FINISHED 24
int currState;
unsigned long startGameInMs;

void newGame(){
  startGame = 0;
  points = 0;
}

void setup() {
  for(int k = 0; k < LEDS_NUMBERS; k++){
    pinMode(leds[k], OUTPUT);
  }
 Serial.begin(9600);
 holdingTimeOfTheJoystick = millis();
 TimeForTurningOnTheLight = millis();
 isLedsOn = false;
 randomSeed(analogRead(A6));
 currState = WAIT_4_START;
}

void loop() {
  switch(currState){
   case WAIT_4_START:
      joySituation();
      if(startGame == 2){
        allLedsOff();
        currState = START_GAME;
      }
      break;
    case START_GAME:
        startGameInMs = millis();
        currState = GAME_ON;
      break;
    case GAME_ON:
        gameManagement();
        if((millis() - startGameInMs) > 20000){
          currState = GAME_FINISHED;
        }
      break;
     case GAME_FINISHED:
        endGame();
        newGame();
        currState = WAIT_4_START;
      break;   
  }

}
