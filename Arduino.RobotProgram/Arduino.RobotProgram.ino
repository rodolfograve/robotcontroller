#include <Keypad.h>
#include <Servo.h>

// PIN Assignments
int Led13 = 13;                       // Define LED port; this is the LED built in to the Arduino (labled L)
int buzzer = 12;
int IRSensor = 8;

// Keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

Servo leftArmServo;
Servo rightArmServo;
int leftArmServoAngle = 0;
int rightArmServoAngle = 0;
const int armStep = 50;

String latestCommand = "";
String currentMode = "KeyPad";

int loopControl = 0;
                              
void setup() {
  currentMode = "KeyPad";
  loopControl = 0;
  Serial.begin(9600); // The IDE settings for Serial Monitor/Plotter (preferred) must match this speed
  pinMode(Led13, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(IRSensor, INPUT);
  leftArmServo.attach(10);
  rightArmServo.attach(11);
  leftArmServo.write(40);
  rightArmServo.write(40);
}

void loop() {
  if (currentMode == "AllowRemote") {
    if (Serial.available() > 0) {
      latestCommand = Serial.readString();
      latestCommand.replace("\n", "");
      Serial.println("Received: " + latestCommand);
  
      if (latestCommand == "Buzz") {
        buzz(2, 700);
      }
      else if (latestCommand == "LedOn") {
        ledOn();
      }
      else if (latestCommand == "LedOff") {
        ledOff();
      }
      else if (latestCommand == "UseKeyPad") {
        setModeKeyPad();
      }
      else if (latestCommand == "LeftArmUp") {
        turnLeftArmNegative(armStep);
      }
      else if (latestCommand == "LeftArmDown") {
        turnLeftArmPositive(armStep);
      }
      else if (latestCommand == "RightArmUp") {
        turnRightArmNegative(armStep);
      }
      else if (latestCommand == "RightArmDown") {
        turnRightArmPositive(armStep);
      }
      else {
        log("Unknown remote command: " + latestCommand);
      }
    }
  }
  
  char customKey = getKeypadKey();
  if (customKey) {
    log("KeyPressed: " + customKey);
    switch (customKey) {
      case '1': ledOn(); beep(1); break;
      case '2': ledOff(); break;
      case '3': buzz(2, 700); break;
      case '4': resetLeftArm(); break;
      case '5': turnLeftArmPositive(armStep); break;
      case '6': turnLeftArmNegative(armStep); break;
      case '7': resetRightArm(); break;
      case '8': turnRightArmPositive(armStep); break;
      case '9': turnRightArmNegative(armStep); break;
      case 'A': setModeRemote(); break;
      case 'D': setModeKeyPad(); break;
      default: log("No action for key " + customKey);
    }
  }

  int irSensorStatus = digitalRead(IRSensor);
  log("IR Sensor Status: " + irSensorStatus);
  
  loopControl++;
  if (loopControl >= 30000) {
    log("Current mode is " + currentMode);
    loopControl = 0;
  }
}

void ledOn() {
  digitalWrite(Led13, HIGH);
}

void ledOff() {
  digitalWrite(Led13, LOW);
}

char getKeypadKey() {
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
  return customKey;
}

void beep(int frequency) {
  buzz(frequency, 30);
}

void buzz(int frequency, unsigned char length) {
  unsigned char i;
  //output an frequency
  for(i = 0; i < length; i++)
  {
    digitalWrite(buzzer,HIGH);
    delay(frequency);
    digitalWrite(buzzer,LOW);
    delay(frequency);
  }
}

void setModeRemote() {
  currentMode = "AllowRemote";
  log("Switched to Remote mode");
}

void setModeKeyPad() {
  currentMode = "KeyPad";
  log("Switched to KeyPad mode");
}

void turnLeftArmPositive(int step) {
  turnServoPositive(leftArmServo, step);
}

void turnLeftArmNegative(int step) {
  turnServoNegative(leftArmServo, step);
}

void resetLeftArm() {
  resetServo(leftArmServo);
}

void turnRightArmPositive(int step) {
  turnServoPositive(rightArmServo, step);
}

void turnRightArmNegative(int step) {
  turnServoNegative(rightArmServo, step);
}

void resetRightArm() {
  resetServo(rightArmServo);
}

void turnServoPositive(Servo servo, int step) {
  servo.write(servo.read() + step);
}

void turnServoNegative(Servo servo, int step) {
  servo.write(servo.read() - step);
}

void resetServo(Servo servo) {
  servo.write(0);
}

void log(String message) {
  Serial.println("LOG: " + message);
}
