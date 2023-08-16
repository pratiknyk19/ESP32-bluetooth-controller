// Define the use of NimBLE (Bluetooth Low Energy stack)
#define USE_NIMBLE
// Include the BleKeyboard library
#include <BleKeyboard.h>

// Create a BleKeyboard instance
BleKeyboard bleKeyboard;

// Define pins for gamepad buttons and LED
#define GP_UP 23     // w
#define GP_DOWN 19   // s
#define GP_LEFT 21   // a
#define GP_RIGHT 18  // d
#define GP_F 4
#define GP_E 16
#define GP_X 17
#define GP_Y 22
#define LED_PIN 2

// Array to store the state of each button
bool keyStates[8] = {false, false, false, false, false, false, false, false};
// Array to store the pins for each button
int keyPins[8] = {GP_UP, GP_DOWN, GP_LEFT, GP_RIGHT, GP_F, GP_E, GP_X, GP_Y};
// Array to store the key codes for each button
uint8_t keyCodes[8] = {'w', 's', 'a', 'd', 'f', 'e', 'x', 'y'};

// Setup function runs once at startup
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("Code running...");
  
  // Set up input pins for buttons
  setInputs();
  
  // Initialize the BleKeyboard
  bleKeyboard.begin();

  // Set up LED pin as an output and turn it off initially
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

// Flag to track if a connection notification was sent
bool connectNotificationSent = false;

// Loop function runs repeatedly
void loop() {
  // Counter variable
  int counter;

  // Check if BleKeyboard is connected
  if(bleKeyboard.isConnected()) {
    // If not already sent, print a connection notification
    if (!connectNotificationSent) {
      Serial.println("Code connected...");
      connectNotificationSent = true;
    }
    
    // Iterate through each button and handle its state
    for(counter = 0; counter < 8; counter ++){
      handleButton(counter);
    }
  }
}

// Function to set input pins for buttons with pull-up resistors
void setInputs() {
  pinMode(GP_UP, INPUT_PULLUP);
  pinMode(GP_DOWN, INPUT_PULLUP);
  pinMode(GP_LEFT, INPUT_PULLUP);
  pinMode(GP_RIGHT, INPUT_PULLUP);
  pinMode(GP_F, INPUT_PULLUP);
  pinMode(GP_E, INPUT_PULLUP);
  pinMode(GP_X, INPUT_PULLUP);
  pinMode(GP_Y, INPUT_PULLUP);
}

// Function to handle button press and release
void handleButton(int keyIndex){
  // Check if the button is pressed
  if (!digitalRead(keyPins[keyIndex])){
    // If the button is not already pressed, set its state and send key press
    if (!keyStates[keyIndex]){
      keyStates[keyIndex] = true;
      bleKeyboard.press(keyCodes[keyIndex]);
      digitalWrite(LED_PIN, HIGH); // Turn on the LED
    }
  }
  else {
    // If the button is not pressed, set its state and send key release
    if (keyStates[keyIndex]){
      keyStates[keyIndex] = false;
      bleKeyboard.release(keyCodes[keyIndex]);
      digitalWrite(LED_PIN, LOW); // Turn off the LED
    }
  }
}
