#include <Keypad.h>

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

String latestCommand = "";
                              
void setup() {
  Serial.begin(9600);               // The IDE settings for Serial Monitor/Plotter (preferred) must match this speed
  pinMode(sensorDigitalPin,INPUT);  // Define pin 7 as an input port, to accept digital input
  pinMode(Led13,OUTPUT);            // Define LED13 as an output port, to indicate digital trigger reached
  pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
}

void loop() {
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
    else {
      Serial.println("Unknown command: " + latestCommand);
    }
  }
/*
  char customKey = getKeypadKey();
  if (customKey) {
    switch (customKey) {
      case '1': ledOn(); beep(1); break;
      case '2': ledOff(); break;
      default: buzz(2, 30);
    }
  }
*/
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
