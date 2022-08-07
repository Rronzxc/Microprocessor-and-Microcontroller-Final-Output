/*

In Daytime:
1. Start the system by clicking "Start Simulation".
2. The light bulb, relay and photoresistor act as automated 
light that will operate only when dark. Slide the photoresistor 
to the right side where the sun icon is located.
3.  PIR sensor and slideswitch act as one that detects if someone
is inputting password. (note: lcd will turn off when PIR is not 
detecting any movement anymore so we connect slideswitch and it 
is the representation that the movement is constant for this program).
4. “Enter Password: “  will flash on LCD screen then you can
start inputting the password (the password in this program is 1234). 
5. If the entered password is correct, the door lock system will 
unlock then after 5 seconds the servo will automatically operate 
to go back to its initial position. (note: the green LED should also light up)
6. Once you’ve tried 3 times and it’s still incorrect, 
“Warning” will flash on LCD and there will be 20 seconds
waiting time before you can input another password. 
(note: each incorrect input of password, the red LED should light up)

In Night time:
1. Start the system by clicking "Start Simulation".
2. The light bulb, relay and photoresistor act as automated 
light that will operate only when dark. Slide the photoresistor 
to the left side where the moon icon is located.
3.  PIR sensor and slideswitch act as one that detects if someone
is inputting password. (note: lcd will turn off when PIR is not 
detecting any movement anymore so we connect slideswitch and it 
is the representation that the movement is constant for this program).
4. “Enter Password: “  will flash on LCD screen then you can
start inputting the password (the password in this program is 1234). 
5. If the entered password is correct, the door lock system will unlock
then after 5 seconds the servo will automatically operate to go back to
its initial position. (note: the green LED should also light up)
6. Once you’ve tried 3 times and it’s still incorrect, 
“Warning” will flash on LCD and there will be 20 seconds
waiting time before you can input another password. 
(note: each incorrect input of password, the red LED should light up)


 

*/

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// PIR VARIABLE
int pirPin = 12;
int pirVal;

// RELAY VARIABLE
int relayPin = 8;

// PHOTORESISTOR VARIABLE
int presPin = A3;
int presVal;

//
int i = 20;
int warning = 0;
int servoState;
int morning;
int open = 180;
int close = 0;
int a = 0, b = 0, c = 0, d = 0;
int var = 0;
int C1 = 1, C2 = 2, C3 = 3, C4 = 4;
char f = '*';
const byte rows = 4;
const byte columns = 3;
char keyPad[rows][columns] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte pinRows[rows] = {0, 6, 5, 4};
byte pinColumns[columns] = {3, 2, 1};
Servo lock;
Keypad keypad = Keypad(makeKeymap(keyPad), pinRows, pinColumns, rows, columns );

LiquidCrystal_I2C lcd(32, 16, 2);

void setup()
{
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(0);
  
  pinMode(presPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  lock.attach(7);
  lock.write(close);
  servoState = 1;
}
void loop()
{
  presVal = analogRead(presPin);
  pirVal = digitalRead(pirPin);
  if (presVal > 600)
  {
    morning = 1;
  }
  if (pirVal == 1)
  {
    lcd.setBacklight(1);
    if (morning == 1 && presVal <= 10)
    {
      digitalWrite(relayPin, HIGH);
    }
    else
    {
      digitalWrite(relayPin, LOW);
    }
    char key = keypad.getKey();
    if (key)
    {
      lcd.setCursor(6 + var, 1), lcd.print(key);
      lcd.setCursor(6 + var, 1), lcd.print(f);
      key = key - 48;
      var++;
      switch (var)
      {
        case 1: a = key;
          break;
        case 2: b = key;
          break;
        case 3: c = key;
          break;
        case 4: d = key;
          delay(50);
          if (a == C1 && b == C2 && c == C3 && d == C4)
          {
            warning = 0;
            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print(" Welcome ");
            lcd.setCursor(5, 1);
            lcd.print(" Human !");
            lock.write(open);
            digitalWrite(A0, HIGH);
            delay(1000);
            lcd.clear();
            digitalWrite(A0, LOW);
            lock.write(open);
            delay(5000);
            digitalWrite(relayPin, LOW);
            lock.write(close);
          }
          else
          {
            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("Incorrect");
            lcd.setCursor(4, 1);
            lcd.print("Password");
            digitalWrite(A1, HIGH);
            delay(1000);
            lcd.clear();
            digitalWrite(A1, LOW);
          }
          var = 0;
          lcd.clear();
          warning++;
          break;
      }
      if (warning == 3)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Warning!");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Try again for");
        while (i > 0)
        {
          lcd.setCursor(14, 0);
          lcd.print(i--);
          if (i < 9)
          {
            lcd.setCursor(15, 0);
            lcd.print(" ");
          }
          delay(1000);
        }
        warning = 0;
        i = 20;
        loop();
      }
    }
    if (!key)
    {
      lcd.setCursor(0, 0), lcd.print("Enter Password :");
    }
    delay(2);
  }
  else
  {
    morning = 1;
    warning = 0;
    lcd.clear();
    lcd.setBacklight(0);
    digitalWrite(relayPin, LOW);
  }
}
