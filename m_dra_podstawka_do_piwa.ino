/*********
  by Nicek
*********/

#include <LiquidCrystal_I2C.h>
#include <CountDown.h>

CountDown CD;

bool checkBeerOnDeck(float distanceCm);
void remindAboutBeer();
void resetAfterPickUp();
bool beerBackOndDeck(bool beerPickedUp, bool beerOnDeck);
void resetTime();
void countTimeOnScreen();
void tellToPutItBack();
void sayHello();

int lcdColumns = 16;
int lcdRows = 2;

const int trigPin = 12;
const int echoPin = 14;
const unsigned char Passive_buzzer = 15;

#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
bool beerOnDeck = false;
float lastMeasure;
uint32_t last_remaining = 0;
bool reminderDisplayed = false;
bool beerPickedUp = false;
bool handShaked = false;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  lcd.init();                     
  lcd.backlight();
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode (Passive_buzzer, OUTPUT);
}

void loop(){
  lcd.setCursor(0, 0);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY/2;
  Serial.println(distanceCm);

  if (!handShaked) {
    sayHello();
  }
  if (last_remaining != CD.remaining() || CD.remaining() == 0 )
  {
    last_remaining = CD.remaining();
  }
  if (!last_remaining == 0 && checkBeerOnDeck(distanceCm)) {
    countTimeOnScreen();
  }
  if (last_remaining == 0 && checkBeerOnDeck(distanceCm)) {
    remindAboutBeer();
  }

  if (!checkBeerOnDeck(distanceCm)) {
    tellToPutItBack();
  }

  if (last_remaining == 0 && reminderDisplayed && beerBackOndDeck(beerPickedUp, checkBeerOnDeck(distanceCm))) {
    resetAftePickUp();
  }
  
}

void tellToPutItBack() {
  beerPickedUp = true;
  lcd.setCursor(0, 0);
  lcd.print("MOZESZ ODSTAWIC");
  delay(350);
  resetTime();
  lcd.clear();
}

void countTimeOnScreen() {
  int minutes = CD.remaining() / 60;
  int seconds = CD.remaining() % 60;
  if (seconds < 10) {
    lcd.setCursor(11, 1);
    lcd.print(" ");
  }
  lcd.setCursor(0, 0);
  lcd.print("POZOSTALY CZAS:");
  lcd.setCursor(0, 1);
  lcd.print("Min:" );
  lcd.setCursor(4, 1);
  lcd.print(minutes);
  lcd.setCursor(6, 1);
  lcd.print("Sek:" );
  lcd.setCursor(10, 1);
  lcd.print(seconds);
}

void remindAboutBeer() {
  lcd.clear();
  tone(Passive_buzzer, 659);
  lcd.setCursor(0, 0);
  lcd.print("CZAS NA PICIE");
  delay(500);
  lcd.noBacklight();
  noTone(Passive_buzzer);
  delay(500);
  lcd.backlight();
  reminderDisplayed = true;
}

bool checkBeerOnDeck(float distanceCm) {
  return distanceCm <= 8;
}

void resetTime() {
  if (!checkBeerOnDeck(distanceCm)) {
    CD.start(0, 0, 5, 0);
  }
}

void resetAftePickUp() {
  reminderDisplayed = false;
  beerPickedUp = false;
  resetTime();
}

bool beerBackOndDeck(bool beerPickedUp, bool beerOnDeck) {
  return beerPickedUp && beerOnDeck;
}

void sayHello() {
  lcd.setCursor(0, 0);
  lcd.print("LADUJE CUDA..." );
  lcd.setCursor(8, 1);
  lcd.print("by Nicek" );
  tone(Passive_buzzer, 587);
  delay (200); 
  tone(Passive_buzzer, 659);
  delay (200); 
  tone(Passive_buzzer, 783);
  delay (200); 
  tone(Passive_buzzer, 880);
  delay (200); 
  tone(Passive_buzzer, 987);
  delay (200); 
  tone(Passive_buzzer, 1046);
  delay (200); 
  noTone(Passive_buzzer) ;
  lcd.clear();
  handShaked = true;
}
