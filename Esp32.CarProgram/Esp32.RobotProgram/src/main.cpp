#include <Arduino.h>
#include <FastLED.h>
#include <JC_Button.h>
#include <WiFi.h>
#include "Secrets.h"
#include <PubSubClient.h>

// Starting point: https://github.com/esikora/ESP32App_Alert_BLE/blob/master/src/ESP32App_Alert_BT.cpp

struct RGBColor {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  char* Name;
};

// Pin assignments
const byte PIN_LEDATOM = 27; // M5Stack Atom Lite: internel Neopixel LED
const byte PIN_BUTTON = 39; // M5Stack Atom Lite: internal button

// Motor
const byte PIN_MOTORA_IN1 = 22;
const byte PIN_MOTORA_IN2 = 19;
const byte PIN_MOTORA_ENABLE = 21;
byte MotorA_Status = 0;

const byte PIN_MOTORB_IN1 = 33;
const byte PIN_MOTORB_IN2 = 23;
const byte PIN_MOTORB_ENABLE = 19;
byte MotorB_Status = 0;

// Setting PWM properties
const int MOTORA_FREQUENCY = 30000;
const int MOTORA_PWM_CHANNEL = 0;
const int MOTORA_RESOLUTION = 8;
int MotorA_DutyCycle = 200;

const int MOTORB_FREQUENCY = 30000;
const int MOTORB_PWM_CHANNEL = 0;
const int MOTORB_RESOLUTION = 8;
int MotorB_DutyCycle = 200;

// Status LED: color definitions
const RGBColor COLORS[6] = { 
  RGBColor {200,   0, 0, "Red"},
  RGBColor {0,   200, 0, "Green"},
  RGBColor {255, 165, 0, "Orange"},
  RGBColor {255, 0, 255, "Magenta"},
  RGBColor {0, 0, 150, "Blue"},
  RGBColor {255, 255, 0, "Yellow"}
};

// Brightness factor for LED
uint8_t brightness = 255;

// Cycle time of main loop
const int LOOP_DELAY = 100; // ms
int loopCounter = 0;
int colorIndex = 0;
int temperatureFrequency = 300; // After how many loops do we update temperature

// Hardware gateways
CRGB ledAtom[1]; // Internal LED controller
Button Btn(PIN_BUTTON); // Internal button

void setLEDColor(RGBColor color) {
    Serial.print("Setting LED to new color ");
    Serial.println(color.Name);
    ledAtom[0].setRGB(color.Red, color.Green, color.Blue);
    FastLED.show();
}

void handleMqttMessage(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT Message Received:");
  Serial.println(topic);
}

// Network clients
WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_SERVER_IP, 1883, handleMqttMessage, wifiClient);

// Arduino setup + loop
void setup() {
  Serial.begin(115200);

  // Initialise the Button gateway
  Btn.begin();

  // Initalize the led
  FastLED.addLeds<NEOPIXEL, PIN_LEDATOM>(ledAtom, 1);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  setLEDColor(COLORS[0]);

  Serial.println("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to WiFi yet. Trying again...");
    delay(100);
  }

  Serial.print("Successfully connected to WiFi. ");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to MQTT");

  if (mqttClient.connect("home-brains")) {
    Serial.println("Connected to MQTT!");
    mqttClient.publish("hello/", "home-brains connected!");
    mqttClient.subscribe("test/topic");
  }

  Serial.println("Setting up motors");

  pinMode(PIN_MOTORA_IN1, OUTPUT);
  pinMode(PIN_MOTORA_IN2, OUTPUT);
  pinMode(PIN_MOTORA_ENABLE, OUTPUT);

  pinMode(PIN_MOTORB_IN1, OUTPUT);
  pinMode(PIN_MOTORB_IN2, OUTPUT);
  pinMode(PIN_MOTORB_ENABLE, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(MOTORA_PWM_CHANNEL, MOTORB_FREQUENCY, MOTORB_RESOLUTION);
  ledcSetup(MOTORB_PWM_CHANNEL, MOTORB_FREQUENCY, MOTORB_RESOLUTION);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PIN_MOTORA_ENABLE, MOTORA_PWM_CHANNEL);
  ledcAttachPin(PIN_MOTORB_ENABLE, MOTORB_PWM_CHANNEL);

  Serial.println("***** Setup Completed *****");
}

void loop() {
  loopCounter++;

  Btn.read();
  
  if (Btn.wasReleased()) {
    RGBColor color = COLORS[colorIndex];
    Serial.print("colorIndex=");
    Serial.print(colorIndex);
    Serial.print(". -> ");
    setLEDColor(color);
    colorIndex++;
    if (colorIndex > 5) {
      colorIndex = 0;
    }
    mqttClient.publish("devices/esp32-test/color", color.Name);

    if (MotorB_Status == 0) {
      MotorB_Status = 1;
      Serial.println("Moving Forward");
      digitalWrite(PIN_MOTORA_IN1, HIGH);
      digitalWrite(PIN_MOTORA_IN2, LOW);
      ledcWrite(MOTORA_PWM_CHANNEL, 250);

      digitalWrite(PIN_MOTORB_IN1, HIGH);
      digitalWrite(PIN_MOTORB_IN2, LOW);
      ledcWrite(MOTORB_PWM_CHANNEL, 250);
    }
    else {
      MotorB_Status = 0;
      Serial.println("Stopping motor");
      digitalWrite(PIN_MOTORA_IN1, LOW);
      digitalWrite(PIN_MOTORA_IN2, LOW);
      ledcWrite(MOTORA_PWM_CHANNEL, 0);

      digitalWrite(PIN_MOTORB_IN1, LOW);
      digitalWrite(PIN_MOTORB_IN2, LOW);
      ledcWrite(MOTORB_PWM_CHANNEL, 0);
    }
  }

  mqttClient.loop();

  if (loopCounter > 1000)
  {
    loopCounter = 0;
  }
  delay(LOOP_DELAY);
}