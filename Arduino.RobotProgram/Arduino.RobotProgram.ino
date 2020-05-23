#include <Keypad.h>
#include <Servo.h>

const int sensorAnalogPin = A0;       // Select the Arduino input pin to accept the Sound Sensor's analog output 
const int sensorDigitalPin = 4;       // Select the Arduino input pin to accept the Sound Sensor's digital output
int digitalValue;                     // Define variable to store the digital value coming from the Sound Sensor
int Led13 = 13;                       // Define LED port; this is the LED built in to the Arduino (labled L)
                                      // When D0 from the Sound Sensor (connnected to pin 7 on the
                                      // Arduino) sends High (voltage present), L will light. In practice, you
                                      // should see LED13 on the Arduino blink when LED2 on the Sensor is 100% lit.
int buzzer = 12;                      //the pin of the active buzzer

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
  Serial.begin(9600);               // The IDE settings for Serial Monitor/Plotter (preferred) must match this speed
  pinMode(sensorDigitalPin,INPUT);  // Define pin 7 as an input port, to accept digital input
  pinMode(Led13,OUTPUT);            // Define LED13 as an output port, to indicate digital trigger reached
  pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
  leftArmServo.attach(10);
  rightArmServo.attach(11);
  leftArmServo.write(40);
  rightArmServo.write(40);
}

void loop() {
  if (currentMode == "Remote") {
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
  else {
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
        case 'D': setModeRemote(); break;
        default: log("No action for key " + customKey);
      }
    }
  }
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

int getSoundSensorAnalogValue() {
  int result = analogRead(sensorAnalogPin); // Read the value of the analog interface A0 assigned to digitalValue 
  Serial.println(result); // Send the analog value to the serial transmit interface
  return result;
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
  currentMode = "Remote";
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
